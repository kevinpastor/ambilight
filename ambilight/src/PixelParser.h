#pragma once

#include <vector>

#include "Capture.h"
#include "ColorGrader.h"
#include "Coordinates.h"
#include "MonitorUtility.h"
#include "Pixel.h"
#include "ScreenCapture.h"
#include "SessionUtility.h"

class PixelParser
{
public:
	PixelParser(const std::vector<Coordinates> & coordinates);

	std::vector<Pixel> getPixels(const Capture & capture) const;

	static std::vector<Pixel> mix(const std::vector<Pixel> & first, const std::vector<Pixel> & second, const float & weight);
	static std::vector<Pixel> mix(const std::vector<Pixel> & first, const Pixel & second, const float & weight);
	static Pixel mix(const Pixel & first, const Pixel & second, const float & weight);

private:
	static Pixel average(const std::vector<Pixel> & pixels);

	static std::vector<Pixel> getSurrounding(const Capture & capture, const Coordinates & coorditates);

	const std::vector<Coordinates> coordinates;

	static const unsigned surroundingRadius = 19;

};
