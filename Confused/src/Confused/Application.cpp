#include "Application.h"
#include <iostream>

namespace Confused
{
	Application::Application()
	{

	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		std::cout << "Confused Engine started running :D\n\n";
		std::cout << 'R';

		while (true)
		{
			std::cout << 'E';
		}
	}
}