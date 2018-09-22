#include "ScreenCaptureParser.h"

ScreenCaptureParser::ScreenCaptureParser()
{
}

void ScreenCaptureParser::update()
{
	this->screenCapturer.capture();
}

const std::vector<Pixel> ScreenCaptureParser::getPixels()
{
	return std::vector<Pixel>();
}

const Pixel ScreenCaptureParser::averagePixels(const std::vector<Pixel>& pixels)
{
	unsigned
		red = 0,
		green = 0,
		blue = 0;

	for (auto &pixel : pixels)
	{
		red += pixel.red;
		green += pixel.green;
		blue += pixel.blue;
	}

	red /= pixels.size();
	green /= pixels.size();
	blue /= pixels.size();

	return Pixel({ (unsigned char)red, (unsigned char)green, (unsigned char)blue });
}

const std::vector<Pixel> ScreenCaptureParser::getSurroundingPixels(const unsigned & x, const unsigned & y)
{
	return std::vector<Pixel>();
}

const Pixel ScreenCaptureParser::getPixel(const unsigned & x, const unsigned & y)
{
	return this->screenCapturer.getPixel(x, y);;
}