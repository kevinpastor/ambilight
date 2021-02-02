#pragma once

#include <cassert>
#include <stdexcept>
#include <vector>

#include "Point.h"

class BezierCurve
{
public:
	BezierCurve(const std::vector<Point> & controlPoints);

	Point evaluate(const double & t) const;

private:
	static Point evaluate(const std::vector<Point> & controlPoints, const double & t);

	const std::vector<Point> controlPoints;

};
