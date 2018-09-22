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

	BITMAPINFOHEADER bmi = { 0 };
	bmi.biSize = sizeof(BITMAPINFOHEADER);
	bmi.biPlanes = 1;
	bmi.biBitCount = 32;
	bmi.biWidth = screenWidth;
	bmi.biHeight = -screenHeight;
	bmi.biCompression = BI_RGB;
	bmi.biSizeImage = 0;

	unsigned char * screenCaptureData = new unsigned char[screenWidth * screenHeight * 4]();
	GetDIBits(hdcMem, hBitmap, 0, screenHeight, screenCaptureData, (BITMAPINFO*)&bmi, DIB_RGB_COLORS);

	ReleaseDC(GetDesktopWindow(), hScreen);
	DeleteDC(hdcMem);
	DeleteObject(hBitmap);

	unsigned screenCaptureSize = screenWidth * screenHeight * 4;
	Pixel * test = (Pixel *)screenCaptureData;
	std::vector<Pixel> screenCapture({ {1,2,3} });

	return screenCapture;
}
