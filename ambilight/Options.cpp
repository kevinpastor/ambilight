#include "options.h"

Options::Options()
	: portName(), coordinates(), smoothing(1)
{
	char * buffer = nullptr;
	size_t size = 0;
	_dupenv_s(&buffer, &size, "APPDATA");
	this->path = std::string(buffer);
	this->path += "\\Ambilight\\options.txt";
	this->importOptions();
}

Options::Options(const std::string & optionPath)
	: path(optionPath)
{
	this->importOptions();
}

void Options::reload()
{
	this->importOptions();
}

std::string Options::getPortName() const
{
	return this->portName;
}

std::vector<Coordinates> Options::getCoordinates() const
{
	return this->coordinates;
}

unsigned Options::getSmoothing() const
{
	return this->smoothing;
}

void Options::importOptions()
{
	std::ifstream optionFile(this->path);
	if (optionFile.fail()) {
		throw std::runtime_error("Unable to open option file");
	}

	std::string line;
	while (std::getline(optionFile, line))
	{
		// Parsing PortName string
		if (strncmp(line.data(), "portname=", 9) == 0)
		{
			if (line[9] == ' ')
			{
				throw std::runtime_error("Invalid options file format");
			}
			this->portName = line.substr(9);
		}
		// Parsing Smoothing number
		else if (strncmp(line.data(), "smoothing=", 10) == 0)
		{
			if (line[10] == ' ')
			{
				throw std::runtime_error("Invalid options file format");
			}
			sscanf_s(line.substr(10).data(), "%d", &this->smoothing);
		}
		// Parsing Coordinates dynamic array
		else if (strncmp(line.data(), "coordinates=[", 13) == 0)
		{
			this->coordinates = std::vector<Coordinates>();
			while (std::getline(optionFile, line))
			{
				if (line[0] == ']')
				{
					break;
				}

				unsigned startPosition = (unsigned)line.find('{') + 1;
				unsigned separatorPosition = (unsigned)line.find(',');
				unsigned endPosition = (unsigned)line.find('}');

				if (startPosition == std::string::npos
					|| separatorPosition == std::string::npos
					|| endPosition == std::string::npos)
				{
					throw std::runtime_error("Invalid options file format");
				}

				Coordinates coordinates;

				std::string x = line.substr(startPosition, separatorPosition - startPosition);
				sscanf_s(x.data(), "%d", &(coordinates.x));

				std::string y = line.substr(separatorPosition + 1, endPosition - (separatorPosition + 1));
				sscanf_s(y.data(), "%d", &(coordinates.y));

				this->coordinates.push_back(coordinates);
			}
		}
	}
}
