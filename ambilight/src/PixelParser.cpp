#include "PixelParser.h"

PixelParser::PixelParser(const std::vector<Coordinates> & coordinates)
	: coordinates(coordinates)
{
}

std::vector<Pixel> PixelParser::getPixels(const Capture & capture, const ColorGrader & colorGrader) const
{
	std::vector<Pixel> averages;
	for (const Coordinates & region : this->coordinates)
	{
		const std::vector<Pixel> surrounding = PixelParser::getSurrounding(capture, region);
		const Pixel average = this->average(surrounding);
		averages.push_back(average);
	}

	const unsigned monitorBrightness = MonitorUtility::getBrightness();
	//const ColorGrader colorGrader(monitorBrightness);
	const std::vector<Pixel> corrected = colorGrader.correct(averages);

	return corrected;
}

std::vector<Pixel> PixelParser::mix(const std::vector<Pixel> & first, const std::vector<Pixel> & second, const double & weight)
{
	if (first.size() != second.size())
	{
		throw std::invalid_argument("Vector of different size");
	}

	std::vector<Pixel> fadedPixels;
	for (unsigned i = 0; i < first.size(); ++i)
	{
		Pixel mixed = PixelParser::mix(first[i], second[i], weight);
		fadedPixels.push_back(mixed);
	}

	return fadedPixels;
}

std::vector<Pixel> PixelParser::mix(const std::vector<Pixel> & first, const Pixel & second, const double & weight)
{
	std::vector<Pixel> fadedPixels;
	for (const Pixel & pixel : first)
	{
		Pixel mixed = PixelParser::mix(pixel, second, weight);
		fadedPixels.push_back(mixed);
	}

	return fadedPixels;
}

Pixel PixelParser::mix(const Pixel & first, const Pixel & second, const double & weight)
{
	if (weight < 0)
	{
		throw std::runtime_error("Weight cannot be smaller than 0");
	}
	else if (weight > 1)
	{
		throw std::runtime_error("Weight cannot be larger than 1");
	}

	const unsigned char red = static_cast<unsigned char>(weight * first.red + (1 - weight) * second.red);
	const unsigned char green = static_cast<unsigned char>(weight * first.green + (1 - weight) * second.green);
	const unsigned char blue = static_cast<unsigned char>(weight * first.blue + (1 - weight) * second.blue);

	return Pixel({ red, green, blue });
}

Pixel PixelParser::average(const std::vector<Pixel> & pixels)
{
	if (pixels.size() == 0)
	{
		throw std::runtime_error("Pixels cannot have a size of 0");
	}

	unsigned totalRed = 0;
	unsigned totalGreen = 0;
	unsigned totalBlue = 0;

	for (const Pixel & pixel : pixels)
	{
		totalRed += pixel.red;
		totalGreen += pixel.green;
		totalBlue += pixel.blue;
	}

	const unsigned char red = static_cast<unsigned char>(totalRed / pixels.size());
	const unsigned char green = static_cast<unsigned char>(totalGreen / pixels.size());
	const unsigned char blue = static_cast<unsigned char>(totalBlue / pixels.size());

	return Pixel({ red, green, blue });
}

std::vector<Pixel> PixelParser::getSurrounding(const Capture & capture, const Coordinates & region)
{
	if (region.x == -1 || region.y == -1) {
		return std::vector<Pixel>(1, Pixel({ 0, 0, 0 }));
	}

	std::vector<Pixel> pixels;
	for (int x = (region.x - (PixelParser::surroundingRadius / 2)); x < static_cast<int>(region.x + (PixelParser::surroundingRadius / 2)); ++x)
	{
		if (!capture.isValidXPosition(x))
		{
			continue;
		}

		for (int y = (region.y - (PixelParser::surroundingRadius / 2)); y < static_cast<int>(region.y + (PixelParser::surroundingRadius / 2)); ++y)
		{
			if (!capture.isValidYPosition(y))
			{
				continue;
			}

			const Coordinates coordinates = { x, y };
			const Pixel pixel = capture.getPixel(coordinates);
			pixels.push_back(pixel);
		}
	}

	return pixels;
}
