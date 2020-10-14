#pragma once

#include <chrono>
#include <thread>
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

	void updateState();
	bool locked;
	std::thread thread;
	std::chrono::steady_clock::time_point lastMeasurement;

	static const std::chrono::milliseconds REFRESH_RATE;

};
