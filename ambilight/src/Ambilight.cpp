#include "Ambilight.h"

const std::chrono::milliseconds Ambilight::PAUSED_REFRESH_RATE = std::chrono::milliseconds(500);

Ambilight::Ambilight()
	: isStopped(false),
	isPaused(false),
	pixelParser(this->options.getCoordinates()),
	arduinoSerial(this->options.getPortName()),
	pixels(this->options.getCoordinates().size())
{
}

Ambilight::~Ambilight()
{
	this->pause();
}

void Ambilight::start()
{
	std::thread captureThread;
	std::thread sendingThread;

	while (!this->isStopped)
	{
		if (SessionUtility::isLocked())
		{
			if (!this->isPaused)
			{
				this->pause();
				this->fadeOut();
			}
		}
		else
		{
			if (this->isPaused)
			{
				this->resume();
			}
		}

		if (this->isPaused || this->isUserPaused)
		{
			if (captureThread.joinable())
			{
				captureThread.join();
			}
			if (sendingThread.joinable())
			{
				sendingThread.join();
			}

			std::this_thread::sleep_for(Ambilight::PAUSED_REFRESH_RATE);

			continue;
		}

		if (captureThread.joinable())
		{
			captureThread.join();
		}
		captureThread = this->capture();

		if (sendingThread.joinable())
		{
			sendingThread.join();
		}
		sendingThread = this->send();
	}

	if (captureThread.joinable())
	{
		captureThread.join();
	}

	if (sendingThread.joinable())
	{
		sendingThread.join();
	}
}

void Ambilight::userPause()
{
	this->isUserPaused = true;
}

void Ambilight::userResume()
{
	this->isUserPaused = false;
}

void Ambilight::stop()
{
	this->isStopped = true;
}

void Ambilight::resume()
{
	this->isPaused = false;
}

void Ambilight::pause()
{
	this->isPaused = true;
}

std::thread Ambilight::capture()
{
	return std::thread([this]()
		{
			const Capture capture = this->screenCapture.capture();
			const unsigned monitorBrightness = MonitorUtility::getBrightness();
			const std::vector<Pixel> currentPixels = this->pixelParser.getPixels(capture, this->options.getColorGrader());
			const std::vector<Pixel> data = PixelParser::mix(currentPixels, this->pixels, this->options.getSmoothing());

			this->mutex.lock();
			this->pixels = data;
			this->mutex.unlock();
		});
}

std::thread Ambilight::send() const
{
	return std::thread([this]()
		{
			this->mutex.lock();
			this->arduinoSerial.send(this->pixels);
			this->mutex.unlock();
		});
}

void Ambilight::fadeOut()
{
	const Pixel black({ 0, 0, 0 });
	for (unsigned i = 0; i < 10; ++i)
	{
		this->pixels = PixelParser::mix(this->pixels, black, this->options.getSmoothing());
		this->arduinoSerial.send(this->pixels);
	}

	this->pixels = std::vector<Pixel>(this->options.getCoordinates().size());
	this->arduinoSerial.send(this->pixels);
}
