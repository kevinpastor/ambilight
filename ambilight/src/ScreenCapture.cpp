#include "ScreenCapture.h"

ScreenCapture::ScreenCapture()
	: screenHeight(0),
	screenWidth(0),
	screenCaptureData(nullptr)
{
	this->setScreenSize();
	const unsigned size = this->screenWidth * this->screenHeight * 3;
	this->screenCaptureData = std::shared_ptr<unsigned char[]>(new unsigned char[size]);
	std::memset(this->screenCaptureData.get(), 0, size);
}

Capture ScreenCapture::capture()
{
	HDC hScreen = this->getHDC();
	HDC hdcMem = CreateCompatibleDC(hScreen);
	HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, this->screenWidth, this->screenHeight);
	HGDIOBJ hOld = SelectObject(hdcMem, hBitmap);

	BitBlt(hdcMem, 0, 0, this->screenWidth, this->screenHeight, hScreen, 0, 0, SRCCOPY);
	SelectObject(hdcMem, hOld);

	BITMAPINFOHEADER bmpInfoHeader(this->getBmpInfoHeader());

	GetDIBits(hdcMem, hBitmap, 0, this->screenHeight, this->screenCaptureData.get(), (BITMAPINFO *)&bmpInfoHeader, DIB_RGB_COLORS);

	ReleaseDC(GetDesktopWindow(), hScreen);
	DeleteDC(hdcMem);
	DeleteObject(hBitmap);

	return Capture(this->screenCaptureData, this->screenHeight, screenWidth);
}

void ScreenCapture::setScreenSize()
{
	HDC hScreen(this->getHDC());
	this->screenWidth = GetDeviceCaps(hScreen, HORZRES);
	this->screenHeight = GetDeviceCaps(hScreen, VERTRES);
}

HDC ScreenCapture::getHDC() const
{
	return GetDC(GetDesktopWindow());
}

BITMAPINFOHEADER ScreenCapture::getBmpInfoHeader() const
{
	BITMAPINFOHEADER bmpInfoHeader;
	bmpInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfoHeader.biWidth = this->screenWidth;
	bmpInfoHeader.biHeight = -(int)this->screenHeight;
	bmpInfoHeader.biPlanes = 1;
	bmpInfoHeader.biBitCount = 24;
	bmpInfoHeader.biCompression = BI_RGB;
	bmpInfoHeader.biSizeImage = 0;

	return bmpInfoHeader;
}