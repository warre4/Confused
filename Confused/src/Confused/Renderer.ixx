module;
#include "Macros.h"

#include <vector>

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

			// Vulkan
			CreateInstance();
		}
		void Cleanup()
		{
			CORE_LOGT("Renderer cleaning up");

			// Vulkan
			vkDestroyInstance(m_VkInstance, nullptr);
		}

		void Render() const
		{

		}

		// Getters & Setters
		inline Window* GetWindow() { return m_pWindow; }

		inline void SetWindow(Window* pWindow) { m_pWindow = pWindow; }

	private:
		// Helpers

		void CreateInstance()
		{
			// Application Info
			VkApplicationInfo appInfo{};
			appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
			appInfo.pApplicationName = "Hello Triangle";
			appInfo.applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
			appInfo.pEngineName = "Confused";
			appInfo.engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
			appInfo.apiVersion = VK_API_VERSION_1_0;

			// Instance Create Info
			uint32_t glfwExtensionCount = 0;
			const char** glfwExtensions;
			glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

			if (glfwExtensions == nullptr)
				CORE_EXCEPTION("glfwGetRequiredInstanceExtensions failed");

			VkInstanceCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			createInfo.pApplicationInfo = &appInfo;

			createInfo.enabledExtensionCount = glfwExtensionCount;
			createInfo.ppEnabledExtensionNames = glfwExtensions;

			createInfo.enabledLayerCount = 0;

			// Create instance
			VkResult result = vkCreateInstance(&createInfo, nullptr, &m_VkInstance);

			if (result != VK_SUCCESS)
				CORE_EXCEPTION_VKRES("vkCreateInstance failed", result);
			
			// Print Info
			PrintRequiredInstanceExtensions(glfwExtensions, glfwExtensionCount);
			PrintExtensionPropertiesInfo();
		}

		void PrintRequiredInstanceExtensions(const char** arr, uint32_t count)
		{
			CORE_LOGD("");

			CORE_LOGD("Required instance extensions(" + std::to_string(count) + "):");
			for (uint32_t i = 0; i < count; i++)
				CORE_LOGD(std::string("\t") + arr[i]);

			CORE_LOGD("");
		}

		void PrintExtensionPropertiesInfo()
		{
			uint32_t extensionCount = 0;
			vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
			std::vector<VkExtensionProperties> extensions(extensionCount);
			vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

			CORE_LOGD("");

			CORE_LOGD("Available extensions(" + std::to_string(extensionCount) + "):");
			for (const auto& extension : extensions)
				CORE_LOGD(std::string("\t") + extension.extensionName);

			CORE_LOGD("");
		}

		// Variables
		Window* m_pWindow;
		VkInstance m_VkInstance;
	};
}