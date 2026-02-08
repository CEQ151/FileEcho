

// ==========================================
// 1. 核心宏定义 (必须放在文件最最最开头)
// ==========================================

// 告诉 MinGW 我们要用 Windows 10 的 API (解决 httplib CreateFile2 报错)
#if defined(_WIN32)
    #ifndef _WIN32_WINNT
        #define _WIN32_WINNT 0x0A00 
    #endif
#endif

#define NOMINMAX                // 防止 Windows 宏干扰 std::min/max
#define WEBVIEW_WINAPI          // 【关键】告诉 webview.h 我们在 Windows 上
#define WEBVIEW_IMPLEMENTATION  // 【关键】实现 webview 代码

// ==========================================
// 2. 头文件引入 (顺序很重要)
// ==========================================

#include <winsock2.h>  // 必须在 webview.h (包含 windows.h) 之前引入，解决 httplib 警告
#include "webview.h"   // 核心 GUI 库
#include "webserver.hpp" // 后端服务器

#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <atomic>
#include <csignal>
#include <cstdlib>

// ==========================================
// 3. 主程序逻辑
// ==========================================

// 使用原子变量确保线程安全的退出
std::atomic<bool> g_running(true);

void signal_handler(int signal) {
    g_running = false;
}

int main(int argc, char* argv[]) {
    // 注册信号处理
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    int port = 8080;
    if (argc > 1) {
        try {
            port = std::stoi(argv[1]);
        } catch (...) {}
    }

    // 1. 初始化 WebServer
    WebServer server;

    // 2. 在后台线程启动 HTTP Server
    std::thread server_thread([&]() {
        std::cout << "[Backend] FileEcho server starting on port " << port << "..." << std::endl;
        if (!server.start(port)) {
            std::cerr << "[Error] Failed to start web server." << std::endl;
            g_running = false;
        }
    });

    // 等待服务器启动
    std::this_thread::sleep_for(std::chrono::milliseconds(300));

    try {
        // 3. 创建 GUI 窗口
        // 【关键修复】现在 WEBVIEW_WINAPI 已定义，webview 类可以正常使用了
        webview::webview w(true, nullptr); 

        w.set_title("FileEcho - Scan Deep, Echo Clear");
        
        // 这些枚举现在应该能被识别了
        w.set_size(1920, 1080, WEBVIEW_HINT_NONE); 

        // ================= 【核心修改：注入图标】 =================
        // 获取窗口的原生 Windows 句柄 (HWND)
        HWND hwnd = (HWND)w.window(); 
        if (hwnd) {
            // 从当前程序 (.exe) 中加载 ID 为 1 的图标资源
            // 系统会自动根据用途从 ico 包里选择 16x16 或 256x256
            HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(1)); 
            if (hIcon) {
                // 设置任务栏的大图标
                SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
                // 设置窗口左上角的小图标
                SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
            }
        }

        std::string url = "http://localhost:" + std::to_string(port);
        w.navigate(url);

        // 阻塞主线程直到窗口关闭
        w.run();

    } catch (const std::exception& e) {
        std::cerr << "[Frontend] WebView Error: " << e.what() << std::endl;
    }

    // 4. 清理退出
    std::cout << "[System] Shutting down..." << std::endl;
    g_running = false;
    server.stop();

    std::exit(0);

    return 0;
}