#include "ScreenCapture.h"

std::vector<Pixel> ScreenCapture::getScreenData()
{
	HDC hScreen = GetDC(GetDesktopWindow());
	int screenWidth = GetDeviceCaps(hScreen, HORZRES);
	int screenHeight = GetDeviceCaps(hScreen, VERTRES);
	HDC hdcMem = CreateCompatibleDC(hScreen);
	HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, screenWidth, screenHeight);
	HGDIOBJ hOld = SelectObject(hdcMem, hBitmap);

	BitBlt(hdcMem, 0, 0, screenWidth, screenHeight, hScreen, 0, 0, SRCCOPY);
	SelectObject(hdcMem, hOld);

	BITMAPINFOHEADER bmi;
	bmi.biSize = sizeof(BITMAPINFOHEADER);
	bmi.biWidth = screenWidth;
	bmi.biHeight = -screenHeight;
	bmi.biPlanes = 1;
	bmi.biBitCount = 24;
	bmi.biCompression = BI_RGB;
	bmi.biSizeImage = 0;

	std::vector<Pixel> screenCaptureData(screenWidth * screenHeight * 3);
	GetDIBits(hdcMem, hBitmap, 0, screenHeight, &screenCaptureData[0], (BITMAPINFO*)&bmi, DIB_RGB_COLORS);

	ReleaseDC(GetDesktopWindow(), hScreen);
	DeleteDC(hdcMem);
	DeleteObject(hBitmap);

	return screenCaptureData;
}
