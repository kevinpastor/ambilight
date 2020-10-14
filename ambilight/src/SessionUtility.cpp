#include "SessionUtility.h"

SessionUtility * SessionUtility::instance = nullptr;
const std::chrono::milliseconds SessionUtility::REFRESH_RATE = std::chrono::milliseconds(1000);

SessionUtility::SessionUtility()
	: locked(false)
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
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	if ((now - SessionUtility::getInstance()->lastMeasurement) > SessionUtility::REFRESH_RATE)
	{
		if (SessionUtility::getInstance()->thread.joinable())
		{
			SessionUtility::getInstance()->thread.join();
		}

		SessionUtility::getInstance()->thread = std::thread([]()
			{
				SessionUtility::getInstance()->updateState();
			});

		SessionUtility::getInstance()->lastMeasurement = now;
	}

	return SessionUtility::getInstance()->locked;
}

void SessionUtility::updateState()
{
	const DWORD dwSessionID = WTSGetActiveConsoleSessionId();
	const WTS_INFO_CLASS wtsic = WTSSessionInfoEx;
	LPWSTR ppBuffer = nullptr;
	DWORD dwBytesReturned = 0;
	if (!WTSQuerySessionInformationW(WTS_CURRENT_SERVER_HANDLE, dwSessionID, wtsic, &ppBuffer, &dwBytesReturned))
	{
		this->locked = false;
		return;
	}

	if (dwBytesReturned == 0)
	{
		WTSFreeMemory(ppBuffer);
		this->locked = false;
		return;
	}

	const WTSINFOEXW * pInfo = reinterpret_cast<WTSINFOEXW *>(ppBuffer);
	if (pInfo->Level != 1)
	{
		WTSFreeMemory(ppBuffer);
		this->locked = false;
		return;
	}

	if (pInfo->Data.WTSInfoExLevel1.SessionFlags != WTS_SESSIONSTATE_LOCK)
	{
		WTSFreeMemory(ppBuffer);
		this->locked = false;
		return;
	}

	WTSFreeMemory(ppBuffer);
	this->locked = true;
}
