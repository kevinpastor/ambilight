#pragma once

#include <stdexcept>
#include <string>
#include <future>
#include <windows.h>
#include <Wtsapi32.h>

#include "resources/resource.h"

#include "Ambilight.h"
#include "Logger.h"
#include "SessionUtility.h"

class App
{
public:
	int start(const HINSTANCE & hInstance);

private:
	static LRESULT WindowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

	LRESULT onMessage(HWND window, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT onCreate(const HWND & window) const;
	LRESULT onSessionChange(const WPARAM & wParam);
	LRESULT onFocus(const HWND & window, const LPARAM & lParam);
	LRESULT onClick(const HWND & window, const int & itemId);
	LRESULT onClickToggle(const HWND & window);
	LRESULT onClickLowPowerMode(const HWND & window);
	LRESULT onClickExit(const HWND & window) const;
	LRESULT onDestroy(const HWND & window) const;

	static HWND createWindow(App * app);
	static WNDCLASSEX getWindowClass(const HINSTANCE & instance);
	static NOTIFYICONDATA getNotifyIconData(const HINSTANCE & instance, const HWND & window);
	static int handleMessages();
	static HICON getIcon(const HINSTANCE & instance);

	Ambilight ambilight;

	static const std::string name;

	static const unsigned WM_SYSICON = (WM_USER + 1);
	static const unsigned ID_TRAY_APP_ICON = 1001;
	static const unsigned ID_TRAY_TOGGLE = ID_TRAY_APP_ICON + 1;
	static const unsigned ID_TRAY_LOW_POWER_MODE = ID_TRAY_TOGGLE + 1;
	static const unsigned ID_TRAY_EXIT = ID_TRAY_LOW_POWER_MODE + 1;

};
