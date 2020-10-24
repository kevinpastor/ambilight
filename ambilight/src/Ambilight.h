#pragma once

#include <mutex>
#include <vector>

#include "ArduinoSerial.h"
#include "Coordinates.h"
#include "MonitorUtility.h"
#include "ColorGrader.h"
#include "options.h"
#include "PixelParser.h"
#include "ScreenCapture.h"
#include "SessionUtility.h"

class Ambilight
{
public:
	Ambilight();
	~Ambilight();

	void start();

private:
	void resume();
	void pause();

	std::thread capture();
	std::thread send() const;

	void fadeOut();

	const Options options;
	const ScreenCapture screenCapture;
	const PixelParser pixelParser;
	const ArduinoSerial arduinoSerial;
	const ColorGrader colorGrader;

	mutable std::mutex mutex;

	bool isPaused;

	std::vector<Pixel> pixels;

	static const std::chrono::milliseconds PAUSED_REFRESH_RATE;

};
