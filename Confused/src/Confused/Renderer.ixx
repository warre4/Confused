module;
#include "Macros.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
export module Confused.Renderer;

import Confused.Singleton;
import Confused.Window;

#pragma warning( disable : 4251 ) // needs to have dll-interface to be used by clients of class

namespace Confused
{
	export class CONFUSED_API Renderer final : public Singleton<Renderer>
	{
	public:
		Renderer() = default;
		~Renderer() = default;

		void Initialize(Window* pWindow)
		{
			SetWindow(pWindow);
			CORE_LOGT("Renderer initializing");


		}
		void Cleanup()
		{
			CORE_LOGT("Renderer cleaning up");


		}

		void Render() const
		{

		}

		// Getters & Setters
		inline void SetWindow(Window* pWindow) { m_pWindow = pWindow; }

		// Helpers


	private:


		// Variables
		Window* m_pWindow;
	};
}