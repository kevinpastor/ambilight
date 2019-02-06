#include "Ambilight.h"

Ambilight::Ambilight(Options * options, ScreenCapture * screenCapture, PixelParser * pixelParser, ArduinoSerial * arduinoSerial)
	: isPaused(false),
	isStopped(false),
	options(options),
	screenCapture(screenCapture),
	pixelParser(pixelParser),
	arduinoSerial(arduinoSerial),
	previousPixels(this->options->getCoordinates().size()) // this->pixelParser->getPixels())
{ }

Ambilight::~Ambilight()
{
	this->stop();
}

void Ambilight::resume()
{
	this->isPaused = false;
}

void Ambilight::pause()
{
	this->isPaused = true;
}

void Ambilight::stop()
{
	this->pause();
	this->isStopped = true;
}

void Ambilight::exec()
{
	if (this->isPaused || this->isStopped)
	{
		return;
	}

	this->screenCapture->capture();
	std::vector<Pixel> currentPixels = this->pixelParser->getPixels();
	std::vector<Pixel> data = this->pixelParser->fadePixels(currentPixels, previousPixels, this->options->getSmoothing());
	this->previousPixels = data;

	this->arduinoSerial->send(data);
}

void Ambilight::fadeOut()
{
	std::vector<Pixel> currentPixels(this->previousPixels.size()); // , { 0, 0, 0 });
	for (unsigned i = 0; i < 10; ++i)
	{
		std::vector<Pixel> data = this->pixelParser->fadePixels(currentPixels, this->previousPixels, this->options->getSmoothing());
		this->previousPixels = data;
		this->arduinoSerial->send(data);
	}
}