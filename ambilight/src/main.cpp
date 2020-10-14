#if DEBUG
#include <iostream>
#else
#include <fstream>
#endif

#include "Ambilight.h"

void log(const std::exception& exception)
{
#if DEBUG
	std::cout << exception.what() << std::endl;
	system("pause");
#else
	std::ofstream file;
	file.open("log.txt", std::ofstream::out | std::ofstream::app);
	file << exception.what() << std::endl;
	file.close();
#endif
}

#if DEBUG
int main()
#else
int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
#endif
{
	try
	{
		Ambilight ambilight;
		ambilight.start();
	}
	catch (std::exception & exception)
	{
		log(exception);
	}
	catch (...)
	{
		log(std::exception("Unexcepected error"));
	}

	return 0;
}
