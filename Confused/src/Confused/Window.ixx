module;
#include "Logger.h"
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

		Window(const Window& other) = delete;
		Window(Window&& other) noexcept = delete;
		Window& operator=(const Window& other) = delete;
		Window& operator=(Window&& other) noexcept = delete;

		void Update()
		{
			glfwPollEvents();
		}

		// Vulkan

		VkResult CreateSurface(VkInstance instance, VkSurfaceKHR* pSurface, const VkAllocationCallbacks* pCallback = nullptr)
		{
			return glfwCreateWindowSurface(instance, m_pWindow, pCallback, pSurface);
		}

		void GetFramebufferSize(int* width, int* height)
		{
			glfwGetFramebufferSize(m_pWindow, width, height);
		}

		// Getters & Setters

		[[nodiscard]] inline bool GetShouldClose() const { return glfwWindowShouldClose(m_pWindow); }

		[[nodiscard]] inline const GLFWwindow* GetWindow() const noexcept { return m_pWindow; }
		[[nodiscard]] inline uint32_t GetWidth() const noexcept { return m_Props.Width; }
		[[nodiscard]] inline uint32_t GetHeight() const noexcept { return m_Props.Height; }
		[[nodiscard]] inline const std::string& GetTitle() const noexcept { return m_Props.Title; }
		[[nodiscard]] inline const WindowProps& GetProperties() const noexcept { return m_Props; }

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