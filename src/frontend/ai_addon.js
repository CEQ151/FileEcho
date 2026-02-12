/**
 * FileEcho AI Addon
 * Integrated with Backend API
 * Features: i18n, resizable drawer, markdown rendering, file selection,
 *           folder popover, tree display, cascade selection, context limits,
 *           two-stage smart search, file content reading, PDF awareness
 */
document.addEventListener('DOMContentLoaded', () => {
    setTimeout(init, 100);
});

// ==========================================
// i18n Translation System
// ==========================================
const aiT_ = {
    en: {
        drawerTitle: 'FileEcho AI',
        settings: 'Settings',
        close: 'Close',
        notConfigured: 'Not Configured',
        notConfiguredHint: 'Not Configured — Enter API Key',
        connectedTo: 'Connected — ',
        provider: 'Provider',
        model: 'Model',
        apiKey: 'API Key',
        enterApiKey: 'Enter API Key',
        toggleVisibility: 'Toggle visibility',
        baseUrl: 'Base URL',
        temperature: 'Temperature',
        precise: 'Precise',
        balanced: 'Balanced',
        creative: 'Creative',
        wild: 'Wild',
        preciseTitle: 'Deterministic: Always pick the most likely token',
        balancedTitle: 'Low creativity: Focused & consistent',
        creativeTitle: 'Default: Good mix of coherence & variety',
        wildTitle: 'Maximum randomness: Highly varied output',
        tempDefaultHint: 'Default — Good balance of coherence and variety',
        maxTokens: 'Max Tokens',
        contextLimits: 'Context Limits',
        maxContentChars: 'Max content chars per file',
        maxFileCount: 'Max file count for AI',
        noLimitHint: '0 = unlimited',
        saveConfig: 'Save Configuration',
        cancel: 'Cancel',
        noFolderSelected: 'No folder selected',
        analyzeFile: 'Analyze specific file',
        welcomeMsg: 'Hello! I\'m your FileEcho AI assistant. I can help you analyze your files and project structure.',
        projectSummary: 'Project Summary',
        suggestions: 'Suggestions',
        codeAnalysis: 'Code Analysis',
        fileSummary: 'File Summary',
        smartSearch: 'Smart Search',
        annotateTree: 'Annotate Tree',
        typeMessage: 'Type your message...',
        thinking: 'Thinking...',
        items: 'items',
        configSaved: 'Configuration saved successfully!',
        errorSavingConfig: 'Error saving config: ',
        networkError: 'Network error saving config',
        errorLoadingConfig: 'Error loading configuration. Please check server connection.',
        folderUpdated: 'Folder updated: ',
        noFolderHint: 'No folder selected. Please choose a folder from the file tree on the left.',
        selectFolderFirst: 'Please select a folder first!',
        selectFolderSteps: 'Steps to analyze:\n1. Look at the file tree panel on the **left side**\n2. Enter or paste a folder path in the input field\n3. Click the **"Scan"** button\n4. Then try this command again',
        analyzing: 'Analyzing ',
        noFilesFound: 'No files found. Please scan a folder first.',
        noRegularFiles: 'No regular files found in current folder.',
        selectFileToAnalyze: 'Select a file to analyze:',
        typeFileNumber: 'Type the file number or name to get a summary.',
        analyzingFile: 'Analyzing ',
        fileNotFound: 'File not found. Please try again with a valid file number or name.',
        smartSearchActive: 'Smart Search Mode Active',
        smartSearchHint: 'Describe what you\'re looking for in natural language.\nExample: "configuration files", "database connection code", "image assets"\n\nType your search query below.',
        searchResultsFor: 'Search results for ',
        deepSearching: 'Deep searching file contents...',
        popoverAnalyzing: 'Analyzing...',
        popoverError: 'Unable to analyze.',
        popoverNetworkError: 'Network error.',
        aiStartedSummary: 'AI is analyzing this file...',
        selectedFiles: 'Selected {count} files',
        noSelection: 'No files selected',
        tempHint0: 'Deterministic — Always chooses the most likely output',
        tempHint1: 'Very precise — Minimal variation, highly focused',
        tempHint2: 'Balanced — Slightly creative while staying consistent',
        tempHint3: 'Moderate — Good mix of coherence and variety',
        tempHint4: 'Default — Recommended for most tasks',
        tempHint5: 'Creative — More diverse and surprising outputs',
        tempHint6: 'Very creative — Expect varied, less predictable results',
        tempHint7: 'Maximum randomness — Highly experimental output',
        alreadySaved: '••••••••  (already saved)',
        doubleClickHint: 'Double-click a row to open file',
        collapseAll: 'Collapse All',
        expandAll: 'Expand All',
        readingFile: 'Reading file content...',
        pdfFile: 'PDF document',
        binaryFile: 'Binary file (cannot read)',
        semanticSearchPrompt: 'You are a file search assistant. Given the following file tree and optional content samples, find files that match this search query: "{query}"\n\nFile tree:\n{files}\n\n{contentSection}Return ONLY the matching file names as a numbered list. If no files match, say so. Be concise.',
        newChat: 'New Chat',
        newChatConfirm: 'Chat history cleared.',
        resetConfig: 'Reset All',
        resetConfigConfirm: 'Are you sure you want to clear ALL AI settings and API keys? This cannot be undone.',
        resetConfigDone: 'All AI configuration and API keys have been cleared.',
        testConnection: 'Test Connection',
        testingConnection: 'Testing...',
        testSuccess: 'Connection successful!',
        testFailed: 'Connection failed: ',
        baseUrlHint: 'When using a third-party relay, check if /v1 is needed at the end of the URL',
        exportChat: 'Export Chat',
        exportAsTxt: 'Export as TXT',
        exportAsMd: 'Export as Markdown',
        tokenUsage: 'Tokens used this session',
    },
    zh: {
        drawerTitle: 'FileEcho AI',
        settings: '设置',
        close: '关闭',
        notConfigured: '未配置',
        notConfiguredHint: '未配置 — 请输入 API 密钥',
        connectedTo: '已连接 — ',
        provider: '服务提供商',
        model: '模型',
        apiKey: 'API 密钥',
        enterApiKey: '输入 API 密钥',
        toggleVisibility: '切换可见性',
        baseUrl: '自定义接口地址',
        temperature: '温度',
        precise: '精确',
        balanced: '平衡',
        creative: '创意',
        wild: '狂野',
        preciseTitle: '确定性输出：始终选择最高概率的词元',
        balancedTitle: '低创意：专注且一致',
        creativeTitle: '默认：兼顾连贯性与多样性',
        wildTitle: '最大随机性：高度多样化输出',
        tempDefaultHint: '默认 — 兼顾连贯性与多样性',
        maxTokens: '最大 Tokens',
        contextLimits: '上下文限制',
        maxContentChars: '单文件最大读取字符数',
        maxFileCount: 'AI 最大文件数',
        noLimitHint: '0 = 不限制',
        saveConfig: '保存配置',
        cancel: '取消',
        noFolderSelected: '未选择文件夹',
        analyzeFile: '分析指定文件',
        welcomeMsg: '你好！我是 FileEcho AI 助手，可以帮助你分析文件和项目结构。',
        projectSummary: '项目概述',
        suggestions: '调整建议',
        codeAnalysis: '代码分析',
        fileSummary: '文件摘要',
        smartSearch: '智能搜索',
        annotateTree: '注释树',
        typeMessage: '输入消息...',
        thinking: '思考中...',
        items: '项',
        configSaved: '配置保存成功！',
        errorSavingConfig: '保存配置失败：',
        networkError: '保存配置时网络错误',
        errorLoadingConfig: '加载配置出错，请检查服务器连接。',
        folderUpdated: '文件夹已更新：',
        noFolderHint: '未选择文件夹。请在左侧文件树中选择一个文件夹。',
        selectFolderFirst: '请先选择一个文件夹！',
        selectFolderSteps: '分析步骤：\n1. 查看左侧的**文件树面板**\n2. 在输入框中输入或粘贴文件夹路径\n3. 点击**"扫描"**按钮\n4. 然后重试此命令',
        analyzing: '正在分析 ',
        noFilesFound: '未找到文件，请先扫描文件夹。',
        noRegularFiles: '当前文件夹中没有普通文件。',
        selectFileToAnalyze: '选择要分析的文件：',
        typeFileNumber: '输入文件编号或名称以获取摘要。',
        analyzingFile: '正在分析 ',
        fileNotFound: '未找到该文件，请输入有效的文件编号或名称。',
        smartSearchActive: '智能搜索模式已激活',
        smartSearchHint: '用自然语言描述你要查找的内容。\n示例："配置文件"、"数据库连接代码"、"图片资源"\n\n请在下方输入搜索词。',
        searchResultsFor: '搜索结果：',
        deepSearching: '正在深度搜索文件内容...',
        popoverAnalyzing: '正在分析...',
        popoverError: '无法分析。',
        popoverNetworkError: '网络错误。',
        aiStartedSummary: 'AI 正在分析此文件...',
        selectedFiles: '已选择 {count} 个文件',
        noSelection: '未选择文件',
        tempHint0: '确定性 — 始终选择最高概率的输出',
        tempHint1: '非常精确 — 变化极小，高度聚焦',
        tempHint2: '平衡 — 保持一致性的同时略有创意',
        tempHint3: '适中 — 兼顾连贯与多样性',
        tempHint4: '默认 — 推荐用于大多数任务',
        tempHint5: '创意 — 更多样化和意外的输出',
        tempHint6: '高度创意 — 结果多变且不太可预测',
        tempHint7: '最大随机性 — 高度实验性输出',
        alreadySaved: '••••••••（已保存）',
        doubleClickHint: '双击行可以打开文件',
        collapseAll: '全部折叠',
        expandAll: '全部展开',
        readingFile: '正在读取文件内容...',
        pdfFile: 'PDF 文档',
        binaryFile: '二进制文件（无法读取）',
        semanticSearchPrompt: '你是一个文件搜索助手。根据以下文件树和可选的内容片段，找出与搜索查询匹配的文件："{query}"\n\n文件树：\n{files}\n\n{contentSection}只返回匹配的文件名编号列表。如果没有匹配的文件，请说明。简洁回答。',
        newChat: '新对话',
        newChatConfirm: '聊天记录已清除。',
        resetConfig: '清空所有设置',
        resetConfigConfirm: '确定要清除所有 AI 配置和 API 密钥吗？此操作不可恢复。',
        resetConfigDone: '所有 AI 配置和 API 密钥已清除。',
        testConnection: '测试连接',
        testingConnection: '正在测试...',
        testSuccess: '连接成功！',
        testFailed: '连接失败：',
        baseUrlHint: '使用第三方中转站时，请注意 URL 末尾是否需要加 /v1',
        exportChat: '导出记录',
        exportAsTxt: '导出为 TXT',
        exportAsMd: '导出为 Markdown',
        tokenUsage: '本次会话已使用 Tokens',
    }
};

