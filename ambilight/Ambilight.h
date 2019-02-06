#pragma once

#include <thread>
#include <vector>

#include "ArduinoSerial.h"
#include "Coordinates.h"
#include "options.h"
#include "PixelParser.h"
#include "ScreenCapture.h"

class Ambilight
{
public:
	Ambilight(const Options & options);
	~Ambilight();

	void start();
	void pause();
	void stop();
	bool isStoppeds() const;
	void exec(const Options & options) const;

private:
	bool isPaused;
	bool isStopped;

	std::thread thread;


};