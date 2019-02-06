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

	void reload();

	std::string getPortName() const;
	std::vector<Coordinates> getCoordinates() const;
	unsigned getSmoothing() const;

private:
	std::string path;

	std::string portName;
	std::vector<Coordinates> coordinates;
	unsigned smoothing;

	void importOptions();

};