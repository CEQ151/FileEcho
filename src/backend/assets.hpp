#pragma once
#include <string>
#include <algorithm>

namespace assets {

const char* INDEX_HTML = R"rawhtml(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>FileEcho - File Manager Web GUI</title>
    <link href="https://fonts.googleapis.com/css2?family=Inter:wght@300;400;500;600;700&display=swap" rel="stylesheet">
    <link href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.4.0/css/all.min.css" rel="stylesheet">
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0-alpha1/dist/css/bootstrap.min.css" rel="stylesheet">
    <style>
{{STYLE_CSS}}
    </style>
</head>
<body>
    <div class="app-layout">
        <!-- Sidebar -->
        <aside class="sidebar" id="sidebar">
            <div class="sidebar-header">
                <div class="logo">
                    <i class="fas fa-network-wired"></i>
                    <span class="logo-text">FileEcho</span>
                    <span class="badge rounded-pill bg-primary">v1.0.4</span>
                </div>
            </div>

            <div class="sidebar-scroll">
                <div class="panel info-panel">
                    <div class="panel-header">
                        <i class="fas fa-chart-pie"></i> File Info
                    </div>
                    <div class="info-content">
                        <div class="info-item" title="Current Path">
                            <i class="fas fa-folder-open"></i>
                            <span id="current-path" class="text-ellipsis">No path selected</span>
                        </div>
                        <div class="info-item">
                            <i class="fas fa-file-alt"></i>
                            <span id="file-count">0</span> Files
                        </div>
                        <div class="info-item">
                            <i class="fas fa-hdd"></i>
                            <span id="total-size">0 B</span>
                        </div>
                    </div>
                </div>

                <div class="panel">
                    <div class="panel-header">
                        <i class="fas fa-search"></i> Target Address
                    </div>
                    <div class="input-group-compact d-flex gap-2">
                        <input type="text" id="directory-path" class="form-control-compact" placeholder="Absolute path...">
                        <button id="scan-btn" class="btn btn-primary btn-sm btn-icon-only">
                            <i class="fas fa-search"></i>
                        </button>
                    </div>
                </div>

                <div class="panel">
                    <div class="panel-header">
                        <i class="fas fa-sliders-h"></i> Settings
                    </div>
                    <div class="settings-grid">
                        <div class="setting-row">
                            <label for="max-depth">Max Depth</label>
                            <input type="number" id="max-depth" value="-1" min="-1" class="form-control-compact">
                        </div>
                        <div class="setting-row">
                            <label for="exclude-patterns">Excludes</label>
                            <input type="text" id="exclude-patterns" placeholder="node_modules, .git" class="form-control-compact">
                        </div>
                        <div class="setting-row checkbox-row">
                            <input type="checkbox" id="show-size" checked>
                            <label for="show-size">Show Size</label>
                        </div>
                    </div>
                </div>

                <div class="panel">
                    <div class="panel-header">
                        <i class="fas fa-trash-alt"></i> Cleanup
                    </div>
                    <div class="d-grid gap-2">
                        <button id="clear-btn" class="btn btn-sm btn-outline-secondary">
                            <i class="fas fa-broom"></i> Clear Results
                        </button>
                        <button id="clear-all-btn" class="btn btn-sm btn-outline-danger">
                            <i class="fas fa-trash"></i> Reset All Data
                        </button>
                    </div>
                </div>
            </div>

            <div class="sidebar-footer">
                <button id="github-btn" class="btn btn-github w-100">
                    <i class="fab fa-github"></i> View on GitHub
                </button>
            </div>
        </aside>

        <!-- Main Content -->
        <main class="main-content" id="main-content">
            <header class="top-bar">
                <div class="top-bar-left">
                    <button id="sidebar-toggle-btn" class="toggle-btn-main">
                        <i class="fas fa-bars"></i>
                    </button>
                </div>
                <div class="top-bar-right">
                    <div class="status-bar">
                        <span id="status-indicator" class="status-indicator active"></span>
                        <span id="server-status">Connected</span>
                        <span id="api-status" class="api-version"></span>
                    </div>
                </div>
            </header>

            <div class="content-split">
                <!-- Left: Tree View -->
                <section class="split-column tree-section">
                    <div class="column-header">
                        <h3><i class="fas fa-stream"></i> File Tree</h3>
                    </div>
                    <div class="tree-viewport">
                        <pre id="tree-output">Enter path in sidebar and scan.</pre>
                    </div>
                    <div class="column-actions-bar">
                        <button id="generate-tree-btn" class="btn btn-outline-secondary btn-sm flex-fill">
                            <i class="fas fa-sync"></i> Refresh
                        </button>
                        <button id="copy-btn" class="btn btn-outline-secondary btn-sm flex-fill">
                            <i class="fas fa-copy"></i> Copy
                        </button>
                        <button id="download-btn" class="btn btn-outline-secondary btn-sm flex-fill">
                            <i class="fas fa-download"></i> Download
                        </button>
                    </div>
                </section>

