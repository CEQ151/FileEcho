#pragma once

#include <string>

namespace DocExtractor {

// Check file extension for supported office documents
bool IsOfficeFile(const std::string& filePath);

// Check specific types
bool IsDocxFile(const std::string& filePath);
bool IsXlsxFile(const std::string& filePath);
bool IsPptxFile(const std::string& filePath);

// Extract text from Office documents (DOCX, XLSX, PPTX)
// Returns extracted text, or error/status message
// maxChars: 0 = no limit
std::string ExtractText(const std::string& filePath, size_t maxChars = 0);

} // namespace DocExtractor
