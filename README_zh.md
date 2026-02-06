# 📡 FileEcho - Scan Deep, Echo Clear

[![C++](https://img.shields.io/badge/Language-C%2B%2B17-blue.svg)](https://en.cppreference.com/w/cpp/17)
[![Framework](https://img.shields.io/badge/Framework-WebView2-green.svg)](https://developer.microsoft.com/en-us/microsoft-edge/webview2/)
[![License](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

**FileEcho** 是一款专为开发者和极客设计的轻量级、高性能目录分析与文件克隆工具。它采用 C++17 编写高性能后端，结合 Webview2 驱动的现代化前端界面，实现了“所见即所得”的目录结构管理。

---

## ✨ 核心特性

- 🚀 **极速扫描**：基于 C++ 原始性能，瞬间处理包含数百文件的复杂目录。
- 📂 **结构预览**：生成精美的 ASCII 树状图，支持深度控制与排除规则。
- ⚡ **秒传克隆 (Echo)**：采用“路径引用”架构，本地文件/文件夹克隆无需经过网络流，实现磁盘级秒传。
- 🎨 **现代 UI**：深色模式/浅色模式适配，流畅的动画与响应式设计。
- 🔒 **安全至上**：严格的路径安全检查，防止目录穿越，拦截非本地 UNC 路径。

## 🛠️ 技术栈

- **后端**: C++17, [cpp-httplib](https://github.com/yhirose/cpp-httplib), [nlohmann/json](https://github.com/nlohmann/json)
- **GUI**: [webview/webview](https://github.com/webview/webview) (基于 Microsoft Edge WebView2)
- **前端**: HTML5, CSS3 (Inter 字体, FontAwesome 图标), JavaScript (Vanilla JS)

## 🚀 编译与运行

### 前置要求
- Windows 10/11
- CMake 3.15+
- MinGW-w64 (推荐 13.1.0+) 或 MSVC
- Microsoft Edge WebView2 运行时

### 编译步骤
1. **克隆仓库**:
   ```bash
   git clone https://github.com/CEQ151/FileEcho.git
   cd FileEcho
   ```

2. **生成构建配置**:
   ```bash
   mkdir build
   cd build
   cmake ..
   ```

3. **执行编译**:
   ```bash
   cmake --build .
   ```

### ⚠️ 运行前必读（重要！）
编译完成后，会在 `build` 目录下生成 `FileEcho.exe`。**直接运行它会导致界面无法显示**，因为后端需要加载前端静态资源。

**请务必按照以下步骤操作：**
1. 找到项目根目录下的 `frontend/` 文件夹。
2. 将整个 `frontend/` 文件夹**复制**到 `build/` 目录下（即 `FileEcho.exe` 所在的同一级目录）。
3. 此时运行 `FileEcho.exe` 即可看到完整的图形界面。

目录结构应如下所示：
```text
build/
├── frontend/        <-- 必须手动放入
│   ├── index.html
│   ├── script.js
│   └── style.css
└── FileEcho.exe
```

## 📖 使用指南

1. **设置扫描路径**：在 "Target Directory Path" 中输入绝对路径，点击 `Refresh Tree` 查看结构。
2. **过滤规则**：在设置面板中配置 `Max Depth` 或 `Exclude Patterns`（如 `node_modules`）。

## 📄 许可证

本项目采用 [MIT License](LICENSE) 开源。

---
*Powered by FileEcho - 让文件管理像回声一样清脆利落。*
