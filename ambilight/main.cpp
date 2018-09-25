#include <iostream>
#include <vector>
#include <ctime>
//#include <Wincodec.h>             // we use WIC for saving images
//#include <d3d9.h>                 // DirectX 9 header
//#pragma comment(lib, "d3d9.lib")  // link to DirectX 9 library

#include "PixelParser.h"
#include "ArduinoSerial.h"
#include "Pixel.h"
#include "Coordinates.h"
#include "Ambilight.h"

//#define RELEASE(__p) {if(__p!=nullptr){__p->Release();__p=nullptr;}}
//
//HRESULT Direct3D9TakeScreenshots(UINT adapter)
//{
//	HRESULT hr = S_OK;
//	IDirect3D9 *d3d = nullptr;
//	IDirect3DDevice9 *device = nullptr;
//	IDirect3DSurface9 *surface = nullptr;
//	D3DPRESENT_PARAMETERS parameters = { 0 };
//	D3DDISPLAYMODE mode;
//	D3DLOCKED_RECT rc;
//	UINT pitch;
//	LPBYTE * shots = nullptr;
//
//	// init D3D and get screen size
//	d3d = Direct3DCreate9(D3D_SDK_VERSION);
//	d3d->GetAdapterDisplayMode(adapter, &mode);
//
//	parameters.Windowed = TRUE;
//	parameters.BackBufferCount = 1;
//	parameters.BackBufferHeight = mode.Height;
//	parameters.BackBufferWidth = mode.Width;
//	parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
//	parameters.hDeviceWindow = NULL;
//
//	// create device & capture surface
//	d3d->CreateDevice(adapter, D3DDEVTYPE_HAL, NULL, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &parameters, &device);
//	device->CreateOffscreenPlainSurface(mode.Width, mode.Height, D3DFMT_A8R8G8B8, D3DPOOL_SYSTEMMEM, &surface, nullptr);
//
//	// compute the required buffer size
//	surface->LockRect(&rc, NULL, 0);
//	pitch = rc.Pitch;
//	surface->UnlockRect();
//
//	// allocate screenshots buffers
//	shots = new LPBYTE[pitch * mode.Height];
//
//	// get the data
//	device->GetFrontBufferData(0, surface);
//
//	// copy it into our buffers
//	surface->LockRect(&rc, NULL, 0);
//	CopyMemory(shots, rc.pBits, rc.Pitch * mode.Height);
//	surface->UnlockRect();
//
//	std::cout << shots[0] << ", " << shots[1] << ", " << shots[2] << ", " << shots[3] << std::endl;
//
//cleanup:
//	if (shots != nullptr)
//	{
//		delete [] shots;
//	}
//	RELEASE(surface);
//	RELEASE(device);
//	RELEASE(d3d);
//	return hr;
//}

int main()
{
	//HRESULT hr = Direct3D9TakeScreenshots(D3DADAPTER_DEFAULT);
	//system("pause");

	const std::vector<Coordinates> coordinates = {
		{ 1151, 1079 }, // bottom right
		{ 1232, 1079 },
		{ 1294, 1079 },
		{ 1360, 1079 },
		{ 1418, 1079 },
		{ 1482, 1079 },
		{ 1535, 1079 },
		{ 1590, 1079 },
		{ 1665, 1079 },
		{ 1726, 1079 },
		{ 1793, 1079 },
		{ 1851, 1079 },
		{ 1913, 1079 },
		{ 1919, 1079 }, // bottom-right corner
		{ 1919, 1073 }, // right
		{ 1919, 1024 },
		{ 1919, 966 },
		{ 1919, 915 },
		{ 1919, 836 },
		{ 1919, 786 },
		{ 1919, 725 },
		{ 1919, 664 },
		{ 1919, 597 },
		{ 1919, 541 },
		{ 1919, 476 },
		{ 1919, 410 },
		{ 1919, 354 },
		{ 1919, 291 },
		{ 1919, 230 },
		{ 1919, 164 },
		{ 1919, 102 },
		{ 1919, 48 },
		{ 1919, 0 }, // top-right corner
		{ 1898, 0 }, // top row
		{ 1834, 0 },
		{ 1775, 0 },
		{ 1712, 0 },
		{ 1650, 0 },
		{ 1590, 0 },
		{ 1525, 0 },
		{ 1467, 0 },
		{ 1400, 0 },
		{ 1339, 0 },
		{ 1272, 0 },
		{ 1214, 0 },
		{ 1149, 0 },
		{ 1086, 0 },
		{ 1023, 0 },
		{ 967, 0 },
		{ 903, 0 },
		{ 839, 0 },
		{ 774, 0 },
		{ 715, 0 },
		{ 653, 0 },
		{ 589, 0 },
		{ 530, 0 },
		{ 467, 0 },
		{ 404, 0 },
		{ 340, 0 },
		{ 281, 0 },
		{ 215, 0 },
		{ 155, 0 },
		{ 93, 0 },
		{ 33, 0 },
		{ 0, 0 }, // top-left corner
	};

	unsigned nbLed = coordinates.size();

	// Arduino communication setup
	char portName[] = "\\\\.\\COM10";
	//ArduinoSerial arduinoSerial(portName, nbLed);

	Ambilight ambilight("\\\\.\\COM10", nbLed, coordinates);

	std::cout << "Started!" << std::endl;
	ambilight.start();

	//// RGB Led
	////std::vector<Pixel> data;// (nbLed);
	//PixelParser pixelParser(coordinates);
	//pixelParser.update();
	//std::vector<Pixel> previousPixels = pixelParser.getPixels();
	//std::vector<Pixel> data;
	//std::vector<Pixel> currentPixels;

	//std::cout << "Started!" << std::endl;
	//clock_t tStart = clock();
	////for (unsigned i = 0; i < 360; i++)
	//while (true)
	//{
	//	pixelParser.update();
	//	currentPixels = pixelParser.getPixels();
	//	data = pixelParser.fadePixels(currentPixels, previousPixels);
	//	previousPixels = data;

	//	// Sending data to the Arduino
	//	arduinoSerial.send(data);
	//}
	//printf("Time taken: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);

	system("pause");
	return 1;
}
