module;
#include "Macros.h"
export module Confused.Renderer;

import Confused.Singleton;

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

			CORE_LOGD("OoOoOoOoH VuLkAn o_o");
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