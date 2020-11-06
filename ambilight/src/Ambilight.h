#pragma once

#include <mutex>
#include <thread>
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
	void userPause();
	void userResume();
	void stop();

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

	bool isUserPaused;
	bool isStopped;
	bool isPaused;

	std::vector<Pixel> pixels;

	static const std::chrono::milliseconds PAUSED_REFRESH_RATE;

};
