#pragma once

#include <vector>

#include "MonitorUtility.h"
#include "Pixel.h"
#include "RGBLUT.h"

class ColorGrader
{
public:
	ColorGrader(const RGBLUT & lut, const RGBLUT & dimmedLut);

	Pixel correct(const Pixel & pixel) const;
	std::vector<Pixel> correct(const std::vector<Pixel> & pixels) const;

private:
	const RGBLUT lut;
	const RGBLUT dimmedLut;

	static const unsigned DIMMED_BRIGHTNESS_LIMIT = 5;

};