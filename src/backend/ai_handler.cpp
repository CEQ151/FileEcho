
#include "FileEcho/ai_handler.hpp"
#include "FileEcho/webserver.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <algorithm>
#include "FileEcho/utils.hpp"
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <winhttp.h>

#pragma comment(lib, "winhttp.lib")

namespace {
    // Helper function to parse URL for WinHTTP
    struct UrlComponents {
        std::wstring host;
        std::wstring path;
        int port;
        bool is_secure;
    };

    UrlComponents ParseUrl(const std::string& url) {
        UrlComponents comp;
        comp.port = 80;
        comp.is_secure = false;
        comp.path = L"/";
        
        std::string working_url = url;
        
        // Check scheme
        if (working_url.find("https://") == 0) {
            comp.is_secure = true;
            comp.port = 443;
            working_url = working_url.substr(8);
        } else if (working_url.find("http://") == 0) {
            working_url = working_url.substr(7);
        }
        
        // Find path start
        size_t path_start = working_url.find('/');
        std::string host_port;
        
        if (path_start == std::string::npos) {
            host_port = working_url;
        } else {
            host_port = working_url.substr(0, path_start);
            comp.path = Utils::ToWString(working_url.substr(path_start));
        }
        
        // Parse port
        size_t port_sep = host_port.find(':');
        if (port_sep != std::string::npos) {
            comp.host = Utils::ToWString(host_port.substr(0, port_sep));
            try {
                comp.port = std::stoi(host_port.substr(port_sep + 1));
            } catch(...) {
                // Keep default port on error
            }
        } else {
            comp.host = Utils::ToWString(host_port);
        }
        
        return comp;
    }
}

using json = nlohmann::json;
using namespace std;
namespace fs = std::filesystem;

