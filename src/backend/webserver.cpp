#if defined(_WIN32)
    #ifndef _WIN32_WINNT
        #define _WIN32_WINNT 0x0A00 
    #endif
#endif


#include "FileEcho/webserver.hpp"
#include "FileEcho/utils.hpp"     // 使用新工具
#include "FileEcho/pdf_extractor.hpp"
#include "FileEcho/doc_extractor.hpp"
#include "FileEcho/frontend_resources.hpp"
#ifdef _WIN32
#include <shellapi.h>
#include <shobjidl.h>
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
    // 初始化 AI 处理器
    ai_handler_ = std::make_unique<AI::AIHandler>();
    ai_handler_->Initialize();

    // 设置 AI 配置文件路径
    char* appdata = getenv("APPDATA");
    if (appdata) {
        ai_config_path_ = std::string(appdata) + "\\FileEcho\\ai_config.json";
        // 确保目录存在
        fs::create_directories(std::string(appdata) + "\\FileEcho");
        // 尝试加载已保存的配置
        ai_handler_->LoadConfig(ai_config_path_);
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
    // server_->set_base_dir("./frontend");
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
    server_->Get("/", [](const httplib::Request&, httplib::Response& res) {
        res.set_content(reinterpret_cast<const char*>(index_html), index_html_len, "text/html");
    });

    server_->Get("/index.html", [](const httplib::Request&, httplib::Response& res) {
        res.set_content(reinterpret_cast<const char*>(index_html), index_html_len, "text/html");
    });

    server_->Get("/script.js", [](const httplib::Request&, httplib::Response& res) {
        res.set_content(reinterpret_cast<const char*>(script_js), script_js_len, "application/javascript");
    });

    server_->Get("/style.css", [](const httplib::Request&, httplib::Response& res) {
        res.set_content(reinterpret_cast<const char*>(style_css), style_css_len, "text/css");
    });
    
    server_->Get("/ai_addon.js", [](const httplib::Request&, httplib::Response& res) {
        res.set_content(reinterpret_cast<const char*>(ai_addon_js), ai_addon_js_len, "application/javascript");
    });

    server_->Get("/ai_addon.css", [](const httplib::Request&, httplib::Response& res) {
        res.set_content(reinterpret_cast<const char*>(ai_addon_css), ai_addon_css_len, "text/css");
    });

    server_->Get("/logo.ico", [](const httplib::Request&, httplib::Response& res) {
        res.set_content(std::string(reinterpret_cast<const char*>(logo_ico), logo_ico_len), "image/x-icon");
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

    server_->Get("/api/pick-folder", [this](const httplib::Request& req, httplib::Response& res) {
        handle_pick_folder(req, res);
    });
    
    // File Operations
    server_->Post("/api/file/read", [this](const httplib::Request& req, httplib::Response& res) {
        handle_file_read(req, res);
    });

    // AI 相关的路由
    server_->Get("/api/ai/models", [this](const httplib::Request& req, httplib::Response& res) {
        handle_ai_models(req, res);
    });

    server_->Get("/api/ai/config", [this](const httplib::Request& req, httplib::Response& res) {
        handle_ai_config_get(req, res);
    });

    server_->Post("/api/ai/config", [this](const httplib::Request& req, httplib::Response& res) {
        handle_ai_config_set(req, res);
    });

    server_->Delete("/api/ai/config", [this](const httplib::Request& req, httplib::Response& res) {
        handle_ai_config_reset(req, res);
    });

    server_->Post("/api/ai/test-connection", [this](const httplib::Request& req, httplib::Response& res) {
        handle_ai_test_connection(req, res);
    });

    server_->Post("/api/ai/chat", [this](const httplib::Request& req, httplib::Response& res) {
        handle_ai_chat(req, res);
    });

    server_->Post("/api/ai/file-summary", [this](const httplib::Request& req, httplib::Response& res) {
        handle_ai_file_summary(req, res);
    });

    server_->Post("/api/ai/project-summary", [this](const httplib::Request& req, httplib::Response& res) {
        handle_ai_project_summary(req, res);
    });

    server_->Post("/api/ai/cleanup-suggestions", [this](const httplib::Request& req, httplib::Response& res) {
        handle_ai_cleanup_suggestions(req, res);
    });

    server_->Post("/api/ai/code-analysis", [this](const httplib::Request& req, httplib::Response& res) {
        handle_ai_code_analysis(req, res);
    });

    server_->Post("/api/ai/tree-annotations", [this](const httplib::Request& req, httplib::Response& res) {
        handle_ai_tree_annotations(req, res);
    });
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

        // 4. 更新缓存（线程安全）
        {
            std::lock_guard<std::mutex> lock(scan_mutex_);
            current_scan_.path = path_utf8;
            current_scan_.files = files;
            current_scan_.options = options;
        }

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
    std::lock_guard<std::mutex> lock(scan_mutex_);
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
    std::lock_guard<std::mutex> lock(scan_mutex_);
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
        {"version", "1.0.5"},
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

void WebServer::handle_pick_folder(const httplib::Request& req, httplib::Response& res) {
#ifndef _WIN32
    res.set_content(json{{"success", false}, {"message", "Not supported on this platform"}}.dump(), "application/json");
#else
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (FAILED(hr)) {
        res.set_content(json{{"success", false}, {"message", "COM initialization failed"}}.dump(), "application/json");
        return;
    }

    IFileOpenDialog* pFileOpen;
    hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

    if (SUCCEEDED(hr)) {
        DWORD dwOptions;
        if (SUCCEEDED(pFileOpen->GetOptions(&dwOptions))) {
            pFileOpen->SetOptions(dwOptions | FOS_PICKFOLDERS);
        }

        hr = pFileOpen->Show(NULL);

        if (SUCCEEDED(hr)) {
            IShellItem* pItem;
            hr = pFileOpen->GetResult(&pItem);
            if (SUCCEEDED(hr)) {
                PWSTR pszFilePath;
                hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

                if (SUCCEEDED(hr)) {
                    string path = Utils::ToUtf8(pszFilePath);
                    CoTaskMemFree(pszFilePath);
                    res.set_content(json{{"success", true}, {"path", path}}.dump(), "application/json");
                }
                pItem->Release();
            }
        } else {
            res.set_content(json{{"success", false}, {"message", "User cancelled or dialog error"}}.dump(), "application/json");
        }
        pFileOpen->Release();
    } else {
        res.set_content(json{{"success", false}, {"message", "Failed to create dialog"}}.dump(), "application/json");
    }

    CoUninitialize();
#endif
}

void WebServer::handle_file_read(const httplib::Request& req, httplib::Response& res) {
    try {
        auto j = json::parse(req.body);
        string file_path = j.value("path", "");
        
        // max_size: 0 = unlimited, default 100KB
        int max_size = j.value("max_size", 100000);
        
        if (file_path.empty()) {
            res.set_content(json{{"success", false}, {"message", "Path required"}}.dump(), "application/json");
            return;
        }

        if (!fs::exists(file_path)) {
            res.set_content(json{{"success", false}, {"message", "File not found"}}.dump(), "application/json");
            return;
        }
        
        if (!fs::is_regular_file(file_path)) {
            res.set_content(json{{"success", false}, {"message", "Not a regular file"}}.dump(), "application/json");
            return;
        }

        // Check if PDF file
        if (PDF::IsPdfFile(file_path)) {
            size_t pdfMaxChars = max_size > 0 ? (size_t)max_size : 0;
            std::string pdfText = PDF::ExtractText(file_path, pdfMaxChars);
            auto fileSize = fs::file_size(file_path);
            res.set_content(json{
                {"success", true},
                {"content", pdfText},
                {"size", fileSize},
                {"truncated", false},
                {"is_binary", false},
                {"is_pdf", true}
            }.dump(), "application/json");
            return;
        }

        // Check if Office document (DOCX, XLSX, PPTX)
        if (DocExtractor::IsOfficeFile(file_path)) {
            size_t docMaxChars = max_size > 0 ? (size_t)max_size : 0;
            std::string docText = DocExtractor::ExtractText(file_path, docMaxChars);
            auto fileSize = fs::file_size(file_path);
            res.set_content(json{
                {"success", true},
                {"content", docText},
                {"size", fileSize},
                {"truncated", false},
                {"is_binary", false},
                {"is_office", true}
            }.dump(), "application/json");
            return;
        }
        
        std::ifstream file(file_path, std::ios::binary);
        if (!file.is_open()) {
            res.set_content(json{{"success", false}, {"message", "Cannot open file"}}.dump(), "application/json");
            return;
        }

        file.seekg(0, std::ios::end);
        size_t file_size = file.tellg();
        file.seekg(0, std::ios::beg);
        
        bool truncated = false;
        size_t read_size = file_size;
        
        if (max_size > 0 && file_size > (size_t)max_size) {
            read_size = max_size;
            truncated = true;
        }
        
        std::string content(read_size, '\0');
        file.read(&content[0], read_size);
        
        bool is_binary = false;
        for (char c : content) {
            if (c == '\0') {
                is_binary = true;
                break;
            }
        }
        
        if (is_binary) {
             res.set_content(json{
                {"success", true}, 
                {"content", "[Binary File]"}, 
                {"size", file_size},
                {"truncated", false},
                {"is_binary", true}
            }.dump(), "application/json");
        } else {
            res.set_content(json{
                {"success", true}, 
                {"content", content}, 
                {"size", file_size},
                {"truncated", truncated},
                {"is_binary", false}
            }.dump(), "application/json");
        }
        
    } catch (const exception& e) {
        res.set_content(json{{"success", false}, {"message", e.what()}}.dump(), "application/json");
    }
}


void WebServer::handle_ai_models(const httplib::Request& req, httplib::Response& res) {
    try {
        auto models = AI::AIHandler::GetSupportedModels();

        json response = {
            {"success", true},
            {"models", json::array()}
        };

        for (const auto& model : models) {
            response["models"].push_back({
                {"provider", static_cast<int>(model.provider)},
                {"name", model.name},
                {"model_id", model.model_id},
                {"base_url", model.base_url},
                {"temperature", model.temperature},
                {"max_tokens", model.max_tokens},
                {"enabled", model.enabled}
            });
        }

        res.set_content(response.dump(), "application/json");
    } catch (const exception& e) {
        res.set_content(json{{"success", false}, {"message", e.what()}}.dump(), "application/json");
    }
}

void WebServer::handle_ai_config_get(const httplib::Request& req, httplib::Response& res) {
    try {
        auto config = ai_handler_->GetCurrentConfig();

        json response = {
            {"success", true},
            {"config", {
                {"provider", static_cast<int>(config.provider)},
                {"name", config.name},
                {"model_id", config.model_id},
                {"base_url", config.base_url},
                {"temperature", config.temperature},
                {"max_tokens", config.max_tokens},
                {"enabled", config.enabled},
                {"has_api_key", !config.api_key.empty()},
                {"max_content_chars", config.max_content_chars},
                {"max_file_count", config.max_file_count}
            }}
        };

        res.set_content(response.dump(), "application/json");
    } catch (const exception& e) {
        res.set_content(json{{"success", false}, {"message", e.what()}}.dump(), "application/json");
    }
}

void WebServer::handle_ai_config_set(const httplib::Request& req, httplib::Response& res) {
    try {
        auto j = json::parse(req.body);

        AI::AIModelConfig config = ai_handler_->GetCurrentConfig();
        config.provider = static_cast<AI::AIProvider>(j.value("provider", static_cast<int>(config.provider)));
        config.name = j.value("name", config.name);
        // Only update api_key if provided (non-empty), preserve existing key otherwise
        if (j.contains("api_key") && !j["api_key"].get<std::string>().empty()) {
            config.api_key = j["api_key"].get<std::string>();
        }
        config.base_url = j.value("base_url", config.base_url);
        config.model_id = j.value("model_id", config.model_id);
        config.temperature = j.value("temperature", config.temperature);
        config.max_tokens = j.value("max_tokens", config.max_tokens);
        config.enabled = j.value("enabled", config.enabled);
        config.max_content_chars = j.value("max_content_chars", config.max_content_chars);
        config.max_file_count = j.value("max_file_count", config.max_file_count);

        if (ai_handler_->SetModelConfig(config)) {
            // 保存配置到文件
            ai_handler_->SaveConfig(ai_config_path_);

            res.set_content(json{{"success", true}, {"message", "Configuration saved successfully"}}.dump(), "application/json");
        } else {
            res.set_content(json{{"success", false}, {"message", "Failed to set configuration"}}.dump(), "application/json");
        }
    } catch (const exception& e) {
        res.set_content(json{{"success", false}, {"message", e.what()}}.dump(), "application/json");
    }
}

void WebServer::handle_ai_config_reset(const httplib::Request& req, httplib::Response& res) {
    try {
        // Delete the config file
        if (fs::exists(ai_config_path_)) {
            fs::remove(ai_config_path_);
        }

        // Reset in-memory config to defaults
        AI::AIModelConfig empty_config;
        empty_config.provider = AI::AIProvider::OPENAI;
        empty_config.name = "";
        empty_config.api_key = "";
        empty_config.base_url = "";
        empty_config.model_id = "";
        empty_config.temperature = 0.7;
        empty_config.max_tokens = 4096;
        empty_config.enabled = true;
        empty_config.max_content_chars = 0;
        empty_config.max_file_count = 0;
        ai_handler_->SetModelConfig(empty_config);

        res.set_content(json{{"success", true}, {"message", "Configuration reset"}}.dump(), "application/json");
    } catch (const exception& e) {
        res.set_content(json{{"success", false}, {"message", e.what()}}.dump(), "application/json");
    }
}

void WebServer::handle_ai_test_connection(const httplib::Request& req, httplib::Response& res) {
    try {
        auto result = ai_handler_->TestConnection();
        json response = {
            {"success", result.success},
            {"message", result.success ? "Connection successful" : result.error}
        };
        res.set_content(response.dump(), "application/json");
    } catch (const exception& e) {
        res.set_content(json{{"success", false}, {"message", e.what()}}.dump(), "application/json");
    }
}

void WebServer::handle_ai_chat(const httplib::Request& req, httplib::Response& res) {
    try {
        auto j = json::parse(req.body);
        string message = j.value("message", "");
        string language = j.value("language", "en");
        json context = j.value("context", json::object());

        if (message.empty()) {
            res.set_content(json{{"success", false}, {"message", "Message is required"}}.dump(), "application/json");
            return;
        }

        auto response = ai_handler_->GenerateChatResponse(message, context, language);

        json result = {
            {"success", response.success},
            {"content", response.content},
            {"prompt_tokens", response.prompt_tokens},
            {"completion_tokens", response.completion_tokens},
            {"total_tokens", response.total_tokens}
        };

        if (!response.success) {
            result["error"] = response.error;
        }

        res.set_content(result.dump(), "application/json");
    } catch (const exception& e) {
        res.set_content(json{{"success", false}, {"message", e.what()}}.dump(), "application/json");
    }
}

void WebServer::handle_ai_file_summary(const httplib::Request& req, httplib::Response& res) {
    try {
        auto j = json::parse(req.body);
        string file_path = j.value("path", "");
        string content = j.value("content", "");
        string language = j.value("language", "en");
        bool brief = j.value("brief", false);
        
        // If content is not provided, try to read file
        if (content.empty() && !file_path.empty() && fs::exists(file_path)) {
            // Use smart content reading: handle PDF, DOCX, XLSX, PPTX
            if (PDF::IsPdfFile(file_path)) {
                content = PDF::ExtractText(file_path, 0);
            } else if (DocExtractor::IsOfficeFile(file_path)) {
                content = DocExtractor::ExtractText(file_path, 0);
            } else {
                std::ifstream file(file_path, std::ios::binary);
                if (file.is_open()) {
                    // Read file and check if binary
                    std::string raw((std::istreambuf_iterator<char>(file)),
                                     std::istreambuf_iterator<char>());
                    bool is_binary = false;
                    for (char c : raw) {
                        if (c == '\0') { is_binary = true; break; }
                    }
                    content = is_binary ? "[Binary File]" : raw;
                }
            }
        }

        auto response = ai_handler_->GenerateFileSummary(fs::path(file_path).filename().string(), file_path, content, language, brief);
        
        json result = {
            {"success", response.success},
            {"content", response.content},
            {"prompt_tokens", response.prompt_tokens},
            {"completion_tokens", response.completion_tokens},
            {"total_tokens", response.total_tokens}
        };
        
        if (!response.success) {
            result["error"] = response.error;
        }
        
        res.set_content(result.dump(), "application/json");
    } catch (const exception& e) {
        res.set_content(json{{"success", false}, {"message", e.what()}}.dump(), "application/json");
    }
}

void WebServer::handle_ai_project_summary(const httplib::Request& req, httplib::Response& res) {
    try {
        auto j = json::parse(req.body);
        string path = j.value("path", "");
        string language = j.value("language", "en");
        string file_tree = j.value("file_tree", "");
        
        auto response = ai_handler_->GenerateProjectSummary(path, file_tree, language);
        
        json result = {
            {"success", response.success},
            {"content", response.content},
            {"prompt_tokens", response.prompt_tokens},
            {"completion_tokens", response.completion_tokens},
            {"total_tokens", response.total_tokens}
        };
        
        if (!response.success) {
            result["error"] = response.error;
        }
        
        res.set_content(result.dump(), "application/json");
    } catch (const exception& e) {
        res.set_content(json{{"success", false}, {"message", e.what()}}.dump(), "application/json");
    }
}

void WebServer::handle_ai_cleanup_suggestions(const httplib::Request& req, httplib::Response& res) {
    try {
        auto j = json::parse(req.body);
        string path = j.value("path", "");
        string language = j.value("language", "en");
        string file_tree = j.value("file_tree", "");

        auto response = ai_handler_->GenerateCleanupSuggestions(path, file_tree, language);
        
        json result = {
            {"success", response.success},
            {"content", response.content},
            {"prompt_tokens", response.prompt_tokens},
            {"completion_tokens", response.completion_tokens},
            {"total_tokens", response.total_tokens}
        };
        
        if (!response.success) {
            result["error"] = response.error;
        }
        
        res.set_content(result.dump(), "application/json");
    } catch (const exception& e) {
        res.set_content(json{{"success", false}, {"message", e.what()}}.dump(), "application/json");
    }
}

void WebServer::handle_ai_code_analysis(const httplib::Request& req, httplib::Response& res) {
    try {
        auto j = json::parse(req.body);
        string path = j.value("path", "");
        string language = j.value("language", "en");
        string file_tree = j.value("file_tree", "");

        auto response = ai_handler_->GenerateCodeAnalysis(path, file_tree, language);
        
        json result = {
            {"success", response.success},
            {"content", response.content},
            {"prompt_tokens", response.prompt_tokens},
            {"completion_tokens", response.completion_tokens},
            {"total_tokens", response.total_tokens}
        };
        
        if (!response.success) {
            result["error"] = response.error;
        }
        
        res.set_content(result.dump(), "application/json");
    } catch (const exception& e) {
        res.set_content(json{{"success", false}, {"message", e.what()}}.dump(), "application/json");
    }
}

void WebServer::handle_ai_tree_annotations(const httplib::Request& req, httplib::Response& res) {
    try {
        auto j = json::parse(req.body);
        string path = j.value("path", "");
        string language = j.value("language", "en");
        string file_tree = j.value("file_tree", "");

        auto response = ai_handler_->GenerateTreeAnnotations(path, file_tree, language);
        
        json result = {
            {"success", response.success},
            {"content", response.content},
            {"prompt_tokens", response.prompt_tokens},
            {"completion_tokens", response.completion_tokens},
            {"total_tokens", response.total_tokens}
        };
        
        if (!response.success) {
            result["error"] = response.error;
        }
        
        res.set_content(result.dump(), "application/json");
    } catch (const exception& e) {
        res.set_content(json{{"success", false}, {"message", e.what()}}.dump(), "application/json");
    }
}
