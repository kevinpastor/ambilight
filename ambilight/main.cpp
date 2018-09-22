#include <iostream>
#include <vector>

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

	// Sending data to the Arduino
	arduinoSerial.send(data);

	// Test
	//std::vector<Pixel> data2 = ScreenCapture::getScreenData();
	//for (size_t i = 0; i < 1; i++)
	//{
	//	std::cout << data2[i].red << std::endl;
	//}

	std::cout << "Ended execution" << std::endl;
	system("pause");
	return 1;
}