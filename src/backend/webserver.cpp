#if defined(_WIN32)
    #ifndef _WIN32_WINNT
        #define _WIN32_WINNT 0x0A00 
    #endif
#endif


#include "webserver.hpp"
#include "utils.hpp"     // 使用新工具
#ifdef _WIN32
#include <shellapi.h>
#endif
#include <iostream>
#include <fstream>
#include <iomanip>
#include <filesystem>
#include <algorithm>

using json = nlohmann::json;
using namespace std;
namespace fs = std::filesystem;

WebServer::WebServer() {
    // 创建上传目录
    try {
        if (!fs::exists(upload_dir_)) {
            fs::create_directory(upload_dir_);
        }
    } catch (const exception& e) {
        cerr << "[Server] Failed to create upload directory: " << e.what() << endl;
    }
}

WebServer::~WebServer() {
    stop();
}

bool WebServer::start(int port) {
    if (running_) {
        cerr << "[Server] Already running" << endl;
        return false;
    }
    
    port_ = port;
    server_ = make_unique<httplib::Server>();
    
    setup_routes();
    
    // 设置静态文件目录
    server_->set_base_dir("./frontend");
    server_->set_payload_max_length(100 * 1024 * 1024); // 100MB
    
    server_thread_ = make_unique<thread>([this]() {
        if (!server_->listen("0.0.0.0", port_)) {
            cerr << "[Server] Failed to listen on port " << port_ << endl;
            running_ = false;
        }
    });
    
    // 稍微等待确保线程启动
    this_thread::sleep_for(chrono::milliseconds(100));
    running_ = true;
    return true;
}

void WebServer::stop() {
    if (server_) server_->stop();
    if (server_thread_ && server_thread_->joinable()) server_thread_->join();
    running_ = false;
}

void WebServer::setup_routes() {
    server_->Get("/", [this](const httplib::Request&, httplib::Response& res) {
        handle_root(httplib::Request(), res); // Hack: 简单的重定向不依赖 req
    });
    
    server_->Post("/api/upload", [this](const httplib::Request& req, httplib::Response& res) {
        handle_upload(req, res);
    });
    
    server_->Post("/api/scan", [this](const httplib::Request& req, httplib::Response& res) {
        handle_scan(req, res);
    });
    
    server_->Post("/api/tree", [this](const httplib::Request& req, httplib::Response& res) {
        handle_tree(req, res);
    });
    
    server_->Get("/api/download/tree", [this](const httplib::Request& req, httplib::Response& res) {
        handle_download(req, res);
    });
    
    server_->Get("/api/info", [this](const httplib::Request& req, httplib::Response& res) {
        handle_api_info(req, res);
    });

    server_->Post("/api/open", [this](const httplib::Request& req, httplib::Response& res) {
        handle_open(req, res);
    });
}

void WebServer::handle_root(const httplib::Request&, httplib::Response& res) {
    res.set_redirect("/index.html");
}

void WebServer::handle_upload(const httplib::Request& req, httplib::Response& res) {
    try {
        auto body = json::parse(req.body);
        string source_path_utf8 = body.value("source_path", "");
        string target_path_utf8 = body.value("target_path", "");

        if (source_path_utf8.empty() || target_path_utf8.empty()) {
            res.set_content(json({{"success", false}, {"message", "Missing source_path or target_path"}}).dump(), "application/json");
            return;
        }

        fs::path source_path = Utils::ToWString(source_path_utf8);
        fs::path target_path = Utils::ToWString(target_path_utf8);

        // 使用 FileSystemScanner 进行增强的安全检查
        if (!FileSystemScanner::is_path_safe(source_path) || !FileSystemScanner::is_path_safe(target_path)) {
            res.set_content(json({{"success", false}, {"message", "Security Check Failed: Illegal path format or access denied"}}).dump(), "application/json");
            return;
        }

        // 确保源路径存在
        if (!fs::exists(source_path)) {
            res.set_content(json({{"success", false}, {"message", "Source path does not exist"}}).dump(), "application/json");
            return;
        }

        // 如果目标目录不存在，则创建
        if (!fs::exists(target_path)) {
            fs::create_directories(target_path);
        }

        // 构建最终目标路径 (保持源文件名/目录名)
        fs::path final_target = target_path / source_path.filename();

        // 执行秒传 (本地拷贝)
        if (fs::is_directory(source_path)) {
            // 递归拷贝文件夹
            fs::copy(source_path, final_target, fs::copy_options::recursive | fs::copy_options::overwrite_existing);
        } else {
            // 拷贝单个文件
            fs::copy(source_path, final_target, fs::copy_options::overwrite_existing);
        }

        json response = {
            {"success", true},
            {"message", "Echo completed (Instant path reference clone)"},
            {"target", Utils::ToUtf8(final_target.wstring())}
        };
        res.set_content(response.dump(), "application/json");

    } catch (const fs::filesystem_error& e) {
        res.set_content(json({{"success", false}, {"message", string("System Error: ") + e.what()}}).dump(), "application/json");
    } catch (const exception& e) {
        res.set_content(json({{"success", false}, {"message", e.what()}}).dump(), "application/json");
    }
}

