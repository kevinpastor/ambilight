#pragma once

#include <chrono>
#include <highlevelmonitorconfigurationapi.h>
#include <thread>
#include <vector>
#include <windows.h>
#include <Wtsapi32.h>

class MonitorUtility
{
public:
	static unsigned getBrightness();

private:
	MonitorUtility();

	static MonitorUtility * getInstance();
	static MonitorUtility * instance;

	void updateBrightness();
	unsigned brightness;
	std::thread thread;
	std::chrono::steady_clock::time_point lastMeasurement;

	static const unsigned DIMMED_BRIGHTNESS_LIMIT = 5;
	static const std::chrono::milliseconds REFRESH_RATE;

};
