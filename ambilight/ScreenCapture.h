#ifndef SCREEN_CAPTURE_H
#define SCREEN_CAPTURE_H

#include <windows.h>
#include <Wincodec.h>
#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")
#include <vector>

#include "Pixel.h"

namespace ScreenCapture
{
	std::vector<Pixel> getScreenData();
};

#endif