function aiT(key, params = {}) {
    const lang = getCurrentLang();
    let text = (aiT_[lang] && aiT_[lang][key]) || aiT_.en[key] || key;
    for (const [k, v] of Object.entries(params)) {
        text = text.replace(`{${k}}`, v);
    }
    return text;
}

function getCurrentLang() {
    if (window.fileManager && window.fileManager.language) return window.fileManager.language;
    return localStorage.getItem('fileecho-lang') || 'en';
}

// ==========================================
// Binary / non-text extensions
// ==========================================
const BINARY_EXTENSIONS = new Set([
    '.exe','.dll','.so','.dylib','.bin','.dat','.o','.obj','.lib','.a',
    '.png','.jpg','.jpeg','.gif','.bmp','.ico','.svg','.webp','.tiff',
    '.mp3','.mp4','.avi','.mkv','.wav','.flac','.ogg','.mov','.wmv',
    '.zip','.rar','.7z','.tar','.gz','.bz2','.xz','.zst',
    '.ttf','.otf','.woff','.woff2','.eot',
    '.psd','.ai','.sketch','.fig',
    '.class','.pyc','.pyo','.wasm',
]);

function isBinaryExtension(name) {
    const dot = name.lastIndexOf('.');
    if (dot < 0) return false;
    return BINARY_EXTENSIONS.has(name.substring(dot).toLowerCase());
}

function isPdfExtension(name) {
    return name.toLowerCase().endsWith('.pdf');
}

// ==========================================
// State & Elements
// ==========================================
const state = {
    isAiDrawerOpen: false,
    apiBaseUrl: '/api/ai',
    config: {},
    models: [],
    providers: { 0: 'OpenAI', 1: 'DeepSeek', 2: 'Gemini', 3: 'Grok', 4: 'Claude', 5: 'Kimi (月之暗面)', 6: 'GLM (智谱)', 7: 'Custom' },
    collapsedDirs: new Set(),
    sessionTokens: { prompt: 0, completion: 0, total: 0 }
};

const elements = {};

function getFileContext() {
    if (!window.fileManager) return null;
    const path = window.fileManager.currentPath || '';
    const files = window.fileManager.currentFiles || [];
    const selectedFiles = window.fileManager.selectedFiles;
    const useSelected = selectedFiles && selectedFiles.size > 0;
    const effectiveFiles = useSelected ? files.filter(f => selectedFiles.has(f.path)) : files;
    return {
        path, files: effectiveFiles, allFiles: files,
        totalSize: window.fileManager.totalSize || 0,
        hasValidPath: path.length > 0 && files.length > 0,
        hasSelection: useSelected
    };
}

// ==========================================
// Build File Tree (for AI context)
// ==========================================
function buildFileTree(files, basePath) {
    if (!files || files.length === 0) return '';
    const maxCount = state.config.max_file_count || 0;
    const sorted = [...files].sort((a, b) => {
        const pa = a.path.replace(/\\/g, '/');
        const pb = b.path.replace(/\\/g, '/');
        return pa.localeCompare(pb);
    });
    let tree = '';
    let count = 0;
    for (const f of sorted) {
        if (maxCount > 0 && count >= maxCount) {
            tree += `  ... and ${sorted.length - maxCount} more items\n`;
            break;
        }
        const indent = '  '.repeat(f.depth || 0);
        tree += indent + f.name + (f.is_directory ? '/' : '') + '\n';
        count++;
    }
    return tree;
}

// ==========================================
// Init
// ==========================================
function init() {
    createAiElements();
    bindEvents();
    initDrawerResize();
    loadAiConfig();
    initPopover();
    listenLanguageChange();
    console.log('FileEcho AI addon initialized');
}

function listenLanguageChange() {
    const langSelect = document.getElementById('language-select');
    if (langSelect) {
        langSelect.addEventListener('change', () => { setTimeout(refreshAiLang, 50); });
    }
}

function refreshAiLang() {
    const d = elements.aiDrawer;
    if (!d) return;
    d.querySelector('.ai-drawer-title').innerHTML = '<i class="fas fa-robot"></i> ' + aiT('drawerTitle');
    d.querySelector('#ai-settings-btn').title = aiT('settings');
    d.querySelector('.ai-drawer-close').title = aiT('close');
    const labels = d.querySelectorAll('.ai-setting-group label');
    const labelKeys = ['provider', 'model', 'apiKey', 'baseUrl', 'temperature', 'maxTokens', 'contextLimits'];
    labels.forEach((l, i) => { if (labelKeys[i]) l.textContent = aiT(labelKeys[i]); });
    elements.apiKeyInput.placeholder = aiT('enterApiKey');
    d.querySelector('#ai-toggle-key-vis').title = aiT('toggleVisibility');
    d.querySelector('#ai-save-config').textContent = aiT('saveConfig');
    d.querySelector('#ai-cancel-config').textContent = aiT('cancel');
    elements.aiInput.placeholder = aiT('typeMessage');
    d.querySelector('#ai-select-file-btn').title = aiT('analyzeFile');
    const presets = d.querySelectorAll('.ai-temp-preset');
    const pk = ['precise', 'balanced', 'creative', 'wild'];
    const ptk = ['preciseTitle', 'balancedTitle', 'creativeTitle', 'wildTitle'];
    presets.forEach((btn, i) => { if (pk[i]) { btn.textContent = aiT(pk[i]); btn.title = aiT(ptk[i]); } });
    elements.tempHint.textContent = aiT('tempDefaultHint');
    // Update context limits labels
    const limitLabels = d.querySelectorAll('.ai-limit-row span');
    if (limitLabels.length >= 2) {
        limitLabels[0].textContent = aiT('maxContentChars');
        limitLabels[1].textContent = aiT('maxFileCount');
    }
    const limitHint = d.querySelector('.ai-limit-hint');
    if (limitHint) limitHint.textContent = aiT('noLimitHint');
    const chips = d.querySelectorAll('.ai-chip');
    const ck = ['projectSummary', 'suggestions', 'codeAnalysis', 'fileSummary', 'smartSearch', 'annotateTree'];
    const ci = ['fa-project-diagram', 'fa-lightbulb', 'fa-code', 'fa-file-alt', 'fa-brain', 'fa-tags'];
    chips.forEach((chip, i) => { if (ck[i]) chip.innerHTML = `<i class="fas ${ci[i]}"></i> ${aiT(ck[i])}`; });
    applyConfigToUI();
    updateAiContextDisplay();
    const hintBar = document.getElementById('dblclick-hint-bar');
    if (hintBar) hintBar.textContent = aiT('doubleClickHint');
    updateSelectionDisplay();
}

