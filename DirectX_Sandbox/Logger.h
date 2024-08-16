#pragma once
#include <iostream>
#include <string>
#include <DirectXMath.h>

void logInfo(const std::string& message);
void logError(const std::string& message);
std::string prettyXMVector(DirectX::XMVECTOR vector);