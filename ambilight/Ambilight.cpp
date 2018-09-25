#include "Ambilight.h"

Ambilight::Ambilight(const std::string & communicationPort, const unsigned & nbLed, const std::vector<Coordinates> coordinates) : arduinoSerial(communicationPort, nbLed), pixelParser(coordinates), isActive(false)
{
}

const void Ambilight::start()
{
	this->isActive = true;
	this->pixelParser.update();
	std::vector<Pixel> previousPixels = this->pixelParser.getPixels();
	std::vector<Pixel> data;
	std::vector<Pixel> currentPixels;
	while (this->isActive)
	{
		this->pixelParser.update();
		currentPixels = this->pixelParser.getPixels();
		data = this->pixelParser.fadePixels(currentPixels, previousPixels);
		previousPixels = data;

		// Sending data to the Arduino
		this->arduinoSerial.send(data);
	}
}

const void Ambilight::stop()
{
	return void();
}
