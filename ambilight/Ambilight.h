#ifndef AMBILIGHT_H
#define AMBILIGHT_H

#include <vector>
#include <iostream>

#include "PixelParser.h"

class Ambilight
{
public:
	Ambilight();

	const void start();
	const void stop();

private:
	PixelParser pixelParser;

};

#endif