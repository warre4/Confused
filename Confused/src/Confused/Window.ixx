module;
#include "Macros.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
export module Confused.Window;

#pragma warning( disable : 4251 ) // needs to have dll-interface to be used by clients of class

namespace Confused
{
	export class CONFUSED_API Window final
	{
	public:
		Window(std::string appName, uint32_t width, uint32_t height, bool isResizable = false)
			: m_AppName{ std::move(appName) }
		{
			m_WindowCounter++;
			if (m_WindowCounter > 1)
				CORE_EXCEPTION("A maximum of 1 window is supported");

			// Initialize
			glfwInit();

			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
			glfwWindowHint(GLFW_RESIZABLE, isResizable);

			// Create window
			m_pWindow = glfwCreateWindow(width, height, m_AppName.c_str(), nullptr, nullptr);
		}
		~Window()
		{
			// Destroy window
			glfwDestroyWindow(m_pWindow);

			// Cleanup
			glfwTerminate();
		}

		// returns true if window should close
		bool Update()
		{
			glfwPollEvents();

			return glfwWindowShouldClose(m_pWindow);
		}

		inline GLFWwindow* GetWindow() const { return m_pWindow; }

		// Getters
		// inline const char* GetWindowName() const { return glfwsize; }

	private:
		GLFWwindow* m_pWindow;
		std::string m_AppName;

		static int m_WindowCounter;
	};

	int Confused::Window::m_WindowCounter = 0;
}