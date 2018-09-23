#include <iostream>
#include <vector>
#include <ctime>

#include "PixelParser.h"
#include "ArduinoSerial.h"
#include "Pixel.h"
#include "Coordinates.h"

int main()
{
	std::vector<Coordinates> coordinates = {
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
		{ 1919, 1073 }, // left
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
	};

	unsigned nbLed = coordinates.size();

	// Arduino communication setup
	char portName[] = "\\\\.\\COM10";
	ArduinoSerial arduinoSerial(portName, nbLed);

	// RGB Led
	//std::vector<Pixel> data;// (nbLed);
	PixelParser screenCaptureParser(coordinates);

	std::cout << "Started!" << std::endl;
	while (true)
	{
		screenCaptureParser.update();
		std::vector<Pixel> data = screenCaptureParser.getPixels();

		// Sending data to the Arduino
		arduinoSerial.send(data);
	}

	system("pause");
	return 1;
}