                <!-- Resizer -->
                <div id="resizer" class="resizer"></div>

                <!-- Right: File List -->
                <section class="split-column list-section">
                    <div class="column-header">
                        <h3><i class="fas fa-list"></i> File Details</h3>
                        <div class="filter-box">
                            <i class="fas fa-search"></i>
                            <input type="text" id="list-filter" placeholder="Filter current list (Press Enter to prune tree)...">
                        </div>
                    </div>
                    <div class="table-viewport">
                        <table class="table table-sm table-hover sticky-header">
                            <thead>
                                <tr>
                                    <th data-sort="name">Name <i class="sort-icon fas fa-sort"></i></th>
                                    <th data-sort="type">Type <i class="sort-icon fas fa-sort"></i></th>
                                    <th data-sort="size">Size <i class="sort-icon fas fa-sort"></i></th>
                                    <th data-sort="depth">Depth <i class="sort-icon fas fa-sort"></i></th>
                                </tr>
                            </thead>
                            <tbody id="file-table-body">
                                <tr>
                                    <td colspan="4" class="empty-message">No files scanned yet</td>
                                </tr>
                            </tbody>
                        </table>
                    </div>
                </section>
            </div>
        </main>
    </div>

    <div id="toast-container"></div>

    <script src="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0-alpha1/dist/js/bootstrap.bundle.min.js"></script>
    <script>
{{SCRIPT_JS}}
    </script>
</body>
</html>
)rawhtml";

const char* STYLE_CSS = R"rawcss(
:root {
    --primary-color: #4361ee;
    --primary-dark: #3a56d4;
    --sidebar-bg: #f8f9fa;
    --main-bg: #ffffff;
    --text-color: #2b2d42;
    --muted-color: #8d99ae;
    --border-color: #e2e8f0;
    --radius: 12px;
    --shadow: 0 4px 12px rgba(0, 0, 0, 0.05);
    --transition: all 0.3s cubic-bezier(0.4, 0, 0.2, 1);
}

* {
    margin: 0;
    padding: 0;
    box-sizing: border-box;
}

body {
    font-family: 'Inter', 'Segoe UI', system-ui, -apple-system, sans-serif;
    color: var(--text-color);
    background-color: #f0f2f5;
    overflow: hidden;
    height: 100vh;
    font-size: 0.9rem;
}

.app-layout {
    display: flex;
    width: 100vw;
    height: 100vh;
}

/* Sidebar Styling */
.sidebar {
    width: 280px;
    min-width: 0; /* 防止内容撑开 */
    background-color: var(--sidebar-bg);
    border-right: 1px solid var(--border-color);
    display: flex;
    flex-direction: column;
    transition: width 0.3s ease, margin-left 0.3s ease;
    flex-shrink: 0;
    position: relative;
    z-index: 100;
    overflow: hidden;
    white-space: nowrap; /* 防止收缩时文字换行难看 */
}

.sidebar.collapsed {
    width: 0;
    margin-left: -1px; /* 微调消除可能的缝隙 */
    border-right: none;
}

.sidebar-header {
    padding: 1rem;
    display: flex;
    align-items: center;
    justify-content: space-between;
    border-bottom: 1px solid var(--border-color);
    height: 48px;
}

.logo {
    display: flex;
    align-items: center;
    gap: 8px;
    white-space: nowrap;
}

.logo i {
    color: var(--primary-color);
    font-size: 1.1rem;
}

.logo-text {
    font-weight: 700;
    font-size: 1rem;
}

.sidebar-scroll {
    flex: 1;
    overflow-y: auto;
    padding: 0.75rem;
}

.panel {
    background: white;
    border-radius: var(--radius);
    padding: 0.75rem;
    margin-bottom: 0.75rem;
    box-shadow: var(--shadow);
    border: 1px solid var(--border-color);
}

.panel-header {
    font-size: 0.7rem;
    text-transform: uppercase;
    font-weight: 700;
    letter-spacing: 0.05em;
    color: var(--muted-color);
    margin-bottom: 0.5rem;
    display: flex;
    align-items: center;
    gap: 6px;
}

.info-content {
    display: flex;
    flex-direction: column;
    gap: 0.4rem;
}

.info-item {
    display: flex;
    align-items: center;
    gap: 8px;
    font-size: 0.8rem;
}

