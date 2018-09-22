#include "ScreenCapture.h"

ScreenCapture::ScreenCapture() : screenHeight(0), screenWidth(0), screenCaptureData(NULL)
{
	this->setScreenSize();
	this->screenCaptureData = new unsigned char[this->screenWidth * this->screenHeight * 3];
}

ScreenCapture::~ScreenCapture()
{
	delete [] this->screenCaptureData;
}

void ScreenCapture::capture()
{
	HDC hScreen = this->getHDC();
	HDC hdcMem = CreateCompatibleDC(hScreen);
	HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, this->screenWidth, this->screenHeight);
	HGDIOBJ hOld = SelectObject(hdcMem, hBitmap);

	BitBlt(hdcMem, 0, 0, this->screenWidth, this->screenHeight, hScreen, 0, 0, SRCCOPY);
	SelectObject(hdcMem, hOld);

	BITMAPINFOHEADER bmpInfoHeader(this->getBmpInfoHeader());

	GetDIBits(hdcMem, hBitmap, 0, this->screenHeight, this->screenCaptureData, (BITMAPINFO*)&bmpInfoHeader, DIB_RGB_COLORS);

	ReleaseDC(GetDesktopWindow(), hScreen);
	DeleteDC(hdcMem);
	DeleteObject(hBitmap);
}

const Pixel ScreenCapture::getPixel(const unsigned & x, const unsigned & y)
{
	if (!this->isValidPosition(x, y)) {
		throw std::invalid_argument("Out of bound coordinates");
	}

	// top left origin
	Pixel pixel({
		this->screenCaptureData[x * 3 + y * 3 * this->screenWidth + 2],
		this->screenCaptureData[x * 3 + y * 3 * this->screenWidth + 1],
		this->screenCaptureData[x * 3 + y * 3 * this->screenWidth],
	});

	return pixel;
}

const bool ScreenCapture::isValidPosition(const unsigned & x, const unsigned & y)
{
	return x < this->screenWidth
		&& y < this->screenHeight;
}

const unsigned ScreenCapture::getScreenHeight()
{
	return this->screenHeight;
}

const unsigned ScreenCapture::getScreenWidth()
{
	return this->screenWidth;
}

void ScreenCapture::setScreenSize()
{
	HDC hScreen(this->getHDC());
	this->screenWidth = GetDeviceCaps(hScreen, HORZRES);
	this->screenHeight = GetDeviceCaps(hScreen, VERTRES);
}

HDC ScreenCapture::getHDC()
{
	return GetDC(GetDesktopWindow());
}

BITMAPINFOHEADER ScreenCapture::getBmpInfoHeader()
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