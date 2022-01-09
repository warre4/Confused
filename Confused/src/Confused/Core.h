#pragma once

#ifdef CF_PLATFORM_WINDOWS
	#ifdef CF_BUILD_DLL
		#define CONFUSED_API __declspec(dllexport)
	#else
		#define CONFUSED_API _declspec(dllimport)
	#endif
#else
	#error Confused Engine doesn't support this platform
#endif

#pragma warning( disable : 4251 ) // needs to have dll-interface to be used by clients of class