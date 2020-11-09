#pragma once

#include <chrono>
#include <ctime> 
#include <fstream>
#include <stdexcept>
#include <string>

class Logger
{
public:
	static void log(const std::exception & exception);
	static std::string getTimestamp();

};
