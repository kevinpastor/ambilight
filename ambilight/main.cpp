#include <iostream>
#include <vector>

#include "ScreenCaptureParser.h"
#include "ArduinoSerial.h"
#include "ScreenCapture.h"
#include "Pixel.h"

int main()
{
	// Arduino communication setup
	char portName[] = "\\\\.\\COM10";
	unsigned nbLed = 97;
	ArduinoSerial arduinoSerial(portName, nbLed);

	// RGB Led
	std::vector<Pixel> data;
	data.push_back(Pixel({ 255,255,255 }));
	//std::cout << (unsigned)data[0].red << ", " << (unsigned)data[0].green << ", " << (unsigned)data[0].blue << std::endl;

	// Sending data to the Arduino
	arduinoSerial.send(data);

	system("pause");
	// Test
	while (true)
	{
		std::vector<Pixel> data2 = ScreenCapture::getScreenData();

		ScreenCaptureParser screenCaptureParser(data2);
		data[0] = screenCaptureParser.getPixel(0, 0);
		arduinoSerial.send(data);
	}

	std::cout << "Ended execution" << std::endl;
	system("pause");
	return 1;
}