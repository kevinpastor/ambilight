#include "Pixel.h"

std::vector<Pixel> Pixel::mix(const std::vector<Pixel> & first, const std::vector<Pixel> & second, const double & weight)
{
	if (first.size() != second.size())
	{
		throw std::invalid_argument("Vector of different size");
	}

	std::vector<Pixel> fadedPixels(first.size());
	for (unsigned i = 0; i < first.size(); ++i)
	{
		const Pixel mixed = Pixel::mix(first[i], second[i], weight);
		fadedPixels[i] = mixed;
	}

	return fadedPixels;
}

std::vector<Pixel> Pixel::mix(const std::vector<Pixel> & first, const Pixel & second, const double & weight)
{
	std::vector<Pixel> fadedPixels(first.size());
	for (unsigned i = 0; i < first.size(); ++i)
	{
		const Pixel pixel = first[i];
		const Pixel mixed = Pixel::mix(pixel, second, weight);
		fadedPixels[i] = mixed;
	}

	return fadedPixels;
}

Pixel Pixel::mix(const Pixel & first, const Pixel & second, const double & weight)
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

Pixel Pixel::average(const std::vector<Pixel> & pixels)
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