.text-ellipsis {
    white-space: nowrap;
    overflow: hidden;
    text-overflow: ellipsis;
}

.form-control-compact {
    width: 100%;
    padding: 0.3rem 0.6rem;
    font-size: 0.85rem;
    border-radius: 8px;
    border: 1px solid var(--border-color);
    outline: none;
    transition: var(--transition);
}

.form-control-compact:focus {
    border-color: var(--primary-color);
    box-shadow: 0 0 0 2px rgba(67, 97, 238, 0.1);
}

.btn-icon-only {
    padding: 0.3rem 0.6rem;
    display: flex;
    align-items: center;
    justify-content: center;
}

.settings-grid {
    display: flex;
    flex-direction: column;
    gap: 0.5rem;
}

.setting-row {
    display: flex;
    flex-direction: column;
    gap: 2px;
}

.setting-row label {
    font-size: 0.7rem;
    font-weight: 600;
}

.checkbox-row {
    flex-direction: row;
    align-items: center;
    gap: 8px;
    cursor: pointer;
    font-size: 0.8rem;
}

.sidebar-footer {
    padding: 0.75rem;
    border-top: 1px solid var(--border-color);
}

.btn-github {
    background: #24292e;
    color: white !important;
    border-radius: var(--radius);
    padding: 0.5rem;
    font-weight: 600;
    font-size: 0.85rem;
}

.btn-github:hover {
    background: #1b1f23;
}

/* Main Content Area */
.main-content {
    flex: 1;
    display: flex;
    flex-direction: column;
    min-width: 0; /* 允许 Flex 项收缩 */
    background-color: var(--main-bg);
    transition: all 0.3s ease;
}

.top-bar {
    height: 48px;
    padding: 0 1rem;
    display: flex;
    align-items: center;
    justify-content: space-between;
    border-bottom: 1px solid var(--border-color);
    background: white;
}

.toggle-btn-main {
    background: none;
    border: none;
    color: var(--text-color);
    font-size: 1.1rem;
    cursor: pointer;
    padding: 5px 10px;
    border-radius: 8px;
    transition: var(--transition);
}

.toggle-btn-main:hover {
    background: #f1f3f5;
    color: var(--primary-color);
}

.status-bar {
    display: flex;
    align-items: center;
    gap: 10px;
    font-size: 0.75rem;
    color: var(--muted-color);
}

.status-indicator {
    width: 8px;
    height: 8px;
    border-radius: 50%;
    background: #ccc;
}

.status-indicator.active {
    background: #10b981;
    box-shadow: 0 0 6px #10b981;
}

.content-split {
    flex: 1;
    display: flex;
    overflow: hidden;
}

.split-column {
    display: flex;
    flex-direction: column;
}

/* Tree Toggle Styles */
.tree-toggle {
    cursor: pointer;
    user-select: none;
    color: var(--primary-color);
    font-family: monospace;
    font-weight: bold;
    margin-right: 4px;
    display: inline-block;
    width: 1.5em;
    text-align: center;
    transition: var(--transition);
    transform-origin: center;
    backface-visibility: hidden;
    box-sizing: border-box;
}

.tree-toggle:hover {
    color: var(--primary-dark);
    transform: scale(1.1);
}

.tree-section {
    width: 300px;
    min-width: 200px;
    max-width: 500px;
}

.list-section {
    flex: 1;
    min-width: 0;
}

.column-header {
    padding: 0.5rem 1rem;
    border-bottom: 1px solid var(--border-color);
    background: #fff;
}

.filter-box {
    display: flex;
    align-items: center;
    background: #f1f3f5;
    border-radius: 8px;
    padding: 0.2rem 0.6rem;
    border: 1px solid transparent;
    transition: var(--transition);
    margin-top: 0.5rem;
}

.filter-box:focus-within {
    background: white;
    border-color: var(--primary-color);
    box-shadow: 0 0 0 2px rgba(67, 97, 238, 0.1);
}

.filter-box i {
    color: var(--muted-color);
    font-size: 0.8rem;
    margin-right: 8px;
}

.filter-box input {
    border: none;
    background: transparent;
    outline: none;
    font-size: 0.85rem;
    width: 100%;
    color: var(--text-color);
}

.highlight {
    background-color: #fff3bf;
    color: #d9480f;
    padding: 0 2px;
    border-radius: 2px;
    font-weight: 600;
}

.column-header h3 {
    font-size: 0.85rem;
    font-weight: 700;
    margin: 0;
    color: var(--muted-color);
}

.tree-viewport {
    flex: 1;
    overflow-x: auto;
    overflow-y: scroll;
    padding: 0.75rem;
}

.table-viewport {
    flex: 1;
    overflow-y: auto;
    padding: 0;
}

