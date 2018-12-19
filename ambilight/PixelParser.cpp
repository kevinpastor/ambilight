#include "PixelParser.h"

PixelParser::PixelParser(ScreenCapture * screencapture, const std::vector<Coordinates> & coordinates)
	: coordinates(coordinates),
	surroundingRadius(19)
{
	this->screenCapture = screencapture;
}

std::vector<Pixel> PixelParser::getPixels() const
{
	std::vector<Pixel> averagePixels;
	for (auto const & coordinate : this->coordinates)
	{
		std::vector<Pixel> surroundingPixels = this->getSurroundingPixels(coordinate);
		Pixel averagePixel = this->averagePixel(surroundingPixels);
		averagePixels.push_back(averagePixel);
	}

	return averagePixels;
}

std::vector<Pixel> PixelParser::fadePixels(const std::vector<Pixel> & currentPixels, const std::vector<Pixel> & previousPixels, const unsigned & smoothing) const
{
	if (currentPixels.size() != previousPixels.size())
	{
		throw std::invalid_argument("Vector of different size");
	}

	std::vector<Pixel> fadedPixel;

	for (unsigned i = 0; i < currentPixels.size(); i++)
	{
		fadedPixel.push_back(Pixel({
			(unsigned char)((previousPixels[i].red * (smoothing - 1) + currentPixels[i].red) / smoothing),
			(unsigned char)((previousPixels[i].green * (smoothing - 1) + currentPixels[i].green) / smoothing),
			(unsigned char)((previousPixels[i].blue * (smoothing - 1) + currentPixels[i].blue) / smoothing)
			}));
	}

	return fadedPixel;
}

Pixel PixelParser::averagePixel(const std::vector<Pixel> & pixels) const
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

std::vector<Pixel> PixelParser::getSurroundingPixels(const Coordinates & coordinates) const
{
	std::vector<Pixel> surroundingPixels;
	for (int x = (coordinates.x - (this->surroundingRadius / 2)); x < (int)(coordinates.x + (this->surroundingRadius / 2)); x++)
	{
		if (!this->screenCapture->isValidXPosition(x))
		{
			continue;
		}

		for (int y = (coordinates.y - (this->surroundingRadius / 2)); y < (int)(coordinates.y + (this->surroundingRadius / 2)); y++)
		{
			if (!this->screenCapture->isValidYPosition(y))
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

Pixel PixelParser::getPixel(const Coordinates & coordinates) const
{
	return this->screenCapture->getPixel(coordinates);
}