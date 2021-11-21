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