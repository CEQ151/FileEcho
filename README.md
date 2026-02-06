# 📡 FileEcho - Scan Deep, Echo Clear

[![C++](https://img.shields.io/badge/Language-C%2B%2B17-blue.svg)](https://en.cppreference.com/w/cpp/17)
[![Framework](https://img.shields.io/badge/Framework-WebView2-green.svg)](https://developer.microsoft.com/en-us/microsoft-edge/webview2/)
[![License](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

**FileEcho** is a lightweight, high-performance directory analysis and file cloning tool designed for developers and geeks. It features a high-performance backend written in C++17 and a modern frontend powered by WebView2, achieving a "what you see is what you get" directory management experience.

---

## ✨ Features

- 🚀 **High-Speed Scanning**: Powered by C++ performance, it processes complex directories with tens of thousands of files in an instant.
- 📂 **Structure Preview**: Generates beautiful ASCII tree diagrams with depth control and exclusion rules.
- ⚡ **Instant Echo (Cloning)**: Uses a "path reference" architecture for local file/folder cloning, achieving disk-level speeds without network overhead.
- 🎨 **Modern UI**: Dark/Light mode support with smooth animations and responsive design.
- 🔒 **Security First**: Strict path safety checks to prevent directory traversal and block non-local UNC paths.

## 🛠️ Tech Stack

- **Backend**: C++17, [cpp-httplib](https://github.com/yhirose/cpp-httplib), [nlohmann/json](https://github.com/nlohmann/json)
- **GUI**: [webview/webview](https://github.com/webview/webview) (based on Microsoft Edge WebView2)
- **Frontend**: HTML5, CSS3 (Inter font, FontAwesome icons), JavaScript (Vanilla JS)

## 🚀 Build & Run

### Prerequisites
- Windows 10/11
- CMake 3.15+
- MinGW-w64 (v13.1.0+ recommended) or MSVC
- Microsoft Edge WebView2 Runtime

### Build Steps
1. **Clone the repository**:
   ```bash
   git clone https://github.com/your-repo/FileEcho.git
   cd FileEcho
   ```

2. **Generate build configuration**:
   ```bash
   mkdir build
   cd build
   cmake ..
   ```

3. **Compile**:
   ```bash
   cmake --build .
   ```

### ⚠️ Read Before Running (Important!)
After compilation, `FileEcho.exe` will be generated in the `build` directory. **Running it directly will result in a blank window** because the backend needs to load static frontend resources.

**Please follow these steps exactly:**
1. Locate the `frontend/` folder in the project root.
2. **Copy** the entire `frontend/` folder into the `build/` directory (same level as `FileEcho.exe`).
3. Now run `FileEcho.exe` to see the full graphical interface.

The directory structure should look like this:
```text
build/
├── frontend/        <-- MUST be manually copied here
│   ├── index.html
│   ├── script.js
│   └── style.css
└── FileEcho.exe
```

## 📖 Usage Guide

1. **Set Scan Path**: Enter an absolute path in "Target Directory Path" and click `Refresh Tree`.
2. **Filter Rules**: Configure `Max Depth` or `Exclude Patterns` (e.g., `node_modules`) in the settings panel.

## 📄 License

This project is licensed under the [MIT License](LICENSE).

---
*Powered by FileEcho - Make file management as clear and crisp as an echo.*
