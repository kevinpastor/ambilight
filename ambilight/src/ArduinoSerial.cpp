#include "ArduinoSerial.h"

ArduinoSerial::ArduinoSerial(const std::string & portName)
	: serial("\\\\.\\" + portName)
{
}

void ArduinoSerial::send(const std::vector<Pixel> & pixels) const
{
	std::vector<unsigned char> buffer;

	// Magic word needed for Arduino communication
	const unsigned nbLed = static_cast<unsigned>(pixels.size());

	buffer.push_back('A');
	buffer.push_back('d');
	buffer.push_back('a');
	buffer.push_back(static_cast<unsigned char>((nbLed - 1) >> 8));
	buffer.push_back((nbLed - 1) & 0xff);
	buffer.push_back(buffer[3] ^ buffer[4] ^ 0x55);

	// Adding all RGB value to the buffer
	for (const Pixel & pixel : pixels)
	{
		buffer.push_back(pixel.red);
		buffer.push_back(pixel.green);
		buffer.push_back(pixel.blue);
	}

	this->serial.send(reinterpret_cast<char *>(buffer.data()), static_cast<unsigned>(buffer.size()));
}
