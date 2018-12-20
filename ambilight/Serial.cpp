#include "Serial.h"

Serial::Serial(const char * portName)
{
	this->connected = false;

	this->handle = CreateFile(portName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (this->handle == INVALID_HANDLE_VALUE)
	{
		if (GetLastError() == ERROR_FILE_NOT_FOUND)
		{
			throw std::runtime_error("Serial port not available");
		}
		else
		{
			throw std::runtime_error("Serial handle was not attached");
		}
	}
	else
	{
		DCB dcbSerialParams = { 0 };

		if (!GetCommState(this->handle, &dcbSerialParams))
		{
			throw std::runtime_error("Unable to get serial parameters");
		}
		else
		{
			// Define serial connection parameters for the arduino board
			dcbSerialParams.BaudRate = CBR_115200;
			dcbSerialParams.ByteSize = 8;
			dcbSerialParams.StopBits = ONESTOPBIT;
			dcbSerialParams.Parity = NOPARITY;

			// Setting the DTR to Control_Enable ensures that the Arduino is properly
			// reset upon establishing a connection
			dcbSerialParams.fDtrControl = DTR_CONTROL_ENABLE;

			// Set the parameters and check for their proper application
			if (!SetCommState(handle, &dcbSerialParams))
			{
				throw std::runtime_error("Unable to set serial parameters");
			}
			else
			{
				this->connected = true;

				// Flush any remaining characters in the buffers 
				PurgeComm(this->handle, PURGE_RXCLEAR | PURGE_TXCLEAR);

				// We wait 2s as the arduino board will be reseting
				Sleep(ARDUINO_WAIT_TIME);
			}
		}
	}

}

Serial::~Serial()
{
	if (this->isConnected())
	{
		CloseHandle(this->handle);
	}
}

void Serial::send(const char * buffer, const unsigned & size) const
{
	WriteFile(this->handle, buffer, size, NULL, NULL);
	//if (!WriteFile(this->handle, buffer, size, NULL, 0))
	//{
	//	// In case it don't work get comm error
	//	ClearCommError(this->handle, &this->errors, &this->status);
	//}
}

bool Serial::isConnected() const
{
	return this->connected;
}