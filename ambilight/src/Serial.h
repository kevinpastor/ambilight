#pragma once

#include <stdexcept>
#include <windows.h>

class Serial
{
public:
	Serial(const std::string & portName, const unsigned long & baudRate);
	~Serial();

	void send(const char * buffer, const unsigned & nbChar) const;

private:
	HANDLE handle;

	static const unsigned WAIT_TIME = 2000;

};
