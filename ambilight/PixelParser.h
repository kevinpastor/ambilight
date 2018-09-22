#ifndef PIXEL_PARSER_H
#define PIXEL_PARSER_H

#include <vector>
#include <iostream>

#include "ScreenCapture.h"
#include "Pixel.h"

class PixelParser
{
public:
	PixelParser();
	void update();
	const Pixel getPixel(const unsigned & x, const unsigned & y);
	const std::vector<Pixel> getPixels();

private:
	const Pixel averagePixels(const std::vector<Pixel> & pixels);
	const std::vector<Pixel> getSurroundingPixels(const unsigned & x, const unsigned & y);

	ScreenCapture screenCapturer;

};

#endif