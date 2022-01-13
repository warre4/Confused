module;
#include "Macros.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
export module Confused.Window;

#pragma warning( disable : 4251 ) // needs to have dll-interface to be used by clients of class

namespace Confused
{
	export struct CONFUSED_API WindowProps final
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;
		bool IsResizable;

		WindowProps(const std::string& title = "Confused Engine", uint32_t width = 1600, uint32_t height = 900, bool isResizable = false)
			: Title{ title }
			, Width{ width }
			, Height{ height }
			, IsResizable{ isResizable }
		{
		}
	};

	export class CONFUSED_API Window final
	{
	public:
		Window(const WindowProps& props)
			: m_Props{ props }
		{
			// Create window
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
			glfwWindowHint(GLFW_RESIZABLE, props.IsResizable);

			m_pWindow = glfwCreateWindow(props.Width, props.Height, props.Title.c_str(), nullptr, nullptr);
		}

		~Window()
		{
			// Destroy window
			glfwDestroyWindow(m_pWindow);
		}

		void Update()
		{
			glfwPollEvents();
		}

		// Getters & Setters

		inline bool GetShouldClose() const { return glfwWindowShouldClose(m_pWindow); }

		inline GLFWwindow* GetWindow() const { return m_pWindow; }
		inline uint32_t GetWidth() const { return m_Props.Width; }
		inline uint32_t GetHeight() const { return m_Props.Height; }
		inline const std::string& GetTitle() const { return m_Props.Title; }
		inline const WindowProps& GetProperties() const { return m_Props; }

		void SetTitle(const std::string& title)
		{
			m_Props.Title = title;
			glfwSetWindowTitle(m_pWindow, title.c_str());
		}

	private:
		// Variables

		GLFWwindow* m_pWindow;
		WindowProps m_Props;
	};
}