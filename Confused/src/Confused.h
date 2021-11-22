#pragma once

// For use by Confused applications

import Confused.Application;
#include "Confused/Logger.h" // import + macro definitions

#include "spdlog/spdlog.h"

#include <memory> // shared_ptr Confused::Logger::Initialize()

//----------Entry Point----------
#include "Confused/EntryPoint.h"
//-------------------------------

#define APPLICATION_BEGIN(applicationName) class applicationName final : public Confused::Application {
#define APPLICATION_END(applicationName) }; Confused::Application* Confused::CreateApplication() { return new applicationName(); }