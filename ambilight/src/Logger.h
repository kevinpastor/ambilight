#pragma once

#include <ctime>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>

class Logger
{
public:
	static void log(const std::exception & exception);
	static std::string getTimestamp();

};
