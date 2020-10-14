#include "Serial.h"

Serial::Serial(const std::string & portName)
{
	this->handle = CreateFile(portName.data(), GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (this->handle == INVALID_HANDLE_VALUE)
	{
		if (GetLastError() == ERROR_FILE_NOT_FOUND)
		{
			throw std::runtime_error("Serial port not available");
		}

		throw std::runtime_error("Serial handle was not attached");
	}

	DCB dcb = { 0 };

	if (!GetCommState(this->handle, &dcb))
	{
		throw std::runtime_error("Unable to get serial parameters");
	}

	dcb.BaudRate = CBR_115200;
	dcb.ByteSize = 8;
	dcb.StopBits = ONESTOPBIT;
	dcb.Parity = NOPARITY;
	dcb.fDtrControl = DTR_CONTROL_ENABLE;

	if (!SetCommState(handle, &dcb))
	{
		throw std::runtime_error("Unable to set serial parameters");
	}

	PurgeComm(this->handle, PURGE_RXCLEAR | PURGE_TXCLEAR);
	Sleep(Serial::ARDUINO_WAIT_TIME);
}

Serial::~Serial()
{
	CloseHandle(this->handle);
}

void Serial::send(const char * buffer, const unsigned & size) const
{
	if (!WriteFile(this->handle, buffer, size, NULL, NULL))
	{
		DWORD errors;
		COMSTAT comStat;
		ClearCommError(this->handle, &errors, &comStat);
		throw std::runtime_error("Unable to write on serial");
	}
}
