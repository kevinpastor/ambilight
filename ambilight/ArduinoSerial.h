#pragma once

#include <vector>

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
	const Serial serial;
	const unsigned nbLed;

};