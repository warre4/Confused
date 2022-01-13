module;
#include "Macros.h"

#include <vector>
#include <algorithm>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
export module Confused.Renderer;

import Confused.Singleton;
import Confused.Window;
import Confused.Utils;

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
			// Validation layers
			if (m_EnableValidationLayers)
			{
				PrintStrings(m_ValidationLayers, "Required validation layers");
				CheckValidationLayerSupport();
			}

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
			
			// Check and print extensions
			std::vector<std::string> requiredExtensions = UTILS.ToVec<std::string>(glfwExtensions, glfwExtensionCount);
			std::vector<std::string> supportedExtensions = GetSupportedExtensions();

			PrintStrings(requiredExtensions, "Required extensions");
			//PrintStrings(supportedExtensions, "Supported extensions");

			CheckExtentionSupport(supportedExtensions, requiredExtensions);
		}

#pragma region VkExtensionInfo
		std::vector<std::string> GetSupportedExtensions()
		{
			uint32_t extensionCount = 0;
			vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
			std::vector<VkExtensionProperties> extensions{ extensionCount };
			vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

			std::vector<std::string> supportedExtensions{ extensionCount };

			for (uint32_t i = 0; i < extensionCount; i++)
			{
				supportedExtensions[i] = extensions[i].extensionName;
			}

			return std::move(supportedExtensions);
		}

		void CheckExtentionSupport(const std::vector<std::string>& supportedExtensions, const std::vector<std::string>& requiredExtensions) const
		{
			bool isEverythingSupported = true;
			for (const std::string& requiredExt : requiredExtensions)
			{
				if (std::find(supportedExtensions.cbegin(), supportedExtensions.cend(), requiredExt) == supportedExtensions.cend())
				{
					isEverythingSupported = false;
					CORE_LOGERROR("Extension not supported: " + requiredExt);
				}
			}

			if (isEverythingSupported)
				CORE_LOGI("All required extensions are supported");
			else
				CORE_LOGCRITICAL("Not all required extentions are supported");
		}
#pragma endregion VkExtensionInfo

#pragma region VkValidationLayers
		void CheckValidationLayerSupport() const
		{
			uint32_t layerCount;
			vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

			std::vector<VkLayerProperties> availableLayers(layerCount);
			vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

			bool isEverythingSupported = true;

			for (const std::string& layerName : m_ValidationLayers)
			{
				bool layerFound = false;

				for (const auto& layerProperties : availableLayers)
				{
					if (strcmp(layerName.c_str(), layerProperties.layerName) == 0)
					{
						layerFound = true;
						break;
					}
				}

				if (!layerFound)
				{
					isEverythingSupported = false;
					CORE_LOGERROR("Validation layer not supported: " + std::string(layerName));
				}
			}

			if (isEverythingSupported)
				CORE_LOGI("All required validation layers are supported");
			else
				CORE_LOGCRITICAL("Not all required validation layers are supported");
		}
#pragma endregion VkValidationLayers

		void PrintStrings(const std::vector<std::string>& vec, const std::string& description) const
		{
			CORE_LOGD("");

			CORE_LOGD(description + "(" + std::to_string(vec.size()) + "):");
			for (const std::string& element : vec)
				CORE_LOGD("\t" + element);

			CORE_LOGD("");
		}

		// Variables

		Window* m_pWindow;
		VkInstance m_VkInstance;

#ifdef NDEBUG
		const bool m_EnableValidationLayers = false;
#else
		const bool m_EnableValidationLayers = true;
#endif
		const std::vector<std::string> m_ValidationLayers = { "VK_LAYER_KHRONOS_validation" };
	};
}