#pragma once

#include <fstream>
#include <stdexcept>

class Logger
{
public:
	static void log(const std::exception & exception);

};