.tree-viewport pre {
    font-family: 'JetBrains Mono', 'Consolas', monospace;
    font-size: 0.8rem;
    line-height: 1.5;
    margin: 0;
}

.column-actions-bar {
    padding: 0.5rem 0.75rem;
    border-top: 1px solid var(--border-color);
    display: flex;
    gap: 6px;
    background: #fff;
}

.column-actions-bar .btn {
    padding: 2px 0; /* 紧凑高度 */
    font-size: 0.75rem; /* 小字体 */
    height: 28px;
    display: flex;
    align-items: center;
    justify-content: center;
}

.sticky-header thead th {
    position: sticky;
    top: 0;
    background: #ffffff;
    z-index: 10;
    font-size: 0.75rem;
    text-transform: uppercase;
    color: var(--muted-color);
    border-bottom: 1px solid var(--border-color);
    box-shadow: 0 1px 2px rgba(0,0,0,0.1);
    cursor: pointer;
    user-select: none;
    transition: background-color 0.2s;
    white-space: nowrap;
}

.sticky-header thead th:hover {
    background-color: #f8f9fa;
    color: var(--primary-color);
}

.sticky-header thead th.active-sort {
    color: var(--primary-color);
    border-bottom: 2px solid var(--primary-color);
}

.sticky-header thead th .sort-icon {
    margin-left: 5px;
    font-size: 0.7rem;
    opacity: 0.3;
    transition: var(--transition);
}

.sticky-header thead th.active-sort .sort-icon {
    opacity: 1;
}

#file-table-body {
    transition: opacity 0.15s ease-in-out;
}

.table-sm td {
    padding: 0.2rem 0.5rem !important;
    font-size: 0.8rem;
    vertical-align: middle;
    max-width: 0;
    overflow: hidden;
    text-overflow: ellipsis;
    white-space: nowrap;
}

/* Global Rounded Corners & Buttons */
.btn {
    border-radius: 10px !important;
    font-weight: 500;
}

.panel, .form-control-compact, .btn {
    border-radius: var(--radius);
}

#toast-container {
    position: fixed;
    bottom: 1.5rem;
    right: 1.5rem;
    z-index: 1000;
}

.toast {
    background: white;
    border-radius: var(--radius);
    padding: 0.6rem 1rem;
    margin-top: 0.5rem;
    box-shadow: 0 10px 25px rgba(0,0,0,0.1);
    display: flex;
    align-items: center;
    gap: 10px;
    font-size: 0.85rem;
    animation: slideIn 0.3s ease;
}

@keyframes slideIn {
    from { transform: translateX(100%); opacity: 0; }
    to { transform: translateX(0); opacity: 1; }
}

/* Global Rounded Corners */
.btn {
    border-radius: 8px !important;
}

#toast-container {
    position: fixed;
    bottom: 20px;
    right: 20px;
    z-index: 1000;
}

.toast {
    background: white;
    border-radius: var(--radius);
    padding: 0.75rem 1rem;
    margin-top: 10px;
    box-shadow: 0 10px 25px rgba(0,0,0,0.1);
    display: flex;
    align-items: center;
    gap: 10px;
    animation: slideIn 0.3s ease;
}

@keyframes slideIn {
    from { transform: translateX(100%); opacity: 0; }
    to { transform: translateX(0); opacity: 1; }
}

.checkbox-label input[type="checkbox"] {
    display: none;
}

.checkbox-custom {
    width: 20px;
    height: 20px;
    border: 2px solid var(--primary-color);
    border-radius: 4px;
    display: flex;
    align-items: center;
    justify-content: center;
    transition: var(--transition);
}

.checkbox-label input[type="checkbox"]:checked + .checkbox-custom {
    background-color: var(--primary-color);
}

.checkbox-label input[type="checkbox"]:checked + .checkbox-custom::after {
    content: '✓';
    color: white;
    font-weight: bold;
}

.setting-item input[type="number"],
.setting-item input[type="text"] {
    padding: 8px 12px;
    border: 1px solid var(--border-color);
    border-radius: 4px;
    font-family: inherit;
}

.setting-item small {
    color: var(--gray-color);
    font-size: 0.85rem;
}

/* Info Box */
.info-box {
    background-color: white;
    border-radius: var(--border-radius);
    padding: 15px;
    box-shadow: var(--shadow);
}

.info-box p {
    display: flex;
    align-items: center;
    gap: 10px;
    margin-bottom: 10px;
    color: var(--gray-color);
}

.info-box i {
    color: var(--primary-color);
    width: 20px;
}

/* Content Area */
.content {
    flex: 1;
    padding: 25px;
    overflow-y: auto;
    display: flex;
    flex-direction: column;
    gap: 25px;
}

