#include "MonitorUtility.h"

MonitorUtility * MonitorUtility::instance = nullptr;
const std::chrono::milliseconds MonitorUtility::REFRESH_RATE = std::chrono::milliseconds(1000);

MonitorUtility::MonitorUtility()
	: physicalMonitor(MonitorUtility::getPhysicalMonitor()),
	brightness(MonitorUtility::getCurrentBrightness(this->physicalMonitor))
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
	MonitorUtility * singleton = MonitorUtility::getInstance();

	const std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	if ((now - singleton->lastMeasurement) > MonitorUtility::REFRESH_RATE)
	{
		if (singleton->future.valid())
		{
			singleton->brightness = singleton->future.get();
		}

		singleton->future = std::async(std::launch::async, MonitorUtility::getCurrentBrightness, singleton->physicalMonitor);

		singleton->lastMeasurement = now;
	}

	return singleton->brightness;
}

HANDLE MonitorUtility::getPhysicalMonitor()
{
	HWND window = GetDesktopWindow();
	HMONITOR hMonitor = MonitorFromWindow(window, MONITOR_DEFAULTTOPRIMARY);

	DWORD numberOfPhysicalMonitors;
	if (!GetNumberOfPhysicalMonitorsFromHMONITOR(hMonitor, &numberOfPhysicalMonitors))
	{
		return nullptr;
	}

	std::vector<PHYSICAL_MONITOR> physicalMonitors(numberOfPhysicalMonitors);
	if (!GetPhysicalMonitorsFromHMONITOR(hMonitor, numberOfPhysicalMonitors, physicalMonitors.data()))
	{
		return nullptr;
	}

	const PHYSICAL_MONITOR physicalMonitor = physicalMonitors[0];

	return physicalMonitor.hPhysicalMonitor;
}

unsigned MonitorUtility::getCurrentBrightness(const HANDLE & physicalMonitor)
{
	DWORD minimumBrightness = 0;
	DWORD currentBrightness = 0;
	DWORD maximumBrightness = 0;
	if (!GetMonitorBrightness(physicalMonitor, &minimumBrightness, &currentBrightness, &maximumBrightness))
	{
		return 100;
	}

	return currentBrightness;
}
