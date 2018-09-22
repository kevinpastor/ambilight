#include "ScreenCapture.h"

std::vector<Pixel> ScreenCapture::getScreenData()
{
	HDC hScreen = GetDC(GetDesktopWindow());
	int screenWidth = GetDeviceCaps(hScreen, HORZRES);
	int screenHeight = GetDeviceCaps(hScreen, VERTRES);
	HDC hdcMem = CreateCompatibleDC(hScreen);
	HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, screenWidth, screenHeight);
	HGDIOBJ hOld = SelectObject(hdcMem, hBitmap);

	BitBlt(hdcMem, 0, 0, screenWidth, screenHeight, hScreen, 0, 0, SRCCOPY);
	SelectObject(hdcMem, hOld);

	BITMAPINFOHEADER bmi;
	bmi.biSize = sizeof(BITMAPINFOHEADER);
	bmi.biWidth = screenWidth;
	bmi.biHeight = -screenHeight;
	bmi.biPlanes = 1;
	bmi.biBitCount = 24;
	bmi.biCompression = BI_RGB;
	bmi.biSizeImage = 0;

	//void * screenCaptureData = new unsigned char[screenWidth * screenHeight * 3];
	//GetDIBits(hdcMem, hBitmap, 0, screenHeight, (unsigned char *) screenCaptureData, (BITMAPINFO*)&bmi, DIB_RGB_COLORS);
	std::vector<Pixel> sickData(screenWidth * screenHeight * 3);
	GetDIBits(hdcMem, hBitmap, 0, screenHeight, &sickData[0], (BITMAPINFO*)&bmi, DIB_RGB_COLORS);

	ReleaseDC(GetDesktopWindow(), hScreen);
	DeleteDC(hdcMem);
	DeleteObject(hBitmap);

	

	//Pixel * pixelArray = reinterpret_cast<Pixel *>(screenCaptureData);


	std::cout << "ici" << std::endl;

	//std::vector<Pixel> test((Pixel*)screenCaptureData, (Pixel *)screenCaptureData + 1555429);
	std::cout << (unsigned)sickData[4050070].red << std::endl;
	system("pause");
	//std::vector<Pixel> screenCapture(pixelArray, pixelArray);
	//screenCapture.insert(screenCapture.begin(), pixelArray, pixelArray + screenWidth * screenHeight * 3);
	//screenCapture.assign(pixelArray, pixelArray + 1555428);
	//return screenCapture;
	return sickData;
}
