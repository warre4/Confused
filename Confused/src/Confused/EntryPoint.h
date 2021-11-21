#pragma once

#ifdef CF_PLATFORM_WINDOWS

extern Confused::Application* Confused::CreateApplication();

int main(int /*argc*/, char** /*argv*/)
{
	Confused::Application* pApplication = Confused::CreateApplication();

	pApplication->Run();

	delete pApplication;
	pApplication = nullptr;
}

#endif