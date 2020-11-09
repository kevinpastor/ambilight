#include "Ambilight.h"

const std::chrono::milliseconds Ambilight::PAUSED_REFRESH_RATE = std::chrono::milliseconds(500);

Ambilight::Ambilight()
	: options(),
	arduinoSerial(this->options.getPortName()),
	colorGrader(this->options.getColorGrader()),
	pixelParser(this->options.getCoordinates(), this->options.getRadius()),
	screenCapture(),
	mutex(),
	isStopped(false),
	isPaused(false),
	pixels(this->options.getCoordinates().size())
{
}

Ambilight::~Ambilight()
{
	this->stop();
}

void Ambilight::start()
{
	std::future<void> captureFuture;
	std::future<void> sendingFuture;

	while (!this->isStopped)
	{
		if (SessionUtility::isLocked() || this->isPaused)
		{
			if (captureFuture.valid())
			{
				captureFuture.get();
			}
			if (sendingFuture.valid())
			{
				sendingFuture.get();
			}

			this->fadeOut();

			std::this_thread::sleep_for(Ambilight::PAUSED_REFRESH_RATE);

			continue;
		}

		if (captureFuture.valid())
		{
			captureFuture.get();
		}
		captureFuture = std::async(std::launch::async, &Ambilight::capture, this);

		if (sendingFuture.valid())
		{
			sendingFuture.get();
		}
		sendingFuture = std::async(std::launch::async, &Ambilight::send, this);
	}

	if (captureFuture.valid())
	{
		captureFuture.get();
	}

	if (sendingFuture.valid())
	{
		sendingFuture.get();
	}
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
	this->isStopped = true;
}

void Ambilight::capture()
{
	const Capture capture = this->screenCapture.capture();
	
	const std::vector<Pixel> currentPixels = this->pixelParser.getPixels(capture);
	const std::vector<Pixel> colorCorrectedPixels = this->colorGrader.correct(currentPixels);
	const std::vector<Pixel> data = Pixel::mix(colorCorrectedPixels, this->pixels, this->options.getSmoothing());

	this->mutex.lock();
	this->pixels = data;
	this->mutex.unlock();
}

void Ambilight::send() const
{
	this->mutex.lock();
	this->arduinoSerial.send(this->pixels);
	this->mutex.unlock();
}

void Ambilight::fadeOut()
{
	const Pixel black({ 0, 0, 0 });
	for (unsigned i = 0; i < 20; ++i)
	{
		this->pixels = Pixel::mix(this->pixels, black, 0.70);
		this->arduinoSerial.send(this->pixels);
	}

	this->pixels = std::vector<Pixel>(this->options.getCoordinates().size());
	this->arduinoSerial.send(this->pixels);
}
