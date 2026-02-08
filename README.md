# FileEcho

**FileEcho** is a high-performance, native local file manager and analysis tool for Windows. It combines the raw power of **C++17** with the modern UI capabilities of **WebView2**, featuring a hybrid architecture (Local C++ Backend + Web Frontend).

**FileEcho** 是一个高性能的 Windows 本地文件管理与分析工具。它结合了 **C++17** 的强大性能与 **WebView2** 的现代 UI 能力，采用混合架构（本地 C++ 后端 + Web 前端）构建。

![License](https://img.shields.io/badge/license-MIT-blue.svg)
![Platform](https://img.shields.io/badge/platform-Windows%2010%2F11-blue)
![Compiler](https://img.shields.io/badge/compiler-MinGW--w64-green)

## 🚀 Key Features (核心特性)

-   **⚡ High-Performance Scanning (高性能扫描)**
    -   Utilizes **Multi-threading (`std::async`)** to scan directories in parallel.
    -   利用 **多线程并行 (`std::async`)** 技术实现极速目录扫描。
    -   **Smart Filtering**: Automatically skips system-protected folders (e.g., `C:\Windows`, `$Recycle.Bin`) to prevent hanging and ensure stability.

-   **🔄 Echo (Instant Clone)**
    -   Select a source file/folder and "Echo" it to the target directory instantly using local file system APIs (Zero-latency copying).
    -   **本地秒传**：选中源文件/文件夹，利用本地文件系统 API 将其瞬间“回响”（克隆）到目标目录。

-   **🌲 Interactive Visualization (交互式视图)**
    -   Generate tree-structured text views of your file system.
    -   生成文件系统的树状结构文本视图，支持导出。

-   **🛠️ Native Experience (原生体验)**
    -   **Console-less**: Runs as a background GUI application without a command prompt window.
    -   **Clean Shutdown**: Optimized process management ensures no zombie processes remain after closing.
    -   **无黑窗口**：作为纯 GUI 应用运行。
    -   **彻底退出**：优化的进程管理，关闭窗口即彻底结束后台线程，无残留。

## 🛠️ Tech Stack (技术栈)

* **Language**: C++17
* **Compiler**: MinGW-w64 (GCC)
* **GUI**: [webview](https://github.com/webview/webview) (Edge Chromium)
* **Server**: [cpp-httplib](https://github.com/yhirose/cpp-httplib) (Multi-threaded)
* **JSON**: [nlohmann/json](https://github.com/nlohmann/json)
* **Build System**: CMake 3.15+

## 📦 How to Build (如何构建)

### Prerequisites (前置要求)
* CMake
* MinGW-w64 (GCC)
* Git

### Build Steps (构建步骤)

#### 1. Clone the repository
```bash
git clone https://github.com/CEQ151/FileEcho.git
cd FileEcho
```
#### 2. Create build directory
```bash
mkdir build && cd build
```

#### 3. Configure (MinGW)
```bash
cmake -G "MinGW Makefiles" ..
```

#### 4. Build

```bash
cmake --build .
```

> **Note**: The build process automatically copies the `src/frontend` resources to the build directory. You don't need to move files manually!
> **注意**：构建过程会自动将 `src/frontend` 资源复制到构建目录，无需手动移动文件！

## 🖥️ Usage (使用方法)

1. Run `FileEcho.exe` from the `build` directory.
2. **Scan**: Enter a path (e.g., `D:/Projects`) and click scan to see the file structure.
3. **Echo**: Enter a "Source Path" and click "Echo" to clone it to the current directory.

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](https://www.google.com/search?q=LICENSE) file for details.