// ==========================================
// Create Elements
// ==========================================
function createAiElements() {
    const aiOrb = document.createElement('div');
    aiOrb.className = 'ai-orb';
    aiOrb.innerHTML = '<i class="fas fa-robot"></i>';
    document.body.appendChild(aiOrb);
    elements.aiOrb = aiOrb;

    const aiDrawer = document.createElement('div');
    aiDrawer.className = 'ai-drawer';
    aiDrawer.innerHTML = `
        <div class="ai-resize-handle" id="ai-resize-handle"></div>
        <div class="ai-drawer-header">
            <h3 class="ai-drawer-title"><i class="fas fa-robot"></i> ${aiT('drawerTitle')}</h3>
            <div class="ai-header-actions">
                <div class="ai-export-wrapper">
                    <button class="ai-icon-btn" id="ai-export-btn" title="${aiT('exportChat')}"><i class="fas fa-download"></i></button>
                    <div class="ai-export-menu" id="ai-export-menu">
                        <div class="ai-export-item" data-format="txt"><i class="fas fa-file-alt"></i> ${aiT('exportAsTxt')}</div>
                        <div class="ai-export-item" data-format="md"><i class="fas fa-file-code"></i> ${aiT('exportAsMd')}</div>
                    </div>
                </div>
                <button class="ai-icon-btn" id="ai-new-chat-btn" title="${aiT('newChat')}"><i class="fas fa-plus"></i></button>
                <button class="ai-icon-btn" id="ai-settings-btn" title="${aiT('settings')}"><i class="fas fa-cog"></i></button>
                <button class="ai-icon-btn ai-drawer-close" title="${aiT('close')}"><i class="fas fa-times"></i></button>
            </div>
        </div>
        <div class="ai-settings-panel" style="display: none;">
            <div id="ai-config-status" class="ai-config-status not-configured">
                <i class="fas fa-circle"></i>
                <span class="ai-config-status-text">${aiT('notConfigured')}</span>
            </div>
            <div class="ai-setting-group"><label>${aiT('provider')}</label><select id="ai-provider-select" class="ai-form-control"></select></div>
            <div class="ai-setting-group"><label>${aiT('model')}</label><select id="ai-model-select" class="ai-form-control"></select></div>
            <div class="ai-setting-group">
                <label>${aiT('apiKey')}</label>
                <div class="ai-apikey-wrapper">
                    <input type="password" id="ai-api-key" class="ai-form-control" placeholder="${aiT('enterApiKey')}">
                    <button id="ai-toggle-key-vis" class="ai-icon-btn small" title="${aiT('toggleVisibility')}" type="button"><i class="fas fa-eye"></i></button>
                </div>
            </div>
            <div class="ai-setting-group" id="ai-base-url-group" style="display: none;"><label>${aiT('baseUrl')}</label><input type="text" id="ai-base-url" class="ai-form-control" placeholder="https://api.example.com/v1"><div class="ai-base-url-hint">${aiT('baseUrlHint')}</div></div>
            <div class="ai-setting-group">
                <label>${aiT('temperature')}</label>
                <div class="ai-temp-control">
                    <div class="ai-temp-presets">
                        <button class="ai-temp-preset" data-temp="0.0" title="${aiT('preciseTitle')}">${aiT('precise')}</button>
                        <button class="ai-temp-preset" data-temp="0.3" title="${aiT('balancedTitle')}">${aiT('balanced')}</button>
                        <button class="ai-temp-preset" data-temp="0.7" title="${aiT('creativeTitle')}">${aiT('creative')}</button>
                        <button class="ai-temp-preset" data-temp="1.0" title="${aiT('wildTitle')}">${aiT('wild')}</button>
                    </div>
                    <div class="ai-temp-slider-row">
                        <input type="range" id="ai-temperature" class="ai-range" min="0" max="2" step="0.05" value="0.7">
                        <span id="ai-temp-val" class="ai-temp-value">0.70</span>
                    </div>
                    <div class="ai-temp-labels"><span>0</span><span>|</span><span>1</span><span>|</span><span>2</span></div>
                    <div class="ai-temp-hint" id="ai-temp-hint">${aiT('tempDefaultHint')}</div>
                </div>
            </div>
            <div class="ai-setting-group"><label>${aiT('maxTokens')}</label><input type="number" id="ai-max-tokens" class="ai-form-control" value="4096" min="1" max="8192" step="256"></div>
            <div class="ai-setting-group">
                <label>${aiT('contextLimits')}</label>
                <div class="ai-context-limits">
                    <div class="ai-limit-row">
                        <span>${aiT('maxContentChars')}</span>
                        <input type="number" id="ai-max-content-chars" class="ai-form-control small-input" value="0" min="0" step="1000" placeholder="0">
                    </div>
                    <div class="ai-limit-row">
                        <span>${aiT('maxFileCount')}</span>
                        <input type="number" id="ai-max-file-count" class="ai-form-control small-input" value="0" min="0" step="50" placeholder="0">
                    </div>
                    <div class="ai-limit-hint">${aiT('noLimitHint')}</div>
                </div>
            </div>
            <div class="ai-actions">
                <button id="ai-save-config" class="ai-btn primary">${aiT('saveConfig')}</button>
                <button id="ai-test-connection" class="ai-btn secondary" style="background:#17a2b8;color:#fff;">${aiT('testConnection')}</button>
                <button id="ai-cancel-config" class="ai-btn secondary">${aiT('cancel')}</button>
                <button id="ai-reset-config" class="ai-btn danger" style="background:#dc3545;margin-top:6px;width:100%;">${aiT('resetConfig')}</button>
            </div>
        </div>
        <div class="ai-drawer-body">
            <div class="ai-context-panel">
                <div class="ai-context-info">
                    <i class="fas fa-folder-open"></i> <span id="ai-current-path">${aiT('noFolderSelected')}</span>
                    <span id="ai-file-count" class="badge">0 ${aiT('items')}</span>
                </div>
                <button id="ai-select-file-btn" class="ai-icon-btn small" title="${aiT('analyzeFile')}"><i class="fas fa-file-code"></i></button>
            </div>
            <div class="ai-chat-history" id="ai-chat-history">
                <div class="ai-message ai">${aiT('welcomeMsg')}</div>
            </div>
            <div class="ai-suggestions">
                <span class="ai-chip" data-command="project-summary"><i class="fas fa-project-diagram"></i> ${aiT('projectSummary')}</span>
                <span class="ai-chip" data-command="cleanup-suggestions"><i class="fas fa-lightbulb"></i> ${aiT('suggestions')}</span>
                <span class="ai-chip" data-command="code-analysis"><i class="fas fa-code"></i> ${aiT('codeAnalysis')}</span>
                <span class="ai-chip" data-command="file-summary"><i class="fas fa-file-alt"></i> ${aiT('fileSummary')}</span>
                <span class="ai-chip" data-command="semantic-search"><i class="fas fa-brain"></i> ${aiT('smartSearch')}</span>
                <span class="ai-chip" data-command="annotate-tree"><i class="fas fa-tags"></i> ${aiT('annotateTree')}</span>
            </div>
        </div>
        <div class="ai-drawer-footer">
            <div class="ai-token-bar" id="ai-token-bar" style="display:none;">
                <i class="fas fa-coins"></i> <span id="ai-token-text">Tokens: 0</span>
            </div>
            <div class="ai-input-row">
                <input type="text" class="ai-input" placeholder="${aiT('typeMessage')}">
                <button class="ai-send-btn"><i class="fas fa-paper-plane"></i></button>
            </div>
        </div>
    `;
    document.body.appendChild(aiDrawer);

    elements.aiDrawer = aiDrawer;
    elements.aiOrb = aiOrb;
    elements.aiDrawerClose = aiDrawer.querySelector('.ai-drawer-close');
    elements.aiSettingsBtn = aiDrawer.querySelector('#ai-settings-btn');
    elements.aiSettingsPanel = aiDrawer.querySelector('.ai-settings-panel');
    elements.aiChatBody = aiDrawer.querySelector('.ai-drawer-body');
    elements.aiChatHistory = aiDrawer.querySelector('#ai-chat-history');
    elements.aiInput = aiDrawer.querySelector('.ai-input');
    elements.aiSendBtn = aiDrawer.querySelector('.ai-send-btn');
    elements.currentPath = aiDrawer.querySelector('#ai-current-path');
    elements.fileCount = aiDrawer.querySelector('#ai-file-count');
    elements.selectFileBtn = aiDrawer.querySelector('#ai-select-file-btn');
    elements.providerSelect = aiDrawer.querySelector('#ai-provider-select');
    elements.modelSelect = aiDrawer.querySelector('#ai-model-select');
    elements.apiKeyInput = aiDrawer.querySelector('#ai-api-key');
    elements.baseUrlInput = aiDrawer.querySelector('#ai-base-url');
    elements.baseUrlGroup = aiDrawer.querySelector('#ai-base-url-group');
    elements.tempInput = aiDrawer.querySelector('#ai-temperature');
    elements.tempVal = aiDrawer.querySelector('#ai-temp-val');
    elements.saveConfigBtn = aiDrawer.querySelector('#ai-save-config');
    elements.cancelConfigBtn = aiDrawer.querySelector('#ai-cancel-config');
    elements.configStatus = aiDrawer.querySelector('#ai-config-status');
    elements.configStatusText = aiDrawer.querySelector('.ai-config-status-text');
    elements.toggleKeyVisBtn = aiDrawer.querySelector('#ai-toggle-key-vis');
    elements.maxTokensInput = aiDrawer.querySelector('#ai-max-tokens');
    elements.tempHint = aiDrawer.querySelector('#ai-temp-hint');
    elements.maxContentCharsInput = aiDrawer.querySelector('#ai-max-content-chars');
    elements.maxFileCountInput = aiDrawer.querySelector('#ai-max-file-count');

    // Restore saved width
    const savedWidth = localStorage.getItem('ai-drawer-width');
    if (savedWidth) {
        const w = parseInt(savedWidth);
        if (w >= 320 && w <= window.innerWidth * 0.8) {
            aiDrawer.style.width = w + 'px';
        }
    }
}

// ==========================================
// Drawer Resize
// ==========================================
function initDrawerResize() {
    const handle = elements.aiDrawer.querySelector('#ai-resize-handle');
    if (!handle) return;
    let startX, startWidth;
    handle.addEventListener('mousedown', (e) => {
        e.preventDefault();
        startX = e.clientX;
        startWidth = elements.aiDrawer.offsetWidth;
        document.body.style.cursor = 'col-resize';
        document.body.style.userSelect = 'none';
        const onMove = (ev) => {
            const diff = startX - ev.clientX;
            const newW = Math.max(320, Math.min(startWidth + diff, window.innerWidth * 0.8));
            elements.aiDrawer.style.width = newW + 'px';
        };
        const onUp = () => {
            document.removeEventListener('mousemove', onMove);
            document.removeEventListener('mouseup', onUp);
            document.body.style.cursor = '';
            document.body.style.userSelect = '';
            localStorage.setItem('ai-drawer-width', elements.aiDrawer.offsetWidth);
        };
        document.addEventListener('mousemove', onMove);
        document.addEventListener('mouseup', onUp);
    });
}

