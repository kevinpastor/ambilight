#include <iostream>
#include <vector>

#include "ArduinoSerial.h"
#include "ScreenCapture.h"
#include "Pixel.h"

int main()
{
	// Arduino communication setup
	char portName[] = "\\\\.\\COM10";
	ArduinoSerial arduinoSerial(portName);

	// RGB Led
	std::vector<Pixel> data;
	data.push_back(Pixel({ 255,255,255 }));

	// Sending data to the Arduino
	arduinoSerial.send(data);

	// Test
	std::vector<Pixel> data2 = ScreenCapture::getScreenData();
	for (size_t i = 0; i < 1; i++)
	{
		std::cout << data2[i].red << std::endl;
	}

	system("pause");
	return 1;
}