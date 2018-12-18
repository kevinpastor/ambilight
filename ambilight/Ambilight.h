#pragma once

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

private:
	ArduinoSerial arduinoSerial;
	PixelParser pixelParser;
	bool isPaused;
	bool isStopped;

	std::thread thread;

	const void exec();
	const void stop();

};