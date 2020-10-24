#include "RGBLUT.h"

RGBLUT::RGBLUT(const LUT & rLUT, const LUT & gLUT, const LUT & bLUT)
	: rLUT(rLUT),
	gLUT(gLUT),
	bLUT(bLUT)
{
}

Pixel RGBLUT::get(const Pixel & pixel) const
{
	return Pixel({
		this->rLUT.get(pixel.red),
		this->gLUT.get(pixel.green),
		this->bLUT.get(pixel.blue)
		});
}
