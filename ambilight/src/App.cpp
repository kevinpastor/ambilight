#include "App.h"

const std::string App::name = "Ambilight";

int App::start(const HINSTANCE & hInstance)
{
	const WNDCLASSEX windowClass = App::getWindowClass(hInstance);
	if (RegisterClassEx(&windowClass) == 0)
	{
		return 0;
	}

	HWND hWnd = App::createWindow(this);

	NOTIFYICONDATA notifyIconData = App::getNotifyIconData(hInstance, hWnd);
	if (!Shell_NotifyIcon(NIM_ADD, &notifyIconData))
	{
		throw std::runtime_error("Unable to add the icon to the taskbar's status area");
	}

	std::future<void> ambilightThread = std::async(std::launch::async, [&]()
	{
		try
		{
			this->ambilight.start();
		}
		catch (std::exception & exception)
		{
			Logger::log(exception);

			if (!PostMessage(hWnd, WM_CLOSE, NULL, NULL))
			{
				Logger::log(std::runtime_error("Unable to successfully close the app"));
			}
		}
		catch (...)
		{
			Logger::log(std::exception("Unexcepected error"));

			if (!PostMessage(hWnd, WM_CLOSE, NULL, NULL))
			{
				Logger::log(std::runtime_error("Unable to successfully close the app"));
			}
		}
	});

	const int returnCode = App::handleMessages();

	this->ambilight.stop();

	if (ambilightThread.valid())
	{
		ambilightThread.get();
	}

	return returnCode;
}

