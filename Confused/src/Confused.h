#pragma once

#pragma warning( disable : 4251 ) // needs to have dll-interface to be used by clients of class

// For use by Confused applications
#include "Confused/Logger.h"
#include "Confused/General.h"

import Confused.Application;
import Confused.Utils;

//----------Entry Point----------
#include "Confused/EntryPoint.h"
//-------------------------------

#define APPLICATION_BEGIN(applicationName) class applicationName final : public _CONFUSED Application {
#define APPLICATION_END(applicationName) }; _CONFUSED Application* Confused::CreateApplication() { return new applicationName(); }