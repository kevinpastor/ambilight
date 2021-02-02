#pragma once

#include <chrono>
#include <future>
#include <windows.h>
#include <Wtsapi32.h>

class SessionUtility
{
public:
	static bool hasSignedIn();

private:
	SessionUtility();

	static SessionUtility * getInstance();
	static SessionUtility * instance;

	static bool isCurrentlySignedIn();

	bool isSignedIn;
	std::future<bool> future;
	std::chrono::steady_clock::time_point lastMeasurement;

	static const std::chrono::milliseconds REFRESH_RATE;

};

