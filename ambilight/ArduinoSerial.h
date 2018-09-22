#ifndef ARDUINO_SERIAL_H
#define ARDUINO_SERIAL_H

#include <vector>

#include "Pixel.h"
#include "Serial.h"

class ArduinoSerial
{
public:
	ArduinoSerial(const char * communicationPort, const unsigned & nbLed);

	const void send(const std::vector<Pixel> & data);

	const void clearPixels();

private:
	Serial serial;
	unsigned nbLed;

};

#endif