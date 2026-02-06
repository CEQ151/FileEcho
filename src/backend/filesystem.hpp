#pragma once

#include <string>
#include <vector>
#include <filesystem>
#include <chrono>

// 为了方便使用，定义 fs 别名
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
 * 用于控制扫描行为和输出格式
 */
struct FileTreeOptions {
    bool show_size = false;       // 是否在树中显示文件大小
    bool human_readable = true;   // 大小是否格式化 (KB, MB)
    int max_depth = -1;           // 最大扫描深度 (-1 表示无限制)
    std::vector<std::string> exclude_patterns; // 需要排除的文件名关键字或正则
};

/**
 * 文件系统扫描器类
 * 提供静态方法进行目录扫描和文本树生成
 */
class FileSystemScanner {
public:
    // 扫描指定目录并返回文件列表
    static std::vector<FileInfo> scan_directory(const std::string& path, 
                                               const FileTreeOptions& options = {});
    
    // 将文件列表转换为树状文本字符串 (类似 Linux tree 命令输出)
    static std::string generate_tree_text(const std::vector<FileInfo>& files, 
                                        const FileTreeOptions& options = {});
    
    // 计算目录的总大小 (递归)
    static uintmax_t calculate_directory_size(const fs::path& path);
    
    // 将字节大小格式化为人类可读字符串 (B, KB, MB, GB)
    static std::string format_file_size(uintmax_t size, bool human_readable = true);
    
    // 简单的路径安全检查
    static bool is_path_safe(const fs::path& path);
    
private:
    // 内部递归扫描函数
    static uintmax_t scan_recursive(const fs::path& path, 
                             std::vector<FileInfo>& result, 
                             const FileTreeOptions& options,
                             int depth);
    
    // 检查是否应该排除该路径
    static bool should_exclude(const fs::path& path, 
                             const std::vector<std::string>& patterns);
};