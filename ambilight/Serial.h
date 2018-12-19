#pragma once

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdexcept>

#define ARDUINO_WAIT_TIME 2000

class Serial
{
public:
	Serial(const char * portName);
	~Serial();

	void send(const char * buffer, const unsigned & nbChar) const;
	bool isConnected() const;

private:
	HANDLE handle;
	bool connected;
	COMSTAT status;
	DWORD errors;

};