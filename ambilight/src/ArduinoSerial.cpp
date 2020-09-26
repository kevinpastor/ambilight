#include "ArduinoSerial.h"

ArduinoSerial::ArduinoSerial(const std::string & portName, const unsigned & nbLed)
	: serial("\\\\.\\" + portName),
	nbLed(nbLed)
{
	this->clearPixels();
}

ArduinoSerial::~ArduinoSerial()
{
	this->clearPixels();
}

void ArduinoSerial::send(const std::vector<Pixel> & data) const
{
	std::vector<unsigned char> outputBuffer;

	// Magic word needed for Arduino communication
	unsigned nbLed = (unsigned)data.size();

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

	serial.send((char *)outputBuffer.data(), (unsigned)outputBuffer.size());
}

void ArduinoSerial::clearPixels() const
{
	this->send(std::vector<Pixel>(this->nbLed));
}