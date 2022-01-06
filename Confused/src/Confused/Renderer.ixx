module;
#include "Core.h"
#include "Macros.h"
import Confused.Singleton;
export module Confused.Renderer;

namespace Confused
{
	export class CONFUSED_API Renderer final : public Singleton<Renderer>
	{
	public:
		Renderer() = default;
		~Renderer() = default;

		void Initialize()
		{
			CORE_LOGT("Renderer initializing");


		}
		void Cleanup()
		{
			CORE_LOGT("Renderer cleaning up");


		}

		void Render() const
		{

		}

		// Helpers


	private:

	};
}