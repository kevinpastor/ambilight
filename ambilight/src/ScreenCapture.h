#pragma once

#include <memory>
#include <stdexcept>
#include <vector>
#include <windows.h>

#include "Capture.h"
#include "Coordinates.h"
#include "Pixel.h"

class ScreenCapture
{
public:
	ScreenCapture(const std::vector<Coordinates> & coordinates, const unsigned & radius);
	~ScreenCapture();

	Capture capture() const;

private:
	static HDC getHDC();
	static HWND getHwnd();
	static unsigned getWidth(const HDC & hScreen);
	static unsigned getHeight(const HDC & hScreen);
	static BITMAPINFO getBitmapInfo(const unsigned & width, const unsigned & height);

	const std::vector<Coordinates> coordinates;
	const unsigned radius;

	const HDC hScreen;
	const unsigned width;
	const unsigned height;
	unsigned char * buffer;
	const BITMAPINFO bitmapInfo;
	const HDC hdcMem;
	const HBITMAP hBitmap;
	const HGDIOBJ hOld;

};
