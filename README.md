# FileEcho v1.0.5-stable

[![Release](https://img.shields.io/github/v/release/CEQ151/FileEcho?display_name=tag&style=flat-square&color=blue)](https://github.com/CEQ151/FileEcho/releases)
![License](https://img.shields.io/github/license/CEQ151/FileEcho?style=flat-square&color=green)
![Repo Size](https://img.shields.io/github/repo-size/CEQ151/FileEcho?style=flat-square)
![Stars](https://img.shields.io/github/stars/CEQ151/FileEcho?style=flat-square)
![Forks](https://img.shields.io/github/forks/CEQ151/FileEcho?style=flat-square)

**FileEcho** 是一款专为开发者和系统管理员设计的现代化、高性能本地文件扫描与管理工具。它结合了 C++17 的原生性能与 Web 技术的高效交互，在 v1.0.5 版本中实现了**全资源嵌入**，生成单一的可执行文件，无需安装，即开即用。

## ✨ 核心功能

### 1. 极速扫描与多维排序
* **单文件运行**：所有前端资源（HTML/CSS/JS）已嵌入二进制文件，无需携带 `frontend` 文件夹。
* **深度扫描**：利用 C++17 `std::filesystem` 递归扫描本地目录。
* **文件夹置顶 (Folder First)**：无论应用何种排序规则，文件夹始终排在同级文件的上方，符合原生系统使用习惯。
* **表头点击排序**：支持点击 **Name** (自然语言数字排序)、**Size** (原始字节比较)、**Type** 及 **Depth** 进行升降序排列。

### 2. 智能文件树 (File Tree)

* **路径剪枝搜索**：在搜索框输入并回车，文件树将进入“剪枝模式”，仅显示匹配项及其完整的父级路径，保持上下文层级感。
* **交互式折叠**：每个文件夹前设有 `[+]`/`[-]` 控制符，支持类似思维导图的局部展开与收起。
* **高亮显示**：搜索结果在树状图中通过 `<mark>` 标签进行视觉高亮处理。

### 3. 实时搜索与统计

* **Everything 级过滤**：右侧文件列表支持实时模糊搜索，输入即过滤，体感无延迟。
* **状态同步**：侧边栏实时同步当前视图下的文件总数与总体积（例如：“Found 5 items (2.5 MB)”）。

### 4. 专业级导出与交互

* **所见即所得 (WYSIWYG) 导出**：点击“下载”或“复制”，生成的 `.txt` 树状图将严格遵循您当前的**折叠状态**与**搜索过滤状态**。
* **双击打开**：在列表中双击任何文件或文件夹，将调用 Windows 系统原生关联程序将其打开。

## 🛠️ 技术栈

* **后端**: C++17, `cpp-httplib` (轻量级服务器), `webview` (跨平台原生壳)。
* **前端**: 原生 JavaScript (ES6+), HTML5, CSS3 (Flexbox/Grid 布局)。
* **UI 组件**: FontAwesome 6, Bootstrap 5 (部分辅助样式)。

## 🚀 快速开始

1. **环境要求**: 
   * Windows 10/11 (x64)
   * 支持 C++17 的编译器 (GCC/MinGW, MSVC 2019+)
   * **Python 3.x** (用于构建时自动生成资源头文件)
   * CMake 3.15+

2. **构建项目**:
```bash
# 生成构建配置 (会自动调用 Python 脚本打包前端资源)
cmake -B build -G "Ninja" 

# 编译 Release 版本
cmake --build build --config Release
```

3. **运行**: 
   直接运行 `build/FileEcho.exe` 即可。该文件是独立运行的，您可以将其移动到任何地方。

## 📅 更新日志 (v1.0.4 —> v1.0.5-stable)

* **[重大更新]** 实现了前端资源静态嵌入，生成的 `.exe` 不再依赖外部文件夹。
* **[架构优化]** 重新组织了项目结构，将第三方依赖移至 `include/external`。
* **[功能增强]** 添加了语言切换（中/英）与多主题支持。
* **[体验优化]** 增加了文件夹选择上传功能。
