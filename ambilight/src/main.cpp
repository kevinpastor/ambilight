#include "App.h"
#include "Logger.h"

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
	try
	{
		App app;
		app.start(hInstance);
	}
	catch (std::exception & exception)
	{
		Logger::log(exception);
	}
	catch (...)
	{
		Logger::log(std::exception("Unexcepected error"));
	}

	return 0;
}
