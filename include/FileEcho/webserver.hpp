#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#endif

#pragma once

#include "FileEcho/filesystem.hpp"
#include "FileEcho/ai_handler.hpp"
#include "httplib.h"
#include "json.hpp" // 引入 nlohmann/json
#include <string>
#include <memory>
#include <thread>
#include <atomic>
#include <vector>
#include <mutex>

class WebServer {
public:
    WebServer();
    ~WebServer();
    
    // 启动HTTP服务器
    bool start(int port = 8080);
    
    // 停止HTTP服务器
    void stop();
    
    bool is_running() const { return running_; }
    int get_port() const { return port_; }
    
private:
    // 设置路由
    void setup_routes();
    
    // HTTP请求处理函数
    void handle_scan(const httplib::Request& req, httplib::Response& res);
    void handle_tree(const httplib::Request& req, httplib::Response& res);
    void handle_download(const httplib::Request& req, httplib::Response& res);
    void handle_api_info(const httplib::Request& req, httplib::Response& res);
    void handle_open(const httplib::Request& req, httplib::Response& res);
    void handle_pick_folder(const httplib::Request& req, httplib::Response& res);

    // AI 相关的请求处理函数
    void handle_ai_models(const httplib::Request& req, httplib::Response& res);
    void handle_ai_config_get(const httplib::Request& req, httplib::Response& res);
    void handle_ai_config_set(const httplib::Request& req, httplib::Response& res);
    void handle_ai_config_reset(const httplib::Request& req, httplib::Response& res);
    void handle_ai_test_connection(const httplib::Request& req, httplib::Response& res);
    void handle_ai_chat(const httplib::Request& req, httplib::Response& res);
    void handle_ai_file_summary(const httplib::Request& req, httplib::Response& res);
    void handle_ai_project_summary(const httplib::Request& req, httplib::Response& res);
    void handle_ai_cleanup_suggestions(const httplib::Request& req, httplib::Response& res);
    void handle_ai_code_analysis(const httplib::Request& req, httplib::Response& res);
    
    // File Operations
    void handle_file_read(const httplib::Request& req, httplib::Response& res);

    // 服务器实例
    std::unique_ptr<httplib::Server> server_;
    std::unique_ptr<std::thread> server_thread_;
    
    // 服务器状态
    std::atomic<bool> running_{false};
    int port_{8080};
    
    // 当前扫描的缓存信息
    std::mutex scan_mutex_;
    struct {
        std::string path;
        std::vector<FileInfo> files;
        FileTreeOptions options;
    } current_scan_;

    // AI 处理器
    std::unique_ptr<AI::AIHandler> ai_handler_;
    std::string ai_config_path_;
};