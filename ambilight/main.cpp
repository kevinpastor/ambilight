#include <iostream>
#include <vector>
#include <ctime>

#include "PixelParser.h"
#include "ArduinoSerial.h"
#include "Pixel.h"
#include "Coordinates.h"
#include "Ambilight.h"

int main()
{
	const std::vector<Coordinates> coordinates = {
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
		{ 1919, 1073 }, // right
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
		{ 1898, 0 }, // top row
		{ 1834, 0 },
		{ 1775, 0 },
		{ 1712, 0 },
		{ 1650, 0 },
		{ 1590, 0 },
		{ 1525, 0 },
		{ 1467, 0 },
		{ 1400, 0 },
		{ 1339, 0 },
		{ 1272, 0 },
		{ 1214, 0 },
		{ 1149, 0 },
		{ 1086, 0 },
		{ 1023, 0 },
		{ 967, 0 },
		{ 903, 0 },
		{ 839, 0 },
		{ 774, 0 },
		{ 715, 0 },
		{ 653, 0 },
		{ 589, 0 },
		{ 530, 0 },
		{ 467, 0 },
		{ 404, 0 },
		{ 340, 0 },
		{ 281, 0 },
		{ 215, 0 },
		{ 155, 0 },
		{ 93, 0 },
		{ 33, 0 },
		{ 0, 0 }, // top-left corner
	};

	unsigned nbLed = coordinates.size();
	std::string portName = "\\\\.\\COM10";

	Ambilight ambilight(portName, nbLed, coordinates);

	std::cout << "Started!" << std::endl;
	//clock_t tStart = clock();
	ambilight.start();

	system("pause");
	ambilight.pause();

	//printf("Time taken: %.2fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);
	return 1;
}
