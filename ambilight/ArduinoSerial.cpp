#include "ArduinoSerial.h"

ArduinoSerial::ArduinoSerial(const char * portName) : serial(portName)
{
	// This cleans all pixels from the Arduino
	this->send(std::vector<Pixel>(97));
}

const void ArduinoSerial::send(const std::vector<Pixel> & data)
{
	std::vector<unsigned char> outputBuffer;

	// Magic word needed for Arduino communication
	unsigned nbLed = data.size();
	outputBuffer.push_back('A');
	outputBuffer.push_back('d');
	outputBuffer.push_back('a');
	outputBuffer.push_back((nbLed - 1) >> 8);
	outputBuffer.push_back((nbLed - 1) & 0xff);
	outputBuffer.push_back(outputBuffer[3] ^ outputBuffer[4] ^ 0x55);

	// Adding all RGB value to the output buffer
	for (const Pixel& i : data)
	{
		outputBuffer.push_back(i.red);
		outputBuffer.push_back(i.green);
		outputBuffer.push_back(i.blue);
	}

	// Verifying Arduino communication status
	if (!serial.isConnected())
	{
		throw std::runtime_error("Unable to communicate with the Arduino");
	}

	serial.writeData((char *)outputBuffer.data(), outputBuffer.size());
}