LRESULT App::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LONG_PTR data = GetWindowLongPtr(hWnd, GWLP_USERDATA);
	if (message == WM_NCCREATE)
	{
		data = reinterpret_cast<LONG_PTR>(reinterpret_cast<CREATESTRUCT *>(lParam)->lpCreateParams);
		SetLastError(0);
		if (SetWindowLongPtr(hWnd, GWLP_USERDATA, data) == 0)
		{
			if (GetLastError() != 0)
			{
				return FALSE;
			}
		}

		return true;
	}

	if (data != 0)
	{
		App * app = reinterpret_cast<App *>(data);
		return app->onMessage(hWnd, message, wParam, lParam);
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT App::onMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case (WM_CREATE):
		{
			return this->onCreate(hWnd);
		}
		case (WM_WTSSESSION_CHANGE):
		{
			return this->onSessionChange(wParam);
		}
		case (App::WM_SYSICON):
		{
			return this->onFocus(hWnd, lParam);
		}
		case (WM_DESTROY):
		{
			return this->onDestroy(hWnd);
		}
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT App::onCreate(const HWND & hWnd) const
{
	WTSRegisterSessionNotification(hWnd, NOTIFY_FOR_THIS_SESSION);

	ShowWindow(hWnd, SW_HIDE);
	HMENU mainMenu = CreateMenu();
	HMENU subMenu = CreatePopupMenu();

	if (!AppendMenu(subMenu, MF_STRING, App::ID_TRAY_TOGGLE, "Pause"))
	{
		throw std::runtime_error("Unable to add menu option");
	}

	if (!AppendMenu(subMenu, MF_STRING, App::ID_TRAY_EXIT, "Exit"))
	{
		throw std::runtime_error("Unable to add menu option");
	}

	if (!AppendMenu(mainMenu, MF_STRING | MF_POPUP, reinterpret_cast<UINT_PTR>(subMenu), "File"))
	{
		throw std::runtime_error("Unable to add menu option");
	}

	if (!SetMenu(hWnd, mainMenu))
	{
		throw std::runtime_error("Unable to set the menu");
	}

	return 0;
}

LRESULT App::onSessionChange(const WPARAM & wParam)
{
	switch (wParam)
	{
		case (WTS_SESSION_LOCK):
		{
			this->ambilight.pause();
			return 0;
		}
		case (WTS_SESSION_UNLOCK):
		{
			this->ambilight.resume();
			return 0;
		}
	}

	return -1;
}

LRESULT App::onFocus(const HWND & hWnd, const LPARAM & lParam)
{
	if (lParam == WM_RBUTTONDOWN)
	{
		SetForegroundWindow(hWnd);

		// Get current mouse position.
		POINT curPoint;
		if (!GetCursorPos(&curPoint))
		{
			throw std::runtime_error("Unable to get cursor position");
		}

		HMENU mainMenu = GetMenu(hWnd);
		HMENU subMenu = GetSubMenu(mainMenu, 0);

		// TrackPopupMenu blocks the app until TrackPopupMenu returns
		const BOOL itemId = TrackPopupMenu(subMenu, TPM_RETURNCMD | TPM_NONOTIFY, curPoint.x, curPoint.y, NULL, hWnd, NULL);
		SendMessage(hWnd, WM_NULL, NULL, NULL); // Send benign message to window to make sure the menu goes away.

		return this->onClick(hWnd, itemId);
	}

	return -1;
}

LRESULT App::onClick(const HWND & hWnd, const int & itemId)
{
	switch (itemId)
	{
		case (App::ID_TRAY_TOGGLE):
		{
			return this->onClickToggle(hWnd);
		}

		case (App::ID_TRAY_EXIT):
		{
			return this->onClickExit(hWnd);
		}
	}

	return -1;
}

LRESULT App::onClickToggle(const HWND & hWnd)
{
	HMENU mainMenu = GetMenu(hWnd);
	HMENU subMenu = GetSubMenu(mainMenu, 0);

	MENUITEMINFO menuItemInfo = { sizeof(MENUITEMINFO) };
	menuItemInfo.dwTypeData = NULL;
	menuItemInfo.fMask = MIIM_TYPE;
	menuItemInfo.fType = MFT_STRING;
	if (!GetMenuItemInfo(subMenu, App::ID_TRAY_TOGGLE, FALSE, &menuItemInfo))
	{
		throw std::runtime_error("Unable to get menu item");
	}

	std::vector<char> buffer(1ull + menuItemInfo.cch);
	menuItemInfo.dwTypeData = buffer.data();
	menuItemInfo.cch++;
	if (!GetMenuItemInfo(subMenu, App::ID_TRAY_TOGGLE, FALSE, &menuItemInfo))
	{
		throw std::runtime_error("Unable to get menu item");
	}

	std::string label(buffer.begin(), buffer.end() - 1); // Skipping the null character
	if (label == "Pause")
	{
		menuItemInfo.dwTypeData = "Resume";
		menuItemInfo.cch = sizeof("Resume");
		if (!SetMenuItemInfo(subMenu, App::ID_TRAY_TOGGLE, FALSE, &menuItemInfo))
		{
			throw std::runtime_error("Unable to update menu item");
		}

		this->ambilight.pause();
	}
	else
	{
		menuItemInfo.dwTypeData = "Pause";
		menuItemInfo.cch = sizeof("Pause");
		if (!SetMenuItemInfo(subMenu, App::ID_TRAY_TOGGLE, FALSE, &menuItemInfo))
		{
			throw std::runtime_error("Unable to update menu item");
		}

		this->ambilight.resume();
	}

	return 0;
}

LRESULT App::onClickExit(const HWND & hWnd) const
{
	if (!PostMessage(hWnd, WM_CLOSE, NULL, NULL))
	{
		throw std::runtime_error("Unable to successfully close the app");
	}

	return 0;
}

LRESULT App::onDestroy(const HWND & hWnd) const
{
	NOTIFYICONDATA notifyIconData;
	SecureZeroMemory(&notifyIconData, sizeof(NOTIFYICONDATA));
	notifyIconData.hWnd = hWnd;
	notifyIconData.uID = App::ID_TRAY_APP_ICON;
	if (!Shell_NotifyIcon(NIM_DELETE, &notifyIconData))
	{
		throw std::runtime_error("Unable to remove the icon from the taskbar's status area");
	}

	PostQuitMessage(0);

	return true;
}

HWND App::createWindow(App * app)
{
	HWND hWnd = CreateWindowEx(NULL, App::name.c_str(), App::name.c_str(), NULL, 0, 0, 0, 0, HWND_MESSAGE, NULL, NULL, app);
	if (hWnd == NULL)
	{
		throw std::runtime_error("Unable to start the app");
	}

	return hWnd;
}

WNDCLASSEX App::getWindowClass(const HINSTANCE & instance)
{
	WNDCLASSEX wincl;

	wincl.cbSize = sizeof(WNDCLASSEX);
	wincl.style = NULL;
	wincl.lpfnWndProc = App::WindowProc; // This function is called by windows
	wincl.cbClsExtra = 0; // No extra bytes after the window class
	wincl.cbWndExtra = 0; // structure or the window instance
	wincl.hInstance = instance;
	wincl.hIcon = App::getIcon(instance);
	wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
	wincl.hbrBackground = NULL;
	wincl.lpszMenuName = NULL;
	wincl.lpszClassName = App::name.c_str();
	wincl.hIconSm = App::getIcon(instance);

	return wincl;
}

NOTIFYICONDATA App::getNotifyIconData(const HINSTANCE & instance, const HWND & hWnd)
{
	NOTIFYICONDATA notifyIconData;

	SecureZeroMemory(&notifyIconData, sizeof(NOTIFYICONDATA));
	notifyIconData.cbSize = sizeof(NOTIFYICONDATA);
	notifyIconData.hWnd = hWnd;
	notifyIconData.uID = App::ID_TRAY_APP_ICON;
	notifyIconData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	notifyIconData.uCallbackMessage = App::WM_SYSICON;
	notifyIconData.hIcon = App::getIcon(instance);
	strcpy_s(notifyIconData.szTip, 128, App::name.c_str());

	return notifyIconData;
}

int App::handleMessages()
{
	MSG message;
	while (GetMessage(&message, NULL, 0, 0))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	return static_cast<int>(message.wParam);
}

HICON App::getIcon(const HINSTANCE & instance)
{
	HANDLE handle = LoadImage(instance, MAKEINTRESOURCE(TASKBAR_ICON), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
	if (handle == NULL)
	{
		throw std::runtime_error("Unable to load the app icon");
	}

	return static_cast<HICON>(handle);
}
