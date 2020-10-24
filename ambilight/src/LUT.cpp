#include "LUT.h"

LUT::LUT(const float & u1, const float & u2, const float & u3)
{
	for (float i = 0; i < 256; ++i)
	{
		const unsigned char value = static_cast<unsigned char>((
			3 * u1 * (1 - (i / 255)) * (1 - (i / 255)) * (i / 255)
			+ 3 * u2 * (1 - (i / 255)) * (i / 255) * (i / 255)
			+ u3 * (i / 255) * (i / 255) * (i / 255)
			) * 255);
		this->data.push_back(value);
	}
}

unsigned char LUT::get(const unsigned char & value) const
{
	return this->data[value];
}
