#ifndef SCREEN_CAPTURE_PARSER_H
#define SCREEN_CAPTURE_PARSER_H

#include <vector>

#include "Pixel.h"

class ScreenCaptureParser
{
public:
	ScreenCaptureParser();

private:
	const Pixel averagePixels(const std::vector<Pixel> & pixels);
	const std::vector<Pixel> getSurroundingPixels(const unsigned & x, const unsigned & y);
	const Pixel getPixel(const unsigned & x, const unsigned & y);

};

#endif