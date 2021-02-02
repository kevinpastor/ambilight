#include "options.h"

Options::Options()
	: Options(".\\options.json")
{
}

Options::Options(const std::string & optionPath)
	: json(Options::getJson(optionPath)),
	portName(Options::getPortName(this->json)),
	radius(Options::getRadius(this->json)),
	baudRate(Options::getBaudRate(this->json)),
	coordinates(Options::getCoordinates(this->json)),
	smoothing(Options::getSmoothing(this->json)),
	colorGrader(Options::getColorGrader(this->json)),
	lowPowerModeOptions(Options::getLowPowerModeOptions(this->json))
{
}

std::string Options::getPortName() const
{
	return this->portName;
}

unsigned long Options::getBaudRate() const
{
	return this->baudRate;
}

LowPowerModeOptions Options::getLowPowerModeOptions() const
{
	return this->lowPowerModeOptions;
}

unsigned Options::getRadius() const
{
	return this->radius;
}

std::vector<Coordinates> Options::getCoordinates() const
{
	return this->coordinates;
}

double Options::getSmoothing() const
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

	return json["portname"].get<std::string>();
}

unsigned long Options::getBaudRate(const nlohmann::json & json)
{
	if (!json.contains("baudRate"))
	{
		throw std::runtime_error("Configuration should have attribute \"baudRate\"");
	}

	if (!json["baudRate"].is_number_unsigned())
	{
		throw std::runtime_error("Configuration $[\"baudRate\"] should be a unsigned long");
	}

	return json["baudRate"].get<unsigned long>();
}

LowPowerModeOptions Options::getLowPowerModeOptions(const nlohmann::json & json)
{
	if (!json.contains("lowPowerMode"))
	{
		throw std::runtime_error("Configuration should have attribute \"lowPowerMode\"");
	}

	return {
		Options::getLowPowerModeFrameRenderTime(json["lowPowerMode"])
	};
}

std::chrono::nanoseconds Options::getLowPowerModeFrameRenderTime(const nlohmann::json & json)
{
	if (!json.contains("refreshRate"))
	{
		throw std::runtime_error("Configuration $[\"lowPowerMode\"] should have attribute \"refreshRate\"");
	}

	if (!json["refreshRate"].is_number_unsigned())
	{
		throw std::runtime_error("Configuration $[\"lowPowerMode\"][\"refreshRate\"] should be a unsigned");
	}

	const unsigned refreshRate = json["refreshRate"].get<unsigned>();

	return std::chrono::nanoseconds(1000000000 / refreshRate);
}

unsigned Options::getRadius(const nlohmann::json & json)
{
	if (!json.contains("radius"))
	{
		throw std::runtime_error("Configuration should have attribute \"radius\"");
	}

	if (!json["radius"].is_number_unsigned())
	{
		throw std::runtime_error("Configuration $[\"radius\"] should be a unsigned");
	}

	return json["radius"].get<unsigned>();
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

	std::vector<Coordinates> coordinates(json["coordinates"].size());
	for (unsigned i = 0; i < json["coordinates"].size(); ++i)
	{
		const nlohmann::json & coordinate = json["coordinates"][i];
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

		coordinates[i] = {
			coordinate["x"].get<int>(),
			coordinate["y"].get<int>(),
		};
	}
	return coordinates;
}

double Options::getSmoothing(const nlohmann::json & json)
{
	if (!json.contains("smoothing"))
	{
		throw std::runtime_error("Configuration should have attribute \"smoothing\"");
	}

	if (!json["smoothing"].is_number())
	{
		throw std::runtime_error("Configuration $[\"smoothing\"] should be a double");
	}

	const double smoothing = json["smoothing"].get<double>();

	if (smoothing < 0.0 || smoothing > 1.0)
	{
		throw std::runtime_error("Configuration $[\"smoothing\"] must be between 0.0 and 1.0");
	}

	return smoothing;
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

	std::vector<RGBLUT> luts;// (json["luts"].size());
	for (unsigned i = 0; i < json["luts"].size(); ++i)
	{
		const nlohmann::json lut = json["luts"][i];
		//luts[i] = Options::getRGBLut(lut);
		luts.push_back(Options::getRGBLut(lut));
	}

	return ColorGrader(luts);
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

	if (!json.contains("brightnessThreshold"))
	{
		return RGBLUT(
			Options::getLut(json["r"]),
			Options::getLut(json["g"]),
			Options::getLut(json["b"])
		);
	}
	if (!json["brightnessThreshold"].is_number_unsigned())
	{
		throw std::runtime_error("Configuration $[\"luts\"][*][\"brightnessThreshold\"] should be an unsigned integer");
	}

	return RGBLUT(
		Options::getLut(json["r"]),
		Options::getLut(json["g"]),
		Options::getLut(json["b"]),
		json["brightnessThreshold"].get<unsigned>()
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

	const std::string type = json["type"].get<std::string>();

	if (type != "oneDimensionBezierCurve")
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

	std::vector<double> controlValues(json["controlValues"].size());
	for (unsigned i = 0; i < json["controlValues"].size(); ++i)
	{
		const nlohmann::json controlValue = json["controlValues"][i];
		if (!controlValue.is_number())
		{
			throw std::runtime_error("Configuration $[\"luts\"][*][?(@[\"type\"] == \"oneDimensionBezierCurve\")][\"controlValues\"][*] should be a double");
		}

		controlValues[i] = controlValue.get<double>();
	}

	return OneDimensionBezierCurve(controlValues);
}
