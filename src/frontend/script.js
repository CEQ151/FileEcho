class Starfield {
    constructor() {
        this.canvas = document.createElement('canvas');
        this.canvas.id = 'starfield-canvas';
        this.canvas.style.position = 'fixed';
        this.canvas.style.top = '0';
        this.canvas.style.left = '0';
        this.canvas.style.width = '100%';
        this.canvas.style.height = '100%';
        this.canvas.style.pointerEvents = 'none';
        this.canvas.style.zIndex = '0'; // 修改为 0，确保在背景之上
        this.ctx = this.canvas.getContext('2d');
        this.stars = [];
        this.animationId = null;
        this.active = false;
        
        this.resizeHandler = () => this.resize();
    }

    mount() {
        if (!document.body.contains(this.canvas)) {
            document.body.appendChild(this.canvas);
        }
        window.addEventListener('resize', this.resizeHandler);
        this.resize();
        this.initStars();
        this.active = true;
        this.animate();
    }

    unmount() {
        this.active = false;
        window.removeEventListener('resize', this.resizeHandler);
        if (this.animationId) cancelAnimationFrame(this.animationId);
        if (this.canvas.parentNode) this.canvas.parentNode.removeChild(this.canvas);
    }

    resize() {
        this.canvas.width = window.innerWidth;
        this.canvas.height = window.innerHeight;
    }

    initStars() {
        this.stars = [];
        this.active = true;
        const count = 500; // 进一步增加星星数量
        for (let i = 0; i < count; i++) {
            this.stars.push({
                x: Math.random() * this.canvas.width,
                y: Math.random() * this.canvas.height,
                size: Math.random() * 2.8, // 增大星星
                opacity: Math.random() * 0.9 + 0.1, // 提高亮度
                speed: Math.random() * 0.15 + 0.05
            });
        }
    }

    animate() {
        if (!this.active) return;
        
        this.ctx.clearRect(0, 0, this.canvas.width, this.canvas.height);
        
        for (let star of this.stars) {
            this.ctx.fillStyle = `rgba(255, 255, 255, ${star.opacity})`;
            this.ctx.beginPath();
            this.ctx.arc(star.x, star.y, star.size, 0, Math.PI * 2);
            this.ctx.fill();
            
            star.opacity += (Math.random() - 0.5) * 0.05;
            if (star.opacity < 0.1) star.opacity = 0.1;
            if (star.opacity > 0.8) star.opacity = 0.8;
            
            star.y += star.speed;
            if (star.y > this.canvas.height) {
                star.y = 0;
                star.x = Math.random() * this.canvas.width;
            }
        }
        
        this.animationId = requestAnimationFrame(() => this.animate());
    }
}