// ==========================================
// Bind Events
// ==========================================
function bindEvents() {
    elements.aiOrb.addEventListener('click', () => {
        if (elements.aiDrawer.classList.contains('open')) {
            elements.aiDrawer.classList.remove('open');
            elements.aiOrb.style.display = 'flex';
        } else {
            elements.aiDrawer.classList.add('open');
            elements.aiOrb.style.display = 'none';
            updateAiContextDisplay();
        }
    });
    elements.aiDrawerClose.addEventListener('click', () => {
        elements.aiDrawer.classList.remove('open');
        elements.aiOrb.style.display = 'flex';
    });
    if (elements.selectFileBtn) {
        elements.selectFileBtn.addEventListener('click', () => {
            updateAiContextDisplay();
            const ctx = getFileContext();
            if (ctx && ctx.hasValidPath) addMessageToChat('ai', '✅ ' + aiT('folderUpdated') + ctx.path.substring(ctx.path.lastIndexOf('\\') + 1));
            else addMessageToChat('ai', '❌ ' + aiT('noFolderHint'));
        });
    }
    elements.aiSettingsBtn.addEventListener('click', () => toggleSettings(true));
    elements.cancelConfigBtn.addEventListener('click', () => toggleSettings(false));
    elements.saveConfigBtn.addEventListener('click', saveAiConfig);

    // Reset config button
    const resetBtn = elements.aiDrawer.querySelector('#ai-reset-config');
    if (resetBtn) {
        resetBtn.addEventListener('click', async () => {
            if (!confirm(aiT('resetConfigConfirm'))) return;
            try {
                const r = await fetch(`${state.apiBaseUrl}/config`, { method: 'DELETE' });
                const d = await r.json();
                if (d.success) {
                    state.config = {};
                    elements.apiKeyInput.value = '';
                    elements.apiKeyInput.placeholder = aiT('enterApiKey');
                    elements.baseUrlInput.value = '';
                    elements.tempInput.value = 0.7;
                    elements.tempVal.textContent = '0.70';
                    elements.maxTokensInput.value = 4096;
                    if (elements.maxContentCharsInput) elements.maxContentCharsInput.value = 0;
                    if (elements.maxFileCountInput) elements.maxFileCountInput.value = 0;
                    elements.configStatus.className = 'ai-config-status not-configured';
                    elements.configStatusText.textContent = aiT('notConfiguredHint');
                    const customInput = document.getElementById('custom-model-input');
                    if (customInput) customInput.value = '';
                    toggleSettings(false);
                    popoverCache.clear();
                    addMessageToChat('ai', aiT('resetConfigDone'));
                }
            } catch (e) { alert(aiT('networkError')); }
        });
    }

    // New Chat button
    const newChatBtn = elements.aiDrawer.querySelector('#ai-new-chat-btn');

    // Test Connection button
    const testBtn = elements.aiDrawer.querySelector('#ai-test-connection');
    if (testBtn) {
        testBtn.addEventListener('click', async () => {
            const origText = testBtn.textContent;
            testBtn.textContent = aiT('testingConnection');
            testBtn.disabled = true;
            testBtn.style.opacity = '0.6';
            // Save config first (so backend has current values)
            await saveAiConfig();
            try {
                const r = await fetch(`${state.apiBaseUrl}/test-connection`, { method: 'POST', headers: {'Content-Type': 'application/json'}, body: '{}' });
                const d = await r.json();
                if (d.success) {
                    testBtn.textContent = '✅ ' + aiT('testSuccess');
                    testBtn.style.background = '#28a745';
                } else {
                    testBtn.textContent = '❌ ' + aiT('testFailed');
                    alert(aiT('testFailed') + d.message);
                    testBtn.style.background = '#dc3545';
                }
            } catch (e) {
                testBtn.textContent = '❌ ' + aiT('testFailed');
                alert(aiT('testFailed') + e.message);
                testBtn.style.background = '#dc3545';
            }
            testBtn.disabled = false;
            testBtn.style.opacity = '1';
            setTimeout(() => { testBtn.textContent = aiT('testConnection'); testBtn.style.background = '#17a2b8'; }, 4000);
        });
    }

    if (newChatBtn) {
        newChatBtn.addEventListener('click', () => {
            elements.aiChatHistory.innerHTML = `<div class="ai-message ai">${aiT('welcomeMsg')}</div>`;
            state.smartSearchMode = false;
            state.fileSummaryMode = false;
            addMessageToChat('ai', aiT('newChatConfirm'));
        });
    }
    // Export chat button
    const exportBtn = elements.aiDrawer.querySelector('#ai-export-btn');
    const exportMenu = elements.aiDrawer.querySelector('#ai-export-menu');
    if (exportBtn && exportMenu) {
        exportBtn.addEventListener('click', (e) => {
            e.stopPropagation();
            exportMenu.classList.toggle('visible');
        });
        exportMenu.querySelectorAll('.ai-export-item').forEach(item => {
            item.addEventListener('click', () => {
                const format = item.getAttribute('data-format');
                exportChatHistory(format);
                exportMenu.classList.remove('visible');
            });
        });
        document.addEventListener('click', (e) => {
            if (!e.target.closest('.ai-export-wrapper')) exportMenu.classList.remove('visible');
        });
    }
    if (elements.toggleKeyVisBtn) {
        elements.toggleKeyVisBtn.addEventListener('click', () => {
            const inp = elements.apiKeyInput, ico = elements.toggleKeyVisBtn.querySelector('i');
            if (inp.type === 'password') { inp.type = 'text'; ico.className = 'fas fa-eye-slash'; }
            else { inp.type = 'password'; ico.className = 'fas fa-eye'; }
        });
    }
    elements.tempInput.addEventListener('input', (e) => {
        const v = parseFloat(e.target.value);
        elements.tempVal.textContent = v.toFixed(2);
        updateTempHint(v); updateTempPresetHighlight(v);
    });
    elements.aiDrawer.querySelectorAll('.ai-temp-preset').forEach(btn => {
        btn.addEventListener('click', () => {
            const t = parseFloat(btn.getAttribute('data-temp'));
            elements.tempInput.value = t; elements.tempVal.textContent = t.toFixed(2);
            updateTempHint(t); updateTempPresetHighlight(t);
        });
    });
    elements.providerSelect.addEventListener('change', updateModelOptions);
    elements.aiSendBtn.addEventListener('click', handleUserMessage);
    elements.aiInput.addEventListener('keypress', (e) => { if (e.key === 'Enter') handleUserMessage(); });
    elements.aiDrawer.querySelectorAll('.ai-chip').forEach(chip => {
        chip.addEventListener('click', () => handleAiCommand(chip.getAttribute('data-command')));
    });
}

// ==========================================
// Settings & Config
// ==========================================
function toggleSettings(show) {
    elements.aiSettingsPanel.style.display = show ? 'block' : 'none';
    elements.aiChatBody.style.display = show ? 'none' : 'flex';
    if (show) loadAiConfig();
}

async function loadAiConfig() {
    try {
        const mr = await fetch(`${state.apiBaseUrl}/models`);
        if (mr.ok) { const d = await mr.json(); state.models = d.models || []; populateProviders(); }
        const cr = await fetch(`${state.apiBaseUrl}/config`);
        if (cr.ok) { const d = await cr.json(); state.config = d.config || {}; applyConfigToUI(); }
    } catch (e) {
        console.error('Error loading AI config:', e);
        addMessageToChat('ai', aiT('errorLoadingConfig'));
    }
}

function populateProviders() {
    elements.providerSelect.innerHTML = '';
    const provs = [...new Set(state.models.map(m => m.provider))];
    provs.forEach(p => {
        const o = document.createElement('option'); o.value = p;
        o.textContent = state.providers[p] || `Provider ${p}`;
        elements.providerSelect.appendChild(o);
    });
    if (!provs.includes(7)) {
        const o = document.createElement('option'); o.value = 7; o.textContent = 'Custom';
        elements.providerSelect.appendChild(o);
    }
}

function updateModelOptions() {
    const pid = parseInt(elements.providerSelect.value);
    elements.modelSelect.innerHTML = '';
    state.models.filter(m => m.provider === pid).forEach(m => {
        const o = document.createElement('option'); o.value = m.model_id; o.textContent = m.name;
        elements.modelSelect.appendChild(o);
    });
    // Add custom model option for all providers
    const customOpt = document.createElement('option');
    customOpt.value = '__custom__';
    customOpt.textContent = '✏️ ' + (getCurrentLang() === 'zh' ? '自定义模型...' : 'Custom model...');
    elements.modelSelect.appendChild(customOpt);

    // Show/hide custom model input based on selection
    elements.modelSelect.addEventListener('change', handleCustomModelToggle);
    handleCustomModelToggle();

    elements.baseUrlGroup.style.display = 'block';

    // Show default URL as placeholder hint
    const defaultUrls = {0:'https://api.openai.com/v1',1:'https://api.deepseek.com/v1',2:'https://generativelanguage.googleapis.com/v1beta',3:'https://api.x.ai/v1',4:'https://api.anthropic.com/v1',5:'https://api.moonshot.cn/v1',6:'https://open.bigmodel.cn/api/paas/v4'};
    const defUrl = defaultUrls[pid] || '';
    elements.baseUrlInput.placeholder = defUrl ? (getCurrentLang()==='zh' ? '留空使用默认: '+defUrl : 'Default: '+defUrl) : (getCurrentLang()==='zh' ? '输入 API Base URL' : 'Enter API Base URL');
    // Clear base_url if it matches the default (treat as "not customized")
    if (elements.baseUrlInput.value === defUrl) elements.baseUrlInput.value = '';
}

function handleCustomModelToggle() {
    const isCustom = elements.modelSelect.value === '__custom__';
    let customInput = document.getElementById('custom-model-input');
    if (isCustom) {
        if (!customInput) {
            customInput = document.createElement('input');
            customInput.id = 'custom-model-input';
            customInput.type = 'text';
            customInput.className = 'form-control form-control-sm mt-1';
            customInput.placeholder = getCurrentLang() === 'zh' ? '输入模型 ID，如 gpt-4o-2024-08-06' : 'Enter model ID, e.g. gpt-4o-2024-08-06';
            customInput.style.cssText = 'background:var(--bg-secondary);color:var(--text-primary);border:1px solid var(--border);font-size:0.8rem;';
            elements.modelSelect.parentNode.insertBefore(customInput, elements.modelSelect.nextSibling);
        }
        customInput.style.display = 'block';
    } else {
        if (customInput) customInput.style.display = 'none';
    }
}

function applyConfigToUI() {
    const c = state.config;
    if (c.provider !== undefined) { elements.providerSelect.value = c.provider; updateModelOptions(); }
    if (c.model_id) elements.modelSelect.value = c.model_id;
    if (c.base_url) elements.baseUrlInput.value = c.base_url;
    if (c.temperature !== undefined) {
        elements.tempInput.value = c.temperature;
        elements.tempVal.textContent = parseFloat(c.temperature).toFixed(2);
        updateTempHint(c.temperature); updateTempPresetHighlight(c.temperature);
    }
    if (c.max_tokens) elements.maxTokensInput.value = c.max_tokens;
    // Context limits
    if (elements.maxContentCharsInput) elements.maxContentCharsInput.value = c.max_content_chars || 0;
    if (elements.maxFileCountInput) elements.maxFileCountInput.value = c.max_file_count || 0;
    if (c.has_api_key) {
        elements.apiKeyInput.placeholder = aiT('alreadySaved');
        elements.configStatus.className = 'ai-config-status configured';
        elements.configStatusText.textContent = aiT('connectedTo') + (state.providers[c.provider] || 'Custom');
    } else {
        elements.configStatus.className = 'ai-config-status not-configured';
        elements.configStatusText.textContent = aiT('notConfiguredHint');
    }
}

function updateTempHint(val) {
    const h = [[0,0.1,'tempHint0'],[0.1,0.3,'tempHint1'],[0.3,0.5,'tempHint2'],[0.5,0.7,'tempHint3'],[0.7,0.9,'tempHint4'],[0.9,1.2,'tempHint5'],[1.2,1.5,'tempHint6'],[1.5,2.01,'tempHint7']];
    for (const [lo,hi,k] of h) { if (val >= lo && val < hi) { elements.tempHint.textContent = aiT(k); return; } }
    elements.tempHint.textContent = '';
}

function updateTempPresetHighlight(val) {
    elements.aiDrawer.querySelectorAll('.ai-temp-preset').forEach(btn => {
        btn.classList.toggle('active', Math.abs(parseFloat(btn.getAttribute('data-temp')) - val) < 0.01);
    });
}

