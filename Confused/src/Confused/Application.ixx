module;
#include "Macros.h"
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
		
		void Run();

	protected:
		virtual void Initialize() = 0;
		virtual void Cleanup() = 0;

	private:
		void InitializeCore();
		void CleanupCore();
	};

	// Defined in client
	export Application* CreateApplication();
}