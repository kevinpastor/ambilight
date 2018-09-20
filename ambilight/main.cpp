#include <iostream>
#include <vector>

#include "ArduinoSerial.h"

using namespace std;

int main()
{
	// RGB Led
	vector<unsigned char> data;
	data.push_back(255);
	data.push_back(255);
	data.push_back(255);

	// Sending data to the Arduino
	ArduinoSerial arduinoSerial;
	arduinoSerial.send(data);

	system("pause");
	return 1;
}