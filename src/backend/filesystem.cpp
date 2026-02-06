#include "filesystem.hpp"
#include "utils.hpp" // 引入公共工具
#include <iostream>
#include <sstream>
#include <iomanip>
#include <regex>
#include <algorithm>

using namespace std;

// scan_directory 实现
vector<FileInfo> FileSystemScanner::scan_directory(const string& path, const FileTreeOptions& options) {
    vector<FileInfo> result;
    
    if (!is_path_safe(path)) {
        cerr << "[Scanner] Unsafe path: " << path << endl;
        return result;
    }
    
    try {
        fs::path root_path = Utils::ToWString(path); // 使用 Utils 进行转换

        if (!fs::exists(root_path) || !fs::is_directory(root_path)) {
            return result;
        }
        
        scan_recursive(root_path, result, options, 0);
    } catch (const exception& e) {
        cerr << "[Scanner] Error: " << e.what() << endl;
    }
    
    return result;
}


uintmax_t FileSystemScanner::scan_recursive(const fs::path& path, 
                                     vector<FileInfo>& result, 
                                     const FileTreeOptions& options,
                                     int depth) {
    uintmax_t total_size = 0;
    if (options.max_depth >= 0 && depth > options.max_depth) return 0;

    size_t dir_index = (size_t)-1;

    try {
        // 添加当前目录本身 (深度 > 0)
        if (depth > 0) {
            FileInfo dir_info;
            dir_info.name = Utils::ToUtf8(path.filename().wstring()); // 使用 Utils 转回 UTF-8
            dir_info.path = Utils::ToUtf8(path.wstring());
            dir_info.is_directory = true;
            dir_info.size = 0; // 稍后更新
            dir_info.last_modified = fs::last_write_time(path);
            dir_info.depth = depth;
            
            dir_index = result.size();
            result.push_back(dir_info);
        }

        // 获取并排序子项
        vector<fs::directory_entry> entries;
        for (const auto& entry : fs::directory_iterator(path)) {
            if (!should_exclude(entry.path(), options.exclude_patterns)) {
                entries.push_back(entry);
            }
        }

        // 排序：目录优先，再按名称
        sort(entries.begin(), entries.end(), [](const fs::directory_entry& a, const fs::directory_entry& b) {
            if (a.is_directory() != b.is_directory()) {
                return a.is_directory() > b.is_directory();
            }
            return a.path().filename() < b.path().filename();
        });

        // 遍历处理
        for (const auto& entry : entries) {
            if (entry.is_directory()) {
                total_size += scan_recursive(entry.path(), result, options, depth + 1);
            } else {
                FileInfo info;
                info.name = Utils::ToUtf8(entry.path().filename().wstring());
                info.path = Utils::ToUtf8(entry.path().wstring());
                info.is_directory = false;
                info.depth = depth + 1;
                info.last_modified = fs::last_write_time(entry.path());
                info.size = entry.file_size();
                result.push_back(info);
                
                total_size += info.size;
            }
        }
        
        // 更新目录大小
        if (dir_index != (size_t)-1 && dir_index < result.size()) {
            result[dir_index].size = total_size;
        }

    } catch (const exception& e) {
        // 忽略单个文件访问错误，继续扫描
    }
    
    return total_size;
}

// ... 这里的 generate_tree_text, format_file_size 等函数逻辑保持不变，可以直接复用原代码 ...
// 唯一需要注意的是如果这些函数内部有手动 UTF-8 转换，请替换为 Utils::ToUtf8

// generate_tree_text 示例实现 (简化版，确保能用)
string FileSystemScanner::generate_tree_text(const vector<FileInfo>& files, const FileTreeOptions& options) {
    ostringstream ss;
    if (files.empty()) return "No files found.";
    
    // 这里需要一个简单的逻辑来绘制树，为了节省篇幅复用你之前的逻辑即可
    // 关键是 files 里的 name 已经是 UTF-8 了，直接输出即可
    
    vector<bool> is_last_at_depth(256, false);
    for (size_t i = 0; i < files.size(); ++i) {
        const auto& file = files[i];
        if (file.depth == 0) continue; 

        // ... (复用你原有的绘制逻辑) ...
        // 简易版示例：
        for (int d = 1; d < file.depth; ++d) ss << (is_last_at_depth[d] ? "    " : "│   ");
        ss << (file.is_directory ? "📁 " : "📄 ") << file.name;
        if (options.show_size && !file.is_directory) ss << " (" << format_file_size(file.size, options.human_readable) << ")";
        ss << "\n";
    }
    return ss.str();
}

// ... 其他辅助函数实现保持不变 ...
uintmax_t FileSystemScanner::calculate_directory_size(const fs::path& path) {
    uintmax_t size = 0;
    for (const auto& entry : fs::recursive_directory_iterator(path)) {
        if (!fs::is_directory(entry.status())) size += fs::file_size(entry);
    }
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
        // 1. 必须是绝对路径
        if (!path.is_absolute()) {
            return false;
        }

        // 2. 检查路径是否存在 (如果是扫描)
        // 3. 这里的逻辑可以根据需求扩展，例如禁止访问某些系统目录
        string p = Utils::ToUtf8(path.wstring());
        
        // 禁止包含 .. 的路径 (防止目录穿越)
        if (p.find("..") != string::npos) return false;

#if defined(_WIN32)
        // 禁止 UNC 路径 (网络共享)，确保仅限本地磁盘 (A-Z:)
        if (p.length() >= 2 && p[0] == '\\' && p[1] == '\\') {
            return false; 
        }
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