namespace AI {

// 获取支持的模型列表
std::vector<AIModelConfig> AIHandler::GetSupportedModels() {
    return {
        // OpenAI Models
        {AIProvider::OPENAI, "GPT-4.1", "", "https://api.openai.com/v1", "gpt-4.1", 0.7, 4096, true},
        {AIProvider::OPENAI, "GPT-5 Nano", "", "https://api.openai.com/v1", "gpt-5-nano", 0.7, 4096, true},
        {AIProvider::OPENAI, "GPT-5 Mini", "", "https://api.openai.com/v1", "gpt-5-mini", 0.7, 4096, true},
        {AIProvider::OPENAI, "GPT-5", "", "https://api.openai.com/v1", "gpt-5", 0.7, 4096, true},
        {AIProvider::OPENAI, "GPT-5.2", "", "https://api.openai.com/v1", "gpt-5.2", 0.7, 4096, true},
        {AIProvider::OPENAI, "GPT-5.2 Pro", "", "https://api.openai.com/v1", "gpt-5.2-pro", 0.7, 4096, true},

        // DeepSeek Models
        {AIProvider::DEEPSEEK, "DeepSeek Chat (V3)", "", "https://api.deepseek.com/v1", "deepseek-chat", 0.7, 4096, true},
        {AIProvider::DEEPSEEK, "DeepSeek Reasoner (R1)", "", "https://api.deepseek.com/v1", "deepseek-reasoner", 0.7, 4096, true},

        // Gemini Models
        {AIProvider::GEMINI, "Gemini 2.5 Flash Lite", "", "https://generativelanguage.googleapis.com/v1beta", "gemini-2.5-flash-lite", 0.7, 4096, true},
        {AIProvider::GEMINI, "Gemini 2.5 Flash", "", "https://generativelanguage.googleapis.com/v1beta", "gemini-2.5-flash", 0.7, 4096, true},
        {AIProvider::GEMINI, "Gemini 2.5 Pro", "", "https://generativelanguage.googleapis.com/v1beta", "gemini-2.5-pro", 0.7, 4096, true},
        {AIProvider::GEMINI, "Gemini 3.0 Flash (Preview)", "", "https://generativelanguage.googleapis.com/v1beta", "gemini-3-flash-preview", 0.7, 4096, true},
        {AIProvider::GEMINI, "Gemini 3.0 Pro (Preview)", "", "https://generativelanguage.googleapis.com/v1beta", "gemini-3-pro-preview", 0.7, 4096, true},

        // Grok Models
        {AIProvider::GROK, "Grok 4 Fast", "", "https://api.x.ai/v1", "grok-4-fast-non-reasoning", 0.7, 4096, true},
        {AIProvider::GROK, "Grok 4 Fast Reasoning", "", "https://api.x.ai/v1", "grok-4-fast-reasoning", 0.7, 4096, true},
        {AIProvider::GROK, "Grok 4.1 Fast", "", "https://api.x.ai/v1", "grok-4-1-fast-non-reasoning", 0.7, 4096, true},
        {AIProvider::GROK, "Grok 4.1 Fast Reasoning", "", "https://api.x.ai/v1", "grok-4-1-fast-reasoning", 0.7, 4096, true},

        // Claude Models
        {AIProvider::CLAUDE, "Claude Haiku 4.5", "", "https://api.anthropic.com/v1", "claude-haiku-4-5", 0.7, 4096, true},
        {AIProvider::CLAUDE, "Claude Sonnet 4.5", "", "https://api.anthropic.com/v1", "claude-sonnet-4-5", 0.7, 4096, true},
        {AIProvider::CLAUDE, "Claude Opus 4.6", "", "https://api.anthropic.com/v1", "claude-opus-4-6", 0.7, 4096, true},

        // Kimi Models (月之暗面)
        {AIProvider::KIMI, "Kimi K2 Turbo (Preview)", "", "https://api.moonshot.cn/v1", "kimi-k2-turbo-preview", 0.7, 4096, true},
        {AIProvider::KIMI, "Kimi K2.5", "", "https://api.moonshot.cn/v1", "kimi-k2.5", 0.7, 4096, true},

        // GLM Models (智谱)
        {AIProvider::GLM, "GLM-4.5 AirX", "", "https://open.bigmodel.cn/api/paas/v4", "glm-4.5-airx", 0.7, 4096, true},
        {AIProvider::GLM, "GLM-4.6", "", "https://open.bigmodel.cn/api/paas/v4", "glm-4.6", 0.7, 4096, true},
        {AIProvider::GLM, "GLM-4.7", "", "https://open.bigmodel.cn/api/paas/v4", "glm-4.7", 0.7, 4096, true},
        {AIProvider::GLM, "GLM-4.7 FlashX", "", "https://open.bigmodel.cn/api/paas/v4", "glm-4.7-flashx", 0.7, 4096, true},
        {AIProvider::GLM, "GLM-5", "", "https://open.bigmodel.cn/api/paas/v4", "glm-5", 0.7, 4096, true},

        // Custom Model
        {AIProvider::CUSTOM, "Custom Model", "", "", "", 0.7, 4096, true}
    };
}

AIHandler::AIHandler() : initialized_(false) {
    current_config_ = {
        AIProvider::OPENAI,
        "GPT-4.1",
        "",
        "https://api.openai.com/v1",
        "gpt-4.1",
        0.7,
        4096,
        true,
        0,  // max_content_chars (0=unlimited)
        0   // max_file_count (0=unlimited)
    };
}

AIHandler::~AIHandler() {
}

bool AIHandler::Initialize() {
    initialized_ = true;
    return true;
}

bool AIHandler::SetModelConfig(const AIModelConfig& config) {
    current_config_ = config;
    return true;
}

AIModelConfig AIHandler::GetCurrentConfig() const {
    return current_config_;
}

AIResponse AIHandler::GenerateChatResponse(const std::string& message, const json& context, const std::string& language) {
    AIResponse response;
    response.success = false;

    if (!initialized_ || current_config_.api_key.empty()) {
        response.error = "AI not initialized or API key not set";
        return response;
    }

    try {
        // 构建完整消息（附加上下文）
        std::string final_message = message;
        // Add language instruction
        if (language == "zh") {
            final_message = "Please respond in Chinese (中文).\n\n" + final_message;
        }
        if (!context.empty()) {
            final_message += "\n\nContext:\n";
            if (context.contains("current_path")) {
                final_message += "Current Path: " + context["current_path"].get<std::string>() + "\n";
            }
            if (context.contains("file_count")) {
                final_message += "File Count: " + std::to_string(context["file_count"].get<int>()) + "\n";
            }
            if (context.contains("top_files") && context["top_files"].is_array()) {
                final_message += "Files:\n";
                for (const auto& file : context["top_files"]) {
                    final_message += "- " + file.get<std::string>() + "\n";
                }
            }
            if (context.contains("file_tree") && context["file_tree"].is_string()) {
                final_message += "\nFile Tree:\n" + context["file_tree"].get<std::string>() + "\n";
            }
            if (context.contains("file_contents") && context["file_contents"].is_array()) {
                final_message += "\nFile Contents:\n";
                for (const auto& fc : context["file_contents"]) {
                    if (fc.contains("name") && fc.contains("content")) {
                        final_message += "--- " + fc["name"].get<std::string>() + " ---\n";
                        final_message += fc["content"].get<std::string>() + "\n\n";
                    }
                }
            }
        }

        // Gemini uses a completely different API format
        // But relay/proxy stations use OpenAI-compatible format, so detect relay
        std::string base_url = GetApiBaseUrl(current_config_.provider);
        bool using_relay = (!current_config_.base_url.empty() && base_url == current_config_.base_url);
        // Also detect relay for Gemini: if URL is not the official Gemini endpoint
        if (current_config_.provider == AIProvider::GEMINI && base_url.find("generativelanguage.googleapis.com") == std::string::npos) {
            using_relay = true;
        }

        if (current_config_.provider == AIProvider::GEMINI && !using_relay) {
            // Gemini URL: /models/{model}:generateContent?key=API_KEY
            std::string url = GetApiBaseUrl(current_config_.provider);
            url += "/models/" + current_config_.model_id + ":generateContent?key=" + current_config_.api_key;

            // Gemini request body format
            int eff_tokens = std::min(std::max(current_config_.max_tokens, 1), 8192);
            json gemini_body = {
                {"contents", {{
                    {"parts", {{
                        {"text", final_message}
                    }}}
                }}},
                {"generationConfig", {
                    {"temperature", current_config_.temperature},
                    {"maxOutputTokens", eff_tokens}
                }}
            };

            std::vector<std::pair<std::string, std::string>> headers = {
                {"Content-Type", "application/json"}
            };

            std::string api_response = SendHttpRequest(url, "POST", gemini_body.dump(), headers);

            // Parse Gemini response format
            try {
                auto json_resp = json::parse(api_response);
                if (json_resp.contains("error")) {
                    response.error = json_resp["error"]["message"].get<std::string>();
                    return response;
                }
                if (json_resp.contains("candidates") && !json_resp["candidates"].empty()) {
                    auto& candidate = json_resp["candidates"][0];
                    if (candidate.contains("content") && candidate["content"].contains("parts") 
                        && !candidate["content"]["parts"].empty()) {
                        response.content = candidate["content"]["parts"][0]["text"].get<std::string>();
                        response.success = true;
                    } else {
                        response.error = "Invalid Gemini response format";
                    }
                } else {
                    response.error = "No candidates in Gemini response";
                }
                // Extract Gemini token usage
                if (json_resp.contains("usageMetadata")) {
                    auto& usage = json_resp["usageMetadata"];
                    response.prompt_tokens = usage.value("promptTokenCount", 0);
                    response.completion_tokens = usage.value("candidatesTokenCount", 0);
                    response.total_tokens = usage.value("totalTokenCount", 0);
                }
            } catch (const std::exception& e) {
                response.error = "Failed to parse Gemini response: " + std::string(e.what());
            }
        } else {
            // OpenAI-compatible API (OpenAI, DeepSeek, Grok, Kimi, GLM, Custom)
            // Also used for relay/proxy stations regardless of provider
            std::string url = base_url;

            // Claude uses different API format (only for official API, not relay)
            if (current_config_.provider == AIProvider::CLAUDE && !using_relay) {
                url += "/messages";

                int eff_tokens = std::min(std::max(current_config_.max_tokens, 1), 8192);
                json claude_body = {
                    {"model", current_config_.model_id},
                    {"max_tokens", eff_tokens},
                    {"messages", {
                        {{"role", "user"}, {"content", final_message}}
                    }}
                };

                std::vector<std::pair<std::string, std::string>> headers = {
                    {"Content-Type", "application/json"},
                    {"x-api-key", current_config_.api_key},
                    {"anthropic-version", "2023-06-01"}
                };

                std::string api_response = SendHttpRequest(url, "POST", claude_body.dump(), headers);

                // Parse Claude response format
                try {
                    if (api_response.empty()) {
                        response.error = "Empty response from Claude API";
                        return response;
                    }
                    auto json_resp = json::parse(api_response);
                    if (json_resp.contains("error")) {
                        response.error = json_resp["error"]["message"].get<std::string>();
                        return response;
                    }
                    if (json_resp.contains("content") && !json_resp["content"].empty()) {
                        response.content = json_resp["content"][0]["text"].get<std::string>();
                        response.success = true;
                    } else {
                        response.error = "Invalid Claude response format";
                    }
                    // Extract Claude token usage
                    if (json_resp.contains("usage")) {
                        auto& usage = json_resp["usage"];
                        response.prompt_tokens = usage.value("input_tokens", 0);
                        response.completion_tokens = usage.value("output_tokens", 0);
                        response.total_tokens = response.prompt_tokens + response.completion_tokens;
                    }
                } catch (const std::exception& e) {
                    response.error = "Failed to parse Claude response: " + std::string(e.what());
                }
            } else {
                url += "/chat/completions";

                std::string request_body = BuildChatRequestBody(final_message);

                std::vector<std::pair<std::string, std::string>> headers = {
                    {"Content-Type", "application/json"},
                    {"Authorization", "Bearer " + current_config_.api_key}
                };

                std::string api_response = SendHttpRequest(url, "POST", request_body, headers);
                response = ParseAIResponse(api_response);
            }
        }

    } catch (const std::exception& e) {
        response.success = false;
        response.error = e.what();
    }

    return response;
}

AIResponse AIHandler::GenerateFileSummary(const std::string& fileName, const std::string& filePath, const std::string& content, const std::string& language, bool brief) {
    int maxChars = current_config_.max_content_chars;
    std::string truncated;
    if (maxChars > 0 && (int)content.length() > maxChars) {
        truncated = content.substr(0, maxChars) + "\n...[Content Truncated]";
    } else {
        truncated = content.length() > 5000 ? content.substr(0, 5000) + "\n...[Content Truncated]" : content;
    }
    int briefLimit = maxChars > 0 ? std::min(maxChars, 2000) : 2000;
    std::string prompt;
    if (brief) {
        std::string briefContent = (int)content.length() > briefLimit ? content.substr(0, briefLimit) : content;
        if (language == "zh") {
            prompt = "用 1-2 句话简要概括这个文件的用途，务必简洁。\n文件：" + fileName + "\n内容：\n" + briefContent;
        } else {
            prompt = "Summarize this file in 1-2 sentences. Be very concise.\nFile: " + fileName + "\nContent:\n" + briefContent;
        }
    } else {
        if (language == "zh") {
            prompt = "请分析以下文件：\n文件名：" + fileName + "\n文件路径：" + filePath + "\n\n内容：\n" + truncated + "\n\n请简洁地总结此文件的功能、关键组件和潜在问题。";
        } else {
            prompt = "Please analyze the following file:\nFile Name: " + fileName + "\nFile Path: " + filePath + "\n\nContent (truncated if too long):\n" + truncated + "\n\nProvide a concise summary of what this file does, its key components, and any potential issues.";
        }
    }
    return GenerateChatResponse(prompt, {}, language);
}

AIResponse AIHandler::GenerateProjectSummary(const std::string& projectPath, const std::string& fileTree, const std::string& language) {
    if (fileTree.empty()) {
        return {false, "", language == "zh" ? "未找到文件，请先扫描目录。" : "No files found in the context. Please scan the directory first."};
    }

    std::string prompt;
    if (language == "zh") {
        prompt = "你是一个代码分析AI。我将提供一个项目的文件树结构，你必须仅根据这些文件进行分析，不要臆测未列出的文件。\n\n"
                 "项目路径：" + projectPath + "\n\n"
                 "文件树结构：\n" + fileTree + "\n\n"
                 "任务：\n1. 仅根据这些文件识别项目类型和技术栈\n2. 描述列表中的关键模块\n3. 推断项目的用途\n\n如果文件列表太短无法判断，请明确说明。";
    } else {
        prompt = "You are a code analysis AI. I will provide you with a file tree structure from a project. "
                 "You MUST analyze ONLY the provided file tree. Do NOT hallucinate or assume files that are not listed.\n\n"
                 "Project Path: " + projectPath + "\n\n"
                 "File Tree:\n" + fileTree + "\n\n"
                 "Task:\n"
                 "1. Identify the project type and tech stack based ONLY on these files.\n"
                 "2. Describe key modules found in the tree.\n"
                 "3. Infer the likely purpose of the project.\n\n"
                 "If the file tree is too short or generic to determine the project type, state that clearly.";
    }

    return GenerateChatResponse(prompt, {}, language);
}

AIResponse AIHandler::GenerateCleanupSuggestions(const std::string& projectPath, const std::string& fileTree, const std::string& language) {
    if (fileTree.empty()) {
        return {false, "", language == "zh" ? "未找到可分析的文件。" : "No files found to analyze."};
    }

    std::string prompt;
    if (language == "zh") {
        prompt = "分析以下项目文件树结构（" + projectPath + "），基于文件名称和项目结构，为用户提供文件归纳和组织调整建议。\n\n文件树：\n" + fileTree + "\n\n"
                 "识别：\n1. 可以归类或合并的文件\n2. 临时文件、构建产物（如 node_modules, build/, dist/）\n3. 目录结构优化建议\n4. 可能冗余的配置或日志文件";
    } else {
        prompt = "Analyze the following file tree for a project at " + projectPath + " and provide organization and restructuring suggestions.\n\n"
                 "File Tree:\n" + fileTree + "\n\n"
                 "Identify:\n"
                 "1. Files that could be categorized or consolidated\n"
                 "2. Temporary files, build artifacts (like node_modules, build/, dist/)\n"
                 "3. Directory structure optimization opportunities\n"
                 "4. Potentially redundant configs or log files";
    }
    return GenerateChatResponse(prompt, {}, language);
}

AIResponse AIHandler::GenerateCodeAnalysis(const std::string& projectPath, const std::string& fileTree, const std::string& language) {
    if (fileTree.empty()) {
        return {false, "", language == "zh" ? "未找到可分析的文件。" : "No files found to analyze."};
    }

    std::string prompt;
    if (language == "zh") {
        prompt = "根据以下文件树结构分析代码结构：\n" + fileTree + "\n\n提供以下方面的见解：\n1. 项目结构/架构\n2. 命名规范\n3. 使用的主要编程语言";
    } else {
        prompt = "Analyze the code structure based on this file tree:\n" + fileTree + "\n\n"
                 "Provide insights on:\n"
                 "1. Project Structure/Architecture\n"
                 "2. Naming Conventions\n"
                 "3. Main programming languages used";
    }
    return GenerateChatResponse(prompt, {}, language);
}

AIResponse AIHandler::GenerateTreeAnnotations(const std::string& projectPath, const std::string& fileTree, const std::string& language) {
    if (fileTree.empty()) {
        return {false, "", language == "zh" ? "未找到文件，请先扫描目录。" : "No files found. Please scan a directory first."};
    }

    std::string prompt;
    if (language == "zh") {
        prompt = "你是一个代码分析AI。我将提供一个项目的文件树，请为每个文件和文件夹写一个简短的中文注释（不超过15个字）。\n\n"
                 "项目路径：" + projectPath + "\n\n"
                 "文件树：\n" + fileTree + "\n\n"
                 "**输出格式要求（严格遵守）：**\n"
                 "- 每行格式为：`文件名|||注释`\n"
                 "- 例如：`main.cpp|||程序入口`\n"
                 "- 例如：`src/|||源代码目录`\n"
                 "- 不要输出任何其他内容，不要有标题、解释或Markdown格式\n"
                 "- 每个文件名（含目录名）只出现一次\n"
                 "- 目录名不要带斜杠\n"
                 "- 根据文件名和项目上下文推断用途，注释务必精炼";
    } else {
        prompt = "You are a code analysis AI. I will provide a project file tree. Write a brief English annotation (max 10 words) for each file and folder.\n\n"
                 "Project Path: " + projectPath + "\n\n"
                 "File Tree:\n" + fileTree + "\n\n"
                 "**Output format (strictly follow):**\n"
                 "- Each line: `filename|||annotation`\n"
                 "- Example: `main.cpp|||Application entry point`\n"
                 "- Example: `src/|||Source code directory`\n"
                 "- Do NOT output anything else — no titles, explanations, or Markdown\n"
                 "- Each filename (including directory name) appears exactly once\n"
                 "- Directory names without trailing slash\n"
                 "- Infer purpose from filename and project context, keep annotations concise";
    }
    return GenerateChatResponse(prompt, {}, language);
}

AIResponse AIHandler::TestConnection() {
    AIResponse response;
    response.success = false;

    if (!initialized_ || current_config_.api_key.empty()) {
        response.error = "AI not initialized or API key not set";
        return response;
    }

    try {
        std::string url;
        std::string method = "GET";
        std::string body = "";
        std::vector<std::pair<std::string, std::string>> headers = {
            {"Content-Type", "application/json"}
        };

        std::string test_base_url = GetApiBaseUrl(current_config_.provider);
        bool using_relay = (!current_config_.base_url.empty() && test_base_url == current_config_.base_url);
        if (current_config_.provider == AIProvider::GEMINI && test_base_url.find("generativelanguage.googleapis.com") == std::string::npos) {
            using_relay = true;
        }

        if (current_config_.provider == AIProvider::GEMINI && !using_relay && !using_relay) {
            // Gemini: send a minimal generateContent request to verify model
            url = GetApiBaseUrl(current_config_.provider) + "/models/" + current_config_.model_id + ":generateContent?key=" + current_config_.api_key;
            method = "POST";
            json test_body = {
                {"contents", {{{"parts", {{{"text", "Hi"}}}}}}},
                {"generationConfig", {{"maxOutputTokens", 10}}}
            };
            body = test_body.dump();
        } else if (current_config_.provider == AIProvider::CLAUDE && !using_relay) {
            // Claude: send a minimal message to verify model
            url = GetApiBaseUrl(current_config_.provider) + "/messages";
            method = "POST";
            json test_body = {
                {"model", current_config_.model_id},
                {"max_tokens", 10},
                {"messages", {{{"role", "user"}, {"content", "Hi"}}}}
            };
            body = test_body.dump();
            headers.push_back({"x-api-key", current_config_.api_key});
            headers.push_back({"anthropic-version", "2023-06-01"});
        } else {
            // OpenAI-compatible: send a minimal chat completion to verify model
            url = GetApiBaseUrl(current_config_.provider) + "/chat/completions";
            method = "POST";
            json test_body = {
                {"model", current_config_.model_id},
                {"max_tokens", 10},
                {"messages", {{{"role", "user"}, {"content", "Hi"}}}}
            };
            body = test_body.dump();
            headers.push_back({"Authorization", "Bearer " + current_config_.api_key});
        }

        std::string api_response = SendHttpRequest(url, method, body, headers);

        if (api_response.empty()) {
            response.error = "Empty response from server";
            return response;
        }

        auto json_resp = json::parse(api_response);
        if (json_resp.contains("error")) {
            std::string errMsg = json_resp["error"].is_object() 
                ? json_resp["error"]["message"].get<std::string>() 
                : json_resp["error"].get<std::string>();
            response.error = errMsg;
        } else {
            response.success = true;
            response.content = "Connection successful";
        }

    } catch (const std::exception& e) {
        response.error = e.what();
    }

    return response;
}

bool AIHandler::SaveConfig(const std::string& configPath) {
    try {
        json config_json = {
            {"provider", static_cast<int>(current_config_.provider)},
            {"name", current_config_.name},
            {"api_key", current_config_.api_key},
            {"base_url", current_config_.base_url},
            {"model_id", current_config_.model_id},
            {"temperature", current_config_.temperature},
            {"max_tokens", current_config_.max_tokens},
            {"enabled", current_config_.enabled},
            {"max_content_chars", current_config_.max_content_chars},
            {"max_file_count", current_config_.max_file_count}
        };

        std::ofstream config_file(configPath);
        if (!config_file.is_open()) {
            return false;
        }

        config_file << config_json.dump(4);
        config_file.close();

        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error saving AI config: " << e.what() << std::endl;
        return false;
    }
}

bool AIHandler::LoadConfig(const std::string& configPath) {
    try {
        if (!fs::exists(configPath)) {
            return false;
        }

        std::ifstream config_file(configPath);
        if (!config_file.is_open()) {
            return false;
        }

        json config_json;
        config_file >> config_json;
        config_file.close();

        current_config_.provider = static_cast<AIProvider>(config_json.value("provider", 0));
        current_config_.name = config_json.value("name", "");
        current_config_.api_key = config_json.value("api_key", "");
        current_config_.base_url = config_json.value("base_url", "");
        current_config_.model_id = config_json.value("model_id", "");
        current_config_.temperature = config_json.value("temperature", 0.7);
        current_config_.max_tokens = config_json.value("max_tokens", 4096);
        current_config_.enabled = config_json.value("enabled", true);
        current_config_.max_content_chars = config_json.value("max_content_chars", 0);
        current_config_.max_file_count = config_json.value("max_file_count", 0);

        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error loading AI config: " << e.what() << std::endl;
        return false;
    }
}

std::string AIHandler::SendHttpRequest(
    const std::string& url,
    const std::string& method,
    const std::string& body,
    const std::vector<std::pair<std::string, std::string>>& headers) {

    // 解析 URL
    auto urlComp = ParseUrl(url);

    HINTERNET hSession = NULL, hConnect = NULL, hRequest = NULL;
    std::string response;

    // Detect system proxy (IE/Windows settings) for proper proxy support
    // WINHTTP_ACCESS_TYPE_DEFAULT_PROXY only reads netsh winhttp proxy,
    // not the system proxy used by browsers and most proxy tools.
    WINHTTP_CURRENT_USER_IE_PROXY_CONFIG ieConfig = {0};
    bool useNamedProxy = false;
    std::wstring proxyStr;

    if (WinHttpGetIEProxyConfigForCurrentUser(&ieConfig)) {
        if (ieConfig.lpszProxy && wcslen(ieConfig.lpszProxy) > 0) {
            proxyStr = ieConfig.lpszProxy;
            useNamedProxy = true;
        }
        if (ieConfig.lpszProxy) GlobalFree(ieConfig.lpszProxy);
        if (ieConfig.lpszProxyBypass) GlobalFree(ieConfig.lpszProxyBypass);
        if (ieConfig.lpszAutoConfigUrl) GlobalFree(ieConfig.lpszAutoConfigUrl);
    }

    // 1. Open WinHTTP session with detected proxy
    if (useNamedProxy) {
        hSession = WinHttpOpen(L"FileEcho AI Client/1.0",
                               WINHTTP_ACCESS_TYPE_NAMED_PROXY,
                               proxyStr.c_str(),
                               WINHTTP_NO_PROXY_BYPASS, 0);
    } else {
        hSession = WinHttpOpen(L"FileEcho AI Client/1.0",  
                               WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
                               WINHTTP_NO_PROXY_NAME, 
                               WINHTTP_NO_PROXY_BYPASS, 0);
    }

    if (hSession) {
        // Enable TLS 1.2 (required for many modern APIs like Gemini)
        DWORD dwSecFlags = WINHTTP_FLAG_SECURE_PROTOCOL_TLS1_2 | WINHTTP_FLAG_SECURE_PROTOCOL_TLS1_3;
        WinHttpSetOption(hSession, WINHTTP_OPTION_SECURE_PROTOCOLS, &dwSecFlags, sizeof(dwSecFlags));

        // Set 120-second timeout for connect, send, receive (longer for slow APIs)
        WinHttpSetTimeouts(hSession, 120000, 120000, 120000, 120000);

        // 2. Specify an HTTP server.
        hConnect = WinHttpConnect(hSession, urlComp.host.c_str(), 
                                  (INTERNET_PORT)urlComp.port, 0);
    }

    if (hConnect) {
        // 3. Create an HTTP request handle.
        std::wstring wMethod = Utils::ToWString(method);
        DWORD dwFlags = (urlComp.is_secure ? WINHTTP_FLAG_SECURE : 0);
        
        hRequest = WinHttpOpenRequest(hConnect, wMethod.c_str(), urlComp.path.c_str(),
                                      NULL, WINHTTP_NO_REFERER, 
                                      WINHTTP_DEFAULT_ACCEPT_TYPES, 
                                      dwFlags);
    }

    if (hRequest) {
        // Add headers
        std::wstring wHeaders;
        for (const auto& header : headers) {
            wHeaders += Utils::ToWString(header.first) + L": " + Utils::ToWString(header.second) + L"\r\n";
        }
        
        if (!wHeaders.empty()) {
            WinHttpAddRequestHeaders(hRequest, wHeaders.c_str(), (DWORD)-1L, WINHTTP_ADDREQ_FLAG_ADD);
        }

        // 4. Send a request.
        BOOL bResults = WinHttpSendRequest(hRequest,
                                           WINHTTP_NO_ADDITIONAL_HEADERS, 0,
                                           (LPVOID)body.c_str(), (DWORD)body.length(),
                                           (DWORD)body.length(), 0);

        // 5. End the request.
        if (bResults) {
            bResults = WinHttpReceiveResponse(hRequest, NULL);
        }

        // 6. Read data.
        if (bResults) {
            DWORD dwSize = 0;
            DWORD dwDownloaded = 0;
            LPSTR pszOutBuffer;
            
            // Check status code
            DWORD dwStatusCode = 0;
            DWORD dwSize2 = sizeof(dwStatusCode);
            WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER, 
                                WINHTTP_HEADER_NAME_BY_INDEX, &dwStatusCode, &dwSize2, WINHTTP_NO_HEADER_INDEX);
                                
            do {
                dwSize = 0;
                if (!WinHttpQueryDataAvailable(hRequest, &dwSize)) {
                    // Error
                    break;
                }
                
                if (dwSize == 0) break;

                pszOutBuffer = new char[dwSize + 1];
                if (!pszOutBuffer) {
                    // Out of memory
                    break;
                }
                
                ZeroMemory(pszOutBuffer, dwSize + 1);

                if (WinHttpReadData(hRequest, (LPVOID)pszOutBuffer, dwSize, &dwDownloaded)) {
                    response.append(pszOutBuffer, dwDownloaded);
                }
                
                delete[] pszOutBuffer;
            } while (dwSize > 0);

            // Check if response is valid JSON; if not (e.g. HTML error page), wrap error
            if (!response.empty() && response[0] != '{' && response[0] != '[') {
                std::string snippet = response.substr(0, 200);
                if (hRequest) WinHttpCloseHandle(hRequest);
                if (hConnect) WinHttpCloseHandle(hConnect);
                if (hSession) WinHttpCloseHandle(hSession);
                throw std::runtime_error("HTTP " + std::to_string(dwStatusCode) + " at " + url + " - Server returned non-JSON response: " + snippet);
            }
        } else {
            // Report error with helpful message
             DWORD error = GetLastError();
             std::string errorMsg = "WinHttp request failed with error: " + std::to_string(error);
             if (error == 12002) {
                 errorMsg += " (Timeout - the API server did not respond in time. Please check your network connection, proxy settings, or try again later.)";
             } else if (error == 12029) {
                 errorMsg += " (Cannot connect to server. Please check the URL and your internet connection.)";
             } else if (error == 12175) {
                 errorMsg += " (SSL/TLS error. The server certificate may be invalid or TLS version unsupported.)";
             }
             if (hRequest) WinHttpCloseHandle(hRequest);
             if (hConnect) WinHttpCloseHandle(hConnect);
             if (hSession) WinHttpCloseHandle(hSession);
             throw std::runtime_error(errorMsg);
        }
    } else {
         DWORD error = GetLastError();
         if (hSession) WinHttpCloseHandle(hSession);
         throw std::runtime_error("Failed to initialize WinHttp: " + std::to_string(error));
    }

