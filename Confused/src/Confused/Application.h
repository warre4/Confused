#pragma once
#include "Core.h"

namespace Confused
{
	class CONFUSED_API Application
	{
	public:
		Application();
		~Application();

		void Run();
	};

	// Defined in client
	Application* CreateApplication();
}