#include <iostream>

#include "Serial.h"

using namespace std;

int main()
{
	cout << "Hello World" << endl;

	Serial * serialCom = new Serial("\\\\.\\COM10");
	if (serialCom->isConnected())
	{
		unsigned nbLed = 1;
		unsigned char * data = new unsigned char[nbLed * 3 + 6]();
		data[0] = 'A';
		data[1] = 'd';
		data[2] = 'a';
		data[3] = (nbLed - 1) >> 8;
		data[4] = (nbLed - 1) & 0xff;
		data[5] = data[3] ^ data[4] ^ 0x55;
		data[6] = 255;
		data[7] = 255;
		data[8] = 255;
		serialCom->writeData((char*)data, nbLed * 3 + 6);
	}

	system("pause");
	return 1;
}