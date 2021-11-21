module;
#include "Core.h"
export module Confused.Application;

import <iostream>;
//import <thread>;

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
		void Run()
		{
			bool shouldQuit = false;

			std::cout << "\nConfused Engine started running :D\n\n";

			while (!shouldQuit)
			{
				shouldQuit = true;
			}

			std::cout << "Confused Engine stopped running...\n";
			std::cin.get();
		}

	private:

	};

	// Defined in client
	export Application* CreateApplication();
}