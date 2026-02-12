# FileEcho v1.1.1-AI Embedded

<p align="left">
  <a href="https://github.com/CEQ151/FileEcho/releases"><img src="https://img.shields.io/github/v/release/CEQ151/FileEcho?display_name=tag&style=flat-square&color=blue" alt="Release"></a>
  <img src="https://img.shields.io/github/license/CEQ151/FileEcho?style=flat-square&color=green" alt="License">
  <img src="https://img.shields.io/badge/platform-Windows%2010%2F11-0078d4?style=flat-square" alt="Platform">
  <img src="https://img.shields.io/badge/C%2B%2B-17-00599c?style=flat-square&logo=cplusplus" alt="C++17">
  <img src="https://img.shields.io/badge/AI-7%20Providers-ff6b6b?style=flat-square" alt="AI Providers">
</p>

[English] | [简体中文](README.md)

**FileEcho** is a modern, high-performance local file scanning and AI analysis tool designed for developers and system administrators. It combines C++17 native performance with web-based UI interaction, embedding a complete file management system and multi-model AI assistant in a single executable.

> No Python/Node.js runtime needed — just double-click `FileEcho.exe` to use all features.

---

## ✨ Features

### 📁 File Scanning & Management
- **Ultra-fast recursive scanning** — Built on C++17 `std::filesystem`, millisecond-level traversal
- **Smart file tree** — Path-pruning search, interactive collapse/expand, keyword highlighting
- **Multi-sort** — Sort by Name / Size / Depth, folders always on top
- **Real-time filtering** — Instant filter-as-you-type, press Enter to prune tree
- **Double-click to open** — Open files directly with system default application
- **File tree export** — One-click copy or download ASCII file tree
- **File selection** — Checkbox to select file subsets for targeted AI analysis
- **PDF / Office reading** — Auto-extract text from PDF, DOCX, XLSX, PPTX

### 🤖 Built-in AI Assistant
- **7 AI providers, 30+ models** out of the box:

  | Provider | Example Models |
  |----------|---------------|
  | OpenAI | GPT-4.1, GPT-5, GPT-5.2 Pro |
  | DeepSeek | DeepSeek Chat (V3), Reasoner (R1) |
  | Google Gemini | Gemini 2.5 Flash/Pro, 3.0 Preview |
  | xAI Grok | Grok 4 / 4.1 Fast (Reasoning) |
  | Anthropic Claude | Haiku 4.5, Sonnet 4.5, Opus 4.6 |
  | Kimi (Moonshot) | Kimi K2 Turbo, K2.5 |
  | GLM (Zhipu AI) | GLM-4.5 AirX ~ GLM-5 |
  | Custom | Any OpenAI-compatible API |

- **API relay/proxy support** — Custom Base URL with auto-format detection
- **Markdown rendering** — h1-h6 headings, code blocks, tables, links, lists, blockquotes
- **LaTeX math** — KaTeX-based, supports inline `$...$` and block `$$...$$`
- **Token usage tracking** — Real-time prompt / completion / total token display
- **Chat export** — Export as `.txt` or `.md`
- **Smart hover summaries** — Hover over files/folders for AI-generated summary cards
- **Context-aware** — Auto-detects filenames in messages and reads their content
- **Multi-function panel** — Project summary / code analysis / cleanup suggestions / smart search / annotate tree
- **Temperature control** — Precise / Balanced / Creative presets + slider fine-tuning

### 🎨 Customization
- **6 themes** — Light / Obsidian / Sublime Monokai / Windows Dark / Dracula / Deep Space
- **Bilingual UI** — Chinese / English one-click switch
- **Resizable AI panel** — Drag left edge to adjust width

---

## 🚀 Quick Start

### Option 1: Direct Download (Recommended)

