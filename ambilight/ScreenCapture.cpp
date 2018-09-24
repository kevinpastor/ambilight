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

	GetDIBits(hdcMem, hBitmap, 0, this->screenHeight, this->screenCaptureData, (BITMAPINFO *)&bmpInfoHeader, DIB_RGB_COLORS);

	ReleaseDC(GetDesktopWindow(), hScreen);
	DeleteDC(hdcMem);
	DeleteObject(hBitmap);
}

const Pixel ScreenCapture::getPixel(const Coordinates & coordinates)
{
	if (!this->isValidPosition(coordinates)) {
		throw std::invalid_argument("Out of bound coordinates");
	}

	// top left origin
	Pixel pixel({
		this->screenCaptureData[coordinates.x * 3 + coordinates.y * 3 * this->screenWidth + 2],
		this->screenCaptureData[coordinates.x * 3 + coordinates.y * 3 * this->screenWidth + 1],
		this->screenCaptureData[coordinates.x * 3 + coordinates.y * 3 * this->screenWidth],
	});

	return pixel;
}

const bool ScreenCapture::isValidPosition(const Coordinates & coordinates)
{
	return this->isValidXPosition(coordinates.x)
		&& this->isValidYPosition(coordinates.y);
}

const unsigned ScreenCapture::getScreenHeight()
{
	return this->screenHeight;
}

const unsigned ScreenCapture::getScreenWidth()
{
	return this->screenWidth;
}

const bool ScreenCapture::isValidXPosition(const unsigned & x)
{
	return x < (int)this->screenWidth
		&& x >= 0;
}

const bool ScreenCapture::isValidYPosition(const unsigned & y)
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

const HDC ScreenCapture::getHDC()
{
	return GetDC(GetDesktopWindow());
}

const BITMAPINFOHEADER ScreenCapture::getBmpInfoHeader()
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