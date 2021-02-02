#include "ColorGrader.h"

ColorGrader::ColorGrader(const std::vector<RGBLUT> & luts)
	: luts(luts)
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
	for (const RGBLUT & lut : this->luts)
	{
		if (lut.isUsable())
		{
			return lut.get(pixel);
		}
	}

	throw std::runtime_error("No LUT matching current state");
}
