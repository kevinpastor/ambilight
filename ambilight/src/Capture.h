#pragma once

#include <cassert>
#include <memory>
#include <stdexcept>
#include <vector>

#include "Coordinates.h"
#include "Pixel.h"

class Capture
{
public:
	Capture(const unsigned char * buffer, const unsigned & width, const unsigned & height);

	Pixel getPixel(const Coordinates & coordinates) const;
	bool isValidXPosition(const unsigned & x) const;
	bool isValidYPosition(const unsigned & y) const;

private:
	bool isValidPosition(const Coordinates & coordinates) const;

	const unsigned char * buffer;
	const unsigned width;
	const unsigned height;

};