.content-header {
    display: flex;
    justify-content: space-between;
    align-items: center;
}

.content-header h2 {
    display: flex;
    align-items: center;
    gap: 10px;
    color: var(--primary-color);
}

.actions {
    display: flex;
    gap: 10px;
}

/* Buttons */
.btn {
    padding: 10px 20px;
    border: none;
    border-radius: var(--border-radius);
    font-family: inherit;
    font-weight: 600;
    cursor: pointer;
    transition: var(--transition);
    display: flex;
    align-items: center;
    gap: 8px;
}

.btn-primary {
    background-color: var(--primary-color);
    color: white;
}

.btn-primary:hover {
    background-color: var(--primary-dark);
    transform: translateY(-2px);
}

.btn-secondary {
    background-color: var(--secondary-color);
    color: white;
}

.btn-secondary:hover {
    background-color: #5a08a0;
    transform: translateY(-2px);
}

.btn-success {
    background-color: var(--success-color);
    color: white;
}

.btn-success:hover {
    background-color: #3db8dd;
    transform: translateY(-2px);
}

.btn-icon {
    padding: 8px 12px;
    background-color: transparent;
    color: var(--gray-color);
}

.btn-icon:hover {
    background-color: var(--light-gray);
    color: var(--dark-color);
}

/* Path Input */
.path-input {
    background-color: var(--light-color);
    padding: 20px;
    border-radius: var(--border-radius);
}

.path-input label {
    display: block;
    margin-bottom: 8px;
    font-weight: 500;
}

.input-group {
    display: flex;
    gap: 10px;
}

.input-group input {
    flex: 1;
    padding: 10px 15px;
    border: 1px solid var(--border-color);
    border-radius: var(--border-radius);
    font-family: inherit;
}

/* Results */
.results {
    background-color: var(--light-color);
    border-radius: var(--border-radius);
    overflow: hidden;
}

.results-header {
    display: flex;
    justify-content: space-between;
    align-items: center;
    padding: 15px 20px;
    background-color: white;
    border-bottom: 1px solid var(--border-color);
}

.results-header h3 {
    color: var(--primary-color);
}

.results-actions {
    display: flex;
    gap: 5px;
}

.tree-container {
    padding: 20px;
    max-height: 300px;
    overflow-y: auto;
    background-color: #1e1e1e;
    color: #d4d4d4;
    font-family: 'Consolas', 'Monaco', monospace;
    font-size: 14px;
    line-height: 1.5;
    border-radius: 0 0 var(--border-radius) var(--border-radius);
}

#tree-output {
    margin: 0;
    white-space: pre-wrap;
    word-break: break-all;
}

/* File List */
.file-list {
    background-color: var(--light-color);
    border-radius: var(--border-radius);
    padding: 20px;
    flex: 1;
    display: flex;
    flex-direction: column;
}

.file-list h3 {
    display: flex;
    align-items: center;
    gap: 10px;
    margin-bottom: 15px;
    color: var(--primary-color);
}

.table-container {
    flex: 1;
    overflow-y: auto;
    border: 1px solid var(--border-color);
    border-radius: var(--border-radius);
    background-color: white;
}

table {
    width: 100%;
    border-collapse: collapse;
}

thead {
    background-color: var(--light-gray);
    position: sticky;
    top: 0;
}

th {
    padding: 12px 15px;
    text-align: left;
    font-weight: 600;
    color: var(--dark-color);
    border-bottom: 2px solid var(--border-color);
}

td {
    padding: 10px 15px;
    border-bottom: 1px solid var(--border-color);
}

tbody tr:hover {
    background-color: rgba(67, 97, 238, 0.05);
}

.empty-message {
    text-align: center;
    color: var(--gray-color);
    font-style: italic;
    padding: 40px !important;
}

/* Footer */
.footer {
    padding: 15px 30px;
    background-color: var(--light-gray);
    text-align: center;
    color: var(--gray-color);
    border-top: 1px solid var(--border-color);
}

/* Toast Notifications */
#toast-container {
    position: fixed;
    bottom: 20px;
    right: 20px;
    z-index: 1000;
}

.toast {
    background-color: white;
    border-radius: var(--border-radius);
    padding: 15px 20px;
    margin-bottom: 10px;
    box-shadow: var(--shadow-lg);
    display: flex;
    align-items: center;
    gap: 10px;
    animation: slideIn 0.3s ease;
    max-width: 350px;
}

.toast.success {
    border-left: 4px solid var(--success-color);
}

.toast.error {
    border-left: 4px solid var(--danger-color);
}

