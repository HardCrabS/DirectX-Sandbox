#include "Logger.h"

void logInfo(const std::string& message)
{
	std::cout << "[Info] " << message << std::endl;
}

void logError(const std::string& message)
{
	std::cerr << "[Error] " << message << std::endl;
}

std::string prettyXMVector(DirectX::XMVECTOR vector)
{
	return (std::to_string(DirectX::XMVectorGetX(vector)) +
		", " + std::to_string(DirectX::XMVectorGetY(vector)) +
		", " + std::to_string(DirectX::XMVectorGetZ(vector)));
}