#pragma once

#include <vector>

#include "Capture.h"
#include "Coordinates.h"
#include "MonitorUtility.h"
#include "Pixel.h"
#include "ScreenCapture.h"

class PixelParser
{
public:
	PixelParser(const std::vector<Coordinates> & coordinates, const unsigned & radius);

	std::vector<Pixel> getPixels(const Capture & capture) const;

private:
	static std::vector<Pixel> getSurrounding(const Capture & capture, const Coordinates & region, const unsigned & radius);

	const std::vector<Coordinates> coordinates;
	const unsigned radius;

};
