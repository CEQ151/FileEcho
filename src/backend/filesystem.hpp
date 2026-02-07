#pragma once

#include <string>
#include <vector>
#include <filesystem>
#include <chrono>
#include <future>
#include <mutex>

namespace fs = std::filesystem;

/**
 * 文件信息结构体
 * 用于存储扫描到的文件/目录元数据
 */
struct FileInfo {
    std::string name;             // 文件名 (UTF-8)
    std::string path;             // 完整路径 (UTF-8)
    bool is_directory;            // 是否为目录
    uintmax_t size;               // 大小 (字节)
    fs::file_time_type last_modified; // 最后修改时间
    int depth;                    // 在树中的深度
};

/**
 * 扫描选项结构体
 */
struct FileTreeOptions {
    bool show_size = false;       // 是否在树中显示文件大小
    bool human_readable = true;   // 大小是否格式化 (KB, MB)
    int max_depth = -1;           // 最大扫描深度 (-1 表示无限制)
    std::vector<std::string> exclude_patterns; // 需要排除的文件名关键字或正则
};

/**
 * 文件系统扫描器类
 * 提供多线程高性能扫描
 */
class FileSystemScanner {
public:
    static std::vector<FileInfo> scan_directory(const std::string& path, 
                                               const FileTreeOptions& options = {});
    
    static std::string generate_tree_text(const std::vector<FileInfo>& files, 
                                        const FileTreeOptions& options = {});
    
    static uintmax_t calculate_directory_size(const fs::path& path);
    
    static std::string format_file_size(uintmax_t size, bool human_readable = true);
    
    static bool is_path_safe(const fs::path& path);

private:
    static uintmax_t scan_recursive(const fs::path& path, 
                             std::vector<FileInfo>& result, 
                             std::mutex& mtx,
                             const FileTreeOptions& options,
                             int depth);
    
    static bool should_exclude(const fs::path& path, 
                             const std::vector<std::string>& patterns);

    static bool should_skip_system_folder(const fs::path& path);
};