#include "PixelParser.h"

PixelParser::PixelParser(const std::vector<Coordinates> & coordinates, const unsigned & radius)
	: coordinates(coordinates),
	radius(radius)
{
}

std::vector<Pixel> PixelParser::getPixels(const Capture & capture) const
{
	std::vector<Pixel> averages(this->coordinates.size());
	for (unsigned i = 0; i < this->coordinates.size(); ++i)
	{
		const Coordinates region = this->coordinates[i];
		const std::vector<Pixel> surrounding = PixelParser::getSurrounding(capture, region, this->radius);
		const Pixel average = Pixel::average(surrounding);
		averages[i] = average;
	}

	return averages;
}

std::vector<Pixel> PixelParser::getSurrounding(const Capture & capture, const Coordinates & region, const unsigned & radius)
{
	if (region.x == -1 || region.y == -1) {
		return std::vector<Pixel>(1, Pixel({ 0, 0, 0 }));
	}

	if (!capture.isValidPosition(region))
	{
		return std::vector<Pixel>(1, Pixel({ 0, 0, 0 }));
	}

	unsigned minX = region.x;
	if (capture.isValidXPosition(region.x - radius))
	{
		minX = region.x - radius;
	}

	unsigned maxX = region.x;
	if (capture.isValidXPosition(region.x + radius))
	{
		maxX = region.x + radius;
	}

	std::vector<Pixel> pixels;
	pixels.reserve(4ull * radius * radius);
	for (unsigned x = minX; x < maxX; ++x)
	{
		assert(capture.isValidXPosition(x));

		unsigned minY = region.y;
		if (capture.isValidYPosition(region.y - radius))
		{
			minY = region.y - radius;
		}

		unsigned maxY = region.y;
		if (capture.isValidYPosition(region.y + radius))
		{
			maxY = region.y + radius;
		}

		for (unsigned y = minY; y < maxY; ++y)
		{
			assert(capture.isValidYPosition(y));

			const Coordinates coordinates({ static_cast<int>(x), static_cast<int>(y) });
			const Pixel pixel = capture.getPixel(coordinates);
			pixels.push_back(pixel);
		}
	}

	return pixels;
}
