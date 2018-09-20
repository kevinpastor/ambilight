#include "ArduinoSerial.h"

ArduinoSerial::ArduinoSerial() : serial("\\\\.\\COM10")
{
}

ArduinoSerial::~ArduinoSerial()
{
}

void ArduinoSerial::send(const std::vector<unsigned char> data)
{
	if (serial.isConnected())
	{
		unsigned nbLed = 1;
		unsigned dataSize = data.size() + 6;
		std::vector<unsigned char> outputBuffer(dataSize);

		// Magic word needed for Arduino communication
		outputBuffer.push_back('A');
		outputBuffer.push_back('d');
		outputBuffer.push_back('a');
		outputBuffer.push_back((nbLed - 1) >> 8);
		outputBuffer.push_back((nbLed - 1) & 0xff);
		outputBuffer.push_back(outputBuffer[3] ^ outputBuffer[4] ^ 0x55);

		outputBuffer.insert(outputBuffer.end(), std::begin(data), std::end(data));

		serial.writeData((char *)outputBuffer.data(), outputBuffer.size());
	}
	else
	{
		throw std::runtime_error("Unable to communicate with the Arduino");
	}
}
