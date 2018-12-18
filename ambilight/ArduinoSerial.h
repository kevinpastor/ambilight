#pragma once

#include <vector>
#include <string>

#include "Pixel.h"
#include "Serial.h"

class ArduinoSerial
{
public:
	ArduinoSerial(const std::string communicationPort, const unsigned & nbLed);
	~ArduinoSerial();

	const void send(const std::vector<Pixel> & data);

	const void clearPixels();

private:
	Serial serial;
	unsigned nbLed;

};