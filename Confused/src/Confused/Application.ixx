module;
#include "Core.h"
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

		~Application()
		{

		}

		//
		void Run()
		{
			std::cout << "Confused Engine started running :D\nNOW WITH MODULES! :DDDDDDD (fuck me o_o)\n\n";
			std::cout << 'R';

			while (true)
			{
				std::cout << 'E';
			}
		}
	};

	// Defined in client
	export Application* CreateApplication();
}