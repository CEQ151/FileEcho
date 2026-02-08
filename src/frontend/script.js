class FileManagerApp {
    constructor() {
        this.apiBaseUrl = window.location.origin;
        this.currentFiles = [];
        this.currentPath = '';
        this.totalSize = 0;
        this.sortCriteria = { key: 'name', direction: 'asc' };
        
        this.initElements();
        this.initEventListeners();
        this.checkServerStatus();
        this.loadState(); // Load saved state on startup
    }
    
    initElements() {
        // Sidebar elements
        this.sidebar = document.getElementById('sidebar');
        this.sidebarToggleBtn = document.getElementById('sidebar-toggle-btn');
        this.githubBtn = document.getElementById('github-btn');

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
        this.clearAllBtn = document.getElementById('clear-all-btn'); 
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
        // Sidebar Toggle
        this.sidebarToggleBtn.addEventListener('click', () => this.toggleSidebar());

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
        this.clearAllBtn.addEventListener('click', () => this.clearAllData());
        
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
            this.showToast('Please enter a directory path', 'warning');
            return;
        }
        
        this.showToast('Scanning directory...', 'info');
        
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
                
                this.showToast(`Found ${result.file_count} files/directories`, 'success');
                
                // Auto generate tree if requested
                if (autoGenerate) {
                    await this.generateTree();
                }

                this.saveState();
            } else {
                this.showToast(`Scan failed: ${result.message}`, 'error');
            }
        } catch (error) {
            this.showToast(`Scan error: ${error.message}`, 'error');
        }
    }
    
    async generateTree() {
        if (this.currentFiles.length === 0) {
            // If no files, try scanning first
            if (this.directoryPathInput.value.trim()) {
                await this.scanDirectory(false); // prevent infinite loop
                if (this.currentFiles.length === 0) return;
            } else {
                this.showToast('Please scan a directory first', 'warning');
                return;
            }
        }
        
        try {
            const query = this.listFilterInput ? this.listFilterInput.value.trim() : '';
            // Generate tree on client side
            const treeHtml = this.generateTreeContent(true, query, false); // HTML format for display
            
            // For saving state and clipboard, we might want plain text
            // But for display, we use HTML
            this.treeOutput.innerHTML = treeHtml;
            this.saveState();
            // this.showToast('File tree generated successfully!', 'success');
        } catch (error) {
            this.showToast(`Tree generation error: ${error.message}`, 'error');
            console.error(error);
        }
    }
    
    // Generate tree content (HTML or Plain Text)
    generateTreeContent(asHtml, query = '', pruneTree = false, visiblePaths = null) {
        if (this.currentFiles.length === 0) return asHtml ? '' : 'No files found.';

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

        // Strictly ordered by hierarchical path for the tree view
        let filesToDisplay = effectiveVisiblePaths
            ? files.filter(f => effectiveVisiblePaths.has(f.path))
            : [...files];

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
            this.showToast('Please scan a directory first', 'warning');
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
            
            const toastMsg = isSearchActive ? `Download started (search: "${searchQuery}")` : 'Download started';
            this.showToast(toastMsg, 'success');
        } catch (error) {
            this.showToast(`Download error: ${error.message}`, 'error');
        }
    }
    
    // browseDirectory() { ... (unchanged)

    async copyTreeToClipboard() {
        // Use plain text for clipboard
        const treeText = this.generateTreeContent(false);
        
        if (!treeText || treeText === 'No files found.') {
            this.showToast('No tree to copy', 'warning');
            return;
        }
        
        try {
            await navigator.clipboard.writeText(treeText);
            this.showToast('Tree copied to clipboard!', 'success');
        } catch (error) {
            this.showToast(`Failed to copy: ${error.message}`, 'error');
        }
    }

    clearResults() {
        this.treeOutput.innerHTML = 'Select a folder and click "Generate Tree" to see the file structure here.';
        this.currentFiles = [];
        this.currentPath = '';
        this.totalSize = 0;
        
        // Update UI
        this.currentPathElement.textContent = 'No folder selected';
        this.fileCountElement.textContent = '0';
        this.totalSizeElement.textContent = '0 B';
        
        // Clear file table
        this.fileTableBody.innerHTML = '<tr><td colspan="4" class="empty-message">No files scanned yet</td></tr>';
        
        this.showToast('Results cleared', 'success');
    }
    
    clearAllData() {
        if (!confirm('Are you sure you want to clear all data and settings?')) return;
        
        localStorage.removeItem('fmg_state');
        this.clearResults();
        this.directoryPathInput.value = '';
        
        // Reset settings to defaults
        this.showSizeCheckbox.checked = true;
        // humanReadableCheckbox removed
        this.maxDepthInput.value = -1;
        this.excludePatternsInput.value = '';
        
        this.showToast('All data cleared', 'success');
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
            this.fileCountElement.textContent = `Found ${filteredFiles.length}`;
            this.totalSizeElement.textContent = `(${this.formatFileSize(filteredSize)})`;
        } else {
            this.fileCountElement.textContent = this.currentFiles.length;
            this.totalSizeElement.textContent = this.formatFileSize(filteredSize);
        }

        if (this.currentFiles.length === 0) {
            this.fileTableBody.innerHTML = '<tr><td colspan="4" class="empty-message">No files scanned yet</td></tr>';
            this.fileTableBody.style.opacity = '1';
            return;
        }

        if (filteredFiles.length === 0 && query !== '') {
            this.fileTableBody.innerHTML = '<tr><td colspan="4" class="empty-message">No matching files found</td></tr>';
            this.fileTableBody.style.opacity = '1';
            return;
        }

        // Build entire HTML string once to minimize reflows
        let htmlString = '';
        filteredFiles.forEach(file => {
            const type = file.is_directory ? 'Directory' : 'File';
            const typeClass = file.is_directory ? 'directory' : 'file';
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
                        <div class="file-name-cell">
                            <span class="file-icon">${icon}</span>
                            <span class="name-text" title="${file.name}">${displayName}</span>
                        </div>
                    </td>
                    <td><span class="type-badge ${typeClass}">${type}</span></td>
                    <td>${size}</td>
                    <td>${file.depth}</td>
                </tr>
            `;
        });
        
        this.fileTableBody.innerHTML = htmlString;
        this.fileTableBody.style.opacity = '1';

        // Add event listeners
        const rows = this.fileTableBody.querySelectorAll('tr');
        rows.forEach(row => {
            row.addEventListener('dblclick', () => {
                const filePath = row.getAttribute('data-path');
                this.openItem(filePath);
            });
            row.style.cursor = 'pointer';
            row.title = "Double-click to open"; 
        });
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
                this.serverStatusElement.textContent = 'Connected';
                this.apiStatusElement.textContent = `API: v${data.version}`;
                this.showToast('Server connected successfully', 'success');
            } else {
                this.serverStatusElement.textContent = 'Disconnected';
                this.apiStatusElement.textContent = 'API: Not available';
                this.showToast('Server not responding', 'error');
            }
        } catch (error) {
            this.serverStatusElement.textContent = 'Disconnected';
            this.apiStatusElement.textContent = 'API: Not available';
            this.showToast('Cannot connect to server', 'error');
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
            totalSize: this.totalSize
        };
        localStorage.setItem('fmg_state', JSON.stringify(state));
    }

    loadState() {
        const saved = localStorage.getItem('fmg_state');
        if (!saved) return;

        try {
            const state = JSON.parse(saved);
            
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
        // If query is empty, restore full tree
        // If pruneTree is true, re-generate tree with pruning
        if (pruneTree || query === '') {
            const treeHtml = this.generateTreeContent(true, query, pruneTree);
            this.treeOutput.innerHTML = treeHtml;
        }
    }
}

// Initialize the app when the page loads
window.addEventListener('DOMContentLoaded', () => {
    new FileManagerApp();
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