#pragma once

#include <future>
#include <mutex>
#include <vector>

#include "ArduinoSerial.h"
#include "Coordinates.h"
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

	mutable std::mutex mutex;

	unsigned long long time = 0;
	bool isPaused;

	std::vector<Pixel> previousPixels;

	static const std::chrono::nanoseconds MAXIMUM_RERFRESH_RATE;
	static const std::chrono::milliseconds PAUSED_REFRESH_RATE;

};
