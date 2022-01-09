#pragma once

#ifdef CF_PLATFORM_WINDOWS

#ifdef _DEBUG
	#include <vld/vld.h>
#endif

#include <stdexcept>
#include <cstdlib>

extern Confused::Application* Confused::CreateApplication();

int main(int /*argc*/, char** /*argv*/)
{
#ifdef _DEBUG
	try
	{
#endif

	Confused::Application* pApplication = Confused::CreateApplication();

	pApplication->Run();

	delete pApplication;
	pApplication = nullptr;

#if _DEBUG
	}
	catch (std::exception& e)
	{
		int msgboxID = MessageBoxA(NULL, e.what(), "Exception", MB_ICONERROR | MB_OK);
		if (msgboxID == IDOK)
			return EXIT_FAILURE;
	}
	catch (...)
	{
		int msgboxID = MessageBoxA(NULL, "Exception of an undetermined type", "Exception", MB_ICONERROR | MB_OK);
		if (msgboxID == IDOK)
			return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
#endif
}

#endif