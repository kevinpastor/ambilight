#include "options.h"

Options::Options()
	: Options(".\\options.json")
{
}

Options::Options(const std::string & optionPath)
	: json(Options::getJson(optionPath)),
	portName(Options::getPortName(this->json)),
	coordinates(Options::getCoordinates(this->json)),
	smoothing(Options::getSmoothing(this->json)),
	colorGrader(Options::getColorGrader(this->json))
{
}

std::string Options::getPortName() const
{
	return this->portName;
}

std::vector<Coordinates> Options::getCoordinates() const
{
	return this->coordinates;
}

float Options::getSmoothing() const
{
	return this->smoothing;
}

ColorGrader Options::getColorGrader() const
{
	return this->colorGrader;
}

nlohmann::json Options::getJson(const std::string & path)
{
	std::ifstream optionFile(path);
	if (optionFile.fail()) {
		throw std::runtime_error("Unable to open configuration file");
	}

	nlohmann::json json;
	optionFile >> json;

	return json;
}

std::string Options::getPortName(const nlohmann::json & json)
{
	if (!json.contains("portname"))
	{
		throw std::runtime_error("Configuration should have attribute \"portname\"");
	}

	if (!json["portname"].is_string())
	{
		throw std::runtime_error("Configuration $[\"portname\"] should be a string");
	}

	return json["portname"];
}

std::vector<Coordinates> Options::getCoordinates(const nlohmann::json & json)
{
	if (!json.contains("coordinates"))
	{
		throw std::runtime_error("Configuration should have attribute \"coordinates\"");
	}

	if (!json["coordinates"].is_array())
	{
		throw std::runtime_error("Configuration $[\"coordinates\"] should be an array");
	}

	if (json["coordinates"].size() < 1)
	{
		throw std::runtime_error("Configuration $[\"coordinates\"] should have at least one element");
	}

	std::vector<Coordinates> coordinates;
	for (const nlohmann::json & coordinate : json["coordinates"]) {
		if (!coordinate.contains("x"))
		{
			throw std::runtime_error("Configuration $[\"coordinates\"][*] should have attribute \"x\"");
		}

		if (!coordinate.contains("y"))
		{
			throw std::runtime_error("Configuration $[\"coordinates\"][*] should have attribute \"y\"");
		}

		if (!coordinate["x"].is_number_integer())
		{
			throw std::runtime_error("Configuration $[\"coordinates\"][*][\"x\"] should be an integer");
		}

		if (!coordinate["y"].is_number_integer())
		{
			throw std::runtime_error("Configuration $[\"coordinates\"][*][\"y\"] should be an integer");
		}

		coordinates.push_back({
			coordinate["x"],
			coordinate["y"],
			});
	}
	return coordinates;
}

float Options::getSmoothing(const nlohmann::json & json)
{
	if (!json.contains("smoothing"))
	{
		throw std::runtime_error("Configuration should have attribute \"smoothing\"");
	}

	if (!json["smoothing"].is_number())
	{
		throw std::runtime_error("Configuration $[\"smoothing\"] should be a float");
	}

	return json["smoothing"];
}

ColorGrader Options::getColorGrader(const nlohmann::json & json)
{
	if (!json.contains("luts"))
	{
		throw std::runtime_error("Configuration should have attribute \"luts\"");
	}

	if (!json["luts"].is_array())
	{
		throw std::runtime_error("Configuration $[\"luts\"] should be an array");
	}

	if (json["luts"].size() < 1)
	{
		throw std::runtime_error("Configuration $[\"luts\"] should have at least one element");
	}

	return ColorGrader(
		Options::getRGBLut(json["luts"][0]),
		Options::getRGBLut(json["luts"][1])
	);
}

RGBLUT Options::getRGBLut(const nlohmann::json & json)
{
	if (!json.contains("r"))
	{
		throw std::runtime_error("Configuration $[\"luts\"][*] should have attribute \"r\"");
	}

	if (!json.contains("g"))
	{
		throw std::runtime_error("Configuration $[\"luts\"][*] should have attribute \"g\"");
	}

	if (!json.contains("b"))
	{
		throw std::runtime_error("Configuration $[\"luts\"][*] should have attribute \"b\"");
	}

	return RGBLUT(
		Options::getLut(json["r"]),
		Options::getLut(json["g"]),
		Options::getLut(json["r"])
	);
}

LUT Options::getLut(const nlohmann::json & json)
{
	if (!json.contains("type"))
	{
		throw std::runtime_error("Configuration $[\"luts\"][*][*] should have attribute \"type\"");
	}
	if (!json["type"].is_string())
	{
		throw std::runtime_error("Configuration $[\"luts\"][*][*][\"type\"] should be a string");
	}

	if (json["type"] != "oneDimensionBezierCurve")
	{
		throw std::runtime_error("Configuration $[\"luts\"][*][*][\"type\"] should have value \"oneDimensionBezierCurve\"");
	}

	return LUT(Options::getOneDimensionBezierCurve(json));
}

OneDimensionBezierCurve Options::getOneDimensionBezierCurve(const nlohmann::json & json)
{
	if (!json.contains("controlValues"))
	{
		throw std::runtime_error("Configuration $[\"luts\"][*][?(@[\"type\"] == \"oneDimensionBezierCurve\")] should have attribute \"controlValues\"");
	}

	if (!json["controlValues"].is_array())
	{
		throw std::runtime_error("Configuration $[\"luts\"][*][?(@[\"type\"] == \"oneDimensionBezierCurve\")][\"controlValues\"] should be an array");
	}

	if (json["controlValues"].size() < 2)
	{
		throw std::runtime_error("Configuration $[\"luts\"][*][?(@[\"type\"] == \"oneDimensionBezierCurve\")][\"controlValues\"] should have at least two elements");
	}

	std::vector<double> controlValues;
	for (const nlohmann::json & controlValue : json["controlValues"])
	{
		if (!controlValue.is_number())
		{
			throw std::runtime_error("Configuration $[\"luts\"][*][?(@[\"type\"] == \"oneDimensionBezierCurve\")][\"controlValues\"][*] should be a number");
		}

		controlValues.push_back(controlValue);
	}

	return OneDimensionBezierCurve(controlValues);
}
