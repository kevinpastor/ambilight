#include "Capture.h"

Capture::Capture(const std::shared_ptr<unsigned char[]> & buffer, const unsigned & height, const unsigned & width)
	: buffer(buffer),
	height(height),
	width(width)
{
}

Pixel Capture::getPixel(const Coordinates & coordinates) const
{
	if (!this->isValidPosition(coordinates))
	{
		throw std::invalid_argument("Out of bound coordinates");
	}

	return Pixel({
		this->buffer[coordinates.x * 3 + coordinates.y * 3 * this->width + 2],
		this->buffer[coordinates.x * 3 + coordinates.y * 3 * this->width + 1],
		this->buffer[coordinates.x * 3 + coordinates.y * 3 * this->width]
		});
}

bool Capture::isValidPosition(const Coordinates & coordinates) const
{
	return this->isValidXPosition(coordinates.x)
		&& this->isValidYPosition(coordinates.y);
}

bool Capture::isValidXPosition(const unsigned & x) const
{
	return x < this->width
		&& x >= 0;
}

bool Capture::isValidYPosition(const unsigned & y) const
{
	return y < this->height
		&& y >= 0;
}
