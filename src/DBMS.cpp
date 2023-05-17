#include "SafeWindows.h"

#include "App.h"
#include "Exception.h"

int WINAPI wWinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	PWSTR pCmdLine,
	int nCmdShow)
{
	try
	{
		App{}.Run();
	}
	catch (const Exception& e)
	{
		MessageBoxA(nullptr, e.what(), "Custom exception.", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& e)
	{
		MessageBoxA(nullptr, e.what(), "Standart exception.", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		MessageBoxA(nullptr, "", "Unknown exception.", MB_OK | MB_ICONEXCLAMATION);
	}
	return 0;
}
