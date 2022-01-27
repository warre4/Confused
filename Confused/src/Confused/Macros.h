#pragma once

#pragma warning( disable : 4251 ) // needs to have dll-interface to be used by clients of class

#include "Core.h" // makes sure IS_CORE is correctly defined

#include <cstdint>
#include <string>
#include <stdexcept>

// ----------------------------------------------------------------------------------------------
// NAMESPACES
// ----------------------------------------------------------------------------------------------

#define _SPDLOG ::spdlog::
#define _LOGLEVEL ::spdlog::level::

#define _CONFUSED ::Confused::


// ----------------------------------------------------------------------------------------------
// GENERAL
// ----------------------------------------------------------------------------------------------

#define UTILS _CONFUSED Utils::GetInstance()
#define TIME _CONFUSED Time::GetInstance()
#define RENDERER _CONFUSED Renderer::GetInstance()

#define STR(s) std::string(s)
#define WSTR(ws) std::wstring(ws)

// ----------------------------------------------------------------------------------------------
// LOGGING
// ----------------------------------------------------------------------------------------------

// Core logging

#ifdef IS_CORE

//Pointer to the logger
#define LOGGER _CONFUSED Logger::GetCoreLogger()

//Shows info pop-up with ok button
//Pauses program!
#define LOGINFOBOX(message) MessageBoxA(NULL, STR(message).c_str(), "Confused", MB_ICONINFORMATION | MB_OK)
#define LOGBOX(message, flags) MessageBoxA(NULL, STR(message).c_str(), "Confused", flags)

#else

//Pointer to the logger
#define LOGGER _CONFUSED Logger::GetClientLogger()

#endif

//Log trace
#define LOGT(...)        LOGGER->trace(__VA_ARGS__)
//Log debug
#define LOGD(...)        LOGGER->debug(__VA_ARGS__)
//Log info
#define LOGI(...)        LOGGER->info(__VA_ARGS__)
#define LOGWARN(...)     LOGGER->warn(__VA_ARGS__)
#define LOGERROR(...)    LOGGER->error(__VA_ARGS__)
#define LOGCRITICAL(...) LOGGER->critical(__VA_ARGS__)

#define LOG(level, ...) LOGGER->log(level, __VA_ARGS__)


// ----------------------------------------------------------------------------------------------
// EXCEPTIONS
// ----------------------------------------------------------------------------------------------

#ifdef IS_CORE

	#define EXCEPTION(message) throw std::exception((    STR("Confused: ") + message).c_str())
	//Runtime error
	#define RTE(message)       throw std::runtime_error((STR("Confused: ") + message).c_str())

#else

	#define EXCEPTION(message)      throw std::exception((    STR("App: ") + message).c_str())
	//Runtime error
	#define RTE(message)            throw std::runtime_error((STR("App: ") + message).c_str())

#endif


// ----------------------------------------------------------------------------------------------
// VULKAN
// ----------------------------------------------------------------------------------------------

// Put this over a function that produces a VkResult to check the result
#define CHECK(vkResult, message)                                                                \
{                                                                                               \
	VkResult temporaryResultInMacro = vkResult;                                                 \
	if (temporaryResultInMacro != VK_SUCCESS)                                                   \
		RTE(STR(message) + " \nError: " + string_VkResult(temporaryResultInMacro));             \
}


// ----------------------------------------------------------------------------------------------
// TYPEDEFS
// ----------------------------------------------------------------------------------------------

