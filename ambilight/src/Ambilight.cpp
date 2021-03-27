#include "Ambilight.h"

const std::chrono::milliseconds Ambilight::PAUSED_REFRESH_RATE = std::chrono::milliseconds(50);

Ambilight::Ambilight()
	: options(),
	arduinoSerial(this->options.getPortName(), this->options.getBaudRate()),
	colorGrader(this->options.getColorGrader()),
	pixelParser(this->options.getCoordinates(), this->options.getRadius()),
	screenCapture(this->options.getCoordinates(), this->options.getRadius()),
	mutex(),
	isStopped(false),
	isPaused(false),
	isHighFidelityModeOn(false)
{
}

Ambilight::~Ambilight()
{
	this->stop();
}

void Ambilight::start()
{
	//AllocConsole();
	//FILE * fDummy;
	//freopen_s(&fDummy, "CONOUT$", "w", stdout);

	std::vector<Pixel> pixels(this->options.getCoordinates().size());

	std::future<std::vector<Pixel>> captureFuture;
	std::future<void> sendingFuture;

	std::chrono::steady_clock::time_point lastSent = std::chrono::steady_clock::now();

	while (!this->isStopped)
	{
		if (this->isPaused)
		{
			if (captureFuture.valid())
			{
				captureFuture.get();
			}
			if (sendingFuture.valid())
			{
				sendingFuture.get();

				this->fadeOut(pixels);
			}

			std::this_thread::sleep_for(Ambilight::PAUSED_REFRESH_RATE);

			continue;
		}

		captureFuture = std::async(std::launch::async, &Ambilight::capture, this, pixels);
		assert(captureFuture.valid());
		pixels = captureFuture.get();

		if (sendingFuture.valid())
		{
			sendingFuture.get();
		}

		if (!this->isHighFidelityModeOn)
		{
			const std::chrono::steady_clock::duration frameTime = std::chrono::steady_clock::now() - lastSent;
			if (frameTime < this->options.getTargetFrameRenderTime())
			{
				std::this_thread::sleep_for(this->options.getTargetFrameRenderTime() - frameTime);
			}
		}

		sendingFuture = std::async(std::launch::async, &Ambilight::send, this, pixels);
		lastSent = std::chrono::steady_clock::now();
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

void Ambilight::toggleHighFidelityMode()
{
	this->isHighFidelityModeOn = !this->isHighFidelityModeOn;
}

std::vector<Pixel> Ambilight::capture(std::vector<Pixel> previousPixels) const
{
	const Capture capture = this->screenCapture.capture();

	const std::vector<Pixel> currentPixels = this->pixelParser.getPixels(capture);
	const std::vector<Pixel> colorCorrectedPixels = this->colorGrader.correct(currentPixels);

	// TODO Take into consideration render time for mixing weight
	return Pixel::mix(colorCorrectedPixels, previousPixels, this->options.getSmoothing());
}

void Ambilight::send(std::vector<Pixel> pixels) const
{
	this->arduinoSerial.send(pixels);
}

void Ambilight::fadeOut(const std::vector<Pixel> & pixels) const
{
	std::vector<Pixel> fadedPixels = pixels;
	const unsigned iterations = static_cast<unsigned>(-std::log(255) / std::log(this->options.getSmoothing()) + 1);
	for (unsigned i = 0; i < iterations; ++i)
	{
		fadedPixels = Pixel::fadeOut(fadedPixels, this->options.getSmoothing());
		this->arduinoSerial.send(fadedPixels);
	}
}
