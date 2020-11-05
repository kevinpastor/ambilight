#pragma once

#include <fstream>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <vector>

#include "ColorGrader.h"
#include "Coordinates.h"
#include "LUT.h"
#include "OneDimensionBezierCurve.h"
#include "RGBLUT.h"

class Options
{
public:
	Options();
	Options(const std::string & optionPath);

	std::string getPortName() const;
	std::vector<Coordinates> getCoordinates() const;
	double getSmoothing() const;
	ColorGrader getColorGrader() const;

private:
	static nlohmann::json getJson(const std::string & path);
	static std::string getPortName(const nlohmann::json & json);
	static std::vector<Coordinates> getCoordinates(const nlohmann::json & json);
	static double getSmoothing(const nlohmann::json & json);
	static ColorGrader getColorGrader(const nlohmann::json & json);
	static RGBLUT getRGBLut(const nlohmann::json & json);
	static LUT getLut(const nlohmann::json & json);
	static OneDimensionBezierCurve getOneDimensionBezierCurve(const nlohmann::json & json);

	const nlohmann::json json;
	const std::string portName;
	const std::vector<Coordinates> coordinates;
	const double smoothing;
	const ColorGrader colorGrader;

};
