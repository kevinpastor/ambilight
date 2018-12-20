#include "Ambilight.h"

Ambilight::Ambilight(const std::string & communicationPort, const std::vector<Coordinates> & coordinates)
	: isPaused(true),
	isStopped(false),
	thread(&Ambilight::exec, this, communicationPort, coordinates)
{ }

Ambilight::~Ambilight()
{
	this->stop();
	thread.join();
}

void Ambilight::start()
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

void Ambilight::exec(const std::string & communicationPort, const std::vector<Coordinates> & coordinates) const
{
	ScreenCapture screenCapture = ScreenCapture();
	PixelParser pixelParser = PixelParser(&screenCapture, coordinates);
	ArduinoSerial arduinoSerial = ArduinoSerial(communicationPort, (unsigned)coordinates.size());

	std::vector<Pixel> previousPixels = pixelParser.getPixels();
	std::vector<Pixel> data;
	std::vector<Pixel> currentPixels;

	unsigned smoothing = 5;

	while (!this->isStopped)
	{
		while (!this->isPaused)
		{
			// Sending data to the Arduino
			screenCapture.capture();
			currentPixels = pixelParser.getPixels();
			data = pixelParser.fadePixels(currentPixels, previousPixels, smoothing);
			previousPixels = data;

			// Sending data to the Arduino
			arduinoSerial.send(data);
		}
	}

	std::fill(currentPixels.begin(), currentPixels.end(), Pixel({ 0, 0, 0 }));
	for (unsigned i = 0; i < 10; ++i)
	{
		data = pixelParser.fadePixels(currentPixels, previousPixels, smoothing);
		previousPixels = data;

		// Sending data to the Arduino
		arduinoSerial.send(data);
	}
}
