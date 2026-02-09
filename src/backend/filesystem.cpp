#include "FileEcho/filesystem.hpp"
#include "FileEcho/utils.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <regex>
#include <algorithm>
#include <future>
#include <mutex>

using namespace std;

// 替换 src/backend/filesystem.cpp 中的 scan_directory 函数
vector<FileInfo> FileSystemScanner::scan_directory(const string& path, const FileTreeOptions& options) {
    vector<FileInfo> result;
    mutex mtx;
    
    if (!is_path_safe(path)) {
        cerr << "[Scanner] Unsafe path: " << path << endl;
        return result;
    }
    
    try {
        fs::path root_path = Utils::ToWString(path);

        if (!fs::exists(root_path) || !fs::is_directory(root_path)) {
            return result;
        }
        
        // 1. 执行并行递归扫描
        scan_recursive(root_path, result, mtx, options, 0);

        // 【关键修复】多线程扫描后，列表是乱序的。
        // 必须按路径排序，否则生成的树状图连线(Tree View)会断裂或错乱。
        sort(result.begin(), result.end(), [](const FileInfo& a, const FileInfo& b) {
            return a.path < b.path; // 按路径字典序排序，恢复 DFS 顺序
        });

    } catch (const exception& e) {
        cerr << "[Scanner] Error: " << e.what() << endl;
    }
    
    return result;
}

/**
 * 检查是否应该跳过系统文件夹
 * @param path 要检查的文件系统路径
 * @return 如果路径在系统文件夹列表中则返回true，否则返回false
 */
bool FileSystemScanner::should_skip_system_folder(const fs::path& path) {
    // 定义需要跳过的系统文件夹列表
    static const vector<wstring> skip_list = {
        L"C:\\$Recycle.Bin",     // 回收站目录
        L"System Volume Information", // 系统卷信息目录
        L"C:\\System Volume Information" // 系统卷信息目录的完整路径
    };

    // 将输入路径转换为宽字符串
    wstring p = path.wstring();
    // 遍历跳过列表，检查路径是否以任何跳过列表中的路径开头
    for (const auto& skip : skip_list) {
        // 检查路径长度是否足够且是否以跳过路径开头
        if (p.size() >= skip.size() && p.compare(0, skip.size(), skip) == 0) {
            return true; // 路径匹配，应跳过
        }
    }
    return false; // 路径不匹配，不应跳过
}

