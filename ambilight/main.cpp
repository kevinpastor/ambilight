#include <iostream>
#include <vector>
#include <ctime>

#include "ScreenCaptureParser.h"
#include "ArduinoSerial.h"
#include "ScreenCapture.h"
#include "Pixel.h"
#include "Coordinates.h"

int main()
{
	// Arduino communication setup
	char portName[] = "\\\\.\\COM10";
	unsigned nbLed = 97;
	ArduinoSerial arduinoSerial(portName, nbLed);

	Coordinates coordinates[] = {
		{1151, 1070},
		{1232, 1070},
		{1294, 1070},
		{1360, 1070},
		{1418, 1070},
		{1482, 1070}
	};

	// RGB Led
	std::vector<Pixel> data(nbLed);
	ScreenCaptureParser screenCaptureParser;

	std::cout << "Started!" << std::endl;
	while (true)
	{
		screenCaptureParser.update();
		for (unsigned i = 0; i < sizeof(coordinates) / sizeof(Coordinates); i++)
		{
			data[i] = screenCaptureParser.getPixel(coordinates[i].x, coordinates[i].y);
		}
		
		// Sending data to the Arduino
		arduinoSerial.send(data);

		//std::cout << (unsigned)data[0].red << ", " << (unsigned)data[0].green << ", " << (unsigned)data[0].blue << std::endl;
	}

	system("pause");
	return 1;
}