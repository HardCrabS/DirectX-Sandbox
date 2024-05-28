#include "Logger.h"

void logInfo(const std::string& message)
{
	std::cout << "[Info] " << message << std::endl;
}

void logError(const std::string& message)
{
	std::cerr << "[Error] " << message << std::endl;
}