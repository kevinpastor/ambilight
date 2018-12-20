#pragma once

#include <stdexcept>
#include <vector>
#include <windows.h>

#include "Coordinates.h"
#include "Pixel.h"

class ScreenCapture
{
public:
	ScreenCapture();
	~ScreenCapture();

	void capture();
	Pixel getPixel(const Coordinates & coordinates) const;

	unsigned getScreenHeight() const;
	unsigned getScreenWidth() const;
	bool isValidXPosition(const unsigned & x) const;
	bool isValidYPosition(const unsigned & y) const;

private:
	bool isValidPosition(const Coordinates & coordinates) const;
	void setScreenSize();
	HDC getHDC() const;
	BITMAPINFOHEADER getBmpInfoHeader() const;

	unsigned char * screenCaptureData;
	unsigned screenHeight;
	unsigned screenWidth;

};