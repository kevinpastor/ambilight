#include "Pixel.h"

std::vector<Pixel> Pixel::mix(const std::vector<Pixel> & first, const std::vector<Pixel> & second, const double & weight)
{
	assert(first.size() == second.size());

	std::vector<Pixel> mixedPixels(first.size());
	for (unsigned i = 0; i < first.size(); ++i)
	{
		const Pixel mixed = Pixel::mix(first[i], second[i], weight);
		mixedPixels[i] = mixed;
	}

	return mixedPixels;
}

std::vector<Pixel> Pixel::mix(const std::vector<Pixel> & first, const Pixel & second, const double & weight)
{
	std::vector<Pixel> mixedPixels(first.size());
	for (unsigned i = 0; i < first.size(); ++i)
	{
		const Pixel pixel = first[i];
		const Pixel mixed = Pixel::mix(pixel, second, weight);
		mixedPixels[i] = mixed;
	}

	return mixedPixels;
}

Pixel Pixel::mix(const Pixel & first, const Pixel & second, const double & weight)
{
	assert(weight >= 0);
	assert(weight <= 1);

	const unsigned char red = static_cast<unsigned char>(weight * first.red + (1 - weight) * second.red);
	const unsigned char green = static_cast<unsigned char>(weight * first.green + (1 - weight) * second.green);
	const unsigned char blue = static_cast<unsigned char>(weight * first.blue + (1 - weight) * second.blue);

	return Pixel({ red, green, blue });
}

std::vector<Pixel> Pixel::fadeOut(const std::vector<Pixel> & pixels, const double & weight)
{
	const Pixel black({ 0, 0, 0 });
	return Pixel::mix(pixels, black, weight);
}

Pixel Pixel::average(const std::vector<Pixel> & pixels)
{
	assert(pixels.size() != 0);

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
