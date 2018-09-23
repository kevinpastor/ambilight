#include "PixelParser.h"

PixelParser::PixelParser(std::vector<Coordinates> coordinates) : coordinates(coordinates)
{
}

void PixelParser::update()
{
	this->screenCapturer.capture();
}

const std::vector<Pixel> PixelParser::getPixels()
{
	std::vector<Pixel> averagePixels;
	for (auto &coordinate : this->coordinates)
	{
		std::vector<Pixel> surroundingPixels = this->getSurroundingPixels(coordinate);
		Pixel averagePixel = this->averagePixel(surroundingPixels);
		averagePixels.push_back(averagePixel);
	}

	return averagePixels;
}

const Pixel PixelParser::averagePixel(const std::vector<Pixel> & pixels)
{
	unsigned
		totalRed = 0,
		totalGreen = 0,
		totalBlue = 0;

	for (auto & pixel : pixels)
	{
		totalRed += pixel.red;
		totalGreen += pixel.green;
		totalBlue += pixel.blue;
	}

	unsigned char
		averageRed = totalRed / pixels.size(),
		averageGreen = totalGreen / pixels.size(),
		averageBlue = totalBlue / pixels.size();

	return Pixel({ averageRed, averageGreen, averageBlue });
}

const std::vector<Pixel> PixelParser::getSurroundingPixels(const Coordinates & coordinates)
{
	std::vector<Pixel> surroundingPixels;
	const int surroundingRadius = 40;
	for (int i = -surroundingRadius / 2; i < surroundingRadius / 2; i++)
	{
		int x = coordinates.x + i;
		if (x >= 0 && x < this->screenCapturer.getScreenWidth())
		{
			for (int j = -surroundingRadius / 2; j < surroundingRadius / 2; j++)
			{
				int y = coordinates.y + j;
				if (y >= 0 && y < this->screenCapturer.getScreenHeight())
				{
					Coordinates coordinates = { x, y };
					Pixel pixel = this->getPixel(coordinates);
					surroundingPixels.push_back(pixel);
				}
			}
		}
	}

	return surroundingPixels;
}

const Pixel PixelParser::getPixel(const Coordinates coordinates)
{
	return this->screenCapturer.getPixel(coordinates);
}