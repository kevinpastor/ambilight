#pragma once

#include <memory>
#include <stdexcept>
#include <vector>
#include <windows.h>

#include "Coordinates.h"
#include "Pixel.h"
#include "Capture.h"

class ScreenCapture
{
public:
	ScreenCapture();

	Capture capture();

private:
	void setScreenSize();
	HDC getHDC() const;
	BITMAPINFOHEADER getBmpInfoHeader() const;

	std::shared_ptr<unsigned char []> screenCaptureData;
	unsigned screenHeight;
	unsigned screenWidth;

};