void WebServer::handle_scan(const httplib::Request& req, httplib::Response& res) {
    try {
        // 1. 解析请求 JSON
        auto j = json::parse(req.body);
        string path_utf8 = j.value("path", "");
        
        if (path_utf8.empty()) {
            res.set_content(json({{"success", false}, {"message", "Path required"}}).dump(), "application/json");
            return;
        }

        // 2. 填充选项
        FileTreeOptions options;
        options.show_size = j.value("show_size", false);
        options.max_depth = j.value("max_depth", -1);
        if (j.contains("exclude_patterns")) {
            // 支持数组或单个字符串
            if (j["exclude_patterns"].is_array()) {
                options.exclude_patterns = j["exclude_patterns"].get<vector<string>>();
            } else if (j["exclude_patterns"].is_string()) {
                options.exclude_patterns.push_back(j["exclude_patterns"].get<string>());
            }
        }

        // 3. 执行扫描
        vector<FileInfo> files = FileSystemScanner::scan_directory(path_utf8, options);

        // 4. 更新缓存
        current_scan_.path = path_utf8;
        current_scan_.files = files;
        current_scan_.options = options;

        // 5. 构建响应
        json response;
        response["success"] = true;
        response["path"] = path_utf8;
        response["file_count"] = files.size();
        response["files"] = json::array();

        for (const auto& f : files) {
            response["files"].push_back({
                {"name", f.name},
                {"path", f.path},
                {"is_directory", f.is_directory},
                {"depth", f.depth},
                {"size", f.size},
                {"size_formatted", FileSystemScanner::format_file_size(f.size, options.human_readable)}
            });
        }

        res.set_content(response.dump(), "application/json");

    } catch (const exception& e) {
        res.set_content(json({{"success", false}, {"message", e.what()}}).dump(), "application/json");
    }
}

void WebServer::handle_tree(const httplib::Request& req, httplib::Response& res) {
    if (current_scan_.files.empty()) {
        res.set_content(json({{"success", false}, {"message", "No scan data"}}).dump(), "application/json");
        return;
    }

    try {
        string tree_text = FileSystemScanner::generate_tree_text(current_scan_.files, current_scan_.options);
        
        json response = {
            {"success", true},
            {"path", current_scan_.path},
            {"file_count", current_scan_.files.size()},
            {"tree_text", tree_text}
        };
        
        res.set_content(response.dump(), "application/json");
    } catch (const exception& e) {
        res.set_content(json({{"success", false}, {"message", e.what()}}).dump(), "application/json");
    }
}

void WebServer::handle_download(const httplib::Request& req, httplib::Response& res) {
    if (current_scan_.files.empty()) {
        res.set_content("No data to download", "text/plain");
        return;
    }

    string tree_text = FileSystemScanner::generate_tree_text(current_scan_.files, current_scan_.options);
    
    // 生成带时间戳的文件名
    time_t now = time(nullptr);
    string filename = "file_tree_" + to_string(now) + ".txt";
    
    res.set_header("Content-Type", "text/plain; charset=utf-8");
    res.set_header("Content-Disposition", "attachment; filename=" + filename);
    res.set_content(tree_text, "text/plain");
}

void WebServer::handle_api_info(const httplib::Request& req, httplib::Response& res) {
    json info = {
        {"name", "FileEcho"},
        {"version", "1.0.1"},
        {"status", running_ ? "running" : "stopped"},
        {"port", port_},
        {"endpoints", {
            "/api/scan (POST)", "/api/tree (POST)", "/api/upload (POST)"
        }}
    };
    res.set_content(info.dump(), "application/json");
}

void WebServer::handle_open(const httplib::Request& req, httplib::Response& res) {
    try {
        auto body = json::parse(req.body);
        string path = body.value("path", "");
        
        if (path.empty()) {
            res.set_content(json{{"success", false}, {"message", "Empty path"}}.dump(), "application/json");
            return;
        }

        // 【关键修复 1】: 强制将所有 '/' 替换为 '\'
        // Windows Shell API 严格要求使用反斜杠
        std::replace(path.begin(), path.end(), '/', '\\');

        // 【关键修复 2】: 初始化 COM 组件
        // 后台线程默认没有 COM 环境，必须手动初始化才能调用 ShellExecute
        HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

        wstring wpath = Utils::ToWString(path);
        
        // 调用系统 Shell 打开文件或文件夹
        // SW_SHOWNORMAL 确保窗口正常显示
        HINSTANCE result = ShellExecuteW(NULL, L"open", wpath.c_str(), NULL, NULL, SW_SHOWNORMAL);
        
        // 操作完成后清理 COM
        if (SUCCEEDED(hr)) {
            CoUninitialize();
        }

        // ShellExecute 返回值大于 32 表示成功
        if ((INT_PTR)result > 32) {
            res.set_content(json{{"success", true}}.dump(), "application/json");
        } else {
            // 获取错误代码方便调试
            res.set_content(json{{"success", false}, {"message", "ShellExecute failed code: " + to_string((INT_PTR)result)}}.dump(), "application/json");
        }
    } catch (const exception& e) {
        res.set_content(json{{"success", false}, {"message", e.what()}}.dump(), "application/json");
    }
}