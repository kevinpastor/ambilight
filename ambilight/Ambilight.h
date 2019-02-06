#pragma once

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

	void resume();
	void pause();
	void stop();

	void exec();

private:
	bool isPaused;
	bool isStopped;

	Options options;
	ScreenCapture screenCapture;
	PixelParser pixelParser;
	ArduinoSerial arduinoSerial;

	std::vector<Pixel> previousPixels;

	void fadeOut();

};