    // Close handles
    if (hRequest) WinHttpCloseHandle(hRequest);
    if (hConnect) WinHttpCloseHandle(hConnect);
    if (hSession) WinHttpCloseHandle(hSession);
    
    return response;
}

std::string AIHandler::BuildChatRequestBody(const std::string& message) {
    int eff_tokens = std::min(std::max(current_config_.max_tokens, 1), 8192);
    json request_body = {
        {"model", current_config_.model_id},
        {"messages", {
            {{"role", "user"}, {"content", message}}
        }},
        {"temperature", current_config_.temperature},
        {"max_tokens", eff_tokens}
    };

    return request_body.dump();
}

AIResponse AIHandler::ParseAIResponse(const std::string& response) {
    AIResponse result;
    result.success = false;

    if (response.empty()) {
        result.error = "Empty response from API (possible timeout or network error)";
        return result;
    }

    try {
        auto json_response = json::parse(response);

        if (json_response.contains("error")) {
            result.error = json_response["error"]["message"].get<std::string>();
            return result;
        }

        if (json_response.contains("choices") && !json_response["choices"].empty()) {
            result.content = json_response["choices"][0]["message"]["content"].get<std::string>();
            result.success = true;
        } else {
            result.error = "Invalid response format";
        }
        // Extract OpenAI-compatible token usage
        if (json_response.contains("usage")) {
            auto& usage = json_response["usage"];
            result.prompt_tokens = usage.value("prompt_tokens", 0);
            result.completion_tokens = usage.value("completion_tokens", 0);
            result.total_tokens = usage.value("total_tokens", 0);
        }
    } catch (const std::exception& e) {
        result.error = "Failed to parse response: " + std::string(e.what());
    }

    return result;
}

