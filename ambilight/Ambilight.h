#pragma once

#include <thread>
#include <vector>

#include "ArduinoSerial.h"
#include "Coordinates.h"
#include "PixelParser.h"
#include "ScreenCapture.h"

class Ambilight
{
public:
	Ambilight(const std::string & communicationPort, const std::vector<Coordinates> & coordinates);
	~Ambilight();

	void start();
	void pause();

private:
	bool isPaused;
	bool isStopped;

	std::thread thread;

	void exec(const std::string & communicationPort, const std::vector<Coordinates> & coordinates) const;
	void stop();

};