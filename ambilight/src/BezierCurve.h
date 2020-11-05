#pragma once

#include "Point.h"

#include <stdexcept>
#include <vector>

class BezierCurve
{
public:
	BezierCurve(const std::vector<Point> & controlPoints);

	Point evaluate(const double & t) const;

private:
	static Point evaluate(const std::vector<Point> & controlPoints, const double & t);

	const std::vector<Point> controlPoints;

};
