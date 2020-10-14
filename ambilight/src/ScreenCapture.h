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
	ScreenCapture();
	~ScreenCapture();

	Capture capture() const;

private:
	static HDC getHDC();
	static HWND getHwnd();
	static unsigned getWidth(const HDC & hScreen);
	static unsigned getHeight(const HDC & hScreen);
	static BITMAPINFO getBitmapInfo(const unsigned & width, const unsigned & height);

	const HDC hScreen;
	const unsigned width;
	const unsigned height;
	const HDC hdcMem;
	const HBITMAP hBitmap;
	const HGDIOBJ hOld;
	const BITMAPINFO bitmapInfo;

};
