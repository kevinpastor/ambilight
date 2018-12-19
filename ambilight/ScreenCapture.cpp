#include "ScreenCapture.h"

ScreenCapture::ScreenCapture()
	: screenHeight(0),
	screenWidth(0),
	screenCaptureData(NULL)
{
	this->setScreenSize();
	const unsigned size = this->screenWidth * this->screenHeight * 3;
	this->screenCaptureData = new unsigned char[size];
	std::memset(this->screenCaptureData, 0, size);
}

ScreenCapture::~ScreenCapture()
{
	delete[ ] this->screenCaptureData;
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

	GetDIBits(hdcMem, hBitmap, 0, this->screenHeight, this->screenCaptureData, (BITMAPINFO *)&bmpInfoHeader, DIB_RGB_COLORS);

	ReleaseDC(GetDesktopWindow(), hScreen);
	DeleteDC(hdcMem);
	DeleteObject(hBitmap);
}

Pixel ScreenCapture::getPixel(const Coordinates & coordinates) const
{
	if (!this->isValidPosition(coordinates))
	{
		throw std::invalid_argument("Out of bound coordinates");
	}

	// top left origin
	return Pixel({
		this->screenCaptureData[coordinates.x * 3 + coordinates.y * 3 * this->screenWidth + 2],
		this->screenCaptureData[coordinates.x * 3 + coordinates.y * 3 * this->screenWidth + 1],
		this->screenCaptureData[coordinates.x * 3 + coordinates.y * 3 * this->screenWidth]
		});
}

bool ScreenCapture::isValidPosition(const Coordinates & coordinates) const
{
	return this->isValidXPosition(coordinates.x)
		&& this->isValidYPosition(coordinates.y);
}

unsigned ScreenCapture::getScreenHeight() const
{
	return this->screenHeight;
}

unsigned ScreenCapture::getScreenWidth() const
{
	return this->screenWidth;
}

bool ScreenCapture::isValidXPosition(const unsigned & x) const
{
	return x < (int)this->screenWidth
		&& x >= 0;
}

bool ScreenCapture::isValidYPosition(const unsigned & y) const
{
	return y < (int)this->screenHeight
		&& y >= 0;
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