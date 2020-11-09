#pragma once

#include <chrono>
#include <future>
#include <mutex>
#include <vector>

#include "ArduinoSerial.h"
#include "ColorGrader.h"
#include "Coordinates.h"
#include "Options.h"
#include "PixelParser.h"
#include "ScreenCapture.h"
#include "SessionUtility.h"

class Ambilight
{
public:
	Ambilight();
	~Ambilight();

	void start();
	void resume();
	void pause();
	void stop();

private:
	void capture();
	void send() const;

	void fadeOut();

	const Options options;
	const ArduinoSerial arduinoSerial;
	const ColorGrader colorGrader;
	const PixelParser pixelParser;
	const ScreenCapture screenCapture;

	mutable std::mutex mutex;

	bool isStopped;
	bool isPaused;

	std::vector<Pixel> pixels;

	static const std::chrono::milliseconds PAUSED_REFRESH_RATE;

};
