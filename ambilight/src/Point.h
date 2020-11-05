#pragma once

struct Point {
	double x = 0;
	double y = 0;

	friend Point operator*(const Point & point, const double & multiplier)
	{
		return Point({
			point.x * multiplier,
			point.y * multiplier
			});
	}

	friend Point operator+(const Point & first, const Point & second)
	{
		return Point({
			first.x + second.x,
			first.y + second.y
			});
	}

	friend bool operator==(const Point & lhs, const Point & rhs)
	{
		const double epsilon = 0.001;
		if (lhs.x > rhs.x)
		{
			if ((lhs.x - rhs.x) > epsilon)
			{
				return false;
			}
		}
		else {
			if ((rhs.x - lhs.x) > epsilon)
			{
				return false;
			}
		}

		if (lhs.y > rhs.y)
		{
			if ((lhs.y - rhs.y) > epsilon)
			{
				return false;
			}
		}
		else {
			if ((rhs.y - lhs.y) > epsilon)
			{
				return false;
			}
		}

		return true;
	}

	friend bool operator!=(const Point & lhs, const Point & rhs)
	{
		return !(lhs == rhs);
	}

};
