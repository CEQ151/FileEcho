# FileEcho v1.1.1-AI Embedded

<p align="left">
  <a href="https://github.com/CEQ151/FileEcho/releases"><img src="https://img.shields.io/github/v/release/CEQ151/FileEcho?display_name=tag&style=flat-square&color=blue" alt="Release"></a>
  <img src="https://img.shields.io/github/license/CEQ151/FileEcho?style=flat-square&color=green" alt="License">
  <img src="https://img.shields.io/badge/platform-Windows%2010%2F11-0078d4?style=flat-square" alt="Platform">
  <img src="https://img.shields.io/badge/C%2B%2B-17-00599c?style=flat-square&logo=cplusplus" alt="C++17">
  <img src="https://img.shields.io/badge/AI-7%20Providers-ff6b6b?style=flat-square" alt="AI Providers">
</p>

[简体中文] | [English](README.en.md)

**FileEcho** 是一款面向开发者和系统管理员的**现代化本地文件扫描与 AI 分析工具**。它将 C++17 原生性能与 Web 前端交互相结合，在单一可执行文件中嵌入了完整的文件管理系统和多模型 AI 助手。

> 无需安装 Python/Node.js 运行时，双击 `FileEcho.exe` 即可使用全部功能。

---

## ✨ 功能亮点

### 📁 文件扫描与管理
- **极速递归扫描** — 基于 C++17 `std::filesystem`，毫秒级遍历大型目录
- **智能文件树** — 路径剪枝搜索、交互式折叠、关键词高亮
- **多维排序** — Name / Size / Depth 三维排序，文件夹始终置顶
- **实时筛选** — 输入即过滤，支持 Enter 回车剪枝文件树
- **双击打开** — 表格行双击直接调用系统程序打开文件
- **文件树导出** — 一键复制或下载 ASCII 文件树文本
- **文件勾选** — 可勾选文件子集，仅对选中文件进行 AI 分析
- **PDF / Office 读取** — 自动提取 PDF、DOCX、XLSX、PPTX 文本内容

### 🤖 AI 智能助手（内嵌）
- **7 大 AI 提供商，30+ 模型** — 开箱即用：

  | 提供商 | 模型示例 |
  |--------|----------|
  | OpenAI | GPT-4.1, GPT-5, GPT-5.2 Pro |
  | DeepSeek | DeepSeek Chat (V3), Reasoner (R1) |
  | Google Gemini | Gemini 2.5 Flash/Pro, 3.0 Preview |
  | xAI Grok | Grok 4 / 4.1 Fast (Reasoning) |
  | Anthropic Claude | Haiku 4.5, Sonnet 4.5, Opus 4.6 |
  | Kimi (月之暗面) | Kimi K2 Turbo, K2.5 |
  | GLM (智谱) | GLM-4.5 AirX ~ GLM-5 |
  | 自定义 | 任意 OpenAI 兼容 API |

- **API 中转/代理支持** — 自定义 Base URL，自动检测并转换请求格式
- **Markdown 渲染** — h1-h6 标题、代码块、表格、链接、列表、引用
- **LaTeX 数学公式** — 基于 KaTeX，支持行内 `$...$` 和块级 `$$...$$`
- **Token 用量统计** — 实时显示每次对话 prompt / completion / total tokens，新对话自动归零
- **聊天记录复制** — 一键复制聊天记录到剪贴板（Markdown 格式）
- **智能悬浮摘要** — 鼠标悬停文件/文件夹，AI 自动生成摘要卡片
- **上下文感知** — 自动识别消息中的文件名并读取内容发送给 AI
- **多功能面板** — 项目总结 / 代码分析 / 清理建议 / 智能搜索 / 注释树
- **温度调节** — 精准/平衡/创意预设 + 滑块微调

### 🎨 个性化
- **6 套主题** — Light / Obsidian / Sublime Monokai / Windows Dark / Dracula / Deep Space
- **双语 UI** — 中文 / English 一键切换
- **AI 面板可拖拽** — 左边缘拖拽调整宽度

---

## 📸 截图

<!-- 请替换为实际截图 URL -->
> 截图占位：主界面 / AI 聊天 / 多主题切换 / LaTeX 渲染

---

## 🚀 快速开始

### 方式一：直接下载（推荐）

