#include "Ambilight.h"

Ambilight::Ambilight(const Options & options)
	: isPaused(true),
	isStopped(false),
	thread(&Ambilight::exec, this, options)
{ }

Ambilight::~Ambilight()
{
	this->stop();
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

	this->thread.join();
}

bool Ambilight::isStoppeds() const
{
	return this->isStopped;
}

void Ambilight::exec(const Options & options) const
{
	ScreenCapture screenCapture = ScreenCapture();
	PixelParser pixelParser = PixelParser(&screenCapture, options.getCoordinates());
	ArduinoSerial arduinoSerial = ArduinoSerial(options.getPortName(), (unsigned)options.getCoordinates().size());

	std::vector<Pixel> previousPixels = pixelParser.getPixels();
	std::vector<Pixel> data;
	std::vector<Pixel> currentPixels;

	bool hasFaded = false;
	while (!this->isStopped)
	{
		while (!this->isPaused)
		{
			// Sending data to the Arduino
			screenCapture.capture();
			currentPixels = pixelParser.getPixels();
			data = pixelParser.fadePixels(currentPixels, previousPixels, options.getSmoothing());
			previousPixels = data;

			// Sending data to the Arduino
			arduinoSerial.send(data);
			hasFaded = false;
		}

		if (!hasFaded)
		{

			std::fill(currentPixels.begin(), currentPixels.end(), Pixel({ 0, 0, 0 }));
			for (unsigned i = 0; i < 10; ++i)
			{
				data = pixelParser.fadePixels(currentPixels, previousPixels, options.getSmoothing());
				previousPixels = data;

				// Sending data to the Arduino
				arduinoSerial.send(data);
			}
			hasFaded = true;
		}
	}

}
