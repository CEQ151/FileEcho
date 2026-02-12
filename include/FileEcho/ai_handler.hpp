
#pragma once

#include <string>
#include <memory>
#include <vector>
#include <functional>
#include "json.hpp"

namespace AI {

// AI模型提供商枚举
enum class AIProvider {
    OPENAI,
    DEEPSEEK,
    GEMINI,
    GROK,
    CLAUDE,
    KIMI,
    GLM,
    CUSTOM
};

// AI模型配置结构体
struct AIModelConfig {
    AIProvider provider;
    std::string name;              // 模型名称，如 "gpt-4", "deepseek-chat"
    std::string api_key;           // API密钥
    std::string base_url;          // API基础URL
    std::string model_id;          // 模型ID
    double temperature;            // 温度参数
    int max_tokens;               // 最大token数
    bool enabled;                 // 是否启用
    int max_content_chars;        // 单文件最大读取字符数 (0=不限)
    int max_file_count;           // 最大文件数 (0=不限)
};

// AI响应结构体
struct AIResponse {
    bool success;
    std::string content;
    std::string error;
    int prompt_tokens = 0;
    int completion_tokens = 0;
    int total_tokens = 0;
};

// AI处理器类
class AIHandler {
public:
    AIHandler();
    ~AIHandler();

    // 初始化AI处理器
    bool Initialize();

    // 设置模型配置
    bool SetModelConfig(const AIModelConfig& config);

    // 获取当前配置
    AIModelConfig GetCurrentConfig() const;

    // 生成聊天回复
    AIResponse GenerateChatResponse(const std::string& message, const nlohmann::json& context = {}, const std::string& language = "en");

    // 生成文件摘要
    AIResponse GenerateFileSummary(const std::string& fileName, const std::string& filePath, const std::string& content, const std::string& language = "en", bool brief = false);

    // 生成项目总结
    AIResponse GenerateProjectSummary(const std::string& projectPath, const std::string& fileTree, const std::string& language = "en");

    // 生成调整建议
    AIResponse GenerateCleanupSuggestions(const std::string& projectPath, const std::string& fileTree, const std::string& language = "en");

    // 生成代码分析
    AIResponse GenerateCodeAnalysis(const std::string& projectPath, const std::string& fileTree, const std::string& language = "en");

    // 保存配置到文件
    bool SaveConfig(const std::string& configPath);

    // 从文件加载配置
    bool LoadConfig(const std::string& configPath);

    // 测试连接
    AIResponse TestConnection();

    // 获取支持的模型列表
    static std::vector<AIModelConfig> GetSupportedModels();

private:
    // HTTP请求辅助函数
    std::string SendHttpRequest(
        const std::string& url,
        const std::string& method,
        const std::string& body,
        const std::vector<std::pair<std::string, std::string>>& headers
    );

    // 构造API请求体
    std::string BuildChatRequestBody(const std::string& message);

    // 解析API响应
    AIResponse ParseAIResponse(const std::string& response);

    // 获取API基础URL
    std::string GetApiBaseUrl(AIProvider provider);

    // 获取模型ID
    std::string GetModelId(AIProvider provider, const std::string& customModelId);

    // 成员变量
    AIModelConfig current_config_;
    bool initialized_;
};

} // namespace AI
