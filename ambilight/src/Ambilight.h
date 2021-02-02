#pragma once

#include <chrono>
#include <cmath>
#include <future>
#include <mutex>
#include <vector>

#include "ArduinoSerial.h"
#include "ColorGrader.h"
#include "Coordinates.h"
#include "Options.h"
#include "PixelParser.h"
#include "ScreenCapture.h"

class Ambilight
{
public:
	Ambilight();
	~Ambilight();

	void start();
	void resume();
	void pause();
	void stop();

	void toggleLowPowerMode();

private:
	std::vector<Pixel> capture(std::vector<Pixel> previousPixels) const;
	void send(std::vector<Pixel> pixels) const;

	void fadeOut(const std::vector<Pixel> & pixels) const;

	const Options options;
	const ArduinoSerial arduinoSerial;
	const ColorGrader colorGrader;
	const PixelParser pixelParser;
	const ScreenCapture screenCapture;

	mutable std::mutex mutex;

	bool isStopped;
	bool isPaused;
	bool isOnLowPowerMode;

	static const std::chrono::milliseconds PAUSED_REFRESH_RATE;

};