std::string AIHandler::GetApiBaseUrl(AIProvider provider) {
    // Get the official default URL for this provider
    std::string default_url;
    switch (provider) {
        case AIProvider::OPENAI:    default_url = "https://api.openai.com/v1"; break;
        case AIProvider::DEEPSEEK:  default_url = "https://api.deepseek.com/v1"; break;
        case AIProvider::GEMINI:    default_url = "https://generativelanguage.googleapis.com/v1beta"; break;
        case AIProvider::GROK:      default_url = "https://api.x.ai/v1"; break;
        case AIProvider::CLAUDE:    default_url = "https://api.anthropic.com/v1"; break;
        case AIProvider::KIMI:      default_url = "https://api.moonshot.cn/v1"; break;
        case AIProvider::GLM:       default_url = "https://open.bigmodel.cn/api/paas/v4"; break;
        case AIProvider::CUSTOM:    return current_config_.base_url;
        default:                    default_url = "https://api.openai.com/v1"; break;
    }

    // If user set a custom base_url that differs from the official default,
    // use it (supports API relay/proxy stations like api.gptsapi.net)
    if (!current_config_.base_url.empty() && current_config_.base_url != default_url) {
        // Strip trailing slash for consistency
        std::string url = current_config_.base_url;
        while (!url.empty() && url.back() == '/') url.pop_back();
        return url;
    }

    return default_url;
}

std::string AIHandler::GetModelId(AIProvider provider, const std::string& customModelId) {
    if (provider == AIProvider::CUSTOM) {
        return customModelId.empty() ? current_config_.model_id : customModelId;
    }
    return current_config_.model_id;
}

} // namespace AI
