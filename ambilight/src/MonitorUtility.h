#pragma once

#include <chrono>
#include <future>
#include <highlevelmonitorconfigurationapi.h>
#include <vector>

class MonitorUtility
{
public:
	static unsigned getBrightness();

private:
	MonitorUtility();

	static MonitorUtility * getInstance();
	static MonitorUtility * instance;

	static HANDLE getPhysicalMonitor();
	static unsigned getCurrentBrightness(const HANDLE & physicalMonitor);

	const HANDLE physicalMonitor;
	unsigned brightness;
	std::future<unsigned> future;
	std::chrono::steady_clock::time_point lastMeasurement;

	static const std::chrono::milliseconds REFRESH_RATE;

};
