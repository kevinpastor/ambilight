#include "ColorGrader.h"

ColorGrader::ColorGrader(const RGBLUT & lut, const RGBLUT & dimmedLut)
	: lut(lut),
	dimmedLut(dimmedLut)
{
}

std::vector<Pixel> ColorGrader::correct(const std::vector<Pixel> & pixels) const
{
	std::vector<Pixel> corrected(pixels.size());
	for (unsigned i = 0; i < pixels.size(); ++i)
	{
		corrected[i] = this->correct(pixels[i]);
	}

	return corrected;
}

Pixel ColorGrader::correct(const Pixel & pixel) const
{
	const unsigned monitorBrightness = MonitorUtility::getBrightness();
	if (monitorBrightness > ColorGrader::DIMMED_BRIGHTNESS_LIMIT)
	{
		return this->lut.get(pixel);
	}

	return this->dimmedLut.get(pixel);
}
