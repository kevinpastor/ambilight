#pragma once

#include <string>
#include <vector>

#include "Pixel.h"
#include "Serial.h"

class ArduinoSerial
{
public:
	ArduinoSerial(const std::string & communicationPort, const unsigned long & baudRate);

	void send(const std::vector<Pixel> & pixels) const;

private:
	const Serial serial;

};
