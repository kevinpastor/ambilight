#include "LUT.h"

LUT::LUT(const AbstractCurve & curve)
	: data(LUT::getData(curve))
{
}

unsigned char LUT::get(const unsigned char & value) const
{
	return this->data[value];
}

std::vector<unsigned char> LUT::getData(const AbstractCurve & curve)
{
	std::vector<unsigned char> data(256);
	for (unsigned i = 0; i <= 255; ++i)
	{
		const double x = i / 255.0;
		const double value = curve.evaluate(x) * 255;

		if (value < 0)
		{
			data[i] = 0;
		}
		else if (value > 255)
		{
			data[i] = 255;
		}
		else
		{
			data[i] = static_cast<unsigned char>(value);
		}
	}

	return data;
}
