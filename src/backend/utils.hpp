#pragma once
#include <string>
#include <vector>
#ifdef _WIN32
#include <windows.h>
#endif

namespace Utils {

// 封装跨平台转换逻辑
inline std::wstring ToWString(const std::string& str) {
#ifdef _WIN32
    if (str.empty()) return std::wstring();
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    if (size_needed <= 0) return std::wstring();
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
#else
    return std::wstring(str.begin(), str.end()); // Linux/Mac 简化处理
#endif
}

inline std::string ToUtf8(const std::wstring& wstr) {
#ifdef _WIN32
    if (wstr.empty()) return std::string();
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
    if (size_needed <= 0) return std::string();
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
    return strTo;
#else
    return std::string(wstr.begin(), wstr.end());
#endif
}

} // namespace Utils