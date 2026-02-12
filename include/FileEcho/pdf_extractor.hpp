#pragma once

#include <string>

namespace PDF {

// Extract text from a PDF file
// Returns extracted text, or error/status message if extraction fails
// maxChars: 0 = no limit
std::string ExtractText(const std::string& filePath, size_t maxChars = 0);

// Check file extension
bool IsPdfFile(const std::string& filePath);

} // namespace PDF
