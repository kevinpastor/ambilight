#ifndef ARDUINO_SERIAL_H
#define ARDUINO_SERIAL_H

#include <vector>
#include <string>

#include "Pixel.h"
#include "Serial.h"

class ArduinoSerial
{
public:
	ArduinoSerial(const std::string communicationPort, const unsigned & nbLed);

	const void send(const std::vector<Pixel> & data);

	const void clearPixels();

private:
	Serial serial;
	unsigned nbLed;

};

#endif