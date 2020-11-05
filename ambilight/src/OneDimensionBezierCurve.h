#pragma once

#include <stdexcept>
#include <vector>

#include "BezierCurve.h"
#include "AbstractCurve.h"
#include "Point.h"

class OneDimensionBezierCurve : public AbstractCurve
{
public:
	OneDimensionBezierCurve(const std::vector<double> & controlValues);

	virtual double evaluate(const double & x) const;

private:
	static std::vector<Point> convertToControlPoints(const std::vector<double> & controlValues);

	const BezierCurve bezierCurve;

};
