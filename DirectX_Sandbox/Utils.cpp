#include "Utils.h"

std::string Utils::ConvertLPCWSTRToString(LPCWSTR lpcwszStr)
{
    // Determine the length of the converted string 
    int strLength
        = WideCharToMultiByte(CP_UTF8, 0, lpcwszStr, -1,
            nullptr, 0, nullptr, nullptr);

    // Create a std::string with the determined length 
    std::string str(strLength, 0);

    // Perform the conversion from LPCWSTR to std::string 
    WideCharToMultiByte(CP_UTF8, 0, lpcwszStr, -1, &str[0],
        strLength, nullptr, nullptr);

    return str;
}

std::wstring Utils::ConvertToWideString(const char* str) {
    if (str == nullptr) {
        return std::wstring();
    }

    // Get the length of the required wide string buffer
    int len = MultiByteToWideChar(CP_UTF8, 0, str, -1, nullptr, 0);
    if (len == 0) {
        // Handle error (len is 0 if the function fails)
        return std::wstring();
    }

    // Allocate buffer for wide string
    std::wstring wstr(len, 0);

    // Perform the conversion
    MultiByteToWideChar(CP_UTF8, 0, str, -1, &wstr[0], len);

    return wstr;
}

std::wstring Utils::StringToWString(const std::string& str)
{
    std::wstring wstr(str.begin(), str.end());
    return wstr;
}
