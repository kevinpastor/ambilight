#include "Ambilight.h"

//TODO Remove
#include <chrono>
#include <iostream>
#include "MonitorUtility.h"

const std::chrono::nanoseconds Ambilight::MAXIMUM_RERFRESH_RATE = std::chrono::nanoseconds(1000000000 / 20);
const std::chrono::milliseconds Ambilight::PAUSED_REFRESH_RATE = std::chrono::milliseconds(500);

Ambilight::Ambilight()
	: isPaused(false),
	pixelParser(this->options.getCoordinates()),
	arduinoSerial(this->options.getPortName()),
	previousPixels(this->options.getCoordinates().size())
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

	//std::chrono::steady_clock::time_point lastUpdate;

	while (true)
	{
		if (SessionUtility::isLocked())
		{
			if (!this->isPaused)
			{
				std::cout << "Locked" << std::endl;
				this->pause();
				this->fadeOut();
			}
		}
		else
		{
			if (this->isPaused)
			{
				std::cout << "Unlocked" << std::endl;
				this->resume();
			}
		}

		if (this->isPaused)
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

		//std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
		//if ((now - lastUpdate) < Ambilight::MAXIMUM_RERFRESH_RATE)
		//{
			//std::this_thread::sleep_for(Ambilight::MAXIMUM_RERFRESH_RATE - (now - lastUpdate));
		//}

		//std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

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

		//std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
		//std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds> (end - begin).count() << "[ms]" << std::endl;
		//std::cout << "Time between frames = " << std::chrono::duration_cast<std::chrono::milliseconds> (end - lastUpdate).count() << "[ms]" << std::endl;
		//lastUpdate = end;
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

std::thread Ambilight::capture()
{
	return std::thread([this]()
		{
			std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
			const Capture capture = this->screenCapture.capture();
			std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
			this->time += std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
			//std::cout << "Capture time = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;
			const std::vector<Pixel> currentPixels = this->pixelParser.getPixels(capture);
			const std::vector<Pixel> data = PixelParser::mix(currentPixels, this->previousPixels, this->options.getSmoothing());

			this->mutex.lock();
			this->previousPixels = data;
			this->mutex.unlock();
		});
}

std::thread Ambilight::send() const
{
	return std::thread([this]()
		{
			this->mutex.lock();
			this->arduinoSerial.send(this->previousPixels);
			this->mutex.unlock();
		});
}

void Ambilight::fadeOut()
{
	const Pixel black({ 0, 0, 0 });
	for (unsigned i = 0; i < 10; ++i)
	{
		const std::vector<Pixel> data = PixelParser::mix(this->previousPixels, black, this->options.getSmoothing());
		this->arduinoSerial.send(data);
		this->previousPixels = data;
	}

	this->previousPixels = std::vector<Pixel>(this->options.getCoordinates().size());
	this->arduinoSerial.send(this->previousPixels);
}
