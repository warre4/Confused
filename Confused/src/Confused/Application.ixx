module;
#include "Macros.h"
export module Confused.Application;

import Confused.Window;

#pragma warning( disable : 4251 ) // needs to have dll-interface to be used by clients of class

namespace Confused
{
	export class CONFUSED_API Application
	{
	public:
		Application(std::string name)
			: m_Name{ std::move(name) }
			, m_pMainWindow{ nullptr }
		{

		}
		virtual ~Application()
		{

		}

		Application(const Application& other) = delete;
		Application(Application&& other) = delete;
		Application& operator=(const Application& other) = delete;
		Application& operator=(Application&& other) = delete;
		
		void Run();

		// Helpers
		inline const std::string& GetName() { return m_Name; }

	protected:
		virtual void Initialize() = 0;
		virtual void Cleanup() = 0;

	private:
		void InitializeCore();
		void CleanupCore();

		// Variables
		Window* m_pMainWindow;
		std::string m_Name;
	};

	// Defined in client
	export Application* CreateApplication();
}