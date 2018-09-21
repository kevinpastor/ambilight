#ifndef SCREEN_CAPTURE_H
#define SCREEN_CAPTURE_H

#include <windows.h>
#include <Wincodec.h>
#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

namespace ScreenCapture
{
	void getScreenData(unsigned char * dest);
};

#endif