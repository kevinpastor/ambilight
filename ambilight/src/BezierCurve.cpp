#include "BezierCurve.h"

BezierCurve::BezierCurve(const std::vector<Point> & controlPoints)
	: controlPoints(controlPoints)
{
	if (this->controlPoints.size() < 2)
	{
		throw std::invalid_argument("Argument \"controlPoints\" should have at least 2 elements");
	}
}

Point BezierCurve::evaluate(const double & t) const
{
	if (t < 0)
	{
		throw std::invalid_argument("Argument \"t\" should be greater than 0");
	}
	if (t > 1)
	{
		throw std::invalid_argument("Argument \"t\" should be smaller than 1");
	}

	return BezierCurve::evaluate(this->controlPoints, t);
}

Point BezierCurve::evaluate(const std::vector<Point> & controlPoints, const double & t)
{
	assert(t >= 0 && t <= 1);

	if (controlPoints.size() - 1 > 1)
	{
		return BezierCurve::evaluate(std::vector<Point>(controlPoints.begin(), controlPoints.end() - 1), t) * (1 - t)
			+ BezierCurve::evaluate(std::vector<Point>(controlPoints.begin() + 1, controlPoints.end()), t) * t;
	}

	return controlPoints[0] * (1 - t) + controlPoints[1] * t;
}
