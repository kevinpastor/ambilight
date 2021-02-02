#pragma once

#include <algorithm>
#include <cassert>
#include <stdexcept>
#include <vector>

struct Pixel {
	unsigned char red = 0;
	unsigned char green = 0;
	unsigned char blue = 0;

	static std::vector<Pixel> mix(const std::vector<Pixel> & first, const std::vector<Pixel> & second, const double & weight);
	static std::vector<Pixel> mix(const std::vector<Pixel> & first, const Pixel & second, const double & weight);
	static Pixel mix(const Pixel & first, const Pixel & second, const double & weight);

	static std::vector<Pixel> fadeOut(const std::vector<Pixel> & pixels, const double & weight);

	static Pixel average(const std::vector<Pixel> & pixels);

};
