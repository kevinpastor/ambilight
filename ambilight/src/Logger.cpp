#include "Logger.h"

void Logger::log(const std::exception & exception)
{
	std::ofstream file;
	file.open("log.txt", std::ofstream::out | std::ofstream::app);
	file << Logger::getTimestamp() << " " <<exception.what() << std::endl;
	file.close();
}

std::string Logger::getTimestamp()
{
	// TODO
	std::time_t result = std::time(nullptr);
	return std::to_string(result);
}