const translations = {
    en: {
        language: "Language",
        fileInfo: "File Info",
        noPathSelected: "No path selected",
        files: "Files",
        targetAddress: "Target Address",
        pathPlaceholder: "Absolute path...",
        settings: "Settings",
        theme: "Theme",
        themeLight: "Default Light",
        themeObsidian: "Obsidian",
        themeSublime: "Sublime",
        themeWindows: "Windows Dark",
        themeDracula: "Dracula",
        themeSpace: "Deep Space",
        maxDepth: "Max Depth",
        excludes: "Excludes",
        excludesPlaceholder: "node_modules, .git",
        showSize: "Show Size",
        cleanup: "Cleanup",
        clearResults: "Clear Results",
        resetAllData: "Reset All Data",
        viewOnGitHub: "View on GitHub",
        treeView: "File Tree",
        treePlaceholder: "Enter path in sidebar and scan.",
        refresh: "Refresh",
        copy: "Copy",
        download: "Download",
        listView: "File Details",
        filterPlaceholder: "Filter current list (Press Enter to prune tree)...",
        name: "Name",
        type: "Type",
        size: "Size",
        depth: "Depth",
        scanning: "Scanning directory...",
        foundFiles: "Found {count} files/directories",
        scanFailed: "Scan failed: {message}",
        scanError: "Scan error: {message}",
        pleaseScanFirst: "Please scan a directory first",
        treeGenError: "Tree generation error: {message}",
        noFilesFound: "No files found.",
        downloadStarted: "Download started",
        downloadStartedSearch: "Download started (search: \"{query}\")",
        downloadError: "Download error: {message}",
        noFilesToCopy: "No files to copy",
        noTreeToCopy: "No tree to copy",
        treeCopied: "Tree copied to clipboard!",
        copyFailed: "Failed to copy: {message}",
        resultsCleared: "Results cleared",
        confirmReset: "Are you sure you want to clear all data and settings?",
        allDataCleared: "All data cleared",
        serverConnected: "Server connected successfully",
        serverDisconnected: "Disconnected",
        apiNotAvailable: "API: Not available",
        serverNotResponding: "Server not responding",
        cannotConnect: "Cannot connect to server",
        connected: "Connected",
        noFilesScanned: "No files scanned yet",
        noMatchingFiles: "No matching files found",
        doubleClickToOpen: "Double-click to open",
        directory: "Directory",
        file: "File"
    },
    zh: {
        language: "界面语言",
        fileInfo: "文件信息",
        noPathSelected: "未选择路径",
        files: "个文件",
        targetAddress: "目标地址",
        pathPlaceholder: "输入绝对路径...",
        settings: "设置",
        theme: "界面主题",
        themeLight: "经典白色",
        themeObsidian: "Obsidian 紫黑",
        themeSublime: "Sublime 暖灰",
        themeWindows: "Windows 深色",
        themeDracula: "Dracula 高对比",
        themeSpace: "深邃太空",
        maxDepth: "最大深度",
        excludes: "排除模式",
        excludesPlaceholder: "例如: node_modules, .git",
        showSize: "显示大小",
        cleanup: "清理",
        clearResults: "清除结果",
        resetAllData: "重置所有数据",
        viewOnGitHub: "在 GitHub 上查看",
        treeView: "文件树",
        treePlaceholder: "在侧边栏输入路径并扫描以查看树状图。",
        refresh: "刷新",
        copy: "复制",
        download: "下载",
        listView: "文件详情",
        filterPlaceholder: "过滤当前列表 (回车可精简树状图)...",
        name: "名称",
        type: "类型",
        size: "大小",
        depth: "深度",
        scanning: "正在扫描目录...",
        foundFiles: "找到 {count} 个文件/目录",
        scanFailed: "扫描失败: {message}",
        scanError: "扫描错误: {message}",
        pleaseScanFirst: "请先扫描一个目录",
        treeGenError: "树状图生成错误: {message}",
        noFilesFound: "未找到文件。",
        downloadStarted: "下载已开始",
        downloadStartedSearch: "下载已开始 (搜索关键字: \"{query}\")",
        downloadError: "下载错误: {message}",
        noFilesToCopy: "没有可复制的文件",
        noTreeToCopy: "没有可复制的树状图",
        treeCopied: "树状图已复制到剪贴板！",
        copyFailed: "复制失败: {message}",
        resultsCleared: "结果已清除",
        confirmReset: "确定要清除所有数据和设置吗？",
        allDataCleared: "所有数据已清除",
        serverConnected: "服务器连接成功",
        serverDisconnected: "未连接",
        apiNotAvailable: "API: 不可用",
        serverNotResponding: "服务器无响应",
        cannotConnect: "无法连接到服务器",
        connected: "已连接",
        noFilesScanned: "尚未扫描文件",
        noMatchingFiles: "未找到匹配的文件",
        doubleClickToOpen: "双击以打开",
        directory: "目录",
        file: "文件"
    }
};

class FileManagerApp {
    constructor() {
        this.apiBaseUrl = window.location.origin;
        this.currentFiles = [];
        this.currentPath = '';
        this.totalSize = 0;
        this.sortCriteria = { key: 'name', direction: 'asc' };
        this.collapsedPaths = new Set(); // Stores paths of collapsed directories
        this.language = 'en';
        this.theme = 'light';
        
        this.initElements();
        this.initEventListeners();
        this.checkServerStatus();
        this.loadState(); // Load saved state on startup
        this.updateLanguage(); // Initial language apply
        this.applyTheme(); // Initial theme apply
    }
    
    initElements() {
        // Language & Theme
        this.languageSelect = document.getElementById('language-select');
        this.themeSelect = document.getElementById('theme-select');

        // Sidebar elements
        this.sidebar = document.getElementById('sidebar');
        this.sidebarToggleBtn = document.getElementById('sidebar-toggle-btn');
        this.sidebarResizer = document.getElementById('sidebar-resizer');
        this.githubBtn = document.getElementById('github-btn');

        // Folder selection
        this.folderSelector = document.getElementById('folder-selector');
        this.selectFolderBtn = document.getElementById('select-folder-btn');

        // Settings elements
        this.showSizeCheckbox = document.getElementById('show-size');
        this.maxDepthInput = document.getElementById('max-depth');
        this.excludePatternsInput = document.getElementById('exclude-patterns');
        
        // Info elements
        this.currentPathElement = document.getElementById('current-path');
        this.fileCountElement = document.getElementById('file-count');
        this.totalSizeElement = document.getElementById('total-size');
        
        // Action buttons
        this.generateTreeBtn = document.getElementById('generate-tree-btn');
        this.downloadBtn = document.getElementById('download-btn');
        this.copyBtn = document.getElementById('copy-btn');
        this.clearBtn = document.getElementById('clear-btn'); 
        this.scanBtn = document.getElementById('scan-btn'); // New Scan button in sidebar
        
        // Input elements
        this.directoryPathInput = document.getElementById('directory-path');
        this.listFilterInput = document.getElementById('list-filter');
        
        // Output elements
        this.treeOutput = document.getElementById('tree-output');
        this.fileTableBody = document.getElementById('file-table-body');
        
        // Status elements
        this.serverStatusElement = document.getElementById('server-status');
        this.apiStatusElement = document.getElementById('api-status');
    }
    
