#include "Point.h"

Point operator*(const Point & point, const double & multiplier)
{
	return Point({
		point.x * multiplier,
		point.y * multiplier
		});
}

Point operator+(const Point & first, const Point & second)
{
	return Point({
		first.x + second.x,
		first.y + second.y
		});
}
