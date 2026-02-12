#include "FileEcho/doc_extractor.hpp"
#include "deflate_inflate.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <cstdint>

namespace DocExtractor {

// =============================================
// Utility: read little-endian integers
// =============================================
static uint16_t readU16(const char* p) {
    return (uint8_t)p[0] | ((uint16_t)(uint8_t)p[1] << 8);
}

static uint32_t readU32(const char* p) {
    return (uint8_t)p[0] | ((uint32_t)(uint8_t)p[1] << 8) |
           ((uint32_t)(uint8_t)p[2] << 16) | ((uint32_t)(uint8_t)p[3] << 24);
}

// =============================================
// ZIP parser: extract file from ZIP archive
// =============================================
struct ZipEntry {
    std::string filename;
    uint16_t compression;
    uint32_t compressed_size;
    uint32_t uncompressed_size;
    uint32_t local_header_offset;
};

// Find End of Central Directory record
static bool findEOCD(const std::string& data, size_t& eocdOffset) {
    // EOCD signature: 0x06054b50
    // Search backwards from end (EOCD is last record, may have comment)
    if (data.size() < 22) return false;
    size_t searchStart = data.size() < 65557 ? 0 : data.size() - 65557;
    for (size_t i = data.size() - 22; i >= searchStart; i--) {
        if (readU32(data.data() + i) == 0x06054b50) {
            eocdOffset = i;
            return true;
        }
        if (i == 0) break;
    }
    return false;
}

// Parse central directory entries
static std::vector<ZipEntry> parseCentralDirectory(const std::string& data, size_t eocdOffset) {
    std::vector<ZipEntry> entries;
    
    uint16_t totalEntries = readU16(data.data() + eocdOffset + 10);
    uint32_t cdOffset = readU32(data.data() + eocdOffset + 16);
    
    size_t pos = cdOffset;
    for (int i = 0; i < totalEntries && pos + 46 <= data.size(); i++) {
        if (readU32(data.data() + pos) != 0x02014b50) break; // Not a CD entry
        
        ZipEntry e;
        e.compression = readU16(data.data() + pos + 10);
        e.compressed_size = readU32(data.data() + pos + 20);
        e.uncompressed_size = readU32(data.data() + pos + 24);
        uint16_t fnLen = readU16(data.data() + pos + 28);
        uint16_t exLen = readU16(data.data() + pos + 30);
        uint16_t cmLen = readU16(data.data() + pos + 32);
        e.local_header_offset = readU32(data.data() + pos + 42);
        
        if (pos + 46 + fnLen > data.size()) break;
        e.filename = data.substr(pos + 46, fnLen);
        
        entries.push_back(e);
        pos += 46 + fnLen + exLen + cmLen;
    }
    
    return entries;
}

// Extract file data from ZIP using local header
static std::string extractFileData(const std::string& data, const ZipEntry& entry) {
    size_t lhPos = entry.local_header_offset;
    if (lhPos + 30 > data.size()) return "";
    if (readU32(data.data() + lhPos) != 0x04034b50) return ""; // Not a local header
    
    uint16_t fnLen = readU16(data.data() + lhPos + 26);
    uint16_t exLen = readU16(data.data() + lhPos + 28);
    size_t dataStart = lhPos + 30 + fnLen + exLen;
    
    if (dataStart + entry.compressed_size > data.size()) return "";
    
    std::string compressed = data.substr(dataStart, entry.compressed_size);
    
    if (entry.compression == 0) {
        // Stored (uncompressed)
        return compressed;
    } else if (entry.compression == 8) {
        // Deflated — use raw DEFLATE inflate (no zlib header in ZIP)
        return DeflateInflate::Inflate(compressed);
    }
    
    return ""; // Unsupported compression
}

// Find and extract a file from ZIP by name
static std::string getFileFromZip(const std::string& zipData, const std::string& targetName) {
    size_t eocdOffset;
    if (!findEOCD(zipData, eocdOffset)) return "";
    
    auto entries = parseCentralDirectory(zipData, eocdOffset);
    
    for (const auto& entry : entries) {
        if (entry.filename == targetName) {
            return extractFileData(zipData, entry);
        }
    }
    return "";
}

// Find and extract files matching a prefix from ZIP
static std::vector<std::pair<std::string, std::string>> getFilesFromZip(
    const std::string& zipData, const std::string& prefix,
    const std::string& suffix = "", int maxFiles = 100) {
    
    std::vector<std::pair<std::string, std::string>> results;
    
    size_t eocdOffset;
    if (!findEOCD(zipData, eocdOffset)) return results;
    
    auto entries = parseCentralDirectory(zipData, eocdOffset);
    
    for (const auto& entry : entries) {
        if (entry.filename.find(prefix) == 0 &&
            (suffix.empty() || (entry.filename.size() >= suffix.size() &&
             entry.filename.compare(entry.filename.size() - suffix.size(), suffix.size(), suffix) == 0))) {
            std::string content = extractFileData(zipData, entry);
            if (!content.empty()) {
                results.push_back({entry.filename, content});
                if ((int)results.size() >= maxFiles) break;
            }
        }
    }
    return results;
}

// =============================================
// XML text extraction
// =============================================

// Decode common XML entities
static std::string decodeXmlEntities(const std::string& text) {
    std::string result;
    result.reserve(text.size());
    for (size_t i = 0; i < text.size(); i++) {
        if (text[i] == '&') {
            if (text.compare(i, 5, "&amp;") == 0) { result += '&'; i += 4; }
            else if (text.compare(i, 4, "&lt;") == 0) { result += '<'; i += 3; }
            else if (text.compare(i, 4, "&gt;") == 0) { result += '>'; i += 3; }
            else if (text.compare(i, 6, "&quot;") == 0) { result += '"'; i += 5; }
            else if (text.compare(i, 6, "&apos;") == 0) { result += '\''; i += 5; }
            else {
                // Skip unknown entity
                size_t end = text.find(';', i);
                if (end != std::string::npos && end - i < 10) { i = end; }
                else result += text[i];
            }
        } else {
            result += text[i];
        }
    }
    return result;
}

// Extract text between specific XML tags
// e.g., extractTagContent(xml, "w:t") extracts content of all <w:t>...</w:t>
static std::string extractTagContent(const std::string& xml, const std::string& tagName,
                                      const std::string& paraSep = "") {
    std::string result;
    std::string openTag = "<" + tagName;
    std::string closeTag = "</" + tagName + ">";
    
    size_t pos = 0;
    while (pos < xml.size()) {
        size_t tagStart = xml.find(openTag, pos);
        if (tagStart == std::string::npos) break;
        
        // Make sure it's a complete tag (followed by > or space)
        size_t afterTag = tagStart + openTag.size();
        if (afterTag >= xml.size()) break;
        char nextChar = xml[afterTag];
        if (nextChar != '>' && nextChar != ' ' && nextChar != '/') {
            pos = afterTag;
            continue;
        }
        
        // Find the end of opening tag
        size_t contentStart = xml.find('>', tagStart);
        if (contentStart == std::string::npos) break;
        contentStart++;
        
        // Self-closing tag?
        if (xml[contentStart - 2] == '/') { pos = contentStart; continue; }
        
        // Find closing tag
        size_t contentEnd = xml.find(closeTag, contentStart);
        if (contentEnd == std::string::npos) break;
        
        std::string content = xml.substr(contentStart, contentEnd - contentStart);
        // Recursively strip any nested tags
        std::string stripped;
        bool inTag2 = false;
        for (char c : content) {
            if (c == '<') inTag2 = true;
            else if (c == '>') inTag2 = false;
            else if (!inTag2) stripped += c;
        }
        
        if (!stripped.empty()) {
            result += decodeXmlEntities(stripped);
        }
        
        pos = contentEnd + closeTag.size();
    }
    return result;
}

// =============================================
// DOCX text extraction
// =============================================
static std::string extractDocxText(const std::string& zipData) {
    std::string xml = getFileFromZip(zipData, "word/document.xml");
    if (xml.empty()) return "";
    
    // Extract text from <w:p> paragraphs containing <w:t> text runs
    std::string result;
    std::string pOpen = "<w:p";
    std::string pClose = "</w:p>";
    
    size_t pos = 0;
    while (pos < xml.size()) {
        size_t pStart = xml.find(pOpen, pos);
        if (pStart == std::string::npos) break;
        
        size_t pEnd = xml.find(pClose, pStart);
        if (pEnd == std::string::npos) break;
        pEnd += pClose.size();
        
        std::string paragraph = xml.substr(pStart, pEnd - pStart);
        std::string paraText = extractTagContent(paragraph, "w:t");
        
        if (!paraText.empty()) {
            result += paraText + "\n";
        }
        
        pos = pEnd;
    }
    
    return result;
}

// =============================================
// XLSX text extraction (shared strings + sheets)
// =============================================
static std::string extractXlsxText(const std::string& zipData) {
    // First, get shared strings
    std::string ssXml = getFileFromZip(zipData, "xl/sharedStrings.xml");
    std::string ssText;
    if (!ssXml.empty()) {
        ssText = extractTagContent(ssXml, "t");
    }
    
    // Also try to get inline strings from sheets
    auto sheets = getFilesFromZip(zipData, "xl/worksheets/sheet", ".xml", 5);
    std::string sheetText;
    for (const auto& [name, content] : sheets) {
        std::string t = extractTagContent(content, "t");
        if (!t.empty()) {
            sheetText += t + "\n";
        }
    }
    
    std::string result;
    if (!ssText.empty()) result += ssText + "\n";
    if (!sheetText.empty()) result += sheetText;
    
    return result;
}

// =============================================
// PPTX text extraction
// =============================================
static std::string extractPptxText(const std::string& zipData) {
    auto slides = getFilesFromZip(zipData, "ppt/slides/slide", ".xml", 50);
    
    std::string result;
    for (const auto& [name, content] : slides) {
        std::string slideText = extractTagContent(content, "a:t");
        if (!slideText.empty()) {
            result += slideText + "\n";
        }
    }
    
    return result;
}

// =============================================
// Public API
// =============================================

static std::string getExtension(const std::string& filePath) {
    size_t dot = filePath.rfind('.');
    if (dot == std::string::npos) return "";
    std::string ext = filePath.substr(dot);
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    return ext;
}

bool IsDocxFile(const std::string& filePath) { return getExtension(filePath) == ".docx"; }
bool IsXlsxFile(const std::string& filePath) { return getExtension(filePath) == ".xlsx"; }
bool IsPptxFile(const std::string& filePath) { return getExtension(filePath) == ".pptx"; }

bool IsOfficeFile(const std::string& filePath) {
    std::string ext = getExtension(filePath);
    return ext == ".docx" || ext == ".xlsx" || ext == ".pptx";
}

std::string ExtractText(const std::string& filePath, size_t maxChars) {
    // Read entire file
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) return "[Error: Cannot open file]";
    
