#include "ScreenCapture.h"

ScreenCapture::ScreenCapture()
	: hScreen(ScreenCapture::getHDC()),
	width(ScreenCapture::getWidth(this->hScreen)),
	height(ScreenCapture::getHeight(this->hScreen)),
	hdcMem(CreateCompatibleDC(this->hScreen)),
	hBitmap(CreateCompatibleBitmap(this->hScreen, this->width, this->height)),
	hOld(SelectObject(this->hdcMem, this->hBitmap)),
	bitmapInfo(ScreenCapture::getBitmapInfo(this->width, this->height))
{
}

ScreenCapture::~ScreenCapture()
{
	SelectObject(this->hdcMem, this->hOld);
	DeleteObject(this->hBitmap);
	DeleteDC(this->hdcMem);
	ReleaseDC(ScreenCapture::getHwnd(), this->hScreen);
}

Capture ScreenCapture::capture() const
{
	if (!BitBlt(this->hdcMem, 0, 0, this->width, this->height, hScreen, 0, 0, SRCCOPY))
	{
		throw std::runtime_error("Unable to take a screen capture");
	}

	const std::shared_ptr<std::vector<unsigned char>> screenCaptureData = std::make_shared<std::vector<unsigned char>>(this->width * this->height * 3);
	if (!GetDIBits(this->hdcMem, hBitmap, 0, this->height, screenCaptureData.get()->data(), const_cast<BITMAPINFO *>(&this->bitmapInfo), DIB_RGB_COLORS))
	{
		throw std::runtime_error("Unable to retrieve the screen capture");
	}

	return Capture(screenCaptureData, this->width, this->height);
}

HDC ScreenCapture::getHDC()
{
	HDC hdc = GetDC(ScreenCapture::getHwnd());

	if (hdc == NULL)
	{
		throw std::runtime_error("Unable to get a handle for the screen");
	}

	return hdc;
}

HWND ScreenCapture::getHwnd()
{
	return GetDesktopWindow();
}

unsigned ScreenCapture::getWidth(const HDC & hScreen)
{
	return GetDeviceCaps(hScreen, HORZRES);
}

unsigned ScreenCapture::getHeight(const HDC & hScreen)
{
	return GetDeviceCaps(hScreen, VERTRES);
}

BITMAPINFO ScreenCapture::getBitmapInfo(const unsigned & width, const unsigned & height)
{
	BITMAPINFOHEADER header;
	header.biSize = sizeof(BITMAPINFOHEADER);
	header.biWidth = width;
	header.biHeight = -(int)height;
	header.biPlanes = 1;
	header.biBitCount = 24;
	header.biCompression = BI_RGB;
	header.biSizeImage = width * height;

	BITMAPINFO info;
	info.bmiHeader = header;

	return info;
}
