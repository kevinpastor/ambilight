#ifndef ARDUINO_SERIAL_H
#define ARDUINO_SERIAL_H

#include <vector>

#include "Serial.h"

class ArduinoSerial
{
public:
	ArduinoSerial();
	~ArduinoSerial();

	void send(const std::vector<unsigned char> data);

private:
	Serial serial;

};

#endif