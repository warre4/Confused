module;
#include "Core.h"
#include "Logger.h"
export module Confused.Application;

import <iostream>;

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
		virtual void Run()
		{
			bool shouldQuit = false;

			CF_CORE_INFO("Confused Engine started running :D");

			while (!shouldQuit)
			{
				shouldQuit = true;
			}

			CF_CORE_INFO("Confused Engine stopped running...");
			std::cin.get();
		}

	private:

	};

	// Defined in client
	export Application* CreateApplication();
}