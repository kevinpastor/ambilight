#include <iostream>
#include <vector>

#include "ArduinoSerial.h"
#include "Pixel.h"

int main()
{
	// RGB Led
	std::vector<Pixel> data;

	data.push_back(Pixel({ 255,255,255 }));

	// Sending data to the Arduino
	char portName[] = "\\\\.\\COM10";
	ArduinoSerial arduinoSerial(portName);
	arduinoSerial.send(data);

	system("pause");
	return 1;
}