    initEventListeners() {
        // Language switch
        this.languageSelect.addEventListener('change', (e) => {
            this.language = e.target.value;
            this.updateLanguage();
            this.saveState();
        });

        // Theme switch
        this.themeSelect.addEventListener('change', (e) => {
            this.theme = e.target.value;
            this.applyTheme();
            this.saveState();
        });

        // Sidebar Toggle
        this.sidebarToggleBtn.addEventListener('click', () => this.toggleSidebar());

        // Native Folder Selector via Backend
        this.selectFolderBtn.addEventListener('click', async () => {
            try {
                const response = await fetch(`${this.apiBaseUrl}/api/pick-folder`);
                const result = await response.json();
                
                if (result.success && result.path) {
                    // Action 1: Update UI
                    this.directoryPathInput.value = result.path;
                    // Action 2: Auto Scan
                    this.scanDirectory(true);
                } else if (result.message && !result.message.includes('cancelled')) {
                    this.showToast(result.message, 'error');
                }
            } catch (error) {
                console.error('Failed to pick folder:', error);
                // Fallback to hidden input if API fails
                this.folderSelector.click();
            }
        });

        // GitHub Button
        this.githubBtn.addEventListener('click', () => {
            this.openItem('https://github.com/CEQ151/fileecho');
        });

        // Button events
        this.scanBtn.addEventListener('click', () => this.scanDirectory(true));
        this.generateTreeBtn.addEventListener('click', () => this.scanDirectory(true));
        this.downloadBtn.addEventListener('click', () => this.downloadTree());
        this.copyBtn.addEventListener('click', () => this.copyTreeToClipboard());
        this.clearBtn.addEventListener('click', () => this.clearResults());
        
        // Input events
        this.directoryPathInput.addEventListener('keypress', (e) => {
            if (e.key === 'Enter') {
                this.scanDirectory(true);
            }
        });

        // Search events
        this.listFilterInput.addEventListener('input', () => this.applySearch(false));
        this.listFilterInput.addEventListener('keypress', (e) => {
            if (e.key === 'Enter') {
                this.applySearch(true);
            }
        });

        // Tree Toggle Interaction (Event Delegation)
        this.treeOutput.addEventListener('click', (e) => {
            const toggle = e.target.closest('.tree-toggle');
            if (toggle) {
                const path = toggle.getAttribute('data-path');
                if (this.collapsedPaths.has(path)) {
                    this.collapsedPaths.delete(path);
                } else {
                    this.collapsedPaths.add(path);
                }
                this.generateTree(); // Re-render the UI
            }
        });

        // Table sorting
        document.querySelectorAll('.sticky-header thead th[data-sort]').forEach(th => {
            th.addEventListener('click', () => {
                const key = th.getAttribute('data-sort');
                this.handleSort(key);
            });
        });

        // Save settings when changed
        [this.showSizeCheckbox, this.maxDepthInput, this.excludePatternsInput].forEach(el => {
            el.addEventListener('change', () => this.saveState());
        });
        
        // Resizer logic
        const resizer = document.getElementById('resizer');
        const treeSection = document.querySelector('.tree-section');
        const sidebar = document.getElementById('sidebar');

        let isResizing = false;

        resizer.addEventListener('mousedown', (e) => {
            isResizing = true;
            document.body.style.cursor = 'col-resize';
            document.body.style.userSelect = 'none';
        });

        document.addEventListener('mousemove', (e) => {
            if (!isResizing) return;

            const sidebarWidth = sidebar.getBoundingClientRect().width;
            const newTreeWidth = e.clientX - sidebarWidth;

            if (newTreeWidth > 150 && newTreeWidth < 600) {
                treeSection.style.width = `${newTreeWidth}px`;
            }
        });

        document.addEventListener('mouseup', () => {
            if (isResizing) {
                isResizing = false;
                document.body.style.cursor = '';
                document.body.style.userSelect = '';
            }
        });
    }

    toggleSidebar() {
        this.sidebar.classList.toggle('collapsed');
        const icon = this.sidebarToggleBtn.querySelector('i');
        if (this.sidebar.classList.contains('collapsed')) {
            icon.className = 'fas fa-chevron-right'; // 收起时显示向右箭头
        } else {
            icon.className = 'fas fa-bars'; // 展开时显示菜单图标
        }
    }

    updateLanguage() {
        const langData = translations[this.language];
        
        // Update elements with data-i18n
        document.querySelectorAll('[data-i18n]').forEach(el => {
            const key = el.getAttribute('data-i18n');
            if (langData[key]) {
                // If it's the tree output and it's empty/placeholder, update it
                if (key === 'treePlaceholder' && (this.treeOutput.innerHTML === '' || this.treeOutput.innerHTML.includes('sidebar') || this.treeOutput.innerHTML.includes('侧边栏'))) {
                     el.textContent = langData[key];
                } else if (key !== 'treePlaceholder') {
                    el.textContent = langData[key];
                }
            }
        });

        // Update placeholders
        document.querySelectorAll('[data-i18n-placeholder]').forEach(el => {
            const key = el.getAttribute('data-i18n-placeholder');
            if (langData[key]) {
                el.placeholder = langData[key];
            }
        });

        // Update language select value
        if (this.languageSelect) {
            this.languageSelect.value = this.language;
        }

        // Special case for dynamic text if already displayed
        if (!this.currentPath) {
            this.currentPathElement.textContent = langData.noPathSelected;
        }

        // Refresh dynamic UI components
        if (this.currentFiles.length > 0) {
            this.updateFileTable(this.listFilterInput.value.trim());
        }
    }

