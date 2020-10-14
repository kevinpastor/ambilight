#pragma once

#include <fstream>
#include <sstream>
#include <vector>

#include "Coordinates.h"

class Options
{
public:
	Options();
	Options(const std::string & optionPath);

	std::string getPortName() const;
	std::vector<Coordinates> getCoordinates() const;
	float getSmoothing() const;

private:
	std::string path;

	std::string portName;
	std::vector<Coordinates> coordinates;
	float smoothing;

	void importOptions();

};
