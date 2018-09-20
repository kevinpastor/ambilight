#include <iostream>
#include <vector>

#include "ArduinoSerial.h"
#include "Pixel.h"

int main()
{
	// RGB Led
	std::vector<Pixel> data;

	data.push_back(Pixel({ 3, 4, 255 }));
	data.push_back(Pixel({ 1, 255, 2 }));
	data.push_back(Pixel({ 3, 255, 255 }));
	data.push_back(Pixel({ 255, 0, 2 }));

	// Sending data to the Arduino
	ArduinoSerial arduinoSerial;
	Sleep(1000);
	arduinoSerial.send(data);

	system("pause");
	return 1;
}