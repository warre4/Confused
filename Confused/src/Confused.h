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

// Start class and define constructors
#define CREATE_APPLICATION(appName, appNameStr)					\
class appName final : public _CF Application					\
{																\
	public:														\
		appName() : Confused::Application(appNameStr) {}		\
		~appName() {}											\
	private:													\
		void Initialize() override;								\
		void Cleanup() override;								\
};																\
_CF Application* Confused::CreateApplication()					\
{																\
	return new appName();										\
}