module;
#include "Core.h"
#include "Logger.h"
#include <iostream>
export module Confused.Application;

namespace Confused
{
	export class CONFUSED_API Application
	{
	public:
		//
		Application()
		{

		}

		virtual ~Application()
		{

		}

		//
		virtual void Initialize() = 0;
		virtual void Cleanup() = 0;
		
		void Run();

	private:
		void InitializeCore();
		void CleanupCore();
	};

	// Defined in client
	export Application* CreateApplication();
}