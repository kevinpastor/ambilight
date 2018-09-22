#include <iostream>
#include <vector>
#include <ctime>

#include "PixelParser.h"
#include "ArduinoSerial.h"
#include "Pixel.h"
#include "Coordinates.h"

int main()
{
	Coordinates coordinates[] = {
		{ 1151, 1070 },
		{ 1232, 1070 },
		{ 1294, 1070 },
		{ 1360, 1070 },
		{ 1418, 1070 },
		{ 1482, 1070 }
	};

	unsigned nbLed = sizeof(coordinates) / sizeof(Coordinates);

	// Arduino communication setup
	char portName[] = "\\\\.\\COM10";
	ArduinoSerial arduinoSerial(portName, nbLed);

	// RGB Led
	std::vector<Pixel> data(nbLed);
	PixelParser screenCaptureParser;

	std::cout << "Started!" << std::endl;
	while (true)
	{
		screenCaptureParser.update();
		for (unsigned i = 0; i < nbLed; i++)
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