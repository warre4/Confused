#pragma once

#pragma warning( disable : 4251 ) // needs to have dll-interface to be used by clients of class

// For use by Confused applications
#include "Confused/Macros.h"

import Confused.Application;

//----------Entry Point----------
#include "Confused/EntryPoint.h"
//-------------------------------

#define APPLICATION_BEGIN(applicationName) class applicationName final : public Confused::Application {
#define APPLICATION_END(applicationName) }; Confused::Application* Confused::CreateApplication() { return new applicationName(); }