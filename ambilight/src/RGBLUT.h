#pragma once

#include "LUT.h"
#include "Pixel.h"

class RGBLUT
{
public:
	RGBLUT(const LUT & rLUT, const LUT & gLUT, const LUT & bLUT);

	Pixel get(const Pixel & pixel) const;

private:
	const LUT rLUT;
	const LUT gLUT;
	const LUT bLUT;

};