#pragma once

#include <stdexcept>
#include <string>
#include <thread>
#include <windows.h>
#include <Wtsapi32.h>

#include "resources/resource.h"

#include "Ambilight.h"
#include "Logger.h"

class App
{
public:
	App();

	int start(const HINSTANCE & hInstance);

private:
	HWND createWindow();
	
	static LRESULT WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
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
