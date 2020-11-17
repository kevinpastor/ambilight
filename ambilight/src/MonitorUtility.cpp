#include "MonitorUtility.h"

MonitorUtility * MonitorUtility::instance = nullptr;
const std::chrono::milliseconds MonitorUtility::REFRESH_RATE = std::chrono::milliseconds(1000);

MonitorUtility::MonitorUtility()
	: physicalMonitors(MonitorUtility::getPhysicalMonitors()),
	brightness(MonitorUtility::getCurrentBrightness(this->physicalMonitors))
{
}

MonitorUtility * MonitorUtility::getInstance()
{
	if (MonitorUtility::instance == nullptr)
	{
		MonitorUtility::instance = new MonitorUtility();
	}

	return MonitorUtility::instance;
}

unsigned MonitorUtility::getBrightness()
{
	MonitorUtility * instance = MonitorUtility::getInstance();

	const std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	if ((now - instance->lastMeasurement) > MonitorUtility::REFRESH_RATE)
	{
		if (instance->future.valid())
		{
			instance->brightness = instance->future.get();
		}

		instance->future = std::async(std::launch::async, MonitorUtility::getCurrentBrightness, instance->physicalMonitors);

		instance->lastMeasurement = now;
	}

	return instance->brightness;
}

std::vector<PHYSICAL_MONITOR> MonitorUtility::getPhysicalMonitors()
{
	HWND hWnd = GetDesktopWindow();
	HMONITOR hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTOPRIMARY);

	DWORD numberOfPhysicalMonitors;
	if (!GetNumberOfPhysicalMonitorsFromHMONITOR(hMonitor, &numberOfPhysicalMonitors))
	{
		return std::vector<PHYSICAL_MONITOR>();
	}

	std::vector<PHYSICAL_MONITOR> physicalMonitors(numberOfPhysicalMonitors);
	if (!GetPhysicalMonitorsFromHMONITOR(hMonitor, numberOfPhysicalMonitors, physicalMonitors.data()))
	{
		return std::vector<PHYSICAL_MONITOR>();
	}

	return physicalMonitors;
}

unsigned MonitorUtility::getCurrentBrightness(const std::vector<PHYSICAL_MONITOR> & physicalMonitors)
{
	if (physicalMonitors.size() == 0)
	{
		return 100;
	}

	const HANDLE physicalMonitor = physicalMonitors[0].hPhysicalMonitor;

	DWORD minimumBrightness = 0;
	DWORD currentBrightness = 0;
	DWORD maximumBrightness = 0;
	if (!GetMonitorBrightness(physicalMonitor, &minimumBrightness, &currentBrightness, &maximumBrightness))
	{
		return 100;
	}

	return currentBrightness;
}
