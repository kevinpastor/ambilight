#pragma once

#include <vector>

#include "Coordinates.h"
#include "Pixel.h"
#include "ScreenCapture.h"

class PixelParser
{
public:
	PixelParser(ScreenCapture * screencapture, const std::vector<Coordinates> & coordinates);

	std::vector<Pixel> getPixels() const;
	std::vector<Pixel> fadePixels(const std::vector<Pixel> & currentPixels, const std::vector<Pixel> & previousPixels, const unsigned & smoothing) const;

private:
	Pixel averagePixel(const std::vector<Pixel> & pixels) const;
	std::vector<Pixel> getSurroundingPixels(const Coordinates & coorditates) const;
	Pixel getPixel(const Coordinates & coordinates) const;

	const ScreenCapture * screenCapture;
	const std::vector<Coordinates> coordinates;
	const unsigned surroundingRadius;

};