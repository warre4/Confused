#pragma once
#include "spdlog/spdlog.h"
#include <memory>
import Confused.Logger;

// Core logging
#define CF_CORE_TRACE(...)    Confused::Logger::GetCoreLogger()->trace(__VA_ARGS__)
#define CF_CORE_DEBUG(...)    Confused::Logger::GetCoreLogger()->debug(__VA_ARGS__)
#define CF_CORE_INFO(...)     Confused::Logger::GetCoreLogger()->info(__VA_ARGS__)
#define CF_CORE_WARN(...)     Confused::Logger::GetCoreLogger()->warn(__VA_ARGS__)
#define CF_CORE_ERROR(...)    Confused::Logger::GetCoreLogger()->error(__VA_ARGS__)
#define CF_CORE_CRITICAL(...) Confused::Logger::GetCoreLogger()->critical(__VA_ARGS__)

// Client logging		   
#define CF_TRACE(...)         Confused::Logger::GetClientLogger()->trace(__VA_ARGS__)
#define CF_DEBUG(...)         Confused::Logger::GetClientLogger()->debug(__VA_ARGS__)
#define CF_INFO(...)          Confused::Logger::GetClientLogger()->info(__VA_ARGS__)
#define CF_WARN(...)          Confused::Logger::GetClientLogger()->warn(__VA_ARGS__)
#define CF_ERROR(...)         Confused::Logger::GetClientLogger()->error(__VA_ARGS__)
#define CF_CRITICAL(...)      Confused::Logger::GetClientLogger()->critical(__VA_ARGS__)