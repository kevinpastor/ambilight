#include "ScreenCaptureParser.h"

ScreenCaptureParser::ScreenCaptureParser(const std::vector<Pixel> screenCapture) : screenCapture(screenCapture)
{
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
	// top left origin
	return this->screenCapture[x + y * 1920];
}
