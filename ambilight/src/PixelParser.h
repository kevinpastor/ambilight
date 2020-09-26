#pragma once

#include <vector>

#include "Coordinates.h"
#include "Pixel.h"
#include "ScreenCapture.h"
#include "Capture.h"

class PixelParser
{
public:
	PixelParser(const std::vector<Coordinates> & coordinates);

	std::vector<Pixel> getPixels(const Capture & capture, const bool & isMonitorDimmed) const;
	std::vector<Pixel> fadePixels(const std::vector<Pixel> & currentPixels, const std::vector<Pixel> & previousPixels, const unsigned & smoothing) const;

private:
	Pixel averagePixel(const std::vector<Pixel> & pixels, const bool & isMonitorDimmed) const;
	std::vector<Pixel> getSurroundingPixels(const Capture & capture, const Coordinates & coorditates) const;
	Pixel colorCorrect(const Pixel & pixel, const bool & isMonitorDimmed) const;

	const std::vector<Coordinates> coordinates;
	const unsigned surroundingRadius;

};