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

	BITMAPINFOHEADER bmpInfoHeader;
	bmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfoHeader.biWidth = screenWidth;
	bmpInfoHeader.biHeight = -screenHeight;
	bmpInfoHeader.biPlanes = 1;
	bmpInfoHeader.biBitCount = 24;
	bmpInfoHeader.biCompression = BI_RGB;
	bmpInfoHeader.biSizeImage = 0;

	std::vector<Pixel> screenCaptureData(screenWidth * screenHeight * 3);
	GetDIBits(hdcMem, hBitmap, 0, screenHeight, &screenCaptureData[0], (BITMAPINFO*)&bmpInfoHeader, DIB_RGB_COLORS);

	ReleaseDC(GetDesktopWindow(), hScreen);
	DeleteDC(hdcMem);
	DeleteObject(hBitmap);

	return screenCaptureData;
}
