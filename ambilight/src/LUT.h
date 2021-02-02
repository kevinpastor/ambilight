#pragma once

#include <vector>

#include "AbstractCurve.h"
#include "Pixel.h"

class LUT
{
public:
	LUT(const AbstractCurve & curve);

	unsigned char get(const unsigned char & isSignedIn) const;

private:
	static std::vector<unsigned char> getData(const AbstractCurve & curve);

	const std::vector<unsigned char> data;

};
