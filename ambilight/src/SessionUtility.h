#pragma once

#include <future>
#include <chrono>
#include <windows.h>
#include <Wtsapi32.h>

class SessionUtility
{
public:
	static bool isLocked();

private:
	SessionUtility();

	static SessionUtility * getInstance();
	static SessionUtility * instance;

	static bool isCurrentlyLocked();

	bool locked;
	std::future<bool> future;
	std::chrono::steady_clock::time_point lastMeasurement;

	static const std::chrono::milliseconds REFRESH_RATE;

};
