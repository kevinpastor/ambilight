#include "Logger.h"

void Logger::log(const std::exception & exception)
{
	std::ofstream file;
	file.open("log.txt", std::ofstream::out | std::ofstream::app);
	file << exception.what() << std::endl;
	file.close();
}