    applyTheme() {
        // Set data-theme attribute
        document.documentElement.setAttribute('data-theme', this.theme);

        // Update select UI
        if (this.themeSelect) {
            this.themeSelect.value = this.theme;
        }
        
        // Handle Deep Space Starfield
        if (this.theme === 'space') {
            if (!this.starfield) {
                this.starfield = new Starfield();
            }
            if (!this.starfield.active) {
                this.starfield.mount();
            }
        } else {
            if (this.starfield && this.starfield.active) {
                this.starfield.unmount();
            }
        }
    }

    t(key, params = {}) {
        let text = translations[this.language][key] || key;
        for (const [k, v] of Object.entries(params)) {
            text = text.replace(`{${k}}`, v);
        }
        return text;
    }
    
    // Upload functions removed

    async openItem(path) {
        // Explicitly allow http/https URLs (handled by backend ShellExecute)
        if (path.startsWith('http://') || path.startsWith('https://')) {
            // No specific client-side logic needed, just pass to backend
        }

        try {
            const response = await fetch(`${this.apiBaseUrl}/api/open`, {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json'
                },
                body: JSON.stringify({ path: path })
            });
            const result = await response.json();
            if (!result.success) {
                this.showToast(`Failed to open: ${result.message}`, 'error');
            }
        } catch (error) {
            this.showToast(`Error opening item: ${error.message}`, 'error');
        }
    }
    
    async scanDirectory(autoGenerate = true) {
        const path = this.directoryPathInput.value.trim();
        
        if (!path) {
            this.showToast(this.t('pathPlaceholder'), 'warning');
            return;
        }
        
        this.showToast(this.t('scanning'), 'info');
        
        try {
            const options = this.getTreeOptions();
            
            const response = await fetch(`${this.apiBaseUrl}/api/scan`, {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json'
                },
                body: JSON.stringify({
                    path: path,
                    ...options
                })
            });
            
            const result = await response.json();
            
            if (result.success) {
                this.currentFiles = result.files || [];
                this.currentPath = result.path;
                
                // Update UI Basics
                this.currentPathElement.textContent = this.currentPath;
                
                // Default Sort: Name (ASC) + Folder First
                this.sortCriteria = { key: 'name', direction: 'asc' };
                this.sortFiles();

                // Update file table (handles stats and sorting)
                this.updateFileTable();
                
                // Reset collapsed state on new scan
                this.collapsedPaths.clear();
                
                this.showToast(this.t('foundFiles', { count: result.file_count }), 'success');
                
                // Auto generate tree if requested
                if (autoGenerate) {
                    await this.generateTree();
                }

                this.saveState();
            } else {
                this.showToast(this.t('scanFailed', { message: result.message }), 'error');
            }
        } catch (error) {
            this.showToast(this.t('scanError', { message: error.message }), 'error');
        }
    }
    
    async generateTree() {
        if (this.currentFiles.length === 0) {
            // If no files, try scanning first
            if (this.directoryPathInput.value.trim()) {
                await this.scanDirectory(false); // prevent infinite loop
                if (this.currentFiles.length === 0) return;
            } else {
                this.showToast(this.t('pleaseScanFirst'), 'warning');
                return;
            }
        }
        
        try {
            const query = this.listFilterInput ? this.listFilterInput.value.trim() : '';
            const isSearchActive = query.length > 0;

            // Generate tree on client side
            // Always pass the current query and active search status to ensure 
            // the tree reflects the current UI state (Filters + Collapsed Folders)
            const treeHtml = this.generateTreeContent(true, query, isSearchActive);
            
            this.treeOutput.innerHTML = treeHtml;
            this.saveState();
        } catch (error) {
            this.showToast(this.t('treeGenError', { message: error.message }), 'error');
            console.error(error);
        }
    }
    
    // Generate tree content (HTML or Plain Text)
    generateTreeContent(asHtml, query = '', pruneTree = false, visiblePaths = null) {
        if (this.currentFiles.length === 0) return asHtml ? '' : this.t('noFilesFound');

        const files = this.currentFiles;
        const lowerQuery = query.toLowerCase();
        const pathsToKeep = new Set();

        if (pruneTree && query) {
            files.forEach(file => {
                if (file.name.toLowerCase().includes(lowerQuery)) {
                    let currentPath = file.path;
                    while (currentPath) {
                        pathsToKeep.add(currentPath);
                        const lastSlash = Math.max(currentPath.lastIndexOf('/'), currentPath.lastIndexOf('\\'));
                        if (lastSlash === -1) break;
                        currentPath = currentPath.substring(0, lastSlash);
                    }
                }
            });
            pathsToKeep.add(this.currentPath);
        }

        // Use provided visiblePaths if available (e.g., from download during active search)
        // Otherwise use pathsToKeep if pruneTree is true
        const effectiveVisiblePaths = visiblePaths || (pruneTree && query ? pathsToKeep : null);

        // Helper to check if any ancestor is collapsed
        const isCollapsedByAncestor = (filePath) => {
            for (const collapsedPath of this.collapsedPaths) {
                // If the file path starts with a collapsed path and it's not the collapsed directory itself
                if (filePath.startsWith(collapsedPath) && filePath !== collapsedPath) {
                    // Ensure it's a true sub-path (followed by a slash)
                    const subChar = filePath[collapsedPath.length];
                    if (subChar === '/' || subChar === '\\') return true;
                }
            }
            return false;
        };

        // Filter files based on visiblePaths (search) AND collapsed state
        let filesToDisplay = files.filter(f => {
            // 1. Must be in visible paths (if searching)
            if (effectiveVisiblePaths && !effectiveVisiblePaths.has(f.path)) return false;
            
            // 2. Must not have a collapsed ancestor
            if (isCollapsedByAncestor(f.path)) return false;
            
            return true;
        });

        // Ensure the tree always uses a stable hierarchical pre-order (Folder First + Alphabetical)
        // regardless of the list's sort criteria.
        filesToDisplay.sort((a, b) => {
            const partsA = a.path.split(/[/\\]/).filter(Boolean);
            const partsB = b.path.split(/[/\\]/).filter(Boolean);
            const len = Math.min(partsA.length, partsB.length);
            for (let i = 0; i < len; i++) {
                if (partsA[i] !== partsB[i]) {
                    const isDirA = (i < partsA.length - 1) || a.is_directory;
                    const isDirB = (i < partsB.length - 1) || b.is_directory;
                    if (isDirA !== isDirB) return isDirA ? -1 : 1;
                    return partsA[i].localeCompare(partsB[i], undefined, { numeric: true });
                }
            }
            return partsA.length - partsB.length;
        });

        let output = '';
        
        // 1. Output Root Directory
        let rootName = this.currentPath.split(/[/\\]/).filter(Boolean).pop() || 'root';
        rootName += '/';
        
        let displayRootName = rootName;
        if (asHtml && query && rootName.toLowerCase().includes(lowerQuery)) {
            const regex = new RegExp(`(${query.replace(/[.*+?^${}()|[\]\\]/g, '\\$&')})`, 'gi');
            displayRootName = rootName.replace(regex, '<mark class="highlight">$1</mark>');
        }

        if (asHtml) {
            output += `<span class="tree-icon">📁</span> <span class="tree-item-name" style="font-weight:bold">${displayRootName}</span>\n`;
        } else {
            output += `${rootName}\n`;
        }
        
        // Track the "is last child" status for each depth level
        const isLastAtDepth = [];

        for (let i = 0; i < filesToDisplay.length; i++) {
            const file = filesToDisplay[i];
            
            // O(N) optimized check for last child in a hierarchical pre-order list
            const isLast = (i === filesToDisplay.length - 1) || (filesToDisplay[i + 1].depth < file.depth);
            isLastAtDepth[file.depth] = isLast;

            // Indentation
            for (let d = 1; d < file.depth; d++) {
                output += isLastAtDepth[d] ? '    ' : '│   ';
            }

            // Connector
            output += isLast ? '└── ' : '├── ';

            if (file.is_directory && asHtml) {
                const isCollapsed = this.collapsedPaths.has(file.path);
                const toggleIcon = isCollapsed ? '[+]' : '[-]';
                output += `<span class="tree-toggle" data-path="${file.path.replace(/"/g, '&quot;')}">${toggleIcon}</span> `;
            }

            const iconChar = file.is_directory ? '📁' : '📄';
            let fileName = file.name + (file.is_directory ? '/' : '');
            
            let displayName = fileName;
            if (asHtml && query && fileName.toLowerCase().includes(lowerQuery)) {
                const regex = new RegExp(`(${query.replace(/[.*+?^${}()|[\]\\]/g, '\\$&')})`, 'gi');
                displayName = fileName.replace(regex, '<mark class="highlight">$1</mark>');
            }

            if (asHtml) {
                output += `<span class="tree-icon">${iconChar}</span> <span class="tree-item-name">${displayName}</span>`;
            } else {
                output += `${fileName}`;
            }

            if (this.showSizeCheckbox.checked) {
                output += ` (${this.formatFileSize(file.size || 0)})`;
            }

            output += '\n';
        }

        return output;
    }

    // getIconHtml removed as we use simple emojis directly

    async downloadTree() {
        if (this.currentFiles.length === 0) {
            this.showToast(this.t('pleaseScanFirst'), 'warning');
            return;
        }
        
        try {
            // Check if a search/filter is currently active
            const searchQuery = this.listFilterInput ? this.listFilterInput.value.trim() : '';
            const isSearchActive = searchQuery.length > 0;
            
            // Build visiblePaths set for filtered view (same logic as applySearch)
            let visiblePaths = null;
            if (isSearchActive) {
                visiblePaths = new Set();
                const lowerQuery = searchQuery.toLowerCase();
                this.currentFiles.forEach(file => {
                    if (file.name.toLowerCase().includes(lowerQuery)) {
                        let currentPath = file.path;
                        while (currentPath) {
                            visiblePaths.add(currentPath);
                            const lastSlash = Math.max(currentPath.lastIndexOf('/'), currentPath.lastIndexOf('\\'));
                            if (lastSlash === -1) break;
                            currentPath = currentPath.substring(0, lastSlash);
                        }
                    }
                });
                visiblePaths.add(this.currentPath);
            }
            
            // Generate plain text tree for download
            // Pass visiblePaths to ensure download reflects the filtered view
            const treeText = this.generateTreeContent(false, searchQuery, isSearchActive, visiblePaths);
            
            // Generate dynamic filename
            let filename = 'file_tree';
            if (isSearchActive) {
                // Include search keyword in filename
                const sanitizedKeyword = searchQuery.replace(/[^a-zA-Z0-9_]/g, '_').substring(0, 30);
                filename += `_search_${sanitizedKeyword}`;
            }
            filename += `_${new Date().getTime()}.txt`;
            
            const blob = new Blob([treeText], { type: 'text/plain' });
            const url = window.URL.createObjectURL(blob);
            const a = document.createElement('a');
            a.href = url;
            a.download = filename;
            document.body.appendChild(a);
            a.click();
            window.URL.revokeObjectURL(url);
            document.body.removeChild(a);
            
            const toastMsg = isSearchActive 
                ? this.t('downloadStartedSearch', { query: searchQuery }) 
                : this.t('downloadStarted');
            this.showToast(toastMsg, 'success');
        } catch (error) {
            this.showToast(this.t('downloadError', { message: error.message }), 'error');
        }
    }
    
    // browseDirectory() { ... (unchanged)

    async copyTreeToClipboard() {
        if (this.currentFiles.length === 0) {
            this.showToast(this.t('noFilesToCopy'), 'warning');
            return;
        }

        const query = this.listFilterInput ? this.listFilterInput.value.trim() : '';
        const isSearchActive = query.length > 0;
        
        // Use plain text for clipboard, matching current visible state
        const treeText = this.generateTreeContent(false, query, isSearchActive);
        
        if (!treeText || treeText === this.t('noFilesFound')) {
            this.showToast(this.t('noTreeToCopy'), 'warning');
            return;
        }
        
        try {
            await navigator.clipboard.writeText(treeText);
            this.showToast(this.t('treeCopied'), 'success');
        } catch (error) {
            this.showToast(this.t('copyFailed', { message: error.message }), 'error');
        }
    }

    clearResults() {
        // Hard Clear Logic
        this.directoryPathInput.value = ''; // Clear Path Input Box
        this.currentFiles = []; // Empty internal data
        this.currentPath = '';
        this.totalSize = 0;
        
        this.treeOutput.innerHTML = ''; // Clear DOM (Tree)
        this.fileTableBody.innerHTML = `<tr><td colspan="3" class="empty-message">${this.t('noFilesScanned')}</td></tr>`; // Clear DOM (Table)
        
        // Update Status UI
        this.currentPathElement.textContent = this.t('noPathSelected');
        this.fileCountElement.textContent = '0';
        this.totalSizeElement.textContent = '0 B';
        
        this.showToast(this.t('resultsCleared'), 'success');
    }
    
    getTreeOptions() {
        const excludePatterns = this.excludePatternsInput.value
            .split(',')
            .map(pattern => pattern.trim())
            .filter(pattern => pattern.length > 0);
        
        return {
            show_size: this.showSizeCheckbox.checked,
            human_readable: true, // Always true
            max_depth: parseInt(this.maxDepthInput.value) || -1,
            exclude_patterns: excludePatterns
        };
    }
    
    getFileIcon(filename, isDirectory) {
        if (isDirectory) return '📁';
        return '📄';
    }

    updateFileTable(query = '') {
        try {
            // Visual Feedback: Start processing
            this.fileTableBody.style.opacity = '0.6';

            // Update header UI
            document.querySelectorAll('.sticky-header thead th[data-sort]').forEach(th => {
                const key = th.getAttribute('data-sort');
                const icon = th.querySelector('.sort-icon');
                
                if (key === this.sortCriteria.key) {
                    th.classList.add('active-sort');
                    icon.className = `sort-icon fas fa-sort-${this.sortCriteria.direction === 'asc' ? 'up' : 'down'}`;
                } else {
                    th.classList.remove('active-sort');
                    icon.className = 'sort-icon fas fa-sort';
                }
            });

            const lowerQuery = query.toLowerCase();
            const filteredFiles = query 
                ? this.currentFiles.filter(file => file.name.toLowerCase().includes(lowerQuery))
                : this.currentFiles;

            // Sync Search Statistics
            const filteredSize = filteredFiles.reduce((sum, file) => sum + (file.size || 0), 0);
            if (query) {
                this.fileCountElement.textContent = this.t('foundFiles', { count: filteredFiles.length });
                this.totalSizeElement.textContent = `(${this.formatFileSize(filteredSize)})`;
            } else {
                this.fileCountElement.textContent = filteredFiles.length;
                this.totalSizeElement.textContent = this.formatFileSize(filteredSize);
            }

            if (this.currentFiles.length === 0) {
                this.fileTableBody.innerHTML = `<tr><td colspan="3" class="empty-message">${this.t('noFilesScanned')}</td></tr>`;
                return;
            }

            if (filteredFiles.length === 0 && query !== '') {
                this.fileTableBody.innerHTML = `<tr><td colspan="3" class="empty-message">${this.t('noMatchingFiles')}</td></tr>`;
                return;
            }

            // Build entire HTML string once to minimize reflows
            let htmlString = '';
            filteredFiles.forEach(file => {
                const size = this.formatFileSize(file.size || 0);
                const icon = this.getFileIcon(file.name, file.is_directory);
                
                let displayName = file.name;
                if (query) {
                    const regex = new RegExp(`(${query.replace(/[.*+?^${}()|[\]\\]/g, '\\$&')})`, 'gi');
                    displayName = file.name.replace(regex, '<mark class="highlight">$1</mark>');
                }

                htmlString += `
                    <tr data-path="${file.path.replace(/"/g, '&quot;')}">
                        <td>
                            <div class="file-name-cell" style="padding-left:${file.depth * 12}px">
                                <span class="file-icon">${icon}</span>
                                <span class="name-text" title="${file.name}">${displayName}</span>
                            </div>
                        </td>
                        <td class="col-size">${size}</td>
                        <td class="col-depth">${file.depth}</td>
                    </tr>
                `;
            });
            
            this.fileTableBody.innerHTML = htmlString;

            // Add event listeners
            const rows = this.fileTableBody.querySelectorAll('tr');
            rows.forEach(row => {
                row.addEventListener('dblclick', () => {
                    const filePath = row.getAttribute('data-path');
                    this.openItem(filePath);
                });
                row.style.cursor = 'pointer';
                row.title = this.t('doubleClickToOpen'); 
            });
        } catch (error) {
            console.error('Error updating file table:', error);
        } finally {
            this.fileTableBody.style.opacity = '1';
        }
    }
    
    formatFileSize(bytes) {
        if (bytes === 0) return '0 B';
        
        const k = 1024;
        const sizes = ['B', 'KB', 'MB', 'GB', 'TB'];
        const i = Math.floor(Math.log(bytes) / Math.log(k));
        
        return parseFloat((bytes / Math.pow(k, i)).toFixed(2)) + ' ' + sizes[i];
    }
    
    async checkServerStatus() {
        try {
            const response = await fetch(`${this.apiBaseUrl}/api/info`);
            if (response.ok) {
                const data = await response.json();
                this.serverStatusElement.textContent = this.t('connected');
                this.apiStatusElement.textContent = `API: v${data.version}`;
                this.showToast(this.t('serverConnected'), 'success');
            } else {
                this.serverStatusElement.textContent = this.t('serverDisconnected');
                this.apiStatusElement.textContent = this.t('apiNotAvailable');
                this.showToast(this.t('serverNotResponding'), 'error');
            }
        } catch (error) {
            this.serverStatusElement.textContent = this.t('serverDisconnected');
            this.apiStatusElement.textContent = this.t('apiNotAvailable');
            this.showToast(this.t('cannotConnect'), 'error');
        }
    }
    
    showToast(message, type = 'info') {
        const toastContainer = document.getElementById('toast-container');
        
        const toast = document.createElement('div');
        toast.className = `toast ${type}`;
        
        let icon = 'info-circle';
        if (type === 'success') icon = 'check-circle';
        if (type === 'error') icon = 'exclamation-circle';
        if (type === 'warning') icon = 'exclamation-triangle';
        
        toast.innerHTML = `
            <i class="fas fa-${icon}"></i>
            <span>${message}</span>
        `;
        
        toastContainer.appendChild(toast);
        
        // Remove toast after 5 seconds
        setTimeout(() => {
            toast.style.animation = 'slideIn 0.3s ease reverse';
            setTimeout(() => {
                if (toast.parentNode) {
                    toast.parentNode.removeChild(toast);
                }
            }, 300);
        }, 5000);
    }

    saveState() {
        const state = {
            path: this.directoryPathInput.value,
            settings: this.getTreeOptions(),
            // Don't save tree output HTML, regenerate it from files
            currentFiles: this.currentFiles,
            currentPath: this.currentPath,
            totalSize: this.totalSize,
            language: this.language,
            theme: this.theme
        };
        localStorage.setItem('fmg_state', JSON.stringify(state));
    }

    loadState() {
        const saved = localStorage.getItem('fmg_state');
        if (!saved) return;

        try {
            const state = JSON.parse(saved);
            
            if (state.language) {
                this.language = state.language;
            }

            if (state.theme) {
                this.theme = state.theme;
            }
            
            if (state.path) this.directoryPathInput.value = state.path;
            
            if (state.settings) {
                this.showSizeCheckbox.checked = state.settings.show_size;
                // humanReadableCheckbox removed
                this.maxDepthInput.value = state.settings.max_depth;
                if (state.settings.exclude_patterns) {
                    this.excludePatternsInput.value = state.settings.exclude_patterns.join(', ');
                }
            }
            
            if (state.currentFiles) {
                this.currentFiles = state.currentFiles;
                this.sortFiles();
                this.updateFileTable();
                
                // Regenerate tree view
                if (this.currentFiles.length > 0) {
                    this.treeOutput.innerHTML = this.generateTreeContent(true);
                }
            }
            
            if (state.currentPath) this.currentPathElement.textContent = state.currentPath;
            if (state.totalSize !== undefined) {
                this.totalSize = state.totalSize;
                this.totalSizeElement.textContent = this.formatFileSize(this.totalSize);
            }
            if (state.currentFiles) {
                this.fileCountElement.textContent = state.currentFiles.length;
            }

        } catch (e) {
            console.error("Failed to load state", e);
        }
    }

    handleSort(key) {
        if (this.sortCriteria.key === key) {
            this.sortCriteria.direction = this.sortCriteria.direction === 'asc' ? 'desc' : 'asc';
        } else {
            this.sortCriteria.key = key;
            this.sortCriteria.direction = 'asc';
        }
        
        this.sortFiles();
        this.updateFileTable(this.listFilterInput.value.trim());
    }

    sortFiles() {
        const { key, direction } = this.sortCriteria;
        const multiplier = direction === 'asc' ? 1 : -1;

        // Path-aware Hierarchical Sort
        // Priority 1: Hierarchy (Parent paths stay together)
        // Priority 2: Folder First (Directories before files at the same level)
        // Priority 3: Active Criteria (Name, Size, Depth, etc.)
        this.currentFiles.sort((a, b) => {
            if (a.path === b.path) return 0;

            const partsA = a.path.split(/[/\\]/).filter(Boolean);
            const partsB = b.path.split(/[/\\]/).filter(Boolean);
            const len = Math.min(partsA.length, partsB.length);

            for (let i = 0; i < len; i++) {
                if (partsA[i] !== partsB[i]) {
                    // Difference found at this level of the path
                    const isLastA = i === partsA.length - 1;
                    const isLastB = i === partsB.length - 1;
                    
                    // Determine if the current segments are directories
                    const isDirA = !isLastA || a.is_directory;
                    const isDirB = !isLastB || b.is_directory;

                    // Folder First Logic (within the same level)
                    if (isDirA !== isDirB) {
                        return isDirA ? -1 : 1;
                    }

                    // If both are at their leaf level, apply sort criteria
                    if (isLastA && isLastB) {
                        switch (key) {
                            case 'size':
                                const sizeDiff = (a.size || 0) - (b.size || 0);
                                if (sizeDiff !== 0) return multiplier * sizeDiff;
                                break;
                            case 'depth':
                                const depthDiff = (a.depth || 0) - (b.depth || 0);
                                if (depthDiff !== 0) return multiplier * depthDiff;
                                break;
                        }
                        // Default to natural name sorting within the same type
                        return multiplier * partsA[i].localeCompare(partsB[i], undefined, { numeric: true, sensitivity: 'base' });
                    }

                    // For intermediate directory segments, always use natural alphabetical sort
                    return partsA[i].localeCompare(partsB[i], undefined, { numeric: true });
                }
            }

            // If one path is a prefix of another, the parent comes first
            return partsA.length - partsB.length;
        });
    }

    applySearch(pruneTree = false) {
        const query = this.listFilterInput.value.trim();
        
        // 1. Update File Table (Real-time)
        this.updateFileTable(query);
        
        // 2. Update Tree
        // If pruneTree is true (Enter pressed), clear collapsed state to show full results
        if (pruneTree && query !== '') {
            this.collapsedPaths.clear();
        }

        // Re-generate tree using unified logic
        this.generateTree();
    }
}

