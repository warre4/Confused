
// For use by Confused applications
import Confused.Application;

//----------Entry Point----------
#include "Confused/EntryPoint.h"
//-------------------------------

#define APPLICATION_BEGIN(applicationName) class applicationName final : public Confused::Application {
#define APPLICATION_END(applicationName) }; Confused::Application* Confused::CreateApplication() { return new applicationName(); }