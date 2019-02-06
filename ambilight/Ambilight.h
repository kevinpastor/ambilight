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
	Ambilight(Options * options, ScreenCapture * screenCapture, PixelParser * pixelParser, ArduinoSerial * arduinoSerial);
	~Ambilight();

	void resume();
	void pause();
	void stop();

	void exec();
	void fadeOut();

private:
	bool isPaused;
	bool isStopped;

	Options * const options;
	ScreenCapture * const screenCapture;
	PixelParser * const pixelParser;
	ArduinoSerial * const arduinoSerial;

	std::vector<Pixel> previousPixels;

};