uintmax_t FileSystemScanner::scan_recursive(const fs::path& path, 
                                     vector<FileInfo>& result, 
                                     mutex& mtx,
                                     const FileTreeOptions& options,
                                     int depth) {
    if (should_skip_system_folder(path)) return 0;
    if (options.max_depth >= 0 && depth > options.max_depth) return 0;

    uintmax_t total_size = 0;
    size_t dir_index = (size_t)-1;

    try {
        if (depth > 0) {
            FileInfo dir_info;
            dir_info.name = Utils::ToUtf8(path.filename().wstring());
            dir_info.path = Utils::ToUtf8(path.wstring());
            dir_info.is_directory = true;
            dir_info.size = 0;
            dir_info.last_modified = fs::last_write_time(path);
            dir_info.depth = depth;
            
            lock_guard<mutex> lock(mtx);
            dir_index = result.size();
            result.push_back(dir_info);
        }

        vector<fs::directory_entry> entries;
        for (const auto& entry : fs::directory_iterator(path, fs::directory_options::skip_permission_denied)) {
            if (!should_exclude(entry.path(), options.exclude_patterns)) {
                entries.push_back(entry);
            }
        }

        sort(entries.begin(), entries.end(), [](const fs::directory_entry& a, const fs::directory_entry& b) {
            if (a.is_directory() != b.is_directory()) {
                return a.is_directory() > b.is_directory();
            }
            return a.path().filename() < b.path().filename();
        });

        if (depth < 2) { // Top 2-3 levels use parallelism
            vector<future<uintmax_t>> futures;
            for (const auto& entry : entries) {
                if (entry.is_directory()) {
                    futures.push_back(async(launch::async, &FileSystemScanner::scan_recursive, 
                                          entry.path(), ref(result), ref(mtx), ref(options), depth + 1));
                } else {
                    FileInfo info;
                    info.name = Utils::ToUtf8(entry.path().filename().wstring());
                    info.path = Utils::ToUtf8(entry.path().wstring());
                    info.is_directory = false;
                    info.depth = depth + 1;
                    info.last_modified = fs::last_write_time(entry.path());
                    info.size = entry.file_size();
                    
                    lock_guard<mutex> lock(mtx);
                    result.push_back(info);
                    total_size += info.size;
                }
            }
            for (auto& f : futures) {
                total_size += f.get();
            }
        } else {
            for (const auto& entry : entries) {
                if (entry.is_directory()) {
                    total_size += scan_recursive(entry.path(), result, mtx, options, depth + 1);
                } else {
                    FileInfo info;
                    info.name = Utils::ToUtf8(entry.path().filename().wstring());
                    info.path = Utils::ToUtf8(entry.path().wstring());
                    info.is_directory = false;
                    info.depth = depth + 1;
                    info.last_modified = fs::last_write_time(entry.path());
                    info.size = entry.file_size();
                    
                    lock_guard<mutex> lock(mtx);
                    result.push_back(info);
                    total_size += info.size;
                }
            }
        }
        
        if (dir_index != (size_t)-1) {
            lock_guard<mutex> lock(mtx);
            if (dir_index < result.size()) {
                result[dir_index].size = total_size;
            }
        }

    } catch (...) {}
    
    return total_size;
}

string FileSystemScanner::generate_tree_text(const vector<FileInfo>& files, const FileTreeOptions& options) {
    ostringstream ss;
    if (files.empty()) return "No files found.";
    
    vector<bool> is_last_at_depth(256, false);
    for (size_t i = 0; i < files.size(); ++i) {
        const auto& file = files[i];
        if (file.depth == 0) continue; 

        for (int d = 1; d < file.depth; ++d) ss << (is_last_at_depth[d] ? "    " : "│   ");
        ss << (file.is_directory ? "📁 " : "📄 ") << file.name;
        if (options.show_size && !file.is_directory) ss << " (" << format_file_size(file.size, options.human_readable) << ")";
        ss << "\n";
    }
    return ss.str();
}

uintmax_t FileSystemScanner::calculate_directory_size(const fs::path& path) {
    uintmax_t size = 0;
    try {
        for (const auto& entry : fs::recursive_directory_iterator(path, fs::directory_options::skip_permission_denied)) {
            if (!fs::is_directory(entry.status())) size += fs::file_size(entry);
        }
    } catch (...) {}
    return size;
}

string FileSystemScanner::format_file_size(uintmax_t size, bool human_readable) {
    if (!human_readable) return to_string(size) + " B";
    const char* units[] = {"B", "KB", "MB", "GB", "TB"};
    int i = 0;
    double s = static_cast<double>(size);
    while (s >= 1024 && i < 4) { s /= 1024; i++; }
    ostringstream ss;
    ss << fixed << setprecision(2) << s << " " << units[i];
    return ss.str();
}

bool FileSystemScanner::is_path_safe(const fs::path& path) {
    try {
        if (!path.is_absolute()) return false;
        string p = Utils::ToUtf8(path.wstring());
        if (p.find("..") != string::npos) return false;
#if defined(_WIN32)
        if (p.length() >= 2 && p[0] == '\\' && p[1] == '\\') return false; 
#endif
    } catch (...) {
        return false;
    }
    return true; 
}

bool FileSystemScanner::should_exclude(const fs::path& path, const vector<string>& patterns) {
    if (patterns.empty()) return false;
    string filename = Utils::ToUtf8(path.filename().wstring());
    for (const auto& pat : patterns) {
        if (filename.find(pat) != string::npos) return true;
    }
    return false;
}