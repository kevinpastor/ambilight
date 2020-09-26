#include "Ambilight.h"

Ambilight::Ambilight()
	: isPaused(false),
	isStopped(false),
	options(),
	screenCapture(),
	pixelParser(this->options.getCoordinates()),
	arduinoSerial(this->options.getPortName(), (unsigned)this->options.getCoordinates().size()),
	previousPixels(this->options.getCoordinates().size()),
	i(0)
{ }

Ambilight::~Ambilight()
{
	this->stop();
}

void Ambilight::resume()
{
	this->isPaused = false;
}

void Ambilight::pause()
{
	this->isPaused = true;
}

void Ambilight::stop()
{
	this->pause();
	this->isStopped = true;
}

void Ambilight::exec()
{
	if (this->isPaused || this->isStopped)
	{
		return;
	}

	Capture capture = this->screenCapture.capture();
	bool isMonitorDimmed = false;//this->isMonitorDimmed();
	std::vector<Pixel> currentPixels = this->pixelParser.getPixels(capture, isMonitorDimmed);
	std::vector<Pixel> data = this->pixelParser.fadePixels(currentPixels, this->previousPixels, this->options.getSmoothing());
	this->previousPixels = data;

	this->arduinoSerial.send(data);
}

// TODO Remove
void Ambilight::fadeOut()
{
	std::vector<Pixel> currentPixels(this->previousPixels.size()); // , { 0, 0, 0 });
	for (unsigned i = 0; i < 10; ++i)
	{
		std::vector<Pixel> data = this->pixelParser.fadePixels(currentPixels, this->previousPixels, this->options.getSmoothing());
		this->previousPixels = data;
		this->arduinoSerial.send(data);
	}
}


bool Ambilight::isMonitorDimmed() const
{
	return getMonitorBrightness() <= 5;
}

unsigned Ambilight::getMonitorBrightness() const
{
	HMONITOR hMonitor = NULL;
	DWORD numberOfPhysicalMonitors;
	LPPHYSICAL_MONITOR physicalMonitors = NULL;

	HWND hWnd = GetDesktopWindow();

	// Get the monitor handle.
	hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTOPRIMARY);

	// Get the number of physical monitors.
	BOOL success = GetNumberOfPhysicalMonitorsFromHMONITOR(hMonitor, &numberOfPhysicalMonitors);

	if (success == FALSE)
	{
		return 100;
	}

	// Allocate the array of PHYSICAL_MONITOR structures.
	physicalMonitors = (LPPHYSICAL_MONITOR)malloc(numberOfPhysicalMonitors * sizeof(PHYSICAL_MONITOR));

	if (physicalMonitors == NULL)
	{
		return 100;
	}

	// Get the array.
	success = GetPhysicalMonitorsFromHMONITOR(hMonitor, numberOfPhysicalMonitors, physicalMonitors);

	if (success == FALSE)
	{
		return 100;
	}

	// Get physical monitor handle.
	HANDLE hPhysicalMonitor = physicalMonitors[0].hPhysicalMonitor;

	DWORD pdwMinimumBrightness = 0;
	DWORD pdwCurrentBrightness = 0;
	DWORD pdwMaximumBrightness = 0;
	success = GetMonitorBrightness(hPhysicalMonitor, &pdwMinimumBrightness, &pdwCurrentBrightness, &pdwMaximumBrightness);

	if (success == FALSE)
	{
		return 100;
	}

	// Close the monitor handles.
	DestroyPhysicalMonitors(numberOfPhysicalMonitors, physicalMonitors);

	// Free the array.
	free(physicalMonitors);

	return pdwCurrentBrightness;
}