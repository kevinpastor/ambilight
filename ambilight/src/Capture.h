#pragma once

#include <memory>
#include <stdexcept>
#include <vector>

#include "Coordinates.h"
#include "Pixel.h"

class Capture
{
public:
	Capture(const std::shared_ptr<std::vector<unsigned char>> & buffer, const unsigned & width, const unsigned & height);

	Pixel getPixel(const Coordinates & coordinates) const;
	bool isValidXPosition(const unsigned & x) const;
	bool isValidYPosition(const unsigned & y) const;

private:
	bool isValidPosition(const Coordinates & coordinates) const;

	const std::shared_ptr<std::vector<unsigned char>> buffer;
	const unsigned width;
	const unsigned height;

};
