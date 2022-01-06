#pragma once

#ifdef CF_PLATFORM_WINDOWS

#include <iostream>
#ifdef _DEBUG
	#include <vld/vld.h>
#endif

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
			return 0;
	}
	catch (...)
	{
		int msgboxID = MessageBoxA(NULL, "Exception of an undetermined type", "Exception", MB_ICONERROR | MB_OK);
		if (msgboxID == IDOK)
			return 0;
	}
#endif
}

#endif