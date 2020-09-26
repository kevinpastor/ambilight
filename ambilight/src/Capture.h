#pragma once

#include <memory>
#include <stdexcept>

#include "Coordinates.h"
#include "Pixel.h"

class Capture
{
public:
	Capture(const std::shared_ptr<unsigned char []> & buffer, const unsigned & height, const unsigned & width);

	Pixel getPixel(const Coordinates & coordinates) const;
	bool isValidXPosition(const unsigned & x) const;
	bool isValidYPosition(const unsigned & y) const;

private:
	bool isValidPosition(const Coordinates & coordinates) const;

	const std::shared_ptr<unsigned char[]> buffer;
	const unsigned height;
	const unsigned width;

};