#pragma once
#include <Windows.h> 
#include <iostream> 
#include <string> 

class Utils
{
public:
    static std::string ConvertLPCWSTRToString(LPCWSTR lpcwszStr);
    static std::wstring ConvertToWideString(const char* str);
    static std::wstring StringToWString(const std::string& str);
};