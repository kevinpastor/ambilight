#pragma once

#include <highlevelmonitorconfigurationapi.h>
#include <vector>

#include "ArduinoSerial.h"
#include "Coordinates.h"
#include "options.h"
#include "PixelParser.h"
#include "ScreenCapture.h"

class Ambilight
{
public:
	Ambilight();
	~Ambilight();

	void resume();
	void pause();
	void stop();

	void exec();
	void fadeOut();

private:
	bool isMonitorDimmed() const;
	unsigned getMonitorBrightness() const;

	unsigned i;

	bool isPaused;
	bool isStopped;

	Options options;
	ScreenCapture screenCapture;
	PixelParser pixelParser;
	ArduinoSerial arduinoSerial;

	std::vector<Pixel> previousPixels;

};