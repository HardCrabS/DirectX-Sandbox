#pragma once
#include <Windows.h> 
#include <iostream> 
#include <string> 

class Utils
{
public:
    static std::string ConvertLPCWSTRToString(LPCWSTR lpcwszStr);
};