.toast.warning {
    border-left: 4px solid var(--warning-color);
}

.toast i {
    font-size: 1.2rem;
}

.toast.success i {
    color: var(--success-color);
}

.toast.error i {
    color: var(--danger-color);
}

.toast.warning i {
    color: var(--warning-color);
}

@keyframes slideIn {
    from {
        transform: translateX(100%);
        opacity: 0;
    }
    to {
        transform: translateX(0);
        opacity: 1;
    }
}

.resizer {
    width: 8px;
    background: transparent;
    cursor: col-resize;
    position: relative;
    z-index: 100;
    display: flex;
    justify-content: center;
    flex-shrink: 0;
}

.resizer::after {
    content: '';
    width: 1px;
    background-color: #e2e8f0;
    height: 100%;
}

/* Responsive Design */
@media (max-width: 1200px) {
    .main-content {
        flex-direction: column;
    }
    
    .sidebar {
        width: 100%;
        border-right: none;
        border-bottom: 1px solid var(--border-color);
    }
}

@media (max-width: 768px) {
    .header {
        flex-direction: column;
        gap: 15px;
        text-align: center;
    }
    
    .content-header {
        flex-direction: column;
        gap: 15px;
    }
    
    .actions {
        flex-wrap: wrap;
        justify-content: center;
    }
    
    .input-group {
        flex-direction: column;
    }
}

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

.tree-icon {
    margin-right: 5px;
    min-width: 16px;
    text-align: center;
    display: inline-block;
}
)rawcss";

const char* SCRIPT_JS = R"rawjs(
class FileManagerApp {
    constructor() {
        // 由于 set_html 加载的页面 origin 为空，必须硬编码 API 地址
        this.apiBaseUrl = "http://127.0.0.1:{{PORT}}";
        this.currentFiles = [];
        this.currentPath = '';
        this.totalSize = 0;
        this.sortCriteria = { key: 'name', direction: 'asc' };
        this.collapsedPaths = new Set(); // Stores paths of collapsed directories
        
        this.initElements();
        this.initEventListeners();
        this.disableContextMenu(); // 禁用右键菜单
        this.checkServerStatus();
        this.loadState(); // Load saved state on startup
    }
    
    disableContextMenu() {
        document.addEventListener('contextmenu', e => e.preventDefault());
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
    
    async openItem(path) {
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
                this.currentPathElement.textContent = this.currentPath;
                this.sortCriteria = { key: 'name', direction: 'asc' };
                this.sortFiles();
                this.updateFileTable();
                this.collapsedPaths.clear();
                this.showToast(`Found ${result.file_count} files/directories`, 'success');
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
            if (this.directoryPathInput.value.trim()) {
                await this.scanDirectory(false);
                if (this.currentFiles.length === 0) return;
            } else {
                this.showToast('Please scan a directory first', 'warning');
                return;
            }
        }
        
        try {
            const query = this.listFilterInput ? this.listFilterInput.value.trim() : '';
            const isSearchActive = query.length > 0;
            const treeHtml = this.generateTreeContent(true, query, isSearchActive);
            this.treeOutput.innerHTML = treeHtml;
            this.saveState();
        } catch (error) {
            this.showToast(`Tree generation error: ${error.message}`, 'error');
        }
    }
    
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

        const effectiveVisiblePaths = visiblePaths || (pruneTree && query ? pathsToKeep : null);

        const isCollapsedByAncestor = (filePath) => {
            for (const collapsedPath of this.collapsedPaths) {
                if (filePath.startsWith(collapsedPath) && filePath !== collapsedPath) {
                    const subChar = filePath[collapsedPath.length];
                    if (subChar === '/' || subChar === '\\') return true;
                }
            }
            return false;
        };

        let filesToDisplay = files.filter(f => {
            if (effectiveVisiblePaths && !effectiveVisiblePaths.has(f.path)) return false;
            if (isCollapsedByAncestor(f.path)) return false;
            return true;
        });

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
        
