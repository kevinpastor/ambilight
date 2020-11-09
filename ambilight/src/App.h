#pragma once

#include <stdexcept>
#include <string>
#include <future>
#include <windows.h>
#include <Wtsapi32.h>

#include "resources/resource.h"

#include "Ambilight.h"
#include "Logger.h"

class App
{
public:
	int start(const HINSTANCE & hInstance);

private:
	static LRESULT WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	LRESULT onMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT onCreate(const HWND & hWnd) const;
	LRESULT onFocus(const HWND & hWnd, const LPARAM & lParam);
	void onClick(const HWND & hWnd, const int & itemId);
	void onClickToggle(const HWND & hWnd);
	void onClickExit(const HWND & hWnd) const;
	LRESULT onDestroy(const HWND & hWnd) const;

	static HWND createWindow(App * app);
	static WNDCLASSEX getWindowClass(const HINSTANCE & instance);
	static NOTIFYICONDATA getNotifyIconData(const HINSTANCE & instance, const HWND & hWnd);
	static int handleMessages();
	static HICON getIcon(const HINSTANCE & instance);

	Ambilight ambilight;

	static const std::string name;
	static const unsigned ID_TRAY_APP_ICON = 1001;
	static const unsigned ID_TRAY_EXIT = 1002;
	static const unsigned ID_TRAY_TOGGLE = 1003;
	static const unsigned WM_SYSICON = (WM_USER + 1);

};
