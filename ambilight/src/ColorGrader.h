#pragma once

#include <vector>

#include "MonitorUtility.h"
#include "Pixel.h"
#include "RGBLUT.h"

class ColorGrader
{
public:
	ColorGrader(const std::vector<RGBLUT> & luts);

	std::vector<Pixel> correct(const std::vector<Pixel> & pixels) const;
	Pixel correct(const Pixel & pixel) const;

private:
	const std::vector<RGBLUT> luts;

};