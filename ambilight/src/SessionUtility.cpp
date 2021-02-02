#include "SessionUtility.h"

SessionUtility * SessionUtility::instance = nullptr;
const std::chrono::milliseconds SessionUtility::REFRESH_RATE = std::chrono::milliseconds(1000);

SessionUtility::SessionUtility()
	: isSignedIn(SessionUtility::isCurrentlySignedIn())
{
}

SessionUtility * SessionUtility::getInstance()
{
	if (SessionUtility::instance == nullptr)
	{
		SessionUtility::instance = new SessionUtility();
	}

	return SessionUtility::instance;
}

bool SessionUtility::hasSignedIn()
{
	SessionUtility * singleton = SessionUtility::getInstance();

	if (singleton->isSignedIn)
	{
		return true;
	}

	const std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	if ((now - singleton->lastMeasurement) > SessionUtility::REFRESH_RATE)
	{
		if (singleton->future.valid())
		{
			singleton->isSignedIn = singleton->future.get();
		}

		singleton->future = std::async(std::launch::async, SessionUtility::isCurrentlySignedIn);

		singleton->lastMeasurement = now;
	}

	return singleton->isSignedIn;
}

bool SessionUtility::isCurrentlySignedIn()
{
	const DWORD dwSessionID = WTSGetActiveConsoleSessionId();
	const WTS_INFO_CLASS wtsic = WTSSessionInfoEx;
	LPWSTR ppBuffer = nullptr;
	DWORD dwBytesReturned = 0;
	if (!WTSQuerySessionInformationW(WTS_CURRENT_SERVER_HANDLE, dwSessionID, wtsic, &ppBuffer, &dwBytesReturned))
	{
		return false;
	}

	if (dwBytesReturned == 0)
	{
		WTSFreeMemory(ppBuffer);
		return false;
	}

	const WTSINFOEXW * pInfo = reinterpret_cast<WTSINFOEXW *>(ppBuffer);
	if (pInfo->Level != 1)
	{
		WTSFreeMemory(ppBuffer);
		return false;
	}

	if (pInfo->Data.WTSInfoExLevel1.SessionFlags != WTS_SESSIONSTATE_UNLOCK)
	{
		WTSFreeMemory(ppBuffer);
		return false;
	}

	WTSFreeMemory(ppBuffer);
	return true;
}