async function saveAiConfig() {
    const akv = elements.apiKeyInput.value.trim();
    let modelId = elements.modelSelect.value;
    let modelName = elements.modelSelect.options[elements.modelSelect.selectedIndex]?.textContent || "";
    // Handle custom model selection
    if (modelId === '__custom__') {
        const customInput = document.getElementById('custom-model-input');
        modelId = customInput ? customInput.value.trim() : '';
        if (!modelId) {
            alert(getCurrentLang() === 'zh' ? '请输入自定义模型 ID' : 'Please enter a custom model ID');
            return;
        }
        modelName = modelId;
    }
    const cfg = {
        provider: parseInt(elements.providerSelect.value),
        model_id: modelId,
        name: modelName,
        base_url: elements.baseUrlInput.value,
        temperature: parseFloat(elements.tempInput.value),
        max_tokens: parseInt(elements.maxTokensInput.value) || 4096,
        max_content_chars: parseInt(elements.maxContentCharsInput?.value) || 0,
        max_file_count: parseInt(elements.maxFileCountInput?.value) || 0,
        enabled: true
    };
    if (akv.length > 0) cfg.api_key = akv;
    try {
        const r = await fetch(`${state.apiBaseUrl}/config`, { method:'POST', headers:{'Content-Type':'application/json'}, body: JSON.stringify(cfg) });
        const d = await r.json();
        if (d.success) { toggleSettings(false); addMessageToChat('ai', aiT('configSaved')); await loadAiConfig(); }
        else alert(aiT('errorSavingConfig') + d.message);
    } catch (e) { alert(aiT('networkError')); }
}

// ==========================================
// Chat (with file content awareness)
// ==========================================
async function handleUserMessage() {
    const text = elements.aiInput.value.trim();
    if (!text) return;

    if (state.pendingFileSummary) {
        const files = state.pendingFileSummary; state.pendingFileSummary = null;
        addMessageToChat('user', text); elements.aiInput.value = '';
        let sf = null; const n = parseInt(text);
        if (!isNaN(n) && n >= 1 && n <= files.length) sf = files[n-1];
        else sf = files.find(f => f.name.toLowerCase().includes(text.toLowerCase()));
        if (sf) { addMessageToChat('ai', '📄 ' + aiT('analyzingFile') + '**' + sf.name + '**...'); await executeFileSummary(sf.path); }
        else addMessageToChat('ai', '❌ ' + aiT('fileNotFound'));
        return;
    }
    if (state.pendingSemanticSearch) {
        addMessageToChat('user', text); elements.aiInput.value = '';
        await executeSemanticSearch(text, getFileContext()); return;
    }

    addMessageToChat('user', text); elements.aiInput.value = '';
    showTypingIndicator();
    const ctx = getFileContext();

    // Build enhanced context with file tree
    const contextPayload = {
        current_path: ctx ? ctx.path : '',
        file_count: ctx ? ctx.files.length : 0,
        top_files: ctx ? ctx.files.slice(0,50).map(f => f.name) : []
    };
    // Add file tree to context
    if (ctx && ctx.hasValidPath) {
        contextPayload.file_tree = buildFileTree(ctx.files, ctx.path);
    }

    // Auto-detect file references in the message and read content
    const fileContents = await detectAndReadFiles(text, ctx);
    if (fileContents.length > 0) {
        contextPayload.file_contents = fileContents;
    }

    const payload = { message: text, language: getCurrentLang(), context: contextPayload };
    try {
        const r = await fetch(`${state.apiBaseUrl}/chat`, { method:'POST', headers:{'Content-Type':'application/json'}, body: JSON.stringify(payload) });
        const d = await r.json(); removeTypingIndicator();
        if (d.success) { addMessageToChat('ai', d.content); updateTokenDisplay(d); }
        else addMessageToChat('ai', 'Error: ' + (d.error || d.message || 'Unknown error'));
    } catch (e) { removeTypingIndicator(); addMessageToChat('ai', 'Network Error: ' + e.message); }
}

/**
 * Auto-detect file names/paths mentioned in user message and read their content
 */
async function detectAndReadFiles(message, ctx) {
    if (!ctx || !ctx.hasValidPath) return [];
    const allFiles = ctx.allFiles || [];
    const results = [];
    // Find mentioned file names (exact match on file name)
    for (const f of allFiles) {
        if (f.is_directory) continue;
        if (isBinaryExtension(f.name)) continue;
        // Check if the file name appears in the message
        if (message.toLowerCase().includes(f.name.toLowerCase())) {
            try {
                const maxChars = state.config.max_content_chars || 100000;
                const r = await fetch('/api/file/read', {
                    method: 'POST', headers: {'Content-Type': 'application/json'},
                    body: JSON.stringify({ path: f.path, max_size: maxChars })
                });
                const d = await r.json();
                if (d.success && !d.is_binary) {
                    results.push({ name: f.name, content: d.content });
                }
            } catch (e) { /* skip */ }
            if (results.length >= 5) break; // Limit to 5 files max
        }
    }
    return results;
}

async function handleAiCommand(command) {
    const ctx = getFileContext();
    if (!ctx || !ctx.hasValidPath) {
        addMessageToChat('ai', `❌ **${aiT('selectFolderFirst')}**\n\n📂 ${aiT('selectFolderSteps')}`); return;
    }
    // Build the file tree text for AI
    const fileTree = buildFileTree(ctx.files, ctx.path);
    const payload = { path: ctx.path, language: getCurrentLang(), file_tree: fileTree };

    let endpoint = '', label = '';
    switch (command) {
        case 'project-summary': endpoint='/project-summary'; label=aiT('projectSummary'); break;
        case 'cleanup-suggestions': endpoint='/cleanup-suggestions'; label=aiT('suggestions'); break;
        case 'code-analysis': endpoint='/code-analysis'; label=aiT('codeAnalysis'); break;
        case 'annotate-tree': handleAnnotateTree(ctx); return;
        case 'file-summary': handleFileSummary(ctx); return;
        case 'semantic-search': handleSemanticSearch(ctx); return;
    }
    if (!endpoint) return;
    addMessageToChat('user', '🔍 ' + aiT('analyzing') + label + '...');
    showTypingIndicator();
    try {
        const r = await fetch(`${state.apiBaseUrl}${endpoint}`, { method:'POST', headers:{'Content-Type':'application/json'}, body: JSON.stringify(payload) });
        const d = await r.json(); removeTypingIndicator();
        if (d.success) { addMessageToChat('ai', d.content); updateTokenDisplay(d); } else addMessageToChat('ai', 'Error: ' + (d.error || d.message));
    } catch (e) { removeTypingIndicator(); addMessageToChat('ai', 'Network Error: ' + e.message); }
}