        const isLastAtDepth = [];
        for (let i = 0; i < filesToDisplay.length; i++) {
            const file = filesToDisplay[i];
            const isLast = (i === filesToDisplay.length - 1) || (filesToDisplay[i + 1].depth < file.depth);
            isLastAtDepth[file.depth] = isLast;
            for (let d = 1; d < file.depth; d++) {
                output += isLastAtDepth[d] ? '    ' : '│   ';
            }
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

    async downloadTree() {
        if (this.currentFiles.length === 0) {
            this.showToast('Please scan a directory first', 'warning');
            return;
        }
        try {
            const searchQuery = this.listFilterInput ? this.listFilterInput.value.trim() : '';
            const isSearchActive = searchQuery.length > 0;
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
            const treeText = this.generateTreeContent(false, searchQuery, isSearchActive, visiblePaths);
            let filename = 'file_tree';
            if (isSearchActive) {
                const sanitizedKeyword = searchQuery.replace(/[^a-zA-Z0-9_]/g, '_').substring(0, 30);
                filename += `_search_${sanitizedKeyword}`;
            }
            filename += `_${new Date().getTime()}.txt`;
            const blob = new Blob([treeText], { type: 'text/plain' });
            const url = window.URL.createObjectURL(blob);
            const a = document.createElement('a');
            a.href = url; a.download = filename;
            document.body.appendChild(a); a.click();
            window.URL.revokeObjectURL(url); document.body.removeChild(a);
            const toastMsg = isSearchActive ? `Download started (search: "${searchQuery}")` : 'Download started';
            this.showToast(toastMsg, 'success');
        } catch (error) {
            this.showToast(`Download error: ${error.message}`, 'error');
        }
    }
    
    async copyTreeToClipboard() {
        if (this.currentFiles.length === 0) {
            this.showToast('No files to copy', 'warning'); return;
        }
        const query = this.listFilterInput ? this.listFilterInput.value.trim() : '';
        const isSearchActive = query.length > 0;
        const treeText = this.generateTreeContent(false, query, isSearchActive);
        if (!treeText || treeText === 'No files found.') {
            this.showToast('No tree to copy', 'warning'); return;
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
        this.currentFiles = []; this.currentPath = ''; this.totalSize = 0;
        this.currentPathElement.textContent = 'No folder selected';
        this.fileCountElement.textContent = '0';
        this.totalSizeElement.textContent = '0 B';
        this.fileTableBody.innerHTML = '<tr><td colspan="4" class="empty-message">No files scanned yet</td></tr>';
        this.showToast('Results cleared', 'success');
    }
    
    clearAllData() {
        if (!confirm('Are you sure you want to clear all data and settings?')) return;
        localStorage.removeItem('fmg_state');
        this.clearResults();
        this.directoryPathInput.value = '';
        this.showSizeCheckbox.checked = true;
        this.maxDepthInput.value = -1;
        this.excludePatternsInput.value = '';
        this.showToast('All data cleared', 'success');
    }
    
    getTreeOptions() {
        const excludePatterns = this.excludePatternsInput.value.split(',').map(p => p.trim()).filter(p => p.length > 0);
        return {
            show_size: this.showSizeCheckbox.checked, human_readable: true,
            max_depth: parseInt(this.maxDepthInput.value) || -1, exclude_patterns: excludePatterns
        };
    }
    
    getFileIcon(filename, isDirectory) { return isDirectory ? '📁' : '📄'; }

    updateFileTable(query = '') {
        this.fileTableBody.style.opacity = '0.6';
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
        const filteredFiles = query ? this.currentFiles.filter(f => f.name.toLowerCase().includes(lowerQuery)) : this.currentFiles;
        const filteredSize = filteredFiles.reduce((sum, f) => sum + (f.size || 0), 0);
        if (query) {
            this.fileCountElement.textContent = `Found ${filteredFiles.length}`;
            this.totalSizeElement.textContent = `(${this.formatFileSize(filteredSize)})`;
        } else {
            this.fileCountElement.textContent = this.currentFiles.length;
            this.totalSizeElement.textContent = this.formatFileSize(filteredSize);
        }

        if (this.currentFiles.length === 0) {
            this.fileTableBody.innerHTML = '<tr><td colspan="4" class="empty-message">No files scanned yet</td></tr>';
            this.fileTableBody.style.opacity = '1'; return;
        }

        let htmlString = '';
        filteredFiles.forEach(file => {
            const typeClass = file.is_directory ? 'directory' : 'file';
            const icon = this.getFileIcon(file.name, file.is_directory);
            let displayName = file.name;
            if (query) {
                const regex = new RegExp(`(${query.replace(/[.*+?^${}()|[\]\\]/g, '\\$&')})`, 'gi');
                displayName = file.name.replace(regex, '<mark class="highlight">$1</mark>');
            }
            htmlString += `
                <tr data-path="${file.path.replace(/"/g, '&quot;')}">
                    <td><div class="file-name-cell"><span class="tree-icon">${icon}</span><span class="name-text">${displayName}</span></div></td>
                    <td><span class="type-badge ${typeClass}">${file.is_directory ? 'Dir' : 'File'}</span></td>
                    <td>${this.formatFileSize(file.size || 0)}</td>
                    <td>${file.depth}</td>
                </tr>
            `;
        });
        this.fileTableBody.innerHTML = htmlString;
        this.fileTableBody.style.opacity = '1';
        this.fileTableBody.querySelectorAll('tr').forEach(row => {
            row.addEventListener('dblclick', () => this.openItem(row.getAttribute('data-path')));
            row.style.cursor = 'pointer';
        });
    }
    
    formatFileSize(bytes) {
        if (bytes === 0) return '0 B';
        const k = 1024; const sizes = ['B', 'KB', 'MB', 'GB', 'TB'];
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
            } else {
                this.serverStatusElement.textContent = 'Disconnected';
            }
        } catch (error) { this.serverStatusElement.textContent = 'Disconnected'; }
    }
    
