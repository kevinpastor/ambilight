#pragma once

#include <vector>
#include <string>

#include "Pixel.h"
#include "Serial.h"

class ArduinoSerial
{
public:
	ArduinoSerial(const std::string & communicationPort, const unsigned & nbLed);
	~ArduinoSerial();

	void send(const std::vector<Pixel> & data) const;

	void clearPixels() const;

private:
	Serial serial;
	unsigned nbLed;

};