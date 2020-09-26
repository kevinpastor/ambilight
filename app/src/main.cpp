#include <windows.h>
#include <Wtsapi32.h>
#include <string>

#include "resources/resource.h"

#define ID_TRAY_APP_ICON 1001
#define ID_TRAY_EXIT 1002
#define WM_SYSICON (WM_USER + 1)

HMENU hMenu;
NOTIFYICONDATA notifyIconData;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		// Window is created
		ShowWindow(hwnd, SW_HIDE);
		hMenu = CreatePopupMenu();
		AppendMenu(hMenu, MF_STRING, ID_TRAY_EXIT, TEXT("Exit"));
		break;

	case WM_SYSICON:
		// Taskbar icon is focused
		if (lParam == WM_RBUTTONDOWN)
		{
			// Get current mouse position.
			POINT curPoint;
			GetCursorPos(&curPoint);
			SetForegroundWindow(hwnd);

			// TrackPopupMenu blocks the app until TrackPopupMenu returns
			BOOL clicked = TrackPopupMenu(hMenu, TPM_RETURNCMD | TPM_NONOTIFY, curPoint.x, curPoint.y, NULL, hwnd, NULL);

			SendMessage(hwnd, WM_NULL, NULL, NULL); // Send benign message to window to make sure the menu goes away.
			switch (clicked)
			{
			case ID_TRAY_EXIT:
				// Quit the application
				PostMessage(hwnd, WM_CLOSE, NULL, NULL);
				break;
			}
		}
		break;

	case WM_DESTROY:
		Shell_NotifyIcon(NIM_DELETE, &notifyIconData);
		PostQuitMessage(0);
		break;
	}

	return DefWindowProcW(hwnd, message, wParam, lParam);
}

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	WNDCLASSEX wincl;
	wchar_t appName[] = L"Ambilight";

	wincl.cbSize = sizeof(WNDCLASSEX);
	wincl.style = NULL;
	wincl.lpfnWndProc = WindowProc; // This function is called by windows
	wincl.cbClsExtra = 0; // No extra bytes after the window class
	wincl.cbWndExtra = 0; // structure or the window instance
	wincl.hInstance = hInstance;
	wincl.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(TASKBAR_ICON));
	wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
	wincl.hbrBackground = NULL;
	wincl.lpszMenuName = NULL;
	wincl.lpszClassName = appName;
	wincl.hIconSm = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(TASKBAR_ICON));

	// Register the window class, and if it fails quit the program
	if (!RegisterClassEx(&wincl))
	{
		return 0;
	}

	HWND hWnd = CreateWindowExW(NULL, appName, appName, NULL, 0, 0, 0, 0, HWND_MESSAGE, NULL, NULL, NULL);

	ZeroMemory(&notifyIconData, sizeof(NOTIFYICONDATA));
	notifyIconData.cbSize = sizeof(NOTIFYICONDATA);
	notifyIconData.hWnd = hWnd;
	notifyIconData.uID = ID_TRAY_APP_ICON;
	notifyIconData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	notifyIconData.uCallbackMessage = WM_SYSICON;
	notifyIconData.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(TASKBAR_ICON));
	wcscpy_s(notifyIconData.szTip, 128, appName);

	Shell_NotifyIcon(NIM_ADD, &notifyIconData);

	STARTUPINFO si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);

	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(pi));

	wchar_t path[] = L"ambilight.exe";
	CreateProcess(path, NULL, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi);

	MSG message;
	while (GetMessage(&message, NULL, 0, 0))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	TerminateProcess(pi.hProcess, 0);

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	return (int)message.wParam;
}