// Initialize the app when the page loads
window.addEventListener('DOMContentLoaded', () => {
    // Expose instance globally for addons
    window.fileManager = new FileManagerApp();
});

// Add some CSS for the file table
const style = document.createElement('style');
style.textContent = `
    .type-badge {
        display: inline-block;
        padding: 3px 8px;
        border-radius: 12px;
        font-size: 0.85rem;
        font-weight: 600;
    }
    
    .type-badge.directory {
        background-color: #e3f2fd;
        color: #1976d2;
    }
    
    .type-badge.file {
        background-color: #f3e5f5;
        color: #7b1fa2;
    }
    
    .drag-over {
        background-color: rgba(67, 97, 238, 0.2) !important;
        border-color: #3a56d4 !important;
    }
    
    .file-name-cell {
        display: flex;
        align-items: center;
        gap: 10px;
        white-space: normal;
        word-break: break-all;
    }
    
    .file-name-cell i {
        /* min-width: 20px; */
        /* text-align: center; */
        /* flex-shrink: 0; */
        /* font-size: 1.1em; */
    }
    
    /* Tree View Icons */
    .tree-icon {
        margin-right: 5px;
        min-width: 16px;
        text-align: center;
        display: inline-block;
    }
    
    .tree-item-name {
        /* font-weight: 500; */
    }
`;
document.head.appendChild(style);