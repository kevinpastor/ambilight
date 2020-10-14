#pragma once

#include <vector>

#include "Pixel.h"

class ColorGrader
{
public:
	ColorGrader(const unsigned & monitorBrightness);

	Pixel correct(const Pixel & pixel) const;
	std::vector<Pixel> correct(const std::vector<Pixel> & pixels) const;

private:
	const unsigned LUTIndex;

	static const Pixel LUTs[2][256];
	static const unsigned DIMMED_BRIGHTNESS_LIMIT = 5;

};
