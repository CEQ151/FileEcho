#include "FileEcho/pdf_extractor.hpp"
#include "deflate_inflate.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cctype>

namespace PDF {

bool IsPdfFile(const std::string& filePath) {
    size_t dot = filePath.rfind('.');
    if (dot == std::string::npos) return false;
    std::string ext = filePath.substr(dot);
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    return ext == ".pdf";
}

// Extract text from parenthesized PDF string literals
static std::string ExtractLiteralStrings(const std::string& data) {
    std::string result;
    int depth = 0;
    bool escape = false;

    for (size_t i = 0; i < data.size(); ++i) {
        char c = data[i];
        if (escape) {
            switch (c) {
                case 'n': result += '\n'; break;
                case 'r': result += '\r'; break;
                case 't': result += '\t'; break;
                default: result += c; break;
            }
            escape = false;
            continue;
        }
        if (c == '\\' && depth > 0) { escape = true; continue; }
        if (c == '(') {
            if (depth > 0) result += c;
            depth++;
        } else if (c == ')') {
            depth--;
            if (depth > 0) result += c;
            else if (depth == 0) result += ' ';
        } else if (depth > 0) {
            // Skip non-printable control chars except whitespace
            if (c >= 32 || c == '\n' || c == '\r' || c == '\t')
                result += c;
        }
    }
    return result;
}

// Extract text from BT/ET blocks in decoded content stream
static std::string ExtractTextFromContent(const std::string& content) {
    std::string result;
    size_t pos = 0;

    while (pos < content.size()) {
        // Find BT (Begin Text object)
        size_t bt = content.find("BT", pos);
        if (bt == std::string::npos) break;
        // Verify BT is a standalone operator (preceded by whitespace/newline)
        if (bt > 0 && !std::isspace(static_cast<unsigned char>(content[bt - 1]))) {
            pos = bt + 2;
            continue;
        }

        size_t et = content.find("ET", bt + 2);
        if (et == std::string::npos) break;

        std::string block = content.substr(bt, et - bt + 2);
        // Extract all parenthesized strings within this text block
        std::string text = ExtractLiteralStrings(block);
        if (!text.empty()) {
            result += text;
            result += '\n';
        }
        pos = et + 2;
    }
    return result;
}

static std::string InflateData(const std::string& compressed) {
    if (compressed.empty()) return "";
    return DeflateInflate::InflateZlib(compressed);
}

std::string ExtractText(const std::string& filePath, size_t maxChars) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) return "[Error: Cannot open PDF file]";

    std::string data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    if (data.size() < 5 || data.substr(0, 5) != "%PDF-") {
        return "[Error: Not a valid PDF file]";
    }

    std::string allText;
    size_t pos = 0;

    while (pos < data.size()) {
        // Find stream keyword
        size_t streamKw = data.find("stream", pos);
        if (streamKw == std::string::npos) break;

        // Move past "stream\r\n" or "stream\n"
        size_t streamStart = streamKw + 6;
        if (streamStart < data.size() && data[streamStart] == '\r') streamStart++;
        if (streamStart < data.size() && data[streamStart] == '\n') streamStart++;

        size_t streamEnd = data.find("endstream", streamStart);
        if (streamEnd == std::string::npos) break;

        std::string streamData = data.substr(streamStart, streamEnd - streamStart);

        // Check if FlateDecode by looking back for the dictionary
        bool isFlate = false;
        size_t dictSearch = (streamKw > 500) ? streamKw - 500 : 0;
        std::string before = data.substr(dictSearch, streamKw - dictSearch);
        if (before.find("/FlateDecode") != std::string::npos ||
            before.find("/Fl") != std::string::npos) {
            isFlate = true;
        }

        std::string content;
        if (isFlate) {
            content = InflateData(streamData);
        } else {
            content = streamData;
        }

        if (!content.empty()) {
            std::string text = ExtractTextFromContent(content);
            if (!text.empty()) {
                allText += text;
            }
        }

        pos = streamEnd + 9;

        if (maxChars > 0 && allText.size() >= maxChars) {
            allText = allText.substr(0, maxChars);
            allText += "\n...[PDF text truncated]";
            break;
        }
    }

    if (allText.empty()) {
        return "[PDF text extraction failed: The PDF may be scanned, encrypted, or use unsupported encoding (CIDFont/ToUnicode)]";
    }

    // Clean up excessive whitespace
    std::string cleaned;
    cleaned.reserve(allText.size());
    bool lastSpace = false;
    for (char c : allText) {
        if (c == ' ' || c == '\t') {
            if (!lastSpace) { cleaned += ' '; lastSpace = true; }
        } else {
            cleaned += c;
            lastSpace = (c == '\n');
        }
    }

    return cleaned;
}

} // namespace PDF
