#include "pch.h"
#include <CppUnitTest.h>

#include "LinearBezierCurve.h"
#include "QuadraticBezierCurve.h"
#include "CubicBezierCurve.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

template<> static std::wstring Microsoft::VisualStudio::CppUnitTestFramework::ToString<Point>(const Point & point)
{
	return L"{ " + std::to_wstring(point.x) + L", " + std::to_wstring(point.y) + L" }";
}

namespace Tests
{
	TEST_CLASS(BezierCurveTests)
	{
	public:

		TEST_METHOD(LinearEvaluate)
		{
			std::vector<Point> controlPoints {
				{ 0, 0 },
				{ 255, 255 }
			};

			LinearBezierCurve curve(controlPoints);

			Assert::AreEqual({ 0, 0 }, curve.evaluate(0));
			Assert::AreEqual({ 127, 127 }, curve.evaluate(127));
			Assert::AreEqual({ 255, 255 }, curve.evaluate(255));
		}

		TEST_METHOD(QuadraticEvaluate)
		{
			std::vector<Point> controlPoints {
				{ 0, 0 },
				{ 0, 255 },
				{ 255, 255 }
			};

			QuadraticBezierCurve curve(controlPoints);

			Assert::AreEqual(Point({ 0, 0 }), curve.evaluate(0));
			Assert::AreEqual(Point({ 127, 232.917 }), curve.evaluate(127));
			Assert::AreEqual(Point({ 255, 255 }), curve.evaluate(255));
		}

		TEST_METHOD(CubicEvaluate)
		{
			std::vector<Point> controlPoints {
				{ 0, 0 },
				{ 0, 255 },
				{ 255, 0 },
				{ 255, 255 }
			};

			CubicBezierCurve curve(controlPoints);

			Assert::AreEqual(Point({ 0, 0 }), curve.evaluate(0));
			Assert::AreEqual(Point({ 85, 232.917 }), curve.evaluate(85));
			Assert::AreEqual(Point({ 170, 232.917 }), curve.evaluate(170));
			Assert::AreEqual(Point({ 255, 255 }), curve.evaluate(255));
		}

	private:
		static bool areEqual(const double & expected, const double & actual)
		{
			const double epsilon = 0.00001;
			if (expected > actual)
			{
				return (expected - actual) < epsilon;
			}

			return (actual - expected) < epsilon;
		}
	};
}