    std::string data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    
    if (data.size() < 4) return "[Error: File too small]";
    
    // Check for ZIP signature (PK\x03\x04)
    if (data[0] != 'P' || data[1] != 'K' || data[2] != 3 || data[3] != 4) {
        return "[Error: Not a valid Office document (not a ZIP archive)]";
    }
    
    std::string text;
    std::string ext = getExtension(filePath);
    
    if (ext == ".docx") {
        text = extractDocxText(data);
    } else if (ext == ".xlsx") {
        text = extractXlsxText(data);
    } else if (ext == ".pptx") {
        text = extractPptxText(data);
    } else {
        return "[Error: Unsupported file format]";
    }
    
    if (text.empty()) {
        return "[Office document text extraction failed: The document may be encrypted or use unsupported features]";
    }
    
    // Truncate if needed
    if (maxChars > 0 && text.size() > maxChars) {
        text = text.substr(0, maxChars) + "\n...[Document text truncated]";
    }
    
    // Clean up excessive whitespace
    std::string cleaned;
    cleaned.reserve(text.size());
    bool lastSpace = false;
    for (char c : text) {
        if (c == ' ' || c == '\t') {
            if (!lastSpace) { cleaned += ' '; lastSpace = true; }
        } else {
            cleaned += c;
            lastSpace = (c == '\n');
        }
    }
    
    return cleaned;
}

} // namespace DocExtractor
