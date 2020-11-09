#pragma once

struct Point {
	double x = 0;
	double y = 0;

	friend Point operator*(const Point & point, const double & multiplier);
	friend Point operator+(const Point & first, const Point & second);

};
