#include "PixelParser.h"

PixelParser::PixelParser(const std::vector<Coordinates> & coordinates)
	: coordinates(coordinates),
	surroundingRadius(19)
{
}

std::vector<Pixel> PixelParser::getPixels(const Capture & capture, const bool & isMonitorDimmed) const
{
	std::vector<Pixel> averagePixels;
	for (auto const & coordinate : this->coordinates)
	{
		std::vector<Pixel> surroundingPixels = this->getSurroundingPixels(capture, coordinate);
		Pixel averagePixel = this->averagePixel(surroundingPixels, isMonitorDimmed);
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

Pixel PixelParser::averagePixel(const std::vector<Pixel> & pixels, const bool & isMonitorDimmed) const
{
	if (pixels.size() == 0)
	{
		return Pixel();
	}

	unsigned totalRed = 0;
	unsigned totalGreen = 0;
	unsigned totalBlue = 0;

	for (auto & pixel : pixels)
	{
		totalRed += pixel.red;
		totalGreen += pixel.green;
		totalBlue += pixel.blue;
	}

	unsigned char averageRed = (unsigned char)(totalRed / pixels.size());
	unsigned char averageGreen = (unsigned char)(totalGreen / pixels.size());
	unsigned char averageBlue = (unsigned char)(totalBlue / pixels.size());

	return this->colorCorrect(Pixel({ averageRed, averageGreen, averageBlue }), isMonitorDimmed);
}

std::vector<Pixel> PixelParser::getSurroundingPixels(const Capture & capture, const Coordinates & coordinates) const
{
	std::vector<Pixel> surroundingPixels;
	for (int x = (coordinates.x - (this->surroundingRadius / 2)); x < (int)(coordinates.x + (this->surroundingRadius / 2)); x++)
	{
		if (!capture.isValidXPosition(x))
		{
			continue;
		}

		for (int y = (coordinates.y - (this->surroundingRadius / 2)); y < (int)(coordinates.y + (this->surroundingRadius / 2)); y++)
		{
			if (!capture.isValidYPosition(y))
			{
				continue;
			}

			Coordinates coordinates = { (unsigned)x, (unsigned)y };
			Pixel pixel = capture.getPixel(coordinates);
			surroundingPixels.push_back(pixel);
		}
	}

	return surroundingPixels;
}

Pixel PixelParser::colorCorrect(const Pixel & pixel, const bool & isMonitorDimmed) const
{
	unsigned char red = pixel.red;
	unsigned char green = pixel.green;
	unsigned char blue = pixel.blue;

	if (pixel.red > 200 && pixel.green < 50 && pixel.blue < 50)
	{
		red = (unsigned char)(0.9 * red);
	}
	else if (pixel.red > 150)
	{
		red = (unsigned char)(0.8 * red);
	}
	else
	{
		red = (unsigned char)(0.65 * red);
	}

	if (pixel.blue > 200 && pixel.red < 50 && pixel.green < 50)
	{
		blue = (unsigned char)(0.65 * blue);
	}
	else if (pixel.blue > 150)
	{
		blue = (unsigned char)(0.55 * blue);
	}
	else
	{
		blue = (unsigned char)(0.51 * blue);
	}

	return Pixel({ red, green, blue });
}