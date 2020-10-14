#include "MonitorUtility.h"

MonitorUtility * MonitorUtility::instance = nullptr;
const std::chrono::milliseconds MonitorUtility::REFRESH_RATE = std::chrono::milliseconds(1000);

MonitorUtility::MonitorUtility()
	: brightness(100)
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
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	if ((now - MonitorUtility::getInstance()->lastMeasurement) > MonitorUtility::REFRESH_RATE)
	{
		if (MonitorUtility::getInstance()->thread.joinable())
		{
			MonitorUtility::getInstance()->thread.join();
		}

		MonitorUtility::getInstance()->thread = std::thread([]()
			{
				MonitorUtility::getInstance()->updateBrightness();
			});

		MonitorUtility::getInstance()->lastMeasurement = now;
	}

	return MonitorUtility::getInstance()->brightness;
}

void MonitorUtility::updateBrightness()
{
	const HWND hWnd = GetDesktopWindow();
	const HMONITOR hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTOPRIMARY);

	DWORD numberOfPhysicalMonitors;
	if (!GetNumberOfPhysicalMonitorsFromHMONITOR(hMonitor, &numberOfPhysicalMonitors))
	{
		this->brightness = 100;
		return;
	}

	std::vector<PHYSICAL_MONITOR> physicalMonitors(numberOfPhysicalMonitors);
	if (!GetPhysicalMonitorsFromHMONITOR(hMonitor, numberOfPhysicalMonitors, physicalMonitors.data()))
	{
		this->brightness = 100;
		return;
	}

	const HANDLE hPhysicalMonitor = physicalMonitors[0].hPhysicalMonitor;
	DWORD minimumBrightness = 0;
	DWORD currentBrightness = 0;
	DWORD maximumBrightness = 0;
	if (!GetMonitorBrightness(hPhysicalMonitor, &minimumBrightness, &currentBrightness, &maximumBrightness))
	{
		this->brightness = 100;
		return;
	}

	this->brightness = currentBrightness;
}
