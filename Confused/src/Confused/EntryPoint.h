#pragma once

#ifdef CF_PLATFORM_WINDOWS

#ifdef _DEBUG
	#include <vld/vld.h>
#endif

extern Confused::Application* Confused::CreateApplication();

int main(int /*argc*/, char** /*argv*/)
{
	Confused::Logger::Initialize();
	CF_CORE_INFO("Initialized Logger!");

	Confused::Application* pApplication = Confused::CreateApplication();

	pApplication->Run();

	delete pApplication;
	pApplication = nullptr;
}

#endif