    showToast(message, type = 'info') {
        const container = document.getElementById('toast-container');
        const toast = document.createElement('div');
        toast.className = `toast ${type}`;
        toast.innerHTML = `<span>${message}</span>`;
        container.appendChild(toast);
        setTimeout(() => { if (toast.parentNode) toast.parentNode.removeChild(toast); }, 3000);
    }

    saveState() {
        const state = { path: this.directoryPathInput.value, settings: this.getTreeOptions(), currentFiles: this.currentFiles, currentPath: this.currentPath, totalSize: this.totalSize };
        localStorage.setItem('fmg_state', JSON.stringify(state));
    }

    loadState() {
        const saved = localStorage.getItem('fmg_state'); if (!saved) return;
        try {
            const state = JSON.parse(saved);
            if (state.path) this.directoryPathInput.value = state.path;
            if (state.currentFiles) {
                this.currentFiles = state.currentFiles;
                this.updateFileTable();
                if (this.currentFiles.length > 0) this.treeOutput.innerHTML = this.generateTreeContent(true);
            }
            if (state.currentPath) this.currentPathElement.textContent = state.currentPath;
            if (state.totalSize) this.totalSizeElement.textContent = this.formatFileSize(state.totalSize);
            if (state.currentFiles) this.fileCountElement.textContent = state.currentFiles.length;
        } catch (e) {}
    }

    handleSort(key) {
        if (this.sortCriteria.key === key) { this.sortCriteria.direction = this.sortCriteria.direction === 'asc' ? 'desc' : 'asc'; }
        else { this.sortCriteria.key = key; this.sortCriteria.direction = 'asc'; }
        this.sortFiles(); this.updateFileTable(this.listFilterInput.value.trim());
    }

    sortFiles() {
        const { key, direction } = this.sortCriteria;
        const multiplier = direction === 'asc' ? 1 : -1;
        this.currentFiles.sort((a, b) => {
            const partsA = a.path.split(/[/\\]/).filter(Boolean);
            const partsB = b.path.split(/[/\\]/).filter(Boolean);
            const len = Math.min(partsA.length, partsB.length);
            for (let i = 0; i < len; i++) {
                if (partsA[i] !== partsB[i]) {
                    const isDirA = (i < partsA.length - 1) || a.is_directory;
                    const isDirB = (i < partsB.length - 1) || b.is_directory;
                    if (isDirA !== isDirB) return isDirA ? -1 : 1;
                    if (i === partsA.length - 1 && i === partsB.length - 1) {
                        if (key === 'size') return multiplier * ((a.size || 0) - (b.size || 0));
                        return multiplier * partsA[i].localeCompare(partsB[i], undefined, { numeric: true });
                    }
                    return partsA[i].localeCompare(partsB[i], undefined, { numeric: true });
                }
            }
            return partsA.length - partsB.length;
        });
    }

    applySearch(pruneTree = false) {
        const query = this.listFilterInput.value.trim();
        this.updateFileTable(query);
        if (pruneTree && query !== '') this.collapsedPaths.clear();
        this.generateTree();
    }
}
window.addEventListener('DOMContentLoaded', () => { new FileManagerApp(); });
)rawjs";

inline std::string get_bundled_html(int port) {
    std::string html = INDEX_HTML;
    std::string css = STYLE_CSS;
    std::string js = SCRIPT_JS;

    // Replace CSS placeholder
    size_t style_pos = html.find("{{STYLE_CSS}}");
    if (style_pos != std::string::npos) {
        html.replace(style_pos, 13, css);
    }

    // Replace JS placeholder
    size_t script_pos = html.find("{{SCRIPT_JS}}");
    if (script_pos != std::string::npos) {
        html.replace(script_pos, 13, js);
    }

    // Replace Port placeholder in JS
    std::string port_str = std::to_string(port);
    size_t port_pos = html.find("{{PORT}}");
    while (port_pos != std::string::npos) {
        html.replace(port_pos, 8, port_str);
        port_pos = html.find("{{PORT}}", port_pos + port_str.length());
    }

    return html;
}

} // namespace assets
