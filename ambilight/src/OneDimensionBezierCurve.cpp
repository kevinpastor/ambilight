#include "OneDimensionBezierCurve.h"

OneDimensionBezierCurve::OneDimensionBezierCurve(const std::vector<double> & controlValues)
	: bezierCurve(OneDimensionBezierCurve::convertToControlPoints(controlValues))
{
}

double OneDimensionBezierCurve::evaluate(const double & x) const
{
	if (x < 0)
	{
		throw std::invalid_argument("Argument \"x\" should be greater than or equal to 0");
	}

	if (x > 1)
	{
		throw std::invalid_argument("Argument \"x\" should be smaller than or equal to 1");
	}

	const Point point = this->bezierCurve.evaluate(x);

	return point.y;
}

std::vector<Point> OneDimensionBezierCurve::convertToControlPoints(const std::vector<double> & controlValues)
{
	if (controlValues.size() < 2)
	{
		throw std::invalid_argument("Argument \"controlValues\" should have at least 2 elements");
	}

	const unsigned long long degree = controlValues.size() - 1;

	std::vector<Point> controlPoints(controlValues.size());
	for (unsigned i = 0; i < controlValues.size(); ++i)
	{
		const double x = i * 1.0 / degree;
		const double y = controlValues[i];
		const Point point = { x, y };
		controlPoints[i] = point;
	}

	return controlPoints;
}
