#include "App.h"

const std::string App::name = "Ambilight";

App::App()
{
}

int App::start(const HINSTANCE & hInstance)
{
	const WNDCLASSEX windowClass = App::getWindowClass(hInstance);
	if (!RegisterClassEx(&windowClass))
	{
		return 0;
	}

	const HWND hWnd = this->createWindow();

	NOTIFYICONDATA notifyIconData = App::getNotifyIconData(hInstance, hWnd);
	if (Shell_NotifyIcon(NIM_ADD, &notifyIconData) == FALSE)
	{
		throw std::runtime_error("TODO");
	}

	std::thread ambilightThread = std::thread([this]()
		{
			try
			{
				this->ambilight.start();
			}
			catch (std::exception & exception)
			{
				Logger::log(exception);
			}
			catch (...)
			{
				Logger::log(std::exception("Unexcepected error"));
			}
		});

	const int returnCode = App::handleMessages();

	this->ambilight.stop();
	if (ambilightThread.joinable())
	{
		ambilightThread.join();
	}

	return returnCode;
}

LRESULT App::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_NCCREATE:
		{
			SetLastError(0);
			if (SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(reinterpret_cast<CREATESTRUCT *>(lParam)->lpCreateParams)) == 0)
			{
				if (GetLastError() != 0)
				{
					return FALSE;
				}
			}
			break;
		}

		case WM_CREATE:
		{
			// Window is created
			ShowWindow(hWnd, SW_HIDE);
			HMENU mainMenu = CreateMenu();
			HMENU subMenu = CreatePopupMenu();

			if (AppendMenu(subMenu, MF_STRING, App::ID_TRAY_TOGGLE, "Pause") == FALSE)
			{
				throw std::runtime_error("TODO");
			}

			if (AppendMenu(subMenu, MF_STRING, App::ID_TRAY_EXIT, "Exit") == FALSE)
			{
				throw std::runtime_error("TODO");
			}

			if (AppendMenu(mainMenu, MF_STRING | MF_POPUP, reinterpret_cast<UINT_PTR>(subMenu), "File") == FALSE)
			{
				throw std::runtime_error("TODO");
			}

			if (SetMenu(hWnd, mainMenu) == FALSE)
			{
				throw std::runtime_error("TODO");
			}

			break;
		}

		case App::WM_SYSICON:
		{
			// Taskbar icon is focused
			if (lParam == WM_RBUTTONDOWN)
			{
				SetForegroundWindow(hWnd);

				// Get current mouse position.
				POINT curPoint;
				if (GetCursorPos(&curPoint) == FALSE)
				{
					throw std::runtime_error("TODO");
				}

				HMENU mainMenu = GetMenu(hWnd);
				HMENU subMenu = GetSubMenu(mainMenu, 0);

				// TrackPopupMenu blocks the app until TrackPopupMenu returns
				BOOL clickedId = TrackPopupMenu(subMenu, TPM_RETURNCMD | TPM_NONOTIFY, curPoint.x, curPoint.y, NULL, hWnd, NULL);
				SendMessage(hWnd, WM_NULL, NULL, NULL); // Send benign message to window to make sure the menu goes away.

				switch (clickedId)
				{
					case App::ID_TRAY_TOGGLE:
					{
						MENUITEMINFO menuItemInfo = { sizeof(MENUITEMINFO) };
						menuItemInfo.dwTypeData = NULL;
						menuItemInfo.fMask = MIIM_TYPE;
						menuItemInfo.fType = MFT_STRING;
						if (GetMenuItemInfo(subMenu, App::ID_TRAY_TOGGLE, FALSE, &menuItemInfo) == FALSE)
						{
							throw std::runtime_error("TODO");
						}

						std::vector<char> buffer(static_cast<unsigned long>(menuItemInfo.cch) + 1);
						menuItemInfo.dwTypeData = buffer.data();
						menuItemInfo.cch++;
						if (GetMenuItemInfo(subMenu, App::ID_TRAY_TOGGLE, FALSE, &menuItemInfo) == FALSE)
						{
							throw std::runtime_error("TODO");
						}

						LONG_PTR data = GetWindowLongPtr(hWnd, GWLP_USERDATA);
						if (data == 0)
						{
							throw std::runtime_error("TODO");
						}

						App * app = reinterpret_cast<App *>(data);
						std::string label(buffer.begin(), buffer.end() - 1); // Skipping the null character
						if (label == "Pause")
						{
							const std::string newLabel("Resume");
							menuItemInfo.cch = static_cast<unsigned>(newLabel.length());
							menuItemInfo.dwTypeData = const_cast<char *>(newLabel.data());
							if (SetMenuItemInfo(subMenu, App::ID_TRAY_TOGGLE, FALSE, &menuItemInfo) == FALSE)
							{
								throw std::runtime_error("TODO");
							}

							app->ambilight.userPause();
						}
						else
						{
							const std::string newLabel("Pause");
							menuItemInfo.cch = static_cast<unsigned>(newLabel.length());
							menuItemInfo.dwTypeData = const_cast<char *>(newLabel.data());
							if (SetMenuItemInfo(subMenu, App::ID_TRAY_TOGGLE, FALSE, &menuItemInfo) == FALSE)
							{
								throw std::runtime_error("TODO");
							}

							app->ambilight.userResume();
						}
						break;
					}

					case App::ID_TRAY_EXIT:
					{
						if (PostMessage(hWnd, WM_CLOSE, NULL, NULL) == FALSE)
						{
							throw std::runtime_error("TODO");
						}
						break;
					}
				}
			}
			break;
		}

		case WM_DESTROY:
		{
			NOTIFYICONDATA notifyIconData;
			SecureZeroMemory(&notifyIconData, sizeof(NOTIFYICONDATA));
			notifyIconData.hWnd = hWnd;
			notifyIconData.uID = App::ID_TRAY_APP_ICON;
			if (Shell_NotifyIcon(NIM_DELETE, &notifyIconData) == FALSE)
			{
				throw std::runtime_error("TODO");
			}

			PostQuitMessage(0);
			break;
		}
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
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

HWND App::createWindow()
{
	HWND hWnd = CreateWindowEx(NULL, App::name.c_str(), App::name.c_str(), NULL, 0, 0, 0, 0, HWND_MESSAGE, NULL, NULL, this);
	if (hWnd == NULL)
	{
		throw std::runtime_error("TODO");
	}

	return hWnd;
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
		throw std::runtime_error("TODO");
	}

	return static_cast<HICON>(handle);
}
