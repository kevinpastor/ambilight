#ifndef ARDUINO_SERIAL_H
#define ARDUINO_SERIAL_H

#include <iostream>
#include <vector>

#include "Pixel.h"
#include "Serial.h"

class ArduinoSerial
{
public:
	ArduinoSerial(const char * communicationPort);
	~ArduinoSerial();

	void send(const std::vector<Pixel> data);


private:
	Serial serial;

};

#endif