#ifndef PIXEL_PARSER_H
#define PIXEL_PARSER_H

#include <vector>
#include <iostream>

#include "ScreenCapture.h"
#include "Coordinates.h"
#include "Pixel.h"

class PixelParser
{
public:
	PixelParser(std::vector<Coordinates> coordinates);
	void update();
	const std::vector<Pixel> getPixels();

private:
	const Pixel averagePixel(const std::vector<Pixel> & pixels);
	const std::vector<Pixel> getSurroundingPixels(const Coordinates & coorditates);
	const Pixel getPixel(const Coordinates coordinates);

	ScreenCapture screenCapturer;
	const std::vector<Coordinates> coordinates;
	const unsigned surroundingRadius;
};

#endif