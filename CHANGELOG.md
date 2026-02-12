# Changelog

All notable changes to FileEcho will be documented in this file.

## [1.1.1] - 2025-07-20

### Added
- **Markdown rendering** — AI responses now render h1–h6 headings, code blocks, tables, links, lists, and blockquotes
- **LaTeX math** — KaTeX-powered inline (`$...$`) and block (`$$...$$`) math rendering
- **Token usage display** — Real-time prompt / completion / total token statistics in chat footer
- **Chat export** — Export conversation history as `.txt` or `.md` from header dropdown
- **Base URL `/v1` hint** — Auto-detects missing `/v1` suffix and prompts user
- **Temperature presets** — Precise / Balanced / Creative quick-pick + slider fine-tuning
- **File selection checkboxes** — Select file subsets for targeted AI queries
- **Popover AI summaries** — Hover on files/folders in file tree for AI-generated summaries
- **API relay/proxy support** — Custom Base URL for third-party API proxies
- **PDF / Office reading** — Auto-extract text from PDF, DOCX, XLSX, PPTX for AI

### Changed
- **Full CSS rewrite** — All AI addon styles rebuilt with CSS custom properties for theme adaptation
- **6-theme support** — AI panel now seamlessly inherits all 6 main themes
- **Static linking** — MinGW runtime (libstdc++/libgcc/libwinpthread) statically linked into EXE, no external DLLs needed
- **Binary strip** — Debug symbols stripped, smaller EXE and fewer AV false positives
- **VERSIONINFO resource** — Full version/publisher/description info embedded in EXE properties
- **UAC manifest** — Embedded `asInvoker` manifest for proper Windows trust
- **Disabled browser shortcuts** — F5/F12/Ctrl+R/Ctrl+U/Ctrl+S/Ctrl+P and right-click context menu disabled in UI
- **Logo** — Switched application icon to `logo.ico`
- **Version** — Bumped to v1.1.1-AI Embedded

### Fixed
- **Popover disappear bug** — Popover no longer vanishes when hovering between file tree and popover card
- **TLS 1.2 enforcement** — WinHTTP now explicitly sets TLS 1.2 for all AI API calls
- **max_tokens clamping** — Prevents API rejection by clamping to provider/model limits
- **Gemini model ID** — Fixed incorrect model identifier format
- **Non-JSON response handling** — Gracefully handles plain-text / HTML error responses
- **NOMINMAX** — Added `#define NOMINMAX` to prevent Windows min/max macro conflicts
- **30-file limit** — Removed artificial 30-file scan limit
- **Reset settings** — Reset config now restores defaults without clearing chat history
- **System proxy** — WinHTTP uses IE/system proxy settings by default

## [1.1.0] - 2025-06

### Added
- **AI Assistant** — Embedded multi-model AI chat panel with 7 providers and 30+ models
- **Multi-provider support** — OpenAI, DeepSeek, Gemini, Grok, Claude, Kimi, GLM + Custom
- **Project analysis** — One-click project summary, code analysis, cleanup suggestions
- **Smart search** — AI-powered semantic file search
- **Context-aware chat** — Auto-detects filenames in messages and reads their content
- **Resizable panel** — Drag left edge of AI panel to adjust width
- **Bilingual UI** — Chinese / English one-click language switch

## [1.0.5] - 2025-05

### Added
- **Resource embedding** — All frontend assets (HTML/CSS/JS) packed into EXE via `pack_assets.py`
- **Single executable** — No external resource files needed; fully portable

### Changed
- **WebView2 integration** — Switched from Electron to native WebView2 for smaller footprint
- **HTTP server** — cpp-httplib serves embedded resources at `127.0.0.1:18080`

## [1.0.0] - 2025-04

### Added
- Initial release
- Recursive file scanning with `std::filesystem`
- Path-pruning search with keyword highlighting
- Multi-criterion sort (Name / Size / Depth, folders first)
- ASCII file tree export
- 6 built-in themes (Light / Obsidian / Sublime Monokai / Windows Dark / Dracula / Deep Space)
- Double-click to open with system default application
