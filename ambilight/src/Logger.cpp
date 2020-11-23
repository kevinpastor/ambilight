#include "Logger.h"

void Logger::log(const std::exception & exception)
{
	std::ofstream file;
	file.open("log.txt", std::ofstream::out | std::ofstream::app);
	file << Logger::getTimestamp() << " " << exception.what() << std::endl;
	file.close();
}

std::string Logger::getTimestamp()
{
	std::tm bt({});
	std::time_t now = std::time(nullptr);
	localtime_s(&bt, &now);

	std::stringstream buffer;
	buffer << std::put_time(&bt, "%d/%m/%Y %H:%M:%S");

	return buffer.str();
}
