#pragma once

#pragma warning( disable : 4251 ) // needs to have dll-interface to be used by clients of class

#include "Logger.h"

#include <string>
#include <stdexcept>

// ----------------------------------------------------------------------------------------------
// GENERAL
// ----------------------------------------------------------------------------------------------

#define UTILS Confused::Utils::GetInstance()
#define TIME Confused::Time::GetInstance()
#define RENDERER Confused::Renderer::GetInstance()

#define STR(s) std::string(s)
#define WSTR(ws) std::wstring(ws)


// ----------------------------------------------------------------------------------------------
// LOGGING
// ----------------------------------------------------------------------------------------------

// Core logging

//Log Trace
#define CORE_LOGT(...)        Confused::Logger::GetCoreLogger()->trace(__VA_ARGS__)
//Log Debug
#define CORE_LOGD(...)        Confused::Logger::GetCoreLogger()->debug(__VA_ARGS__)
//Log Info
#define CORE_LOGI(...)        Confused::Logger::GetCoreLogger()->info(__VA_ARGS__)
#define CORE_LOGWARNING(...)  Confused::Logger::GetCoreLogger()->warn(__VA_ARGS__)
#define CORE_LOGERROR(...)    Confused::Logger::GetCoreLogger()->error(__VA_ARGS__)
#define CORE_LOGCRITICAL(...) Confused::Logger::GetCoreLogger()->critical(__VA_ARGS__)


// Client logging

//Log Trace
#define LOGT(...)        Confused::Logger::GetClientLogger()->trace(__VA_ARGS__)
//Log Debug
#define LOGD(...)        Confused::Logger::GetClientLogger()->debug(__VA_ARGS__)
//Log Info
#define LOGI(...)        Confused::Logger::GetClientLogger()->info(__VA_ARGS__)
#define LOGWARNING(...)  Confused::Logger::GetClientLogger()->warn(__VA_ARGS__)
#define LOGERROR(...)    Confused::Logger::GetClientLogger()->error(__VA_ARGS__)
#define LOGCRITICAL(...) Confused::Logger::GetClientLogger()->critical(__VA_ARGS__)


// ----------------------------------------------------------------------------------------------
// EXCEPTIONS
// ----------------------------------------------------------------------------------------------
#define CORE_EXCEPTION(message) throw std::exception((std::string("Confused: ") + message).c_str())
#define CORE_EXCEPTION_VKRES(message, vkResult) throw std::runtime_error((std::string("Confused: ") + message + " \nError code: " + std::to_string((int)vkResult)).c_str())

#define EXCEPTION(message) throw std::exception((std::string("App: ") + message).c_str())


// ----------------------------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------------------------

