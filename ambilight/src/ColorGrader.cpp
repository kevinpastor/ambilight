#include "ColorGrader.h"

ColorGrader::ColorGrader(const RGBLUT & lut, const RGBLUT & dimmedLut)
	: lut(lut),
	dimmedLut(dimmedLut)
{
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

std::vector<Pixel> ColorGrader::correct(const std::vector<Pixel> & pixels) const
{
	std::vector<Pixel> corrected;
	for (const Pixel & pixel : pixels)
	{
		corrected.push_back(this->correct(pixel));
	}

	return corrected;
}
