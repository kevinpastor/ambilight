#include <iostream>
#include <vector>

#include "ArduinoSerial.h"
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

	system("pause");
	return 1;
}