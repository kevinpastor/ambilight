#ifndef AMBILIGHT_H
#define AMBILIGHT_H

#include <vector>
#include <iostream>
#include <string>
#include <thread>

#include "ArduinoSerial.h"
#include "PixelParser.h"
#include "Coordinates.h"

class Ambilight
{
public:
	Ambilight(const std::string & communicationPort, const unsigned & nbLed, const std::vector<Coordinates> coordinates);
	~Ambilight();

	const void start();
	const void pause();
	const void stop();

	const void exec();

private:
	ArduinoSerial arduinoSerial;
	PixelParser pixelParser;
	bool isActive;
	bool isStopped;

	std::thread thread;

};

#endif