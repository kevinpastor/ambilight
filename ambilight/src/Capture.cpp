#include "Capture.h"

Capture::Capture(const std::shared_ptr<std::vector<unsigned char>> & buffer, const unsigned & width, const unsigned & height)
	: buffer(buffer),
	width(width),
	height(height)
{
}

Pixel Capture::getPixel(const Coordinates & coordinates) const
{
	if (!this->isValidPosition(coordinates))
	{
		throw std::invalid_argument("Out of bound coordinates");
	}

	const unsigned long long index = 3ull * coordinates.x + 3ull * coordinates.y * this->width;

	return Pixel({
		this->buffer.get()->operator[](index + 2),
		this->buffer.get()->operator[](index + 1),
		this->buffer.get()->operator[](index)
		});
}

bool Capture::isValidPosition(const Coordinates & coordinates) const
{
	return this->isValidXPosition(coordinates.x)
		&& this->isValidYPosition(coordinates.y);
}

bool Capture::isValidXPosition(const unsigned & x) const
{
	return x >= 0
		&& x < this->width;
}

bool Capture::isValidYPosition(const unsigned & y) const
{
	return y >= 0
		&& y < this->height;
}
