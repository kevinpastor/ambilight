#pragma once

#include "LUT.h"
#include "MonitorUtility.h"
#include "Pixel.h"

class RGBLUT
{
public:
	RGBLUT(const LUT & rLUT, const LUT & gLUT, const LUT & bLUT, const unsigned & brightnessThreshold);
	RGBLUT(const LUT & rLUT, const LUT & gLUT, const LUT & bLUT);

	Pixel get(const Pixel & pixel) const;

	bool isUsable() const;

private:
	const LUT rLUT;
	const LUT gLUT;
	const LUT bLUT;

	const unsigned brightnessThreshold = 100;

};