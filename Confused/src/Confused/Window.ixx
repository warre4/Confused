module;
#include "Macros.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
export module Confused.Window;

#pragma warning( disable : 4251 ) // needs to have dll-interface to be used by clients of class

namespace Confused
{
	export class CONFUSED_API Window final
	{
	public:
		Window(const std::string& appName, uint32_t width, uint32_t height/*, bool isResizable = false*/)
			: m_AppName{ appName }
			, m_Width{ width }
			, m_Height{ height }
			, m_Title{ appName + " | " + std::to_string(width) + " x " + std::to_string(height) }
		{
			m_WindowCounter++;
			if (m_WindowCounter > 1)
				CORE_EXCEPTION("A maximum of 1 window is supported");

			// Initialize
			glfwInit();

			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
			glfwWindowHint(GLFW_RESIZABLE, false);

			// Create window
			m_pWindow = glfwCreateWindow(width, height, m_Title.c_str(), nullptr, nullptr);
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

		// Getters & Setters
		inline GLFWwindow* GetWindow() const { return m_pWindow; }
		inline uint32_t GetWidth() const { return m_Width; }
		inline uint32_t GetHeight() const { return m_Height; }
		inline const std::string& GetTitle() const { return m_Title; }

		void SetTitle(const std::string& title)
		{
			m_Title = title;
			glfwSetWindowTitle(m_pWindow, title.c_str());
		}

	private:
		// Variables
		GLFWwindow* m_pWindow;
		std::string m_AppName;
		std::string m_Title;
		uint32_t m_Width;
		uint32_t m_Height;

		// Static Variables
		static int m_WindowCounter;
	};

	int Confused::Window::m_WindowCounter = 0;
}