// ==========================================
// Annotate Tree (AI-generated file comments)
// ==========================================
async function handleAnnotateTree(ctx) {
    const fileTree = buildFileTree(ctx.files, ctx.path);
    const payload = { path: ctx.path, language: getCurrentLang(), file_tree: fileTree };

    addMessageToChat('user', '🏷️ ' + aiT('analyzing') + aiT('annotateTree') + '...');
    showTypingIndicator();

    try {
        const r = await fetch(`${state.apiBaseUrl}/tree-annotations`, {
            method: 'POST', headers: {'Content-Type': 'application/json'}, body: JSON.stringify(payload)
        });
        const d = await r.json();
        removeTypingIndicator();

        if (!d.success) { addMessageToChat('ai', 'Error: ' + (d.error || d.message)); return; }
        updateTokenDisplay(d);

        // Parse AI response: each line is "filename|||comment"
        const annotations = {};
        (d.content || '').split('\n').forEach(line => {
            const sep = line.indexOf('|||');
            if (sep > 0) {
                const name = line.substring(0, sep).trim();
                const comment = line.substring(sep + 3).trim();
                if (name && comment) annotations[name] = comment;
            }
        });

        // Build annotated tree using the frontend tree generator
        const fm = window.fileManager;
        if (!fm || fm.currentFiles.length === 0) return;

        const files = fm.currentFiles;
        // Sort in hierarchical pre-order (same as generateTreeContent)
        const sorted = [...files].sort((a, b) => {
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

        // Generate tree lines with annotations, then align comments
        const isLastAtDepth = [];
        let rootName = fm.currentPath.split(/[/\\]/).filter(Boolean).pop() || 'root';
        rootName += '/';

        const lines = [];
        // Root line
        const rootComment = annotations[rootName] || annotations[rootName.replace(/\/$/, '')] || '';
        lines.push({ text: rootName, comment: rootComment });

        for (let i = 0; i < sorted.length; i++) {
            const file = sorted[i];
            const isLast = (i === sorted.length - 1) || (sorted[i + 1].depth < file.depth);
            isLastAtDepth[file.depth] = isLast;

            let prefix = '';
            for (let d = 1; d < file.depth; d++) {
                prefix += isLastAtDepth[d] ? '    ' : '│   ';
            }
            prefix += isLast ? '└── ' : '├── ';

            const fileName = file.name + (file.is_directory ? '/' : '');
            const comment = annotations[file.name] || annotations[fileName] || '';
            lines.push({ text: prefix + fileName, comment: comment });
        }

        // Calculate alignment column: max text width + 2 spaces padding
        const maxLen = lines.reduce((m, l) => Math.max(m, l.text.length), 0);
        const alignCol = maxLen + 2;

        // Build final output with aligned # comments
        let output = '```\n';
        for (const l of lines) {
            if (l.comment) {
                const padding = ' '.repeat(Math.max(2, alignCol - l.text.length));
                output += l.text + padding + '# ' + l.comment + '\n';
            } else {
                output += l.text + '\n';
            }
        }
        output += '```';

        addMessageToChat('ai', output);

    } catch (e) {
        removeTypingIndicator();
        addMessageToChat('ai', 'Network Error: ' + e.message);
    }
}

// ==========================================
// Markdown Renderer
// ==========================================
function renderMarkdown(text) {
    let h = text;
    // Protect LaTeX blocks from HTML escaping
    const latexBlocks = [];
    // Block LaTeX: $$...$$
    h = h.replace(/\$\$([\s\S]*?)\$\$/g, (_, tex) => {
        const idx = latexBlocks.length;
        latexBlocks.push({block: true, tex: tex.trim()});
        return `%%LATEX_BLOCK_${idx}%%`;
    });
    // Inline LaTeX: $...$  (but not $$)
    h = h.replace(/(?<!\$)\$(?!\$)(.+?)(?<!\$)\$(?!\$)/g, (_, tex) => {
        const idx = latexBlocks.length;
        latexBlocks.push({block: false, tex: tex.trim()});
        return `%%LATEX_INLINE_${idx}%%`;
    });

    h = h.replace(/&/g,'&amp;').replace(/</g,'&lt;').replace(/>/g,'&gt;');
    // Code blocks
    h = h.replace(/```(\w*)\n([\s\S]*?)```/g, (_,l,c) => {
        const ll = l ? `<span class="ai-code-lang">${l}</span>` : '';
        return `<div class="ai-code-block">${ll}<pre><code>${c.trim()}</code></pre></div>`;
    });
    h = h.replace(/`([^`]+)`/g, '<code class="ai-inline-code">$1</code>');
    // Tables: detect | header | header | lines
    h = h.replace(/^(\|.+\|)\n(\|[\s:\-\|]+\|)\n((?:\|.+\|\n?)+)/gm, (_, hdr, sep, body) => {
        const headers = hdr.split('|').filter(c => c.trim()).map(c => `<th>${c.trim()}</th>`).join('');
        const rows = body.trim().split('\n').map(row => {
            const cells = row.split('|').filter(c => c.trim()).map(c => `<td>${c.trim()}</td>`).join('');
            return `<tr>${cells}</tr>`;
        }).join('');
        return `<table class="ai-md-table"><thead><tr>${headers}</tr></thead><tbody>${rows}</tbody></table>`;
    });
    // Headers h1-h6 (must process from h6 down to h1)
    h = h.replace(/^###### (.+)$/gm, '<h6 class="ai-md-h6">$1</h6>');
    h = h.replace(/^##### (.+)$/gm, '<h5 class="ai-md-h5">$1</h5>');
    h = h.replace(/^#### (.+)$/gm, '<h4 class="ai-md-h4">$1</h4>');
    h = h.replace(/^### (.+)$/gm, '<h3 class="ai-md-h3">$1</h3>');
    h = h.replace(/^## (.+)$/gm, '<h2 class="ai-md-h2">$1</h2>');
    h = h.replace(/^# (.+)$/gm, '<h1 class="ai-md-h1">$1</h1>');
    // Bold/italic
    h = h.replace(/\*\*\*(.*?)\*\*\*/g, '<strong><em>$1</em></strong>');
    h = h.replace(/\*\*(.*?)\*\*/g, '<strong>$1</strong>');
    h = h.replace(/(?<![*])\*([^*]+)\*(?![*])/g, '<em>$1</em>');
    // Links: [text](url)
    h = h.replace(/\[([^\]]+)\]\(([^)]+)\)/g, '<a href="$2" target="_blank" class="ai-md-link">$1</a>');
    // Lists
    h = h.replace(/^[\-\*] (.+)$/gm, '<li>$1</li>');
    h = h.replace(/(<li>.*<\/li>\n?)+/g, m => `<ul class="ai-md-list">${m}</ul>`);
    h = h.replace(/^\d+\. (.+)$/gm, '<li>$1</li>');
    h = h.replace(/(?<!<\/ul>\n?)(<li>.*<\/li>\n?)+/g, m => m.includes('<ul') ? m : `<ol class="ai-md-list">${m}</ol>`);
    h = h.replace(/^---$/gm, '<hr class="ai-md-hr">');
    h = h.replace(/^&gt; (.+)$/gm, '<blockquote class="ai-md-blockquote">$1</blockquote>');
    h = h.replace(/\n/g, '<br>');
    h = h.replace(/(<\/h[1-6]>)<br>/g,'$1').replace(/(<\/li>)<br>/g,'$1').replace(/(<\/ul>)<br>/g,'$1');
    h = h.replace(/(<\/ol>)<br>/g,'$1').replace(/(<\/blockquote>)<br>/g,'$1').replace(/(<hr[^>]*>)<br>/g,'$1');
    h = h.replace(/(<\/div>)<br>/g,'$1').replace(/(<\/pre>)<br>/g,'$1');
    h = h.replace(/(<\/table>)<br>/g,'$1').replace(/(<\/thead>)<br>/g,'$1').replace(/(<\/tbody>)<br>/g,'$1').replace(/(<\/tr>)<br>/g,'$1');

    // Restore LaTeX placeholders
    for (let i = 0; i < latexBlocks.length; i++) {
        const lb = latexBlocks[i];
        if (lb.block) {
            h = h.replace(`%%LATEX_BLOCK_${i}%%`, `<div class="ai-latex-block" data-latex="${encodeURIComponent(lb.tex)}"></div>`);
        } else {
            h = h.replace(`%%LATEX_INLINE_${i}%%`, `<span class="ai-latex-inline" data-latex="${encodeURIComponent(lb.tex)}"></span>`);
        }
    }
    return h;
}

function renderLatexElements(container) {
    if (typeof katex === 'undefined') return;
    container.querySelectorAll('.ai-latex-block').forEach(el => {
        try {
            katex.render(decodeURIComponent(el.getAttribute('data-latex')), el, { displayMode: true, throwOnError: false });
        } catch(e) { el.textContent = decodeURIComponent(el.getAttribute('data-latex')); }
    });
    container.querySelectorAll('.ai-latex-inline').forEach(el => {
        try {
            katex.render(decodeURIComponent(el.getAttribute('data-latex')), el, { displayMode: false, throwOnError: false });
        } catch(e) { el.textContent = decodeURIComponent(el.getAttribute('data-latex')); }
    });
}

function updateTokenDisplay(responseData) {
    if (!responseData) return;
    const pt = responseData.prompt_tokens || 0;
    const ct = responseData.completion_tokens || 0;
    const tt = responseData.total_tokens || (pt + ct);
    if (tt === 0) return;
    state.sessionTokens.prompt += pt;
    state.sessionTokens.completion += ct;
    state.sessionTokens.total += tt;
    const bar = document.getElementById('ai-token-bar');
    const text = document.getElementById('ai-token-text');
    if (bar && text) {
        bar.style.display = 'flex';
        const fmt = n => n >= 1000 ? (n/1000).toFixed(1) + 'k' : n;
        text.textContent = `Tokens: ${fmt(state.sessionTokens.total)} (↑${fmt(state.sessionTokens.prompt)} ↓${fmt(state.sessionTokens.completion)})`;
    }
}

function exportChatHistory(format) {
    const messages = elements.aiChatHistory.querySelectorAll('.ai-message');
    if (messages.length === 0) return;
    const lines = [];
    const now = new Date();
    const ts = now.getFullYear() + String(now.getMonth()+1).padStart(2,'0') + String(now.getDate()).padStart(2,'0') +
        '_' + String(now.getHours()).padStart(2,'0') + String(now.getMinutes()).padStart(2,'0') + String(now.getSeconds()).padStart(2,'0');

    if (format === 'md') {
        lines.push('# FileEcho AI Chat Export', '', `*${now.toLocaleString()}*`, '---', '');
        messages.forEach(msg => {
            const isUser = msg.classList.contains('user');
            const text = msg.getAttribute('data-raw') || msg.textContent.trim();
            if (isUser) { lines.push('> **User:** ' + text, ''); }
            else { lines.push('**AI:**', '', text, '', '---', ''); }
        });
    } else {
        lines.push('FileEcho AI Chat Export - ' + now.toLocaleString(), '='.repeat(50), '');
        messages.forEach(msg => {
            const isUser = msg.classList.contains('user');
            const text = msg.getAttribute('data-raw') || msg.textContent.trim();
            if (isUser) { lines.push('[User]: ' + text, ''); }
            else { lines.push('[AI]:', text, '', '-'.repeat(40), ''); }
        });
    }

    const blob = new Blob([lines.join('\n')], { type: format === 'md' ? 'text/markdown' : 'text/plain' });
    const a = document.createElement('a');
    a.href = URL.createObjectURL(blob);
    a.download = `FileEcho_Chat_${ts}.${format}`;
    a.click();
    URL.revokeObjectURL(a.href);
}

function addMessageToChat(role, text) {
    const div = document.createElement('div');
    div.className = `ai-message ${role}`;
    div.setAttribute('data-raw', text);
    if (role === 'ai') {
        div.innerHTML = renderMarkdown(text);
        renderLatexElements(div);
    } else {
        div.textContent = text;
    }
    elements.aiChatHistory.appendChild(div);
    elements.aiChatHistory.scrollTop = elements.aiChatHistory.scrollHeight;
}

function showTypingIndicator() {
    const div = document.createElement('div');
    div.className = 'ai-message ai typing'; div.id = 'ai-typing';
    div.innerHTML = '<i class="fas fa-circle-notch fa-spin"></i> ' + aiT('thinking');
    elements.aiChatHistory.appendChild(div);
    elements.aiChatHistory.scrollTop = elements.aiChatHistory.scrollHeight;
}
function removeTypingIndicator() { const el = document.getElementById('ai-typing'); if (el) el.remove(); }

function updateAiContextDisplay() {
    const ctx = getFileContext();
    if (ctx && ctx.hasValidPath) {
        const pd = ctx.path.length > 28 ? '...' + ctx.path.slice(-28) : ctx.path;
        elements.currentPath.innerHTML = `<strong>📂 ${pd}</strong>`;
        elements.currentPath.title = ctx.path;
        let ct = ctx.files.length + ' ' + aiT('items');
        if (ctx.hasSelection) ct = aiT('selectedFiles', {count: ctx.files.length});
        elements.fileCount.textContent = ct;
        elements.fileCount.style.display = 'inline-block';
        elements.fileCount.style.background = 'rgba(76,175,80,0.4)';
    } else {
        elements.currentPath.innerHTML = '❌ <em>' + aiT('noFolderSelected') + '</em>';
        elements.fileCount.textContent = '0 ' + aiT('items');
        elements.fileCount.style.display = 'inline-block';
        elements.fileCount.style.background = 'rgba(255,152,0,0.4)';
    }
}

// ==========================================
// File Summary Feature
// ==========================================
const fileSummaryCache = new Map();

async function handleFileSummary(ctx) {
    if (!ctx || !ctx.hasValidPath || ctx.files.length === 0) { addMessageToChat('ai', '❌ ' + aiT('noFilesFound')); return; }
    const rf = ctx.files.filter(f => !f.is_directory);
    if (rf.length === 0) { addMessageToChat('ai', '❌ ' + aiT('noRegularFiles')); return; }
    let html = '📄 **' + aiT('selectFileToAnalyze') + '**\n\n';
    rf.forEach((f,i) => { html += `${i+1}. ${f.name}\n`; });
    html += '\n' + aiT('typeFileNumber');
    addMessageToChat('ai', html);
    state.pendingFileSummary = rf;
}

async function executeFileSummary(filePath) {
    if (fileSummaryCache.has(filePath)) { addMessageToChat('ai', fileSummaryCache.get(filePath)); return; }
    showTypingIndicator();
    try {
        const r = await fetch(`${state.apiBaseUrl}/file-summary`, {
            method:'POST', headers:{'Content-Type':'application/json'},
            body: JSON.stringify({ path: filePath, language: getCurrentLang() })
        });
        const d = await r.json(); removeTypingIndicator();
        if (d.success) { fileSummaryCache.set(filePath, d.content); addMessageToChat('ai', d.content); updateTokenDisplay(d); }
        else addMessageToChat('ai', 'Error: ' + (d.error || d.message));
    } catch (e) { removeTypingIndicator(); addMessageToChat('ai', 'Network Error: ' + e.message); }
}

// ==========================================
// Smart Search v2 (two-stage: tree + content sampling)
// ==========================================
async function handleSemanticSearch(ctx) {
    if (!ctx || !ctx.hasValidPath) { addMessageToChat('ai', '❌ ' + aiT('noFilesFound')); return; }
    addMessageToChat('ai', '🧠 **' + aiT('smartSearchActive') + '**\n\n' + aiT('smartSearchHint'));
    state.pendingSemanticSearch = true;
}

async function executeSemanticSearch(query, ctx) {
    showTypingIndicator();

    // Stage 1: Build file tree for context
    const fileTree = buildFileTree(ctx.allFiles, ctx.path);

    // Stage 2: Content sampling for text files (first 500 chars, respecting user config)
    let contentSection = '';
    const textFiles = ctx.allFiles.filter(f => !f.is_directory && !isBinaryExtension(f.name));
    if (textFiles.length > 0) {
        const samples = [];
        const controller = new AbortController();
        const timeoutId = setTimeout(() => controller.abort(), 60000); // 60s timeout

        try {
            for (const f of textFiles) {
                try {
                    const r = await fetch('/api/file/read', {
                        method: 'POST', headers: {'Content-Type': 'application/json'},
                        body: JSON.stringify({ path: f.path, max_size: 500 }),
                        signal: controller.signal
                    });
                    const d = await r.json();
                    if (d.success && !d.is_binary && d.content && d.content.length > 10) {
                        samples.push(`[${f.name}]: ${d.content.substring(0, 300).replace(/\n/g, ' ')}`);
                    }
                } catch (e) {
                    if (e.name === 'AbortError') throw e;
                    /* skip individual file errors */
                }
            }
        } catch (e) {
            if (e.name === 'AbortError') {
                // Timeout reached - proceed with samples gathered so far
                const hint = getCurrentLang() === 'zh'
                    ? `⏱️ 采样超时（已采集 ${samples.length} 个文件）。提示：可在设置中减小"AI 最大文件数"以加速搜索。`
                    : `⏱️ Sampling timed out (collected ${samples.length} files). Tip: reduce "Max file count for AI" in settings to speed up search.`;
                addMessageToChat('ai', hint);
            }
        } finally {
            clearTimeout(timeoutId);
        }

        if (samples.length > 0) {
            contentSection = 'Content samples (first 300 chars):\n' + samples.join('\n') + '\n\n';
        }
    }

    const prompt = aiT('semanticSearchPrompt')
        .replace('{query}', query)
        .replace('{files}', fileTree)
        .replace('{contentSection}', contentSection);

    const payload = {
        message: prompt,
        language: getCurrentLang(),
        context: { current_path: ctx.path, file_count: ctx.allFiles.length }
    };
    try {
        const r = await fetch(`${state.apiBaseUrl}/chat`, { method:'POST', headers:{'Content-Type':'application/json'}, body: JSON.stringify(payload) });
        const d = await r.json(); removeTypingIndicator();
        if (d.success) { addMessageToChat('ai', `🔍 **${aiT('searchResultsFor')}"${query}":**\n\n${d.content}`); updateTokenDisplay(d); }
        else addMessageToChat('ai', 'Error: ' + (d.error || d.message));
    } catch (e) { removeTypingIndicator(); addMessageToChat('ai', 'Network Error: ' + e.message); }
    state.pendingSemanticSearch = false;
}

// ==========================================
// Popover (File & Folder Hover Summary)
// ==========================================
const popoverCache = new Map();
let popoverTimer = null;
let popoverElement = null;

function initPopover() {
    popoverElement = document.createElement('div');
    popoverElement.className = 'ai-popover';
    popoverElement.innerHTML = '<div class="ai-popover-title"></div><div class="ai-popover-content"></div>';
    document.body.appendChild(popoverElement);

    document.addEventListener('mouseover', (e) => {
        const row = e.target.closest('tr[data-path]');
        if (!row) return;
        const filePath = row.getAttribute('data-path');
        const fileName = row.querySelector('.name-text')?.textContent?.trim() || row.querySelector('td:first-child')?.textContent?.trim();
        if (!filePath || !fileName) return;
        clearTimeout(popoverTimer);
        popoverTimer = setTimeout(() => {
            const isDir = !!row.querySelector('.fa-folder, .fa-folder-open');
            showPopover(filePath, fileName, e, isDir);
        }, 600);
    });

    document.addEventListener('mouseout', (e) => {
        if (e.target.closest('tr[data-path]')) {
            clearTimeout(popoverTimer);
            // Only hide if not moving to another table row or the popover itself
            const related = e.relatedTarget;
            if (!related || (!related.closest('tr[data-path]') && !related.closest('.ai-popover'))) {
                hidePopover();
            }
        }
    });

    document.addEventListener('mousemove', (e) => {
        if (popoverElement && popoverElement.classList.contains('visible')) {
            // Safety: hide popover if mouse is not over a table row or the popover itself
            if (!e.target.closest('tr[data-path]') && !e.target.closest('.ai-popover')) {
                clearTimeout(popoverTimer);
                hidePopover();
                return;
            }
            positionPopover(e);
        }
    });
}

async function showPopover(filePath, fileName, event, isDir) {
    if (!popoverElement) return;
    const titleEl = popoverElement.querySelector('.ai-popover-title');
    const contentEl = popoverElement.querySelector('.ai-popover-content');
    titleEl.textContent = (isDir ? '📁 ' : '📄 ') + fileName;

    if (popoverCache.has(filePath)) {
        contentEl.innerHTML = renderMarkdown(popoverCache.get(filePath));
    } else {
        contentEl.innerHTML = '<i class="fas fa-circle-notch fa-spin"></i> ' + aiT('popoverAnalyzing');
        if (window.fileManager && window.fileManager.showToast) window.fileManager.showToast(aiT('aiStartedSummary'), 'info');

        try {
            let summary = '';
            if (isDir) {
                // Try to get folder files from current scan state
                let folderFiles = [];
                const allFiles = window.fileManager ? window.fileManager.currentFiles || [] : [];
                folderFiles = allFiles.filter(f => f.path.startsWith(filePath + '\\') || f.path.startsWith(filePath + '/'));

                // If no sub-files found in frontend state, fetch from backend directly
                if (folderFiles.length === 0) {
                    try {
                        const scanRes = await fetch('/api/scan', {
                            method: 'POST', headers: {'Content-Type': 'application/json'},
                            body: JSON.stringify({ path: filePath, max_depth: 2 })
                        });
                        const scanData = await scanRes.json();
                        if (scanData.success && scanData.files && scanData.files.length > 0) {
                            folderFiles = scanData.files;
                        }
                    } catch (e) { /* fallback to empty */ }
                }

                // Build sub-tree representation
                const subTree = folderFiles.slice(0, 80).map(f => {
                    const rel = (f.path || '').replace(filePath, '').replace(/^[\\/]/, '');
                    return (f.is_directory ? '📁 ' : '📄 ') + (rel || f.name);
                }).join('\n');
                // Sample first 10 text/document files' content (800 chars each)
                const textSamples = [];
                const textCandidates = folderFiles.filter(f => !f.is_directory && !isBinaryExtension(f.name)).slice(0, 10);
                for (const tf of textCandidates) {
                    try {
                        const r2 = await fetch('/api/file/read', {
                            method: 'POST', headers: {'Content-Type': 'application/json'},
                            body: JSON.stringify({ path: tf.path, max_size: 800 })
                        });
                        const d2 = await r2.json();
                        if (d2.success && !d2.is_binary && d2.content && d2.content.length > 10) {
                            textSamples.push(`[${tf.name}]: ${d2.content.substring(0, 500).replace(/\n/g, ' ')}`);
                        }
                    } catch (e) { /* skip */ }
                }
                const samplesStr = textSamples.length > 0
                    ? '\n\n以下是文件夹中文件的实际文本内容摘录（非文件名），请根据这些内容判断文件夹用途：\nActual text content extracted from files (NOT file names):\n' + textSamples.join('\n')
                    : '';
                const prompt = getCurrentLang() === 'zh'
                    ? '你是文件分析助手。请仅根据以下提供的【文件结构】和【实际文件内容摘录】来分析文件夹用途，用1-2句话概括。严禁根据文件夹名称或文件名进行猜测推断，只能依据真实内容判断。如果提供的内容不足以判断，请明确回答"信息不足，无法判断"。\n文件夹：' + filePath + '\n子文件结构：\n' + subTree + samplesStr
                    : 'You are a file analysis assistant. Analyze this folder based STRICTLY on the provided file structure and actual text content excerpts below. Summarize in 1-2 sentences. You MUST NOT guess or infer based on folder/file names. Only use the actual content provided. If insufficient content, explicitly say so.\nFolder: ' + filePath + '\nContents:\n' + subTree + samplesStr;
                const r = await fetch(`${state.apiBaseUrl}/chat`, { method:'POST', headers:{'Content-Type':'application/json'}, body: JSON.stringify({message: prompt, language: getCurrentLang(), context: {}}) });
                const d = await r.json();
                summary = d.success ? d.content : aiT('popoverError');
            } else {
                const r = await fetch(`${state.apiBaseUrl}/file-summary`, { method:'POST', headers:{'Content-Type':'application/json'}, body: JSON.stringify({path: filePath, brief: true, language: getCurrentLang()}) });
                const d = await r.json();
                summary = d.success ? d.content : aiT('popoverError');
            }
            popoverCache.set(filePath, summary);
            contentEl.innerHTML = renderMarkdown(summary);
        } catch (e) { contentEl.textContent = aiT('popoverNetworkError'); }
    }
    positionPopover(event);
    popoverElement.classList.add('visible');
}

function positionPopover(event) {
    if (!popoverElement) return;
    const x = event.clientX + 15, y = event.clientY + 15;
    const maxX = window.innerWidth - 420, maxY = window.innerHeight - (popoverElement.getBoundingClientRect().height || 200);
    popoverElement.style.left = Math.min(x, maxX) + 'px';
    popoverElement.style.top = Math.min(y, maxY) + 'px';
}

function hidePopover() { if (popoverElement) popoverElement.classList.remove('visible'); }

// ==========================================
// File Selection System (Tree Display + Cascade + Checkboxes)
// ==========================================
function initFileSelection() {
    if (!window.fileManager) return;
    if (!window.fileManager.selectedFiles) window.fileManager.selectedFiles = new Set();
    addSelectionStatusBar();
    addDoubleClickHintBar();
    patchFileTable();
}

function addSelectionStatusBar() {
    const ls = document.querySelector('.list-section');
    if (!ls || document.getElementById('file-selection-bar')) return;
    const bar = document.createElement('div');
    bar.id = 'file-selection-bar'; bar.className = 'file-selection-bar';
    bar.innerHTML = `<div class="selection-info"><i class="fas fa-check-square"></i> <span id="selection-count-text">${aiT('noSelection')}</span></div><span id="dblclick-hint-bar" class="dblclick-hint-text">${aiT('doubleClickHint')}</span><button id="clear-selection-btn" class="btn-clear-selection" title="Clear" style="display:none;"><i class="fas fa-times"></i></button>`;
    const hdr = ls.querySelector('.column-header');
    if (hdr) hdr.after(bar);
    document.getElementById('clear-selection-btn')?.addEventListener('click', () => {
        window.fileManager.selectedFiles.clear(); refreshCheckboxes(); updateSelectionDisplay(); updateAiContextDisplay();
    });
}

function addDoubleClickHintBar() {
    // Now integrated into selection status bar
}

function patchFileTable() {
    if (!window.fileManager) return;
    const orig = window.fileManager.updateFileTable.bind(window.fileManager);
    window.fileManager.updateFileTable = function(q = '') {
        orig(q);
        injectCheckboxes();
        injectTableHeaderCheckbox();
        injectTreeDisplay();
    };
    if (window.fileManager.currentFiles && window.fileManager.currentFiles.length > 0) {
        injectCheckboxes(); injectTableHeaderCheckbox(); injectTreeDisplay();
    }
}

function injectTableHeaderCheckbox() {
    const thead = document.querySelector('.sticky-header thead tr');
    if (!thead || thead.querySelector('.th-checkbox')) return;
    const th = document.createElement('th'); th.className = 'th-checkbox'; th.style.width = '36px';
    th.innerHTML = '<input type="checkbox" class="file-select-all" title="Select All">';
    thead.insertBefore(th, thead.firstChild);
    th.querySelector('.file-select-all').addEventListener('change', (e) => {
        const chk = e.target.checked;
        document.querySelectorAll('.file-select-cb').forEach(cb => {
            cb.checked = chk;
            const p = cb.closest('tr')?.getAttribute('data-path');
            if (p) { if (chk) window.fileManager.selectedFiles.add(p); else window.fileManager.selectedFiles.delete(p); }
        });
        updateSelectionDisplay(); updateAiContextDisplay();
    });
}

function injectCheckboxes() {
    document.querySelectorAll('#file-table-body tr[data-path]').forEach(row => {
        if (row.querySelector('.file-select-cb')) return;
        const td = document.createElement('td'); td.className = 'td-checkbox';
        const cb = document.createElement('input'); cb.type = 'checkbox'; cb.className = 'file-select-cb';
        const p = row.getAttribute('data-path');
        if (window.fileManager.selectedFiles && window.fileManager.selectedFiles.has(p)) cb.checked = true;

        cb.addEventListener('change', (e) => {
            e.stopPropagation();
            if (cb.checked) window.fileManager.selectedFiles.add(p); else window.fileManager.selectedFiles.delete(p);
            // Cascade selection for directories
            const file = (window.fileManager.currentFiles || []).find(f => f.path === p);
            if (file && file.is_directory) {
                cascadeSelection(p, file.depth, cb.checked);
            }
            updateSelectionDisplay(); updateAiContextDisplay();
        });
        cb.addEventListener('dblclick', (e) => e.stopPropagation());
        td.appendChild(cb); row.insertBefore(td, row.firstChild);
        row.removeAttribute('title');
    });
    document.querySelectorAll('#file-table-body .empty-message').forEach(td => td.setAttribute('colspan', '5'));
}

/**
 * Cascade selection: when a directory is checked/unchecked, all children follow
 */
function cascadeSelection(dirPath, dirDepth, checked) {
    const allFiles = window.fileManager.currentFiles || [];
    const normalDir = dirPath.replace(/\\/g, '/');
    for (const f of allFiles) {
        const normalPath = f.path.replace(/\\/g, '/');
        if (normalPath.startsWith(normalDir + '/') && f.depth > dirDepth) {
            if (checked) window.fileManager.selectedFiles.add(f.path);
            else window.fileManager.selectedFiles.delete(f.path);
        }
    }
    refreshCheckboxes();
}

/**
 * Inject tree-style indentation and collapse/expand for directories
 */
function injectTreeDisplay() {
    const rows = document.querySelectorAll('#file-table-body tr[data-path]');
    rows.forEach(row => {
        if (row.getAttribute('data-tree-done')) return;
        row.setAttribute('data-tree-done', '1');
        const path = row.getAttribute('data-path');
        const file = (window.fileManager.currentFiles || []).find(f => f.path === path);
        if (!file) return;

        const nameCell = row.querySelector('.file-name-cell') || row.querySelector('td:nth-child(2)') || row.querySelector('td:first-child');
        if (!nameCell) return;

        // Add depth-based indentation
        const indent = (file.depth || 0) * 18;
        nameCell.style.paddingLeft = (indent + 8) + 'px';

        // Add collapse arrow for directories
        if (file.is_directory) {
            row.classList.add('tree-dir-row');
            const isCollapsed = state.collapsedDirs.has(path);
            const arrow = document.createElement('span');
            arrow.className = 'tree-arrow' + (isCollapsed ? ' collapsed' : ' expanded');
            arrow.textContent = isCollapsed ? '▶' : '▼';
            arrow.style.marginRight = '5px';
            arrow.style.cursor = 'pointer';
            arrow.style.fontSize = '10px';
            arrow.style.opacity = '0.7';
            arrow.style.display = 'inline-block';
            arrow.style.width = '12px';
            arrow.addEventListener('click', (e) => {
                e.stopPropagation();
                e.preventDefault();
                toggleTreeNode(row, path, file.depth);
            });
            nameCell.insertBefore(arrow, nameCell.firstChild);

            // If this dir was previously collapsed, hide children
            if (isCollapsed) {
                hideChildrenOf(row, file.depth);
            }
        }
    });
}

function toggleTreeNode(dirRow, dirPath, dirDepth) {
    const arrow = dirRow.querySelector('.tree-arrow');
    if (!arrow) return;
    const isCollapsed = arrow.classList.contains('collapsed');

    if (isCollapsed) {
        // Expand
        arrow.classList.remove('collapsed');
        arrow.classList.add('expanded');
        arrow.textContent = '▼';
        state.collapsedDirs.delete(dirPath);
        showChildrenOf(dirRow, dirDepth);
    } else {
        // Collapse
        arrow.classList.remove('expanded');
        arrow.classList.add('collapsed');
        arrow.textContent = '▶';
        state.collapsedDirs.add(dirPath);
        hideChildrenOf(dirRow, dirDepth);
    }
}

function hideChildrenOf(dirRow, dirDepth) {
    let next = dirRow.nextElementSibling;
    while (next) {
        const nextPath = next.getAttribute('data-path');
        if (!nextPath) break;
        const nextFile = (window.fileManager.currentFiles || []).find(f => f.path === nextPath);
        if (!nextFile || nextFile.depth <= dirDepth) break;
        next.style.display = 'none';
        next = next.nextElementSibling;
    }
}

function showChildrenOf(dirRow, dirDepth) {
    let next = dirRow.nextElementSibling;
    while (next) {
        const nextPath = next.getAttribute('data-path');
        if (!nextPath) break;
        const nextFile = (window.fileManager.currentFiles || []).find(f => f.path === nextPath);
        if (!nextFile || nextFile.depth <= dirDepth) break;
        // Only show if parent chain is not collapsed
        let shouldShow = true;
        if (nextFile.is_directory && state.collapsedDirs.has(nextPath)) {
            // This dir itself is collapsed, show the row but its children stay hidden
            next.style.display = '';
            // Skip its children
            const skipDepth = nextFile.depth;
            next = next.nextElementSibling;
            while (next) {
                const sp = next.getAttribute('data-path');
                if (!sp) break;
                const sf = (window.fileManager.currentFiles || []).find(f => f.path === sp);
                if (!sf || sf.depth <= skipDepth) break;
                next = next.nextElementSibling;
            }
            continue;
        }
        if (shouldShow) next.style.display = '';
        next = next.nextElementSibling;
    }
}

function refreshCheckboxes() {
    document.querySelectorAll('.file-select-cb').forEach(cb => {
        const p = cb.closest('tr')?.getAttribute('data-path');
        cb.checked = p && window.fileManager.selectedFiles.has(p);
    });
    const sa = document.querySelector('.file-select-all');
    if (sa) sa.checked = false;
}

function updateSelectionDisplay() {
    const ct = document.getElementById('selection-count-text');
    const cb = document.getElementById('clear-selection-btn');
    if (!ct) return;
    const n = window.fileManager && window.fileManager.selectedFiles ? window.fileManager.selectedFiles.size : 0;
    if (n > 0) { ct.textContent = aiT('selectedFiles', {count: n}); if (cb) cb.style.display = 'flex'; }
    else { ct.textContent = aiT('noSelection'); if (cb) cb.style.display = 'none'; }
}

// Init file selection after FileManagerApp is ready
window.addEventListener('load', () => { setTimeout(initFileSelection, 300); });

