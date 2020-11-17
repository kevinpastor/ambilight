#include "Capture.h"

Capture::Capture(const unsigned char * buffer, const unsigned & width, const unsigned & height)
	: buffer(buffer),
	width(width),
	height(height)
{
}

Pixel Capture::getPixel(const Coordinates & coordinates) const
{
	assert(this->isValidPosition(coordinates));

	const unsigned long long index = 3ull * (coordinates.x + coordinates.y * this->width);

	return Pixel({
		this->buffer[index + 2],
		this->buffer[index + 1],
		this->buffer[index]
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
