module;
#include "Macros.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <iostream>
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

			// ----------------------------------------------------------------------------------------
			// TEMPORARY (to test Vulkan)
			// ----------------------------------------------------------------------------------------
			glfwInit();

			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
			GLFWwindow* window = glfwCreateWindow(800, 600, "Vulkan window", nullptr, nullptr);

			uint32_t extensionCount = 0;
			vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

			std::cout << extensionCount << " extensions supported\n";

			glm::mat4 matrix;
			glm::vec4 vec;
			auto test = matrix * vec;

			while (!glfwWindowShouldClose(window)) {
				glfwPollEvents();
			}

			glfwDestroyWindow(window);

			glfwTerminate();
			// ----------------------------------------------------------------------------------------
			// TEMPORARY (to test Vulkan)
			// ----------------------------------------------------------------------------------------
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