Download `FileEcho.exe` from [Releases](https://github.com/CEQ151/FileEcho/releases) and run it directly. No installation needed.

### Option 2: Build from Source

#### Prerequisites

| Dependency | Version | Notes |
|-----------|---------|-------|
| Windows | 10 / 11 (x64) | WebView2 Runtime required (built-in on Win11) |
| MinGW-w64 (GCC) | 13.0+ | 15.x recommended, C++17 support required |
| CMake | 3.15+ | Build system |
| Python | 3.6+ | For `pack_assets.py` resource packing script |

#### Build Steps

```bash
# 1. Clone the repository
git clone https://github.com/CEQ151/FileEcho.git
cd FileEcho

# 2. Pack frontend resources into C++ headers
python pack_assets.py

# 3. Configure CMake
cmake -B build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release

# 4. Build
cmake --build build --config Release

# 5. Run
build\FileEcho.exe
```

> ✅ **Static linking** is enabled — the output is a single `FileEcho.exe` with no external DLL dependencies.
>
> Or use `build_release.bat` to automate everything.

#### One-Click Build

```bash
# Build and collect to dist/
build_release.bat
```

---

## 🏗️ Project Structure

```
FileEcho/
├── CMakeLists.txt                # CMake build configuration
├── pack_assets.py                # Frontend → C++ header packing script
├── build_release.bat             # Windows one-click build script
├── CHANGELOG.md                  # Version changelog
├── USER_MANUAL.md                # User manual (Chinese)
├── include/
│   ├── external/                 # Third-party header-only libraries
│   │   ├── httplib.h             # cpp-httplib HTTP server
│   │   ├── json.hpp              # nlohmann/json
│   │   ├── webview.h             # WebView wrapper
│   │   ├── WebView2.h            # WebView2 API
│   │   └── deflate_inflate.h     # zlib decompression (PDF extraction)
│   └── FileEcho/                 # Project headers
│       ├── ai_handler.hpp        # AI handler
│       ├── filesystem.hpp        # Filesystem operations
│       ├── webserver.hpp         # HTTP server
│       ├── pdf_extractor.hpp     # PDF text extraction
│       ├── doc_extractor.hpp     # Office document extraction
│       └── utils.hpp             # Utilities
├── src/
│   ├── backend/
│   │   ├── main.cpp              # Entry point (WebView2 + HTTP server)
│   │   ├── webserver.cpp         # HTTP routes + static resource serving
│   │   ├── ai_handler.cpp        # Multi-model AI calls (WinHTTP, TLS 1.2)
│   │   ├── filesystem.cpp        # Recursive scan + file tree generation
│   │   ├── pdf_extractor.cpp     # PDF text parsing
│   │   └── doc_extractor.cpp     # DOCX/XLSX/PPTX parsing
│   └── frontend/
│       ├── index.html            # Main page (Bootstrap 5 + KaTeX)
│       ├── script.js             # File manager core logic
│       ├── style.css             # Main styles (6 theme CSS variables)
│       ├── ai_addon.js           # AI assistant (chat / settings / Markdown / LaTeX)
│       └── ai_addon.css          # AI assistant styles (theme-adaptive)
└── resources/
    ├── FileEcho.rc               # Windows resource (icon + version info)
    ├── FileEcho.manifest         # UAC manifest (asInvoker)
    └── logo.ico                  # Application icon
```

---

## 🔧 Tech Stack

| Layer | Technology |
|-------|-----------|
| Backend | C++17, `std::filesystem` |
| HTTP Server | [cpp-httplib](https://github.com/yhirose/cpp-httplib) (header-only) |
| JSON | [nlohmann/json](https://github.com/nlohmann/json) (header-only) |
| GUI Shell | [WebView2](https://developer.microsoft.com/en-us/microsoft-edge/webview2/) |
| AI Networking | WinHTTP (TLS 1.2, system proxy) |
| Frontend | Vanilla JS (ES6+), HTML5, CSS3 |
| UI Framework | Bootstrap 5, FontAwesome 6 |
| Math Rendering | [KaTeX](https://katex.org/) 0.16.11 (CDN) |
| Build System | CMake 3.15+, MinGW-w64 |

---

## 📜 License

[MIT License](LICENSE) © 2026 CEQ151
