#include "PixelParser.h"

PixelParser::PixelParser(std::vector<Coordinates> coordinates) : coordinates(coordinates), surroundingRadius(19)
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
	if (pixels.size() == 0)
	{
		return Pixel();
	}

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
		averageRed = (unsigned char)(totalRed / pixels.size()),
		averageGreen = (unsigned char)(totalGreen / pixels.size()),
		averageBlue = (unsigned char)(totalBlue / pixels.size());

	return Pixel({ averageRed, averageGreen, averageBlue });
}

const std::vector<Pixel> PixelParser::getSurroundingPixels(const Coordinates & coordinates)
{
	std::vector<Pixel> surroundingPixels;
	for (int x = (coordinates.x - (this->surroundingRadius / 2)); x < (int)(coordinates.x + (this->surroundingRadius / 2)); x++)
	{
		if (!this->screenCapturer.isValidXPosition(x))
		{
			continue;
		}
		for (int y = (coordinates.y - (this->surroundingRadius / 2)); y < (int)(coordinates.y + (this->surroundingRadius / 2)); y++)
		{
			if (!this->screenCapturer.isValidYPosition(y))
			{
				continue;
			}
			Coordinates coordinates = { (unsigned)x, (unsigned)y };
			Pixel pixel = this->getPixel(coordinates);
			surroundingPixels.push_back(pixel);
		}
	}

	return surroundingPixels;
}

const Pixel PixelParser::getPixel(const Coordinates coordinates)
{
	return this->screenCapturer.getPixel(coordinates);
}