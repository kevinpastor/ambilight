#include "ScreenCapture.h"

ScreenCapture::ScreenCapture(const std::vector<Coordinates> & coordinates, const unsigned & radius)
	: coordinates(coordinates),
	radius(radius),
	hScreen(ScreenCapture::getHDC()),
	width(ScreenCapture::getWidth(this->hScreen)),
	height(ScreenCapture::getHeight(this->hScreen)),
	bitmapInfo(ScreenCapture::getBitmapInfo(this->width, this->height)),
	hdcMem(CreateCompatibleDC(this->hScreen)),
	hBitmap(CreateDIBSection(this->hScreen, &this->bitmapInfo, DIB_RGB_COLORS, reinterpret_cast<LPVOID *>(&this->buffer), NULL, 0)),
	hOld(SelectObject(this->hdcMem, this->hBitmap))
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
	BitBlt(this->hdcMem, 0, 0, this->width, this->height, this->hScreen, 0, 0, SRCCOPY);

	return Capture(this->buffer, this->width, this->height);
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
	header.biSizeImage = 0;

	BITMAPINFO info;
	info.bmiHeader = header;

	return info;
}
