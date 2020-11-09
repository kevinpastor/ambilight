#include "SessionUtility.h"

SessionUtility * SessionUtility::instance = nullptr;
const std::chrono::milliseconds SessionUtility::REFRESH_RATE = std::chrono::milliseconds(1000);

SessionUtility::SessionUtility()
	: locked(SessionUtility::isCurrentlyLocked())
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

bool SessionUtility::isLocked()
{
	SessionUtility * instance = SessionUtility::getInstance();
	const std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	if ((now - instance->lastMeasurement) > SessionUtility::REFRESH_RATE)
	{
		if (instance->future.valid())
		{
			instance->locked = instance->future.get();
		}

		instance->future = std::async(std::launch::async, SessionUtility::isCurrentlyLocked);

		instance->lastMeasurement = now;
	}

	return instance->locked;
}

bool SessionUtility::isCurrentlyLocked()
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

	if (pInfo->Data.WTSInfoExLevel1.SessionFlags != WTS_SESSIONSTATE_LOCK)
	{
		WTSFreeMemory(ppBuffer);
		return false;
	}

	WTSFreeMemory(ppBuffer);
	return true;
}
