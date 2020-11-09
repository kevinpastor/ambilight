#include "ArduinoSerial.h"

ArduinoSerial::ArduinoSerial(const std::string & portName)
	: serial("\\\\.\\" + portName)
{
}

void ArduinoSerial::send(const std::vector<Pixel> & pixels) const
{
	std::vector<unsigned char> buffer(6 + pixels.size() * 3);

	// Magic word needed for Arduino communication
	const unsigned long long nbLed = pixels.size();

	buffer[0] = 'A';
	buffer[1] = 'd';
	buffer[2] = 'a';
	buffer[3] = static_cast<unsigned char>((nbLed - 1) >> 8);
	buffer[4] = (nbLed - 1) & 0xff;
	buffer[5] = buffer[3] ^ buffer[4] ^ 0x55;

	// Adding all RGB value to the buffer
	for (unsigned long long i = 0; i < pixels.size(); ++i)
	{
		const Pixel pixel = pixels[i];
		buffer[6 + i * 3] = pixel.red;
		buffer[6 + i * 3 + 1] = pixel.green;
		buffer[6 + i * 3 + 2] = pixel.blue;
	}

	this->serial.send(reinterpret_cast<char *>(buffer.data()), static_cast<unsigned>(buffer.size()));
}
