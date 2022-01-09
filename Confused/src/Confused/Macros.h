#pragma once

#pragma warning( push )
#pragma warning( disable : 4251 ) // Since this is just for logging, we can ignore this warning

#include "Logger.h"
#include <string>
#include <stdexcept>

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

#pragma warning( pop )


// ----------------------------------------------------------------------------------------------
// EXCEPTIONS
// ----------------------------------------------------------------------------------------------
#define CORE_EXCEPTION(message) throw std::exception((std::string("Confused: ") + message).c_str())

#define EXCEPTION(message) throw std::exception((std::string("App: ") + message).c_str())


// ----------------------------------------------------------------------------------------------
//