从 [Releases](https://github.com/CEQ151/FileEcho/releases) 下载 `FileEcho.exe`，双击即可运行，无需安装。

### 方式二：从源码编译

#### 环境要求

| 依赖 | 版本 | 说明 |
|------|------|------|
| Windows | 10 / 11 (x64) | 需要 WebView2 Runtime（Win10 需手动安装，Win11 自带） |
| MinGW-w64 (GCC) | 13.0+ | 推荐 15.x，需支持 C++17 |
| CMake | 3.15+ | 构建系统 |
| Python | 3.6+ | 运行 `pack_assets.py` 资源打包脚本 |

#### 编译步骤

```bash
# 1. 克隆仓库
git clone https://github.com/CEQ151/FileEcho.git
cd FileEcho

# 2. 打包前端资源为 C++ 头文件
python pack_assets.py

# 3. 创建构建目录并配置
cmake -B build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release

# 4. 编译
cmake --build build --config Release

# 5. 运行
build\FileEcho.exe
```

> ✅ 已启用**静态链接**，编译产物为单个 `FileEcho.exe`，无需任何外部 DLL。
>
> 也可直接使用 `build_release.bat` 一键完成所有步骤。

#### 一键编译

```bash
# 一键编译，输出到 dist/ 目录
build_release.bat
```

---

## 🏗️ 项目结构

```
FileEcho/
├── CMakeLists.txt                # CMake 构建配置
├── pack_assets.py                # 前端资源 → C++ 头文件打包脚本
├── build_release.bat             # Windows 一键编译脚本
├── CHANGELOG.md                  # 版本更新日志
├── USER_MANUAL.md                # 用户手册
├── include/
│   ├── external/                 # 第三方 header-only 库
│   │   ├── httplib.h             # cpp-httplib HTTP 服务器
│   │   ├── json.hpp              # nlohmann/json
│   │   ├── webview.h             # WebView 封装
│   │   ├── WebView2.h            # WebView2 API
│   │   └── deflate_inflate.h     # zlib 解压（PDF 提取）
│   └── FileEcho/                 # 项目头文件
│       ├── ai_handler.hpp        # AI 处理器
│       ├── filesystem.hpp        # 文件系统操作
│       ├── webserver.hpp         # HTTP 服务器
│       ├── pdf_extractor.hpp     # PDF 文本提取
│       ├── doc_extractor.hpp     # Office 文档提取
│       └── utils.hpp             # 工具函数
├── src/
│   ├── backend/
│   │   ├── main.cpp              # 入口（WebView2 窗口 + HTTP 服务器启动）
│   │   ├── webserver.cpp         # HTTP 路由 + 静态资源服务
│   │   ├── ai_handler.cpp        # AI 多模型调用（WinHTTP, TLS 1.2）
│   │   ├── filesystem.cpp        # 递归扫描 + 文件树生成
│   │   ├── pdf_extractor.cpp     # PDF 文本解析
│   │   └── doc_extractor.cpp     # DOCX/XLSX/PPTX 解析
│   └── frontend/
│       ├── index.html            # 主页面（Bootstrap 5 + KaTeX）
│       ├── script.js             # 文件管理器核心逻辑
│       ├── style.css             # 主样式（6 套主题 CSS 变量）
│       ├── ai_addon.js           # AI 助手（聊天 / 设置 / Markdown / LaTeX）
│       └── ai_addon.css          # AI 助手样式（主题自适应）
└── resources/
    ├── FileEcho.rc               # Windows 资源（图标 + 版本信息）
    ├── FileEcho.manifest         # UAC 清单（asInvoker）
    └── logo.ico                  # 应用图标
```

---

## 🔧 技术栈

| 层级 | 技术 |
|------|------|
| 后端核心 | C++17, `std::filesystem` |
| HTTP 服务 | [cpp-httplib](https://github.com/yhirose/cpp-httplib) (header-only) |
| JSON | [nlohmann/json](https://github.com/nlohmann/json) (header-only) |
| GUI 容器 | [WebView2](https://developer.microsoft.com/en-us/microsoft-edge/webview2/) |
| AI 网络 | WinHTTP (TLS 1.2, 系统代理自动继承) |
| 前端 | Vanilla JS (ES6+), HTML5, CSS3 |
| UI 框架 | Bootstrap 5, FontAwesome 6 |
| 数学渲染 | [KaTeX](https://katex.org/) 0.16.11 (CDN) |
| 构建 | CMake 3.15+, MinGW-w64 |

---

## 📋 AI 使用指南

1. 点击右下角 🤖 悬浮球打开 AI 助手面板
2. 点击 ⚙️ 齿轮进入设置，选择提供商和模型
3. 填入 API Key（支持自定义 Base URL，用于 API 中转站）
4. 点击 **测试连接** 确认配置正常
5. 开始对话，或使用底部快捷按钮：
   - 📊 **项目总结** — 分析整个项目结构
   - 💡 **调整建议** — 生成文件清理建议
   - 🔍 **代码分析** — 代码质量评估
   - 📄 **文件摘要** — 勾选文件后生成摘要
   - 🧠 **智能搜索** — AI 语义化文件搜索
   - 🏷️ **注释树** — AI 为文件树每行追加 `# 注释`

> 🔒 隐私说明：所有 API 请求直接从本机发出，API Key 仅保存在本地 `ai_config.json`，不经过任何第三方中间服务器。

---

## 📜 许可证

[MIT License](LICENSE) © 2026 CEQ151
