#pragma once

#include <vector>

#include "Pixel.h"

class LUT
{
public:
	LUT(const float & u1, const float & u2, const float & u3);

	unsigned char get(const unsigned char & value) const;

private:
	std::vector<unsigned char> data;

};