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
	const std::vector<Pixel> fadePixels(const std::vector<Pixel> & currentPixels, const std::vector<Pixel> & previousPixels);

private:
	const Pixel averagePixel(const std::vector<Pixel> & pixels);
	const std::vector<Pixel> getSurroundingPixels(const Coordinates & coorditates);
	const Pixel getPixel(const Coordinates coordinates);

	ScreenCapture screenCapturer;
	const std::vector<Coordinates> coordinates;
	const unsigned surroundingRadius;
	const unsigned smoothing;

};

#endif