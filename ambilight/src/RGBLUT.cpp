#include "RGBLUT.h"

RGBLUT::RGBLUT(const LUT & rLUT, const LUT & gLUT, const LUT & bLUT, const unsigned & brightnessThreshold)
	: rLUT(rLUT),
	gLUT(gLUT),
	bLUT(bLUT),
	brightnessThreshold(brightnessThreshold)
{
}

RGBLUT::RGBLUT(const LUT & rLUT, const LUT & gLUT, const LUT & bLUT)
	: RGBLUT(rLUT, gLUT, bLUT, 100)
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

bool RGBLUT::isUsable() const
{
	return MonitorUtility::getBrightness() == this->brightnessThreshold;
}
