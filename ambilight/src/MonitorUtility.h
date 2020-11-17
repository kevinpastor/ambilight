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

	static std::vector<PHYSICAL_MONITOR> getPhysicalMonitors();
	static unsigned getCurrentBrightness(const std::vector<PHYSICAL_MONITOR> & physicalMonitors);

	const std::vector<PHYSICAL_MONITOR> physicalMonitors;
	unsigned brightness;
	std::future<unsigned> future;
	std::chrono::steady_clock::time_point lastMeasurement;

	static const unsigned DIMMED_BRIGHTNESS_LIMIT = 5;
	static const std::chrono::milliseconds REFRESH_RATE;

};
