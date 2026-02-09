# FileEcho v1.0.5-stable

<p align="left">
  <a href="https://github.com/CEQ151/FileEcho/releases"><img src="https://img.shields.io/github/v/release/CEQ151/FileEcho?display_name=tag&style=flat-square&color=blue" alt="Release"></a>
  <img src="https://img.shields.io/github/license/CEQ151/FileEcho?style=flat-square&color=green" alt="License">
  <img src="https://img.shields.io/github/repo-size/CEQ151/FileEcho?style=flat-square" alt="Repo Size">
  <img src="https://img.shields.io/github/stars/CEQ151/FileEcho?style=flat-square" alt="Stars">
  <img src="https://img.shields.io/github/forks/CEQ151/FileEcho?style=flat-square" alt="Forks">
</p>

[English] | [简体中文](README.md)

**FileEcho** is a modern, high-performance local file scanning and management tool designed for developers and system administrators. It combines the native performance of C++17 with efficient Web technology interactions. In version v1.0.5, it achieves **full resource embedding**, generating a single executable file that requires no installation and works out of the box.

<img width="2559" height="1475" alt="6" src="https://github.com/user-attachments/assets/d278109e-1dc5-4311-87b4-c4d03d6a28fb" />



## ✨ Core Features

### 1. High-speed Scanning & Multi-dimensional Sorting
* **Single File Execution**: All frontend resources (HTML/CSS/JS) are embedded in the binary. No need to carry the `frontend` folder.
* **Deep Scan**: Utilizes C++17 `std::filesystem` for recursive scanning of local directories.
* **Folder First**: Folders always stay above files regardless of sorting rules, matching native system behavior.
* **Header Click Sorting**: Supports sorting by **Name** (natural sort), **Size**, **Type**, and **Depth**.

### 2. Intelligent File Tree
* **Pruning Search**: Enter a path in the search box to enter "Pruning Mode", showing only matches and their full parent paths.
* **Interactive Folding**: `[+]`/`[-]` controls for each folder, supporting partial expansion and collapse like mind maps.
* **Highlighting**: Search results are visually highlighted using `<mark>` tags in the tree.

### 3. Real-time Search & Statistics
* **Everything-level Filtering**: The right-side file list supports real-time fuzzy search with zero latency.
* **Status Sync**: Sidebar real-time updates for total file count and total size in the current view.

### 4. Professional Export & Interaction
* **WYSIWYG Export**: Generated `.txt` tree maps strictly follow your current **folding** and **search/filter** states.
* **Double-click to Open**: Double-click any file or folder to open it with the system's default program.

## 🛠️ Tech Stack

* **Backend**: C++17, `cpp-httplib` (lightweight server), `webview` (cross-platform native shell).
* **Frontend**: Vanilla JavaScript (ES6+), HTML5, CSS3 (Flexbox/Grid).
* **UI Components**: FontAwesome 6, Bootstrap 5 (partial styles).

## 🚀 Quick Start

1. **Requirements**: 
   * Windows 10/11 (x64)
   * C++17 compliant compiler (GCC/MinGW, MSVC 2019+)
   * **Python 3.x** (for automatic resource header generation during build)
   * CMake 3.15+

2. **Build**:
```bash
# Generate build config (automatically calls Python script to pack resources)
cmake -B build -G "Ninja" 

# Compile Release version
cmake --build build --config Release
```

3. **Run**: 
   Launch `build/FileEcho.exe` directly. The file is portable and can be moved anywhere.

## 📅 Changelog (v1.0.4 —> v1.0.5-stable)

* **[Major]** Implemented static embedding of frontend resources; `.exe` no longer depends on external folders.
* **[Architecture]** Reorganized project structure, moved 3rd-party dependencies to `include/external`.
* **[Features]** Added language switching (CN/EN) and multi-theme support.
* **[UX]** Added folder selection and upload functionality.
