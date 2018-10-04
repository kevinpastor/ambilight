#include "Ambilight.h"

Ambilight::Ambilight(const std::string & communicationPort, const unsigned & nbLed, const std::vector<Coordinates> coordinates)
	: arduinoSerial(communicationPort, nbLed),
	pixelParser(coordinates),
	isPaused(true),
	isStopped(false),
	thread([this] { this->exec(); })
{
}

Ambilight::~Ambilight()
{
	this->stop();
	thread.join();
}

const void Ambilight::start()
{
	this->isPaused = false;
}

const void Ambilight::pause()
{
	this->isPaused = true;
}

const void Ambilight::stop()
{
	this->pause();
	this->isStopped = true;
}

const void Ambilight::exec()
{
	this->pixelParser.update();
	std::vector<Pixel> previousPixels = this->pixelParser.getPixels();
	std::vector<Pixel> data;
	std::vector<Pixel> currentPixels;

	while (!this->isStopped)
	{
		while (!this->isPaused)
		{
			this->pixelParser.update();
			currentPixels = this->pixelParser.getPixels();
			data = this->pixelParser.fadePixels(currentPixels, previousPixels);
			previousPixels = data;

			// Sending data to the Arduino
			this->arduinoSerial.send(data);
		}
	}
}
