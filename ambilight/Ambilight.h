#pragma once

#include <iostream>

#include <vector>
#include <string>
#include <thread>

#include "ArduinoSerial.h"
#include "PixelParser.h"
#include "Coordinates.h"
#include "ScreenCapture.h"

class Ambilight
{
public:
	Ambilight(const std::string & communicationPort, const unsigned & nbLed, const std::vector<Coordinates> & coordinates);
	~Ambilight();

	void start();
	void pause();

private:
	bool isPaused;
	bool isStopped;

	std::thread thread;

	void exec(const std::string & communicationPort, const unsigned & nbLed, const std::vector<Coordinates> & coordinates) const;
	void stop();

};