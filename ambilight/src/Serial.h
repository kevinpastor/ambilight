#pragma once

#include <stdexcept>
#include <windows.h>

class Serial
{
public:
	Serial(const std::string & portName);
	~Serial();

	void send(const char * buffer, const unsigned & nbChar) const;

private:
	HANDLE handle;

	static const unsigned ARDUINO_WAIT_TIME = 2000;

};
