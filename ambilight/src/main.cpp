#include "Ambilight.h"
#include "Command.h"

#include <Wtsapi32.h>
#include <windows.h>
#include <iostream>

bool isSessionLocked()
{
	typedef BOOL(PASCAL * WTSQuerySessionInformation)(HANDLE hServer, DWORD SessionId, WTS_INFO_CLASS WTSInfoClass, LPTSTR * ppBuffer, DWORD * pBytesReturned);
	typedef void (PASCAL * WTSFreeMemory)(PVOID pMemory);

	WTSINFOEXW * pInfo = NULL;
	WTS_INFO_CLASS wtsic = WTSSessionInfoEx;
	bool bRet = false;
	LPTSTR ppBuffer = NULL;
	DWORD dwBytesReturned = 0;
	LONG dwFlags = 0;
	WTSQuerySessionInformation pWTSQuerySessionInformation = NULL;
	WTSFreeMemory pWTSFreeMemory = NULL;

	HMODULE hLib = LoadLibrary("wtsapi32.dll");
	if (!hLib)
	{
		return false;
	}
	pWTSQuerySessionInformation = (WTSQuerySessionInformation)GetProcAddress(hLib, "WTSQuerySessionInformationW");
	if (pWTSQuerySessionInformation)
	{
		pWTSFreeMemory = (WTSFreeMemory)GetProcAddress(hLib, "WTSFreeMemory");
		if (pWTSFreeMemory != NULL)
		{
			DWORD dwSessionID = WTSGetActiveConsoleSessionId();
			if (pWTSQuerySessionInformation(WTS_CURRENT_SERVER_HANDLE, dwSessionID, wtsic, &ppBuffer, &dwBytesReturned))
			{
				if (dwBytesReturned > 0)
				{
					pInfo = (WTSINFOEXW *)ppBuffer;
					if (pInfo->Level == 1)
					{
						dwFlags = pInfo->Data.WTSInfoExLevel1.SessionFlags;
					}
					if (dwFlags == WTS_SESSIONSTATE_LOCK)
					{
						bRet = true;
					}
				}
				pWTSFreeMemory(ppBuffer);
				ppBuffer = NULL;
			}
		}
	}
	if (hLib != NULL)
	{
		FreeLibrary(hLib);
	}
	return bRet;
}

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	Ambilight ambilight;

	while (true)
	{
		if (isSessionLocked())
		{
			ambilight.pause();
			ambilight.fadeOut();
		}
		else
		{
			ambilight.resume();
		}
		ambilight.exec();
	}

	return 0;
}
