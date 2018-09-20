#include <iostream>
#include <vector>

#include "Serial.h"

using namespace std;

int main()
{
	Serial * serialCom = new Serial("\\\\.\\COM10");
	if (serialCom->isConnected())
	{
		cout << "Sending data to the Arduino" << endl;

		unsigned nbLed = 1;
		unsigned dataSize = nbLed * 3 + 6;
		vector<unsigned char> data(dataSize);

		// Magic word needed for Arduino communication
		data.push_back('A');
		data.push_back('d');
		data.push_back('a');
		data.push_back((nbLed - 1) >> 8);
		data.push_back((nbLed - 1) & 0xff);
		data.push_back(data[3] ^ data[4] ^ 0x55);

		// RGB Led
		data.push_back(255);
		data.push_back(255);
		data.push_back(255);

		// Sending data to the Arduino
		serialCom->writeData((char *)data.data(), data.size());
	}
	else
	{
		cout << "Arduino communication error" << endl;
	}

	system("pause");
	return 1;
}