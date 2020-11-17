#include "Ambilight.h"

const std::chrono::milliseconds Ambilight::PAUSED_REFRESH_RATE = std::chrono::milliseconds(50);

Ambilight::Ambilight()
	: options(),
	arduinoSerial(this->options.getPortName()),
	colorGrader(this->options.getColorGrader()),
	pixelParser(this->options.getCoordinates(), this->options.getRadius()),
	screenCapture(this->options.getCoordinates(), this->options.getRadius()),
	mutex(),
	isStopped(false),
	isPaused(false)
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

	//std::chrono::steady_clock::time_point lastSent;
	//const std::chrono::nanoseconds maximumFrameTime = std::chrono::nanoseconds(1000000000 / this->options.getMaximumFrameRate());

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

		//std::chrono::steady_clock::time_point before = std::chrono::steady_clock::now();
		captureFuture = std::async(std::launch::async, &Ambilight::capture, this, pixels);
		assert(captureFuture.valid());
		pixels = captureFuture.get();
		//std::cout << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - before).count() << std::endl;

		//std::this_thread::sleep_for(maximumFrameTime);

		if (sendingFuture.valid())
		{
			sendingFuture.get();

			//std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
			//std::cout << "T  " << std::chrono::duration_cast<std::chrono::microseconds>(now - lastSent).count() << std::endl;
			//lastSent = now;
		}

		//lastSent = std::chrono::steady_clock::now();
		sendingFuture = std::async(std::launch::async, &Ambilight::send, this, pixels);
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

std::vector<Pixel> Ambilight::capture(std::vector<Pixel> previousPixels) const
{
	//std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	const Capture capture = this->screenCapture.capture();
	//std::cout << "C1 " << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - now).count() << std::endl;

	const std::vector<Pixel> currentPixels = this->pixelParser.getPixels(capture);
	const std::vector<Pixel> colorCorrectedPixels = this->colorGrader.correct(currentPixels);

	return Pixel::mix(colorCorrectedPixels, previousPixels, this->options.getSmoothing());
	//const std::vector<Pixel> temp = Pixel::mix(colorCorrectedPixels, previousPixels, this->options.getSmoothing());
	//std::cout << "C  " << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - now).count() << std::endl;
	//return temp;
}

void Ambilight::send(std::vector<Pixel> pixels) const
{
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	this->arduinoSerial.send(pixels);
	//std::cout << "S  " << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - now).count() << std::endl;
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
