module;
#include "Macros.h"

#include <string>
#include <cstring>
#include <vector>
#include <algorithm>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "vulkan/vk_enum_string_helper.h"

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
			SetupDebugMessenger();
		}
		void Cleanup()
		{
			CORE_LOGT("Renderer cleaning up");

			// Vulkan
			if (m_EnableValidationLayers)
				DestroyDebugUtilsMessengerEXT(m_VkInstance, m_DebugMessenger, nullptr);
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
				PrintStrings(UTILS.ChangeType<std::string>(m_ValidationLayers), "Required validation layers");
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
			VkInstanceCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			createInfo.pApplicationInfo = &appInfo;

			std::vector<const char*> requiredExtensionsCStr = GetRequiredExtensions();
			std::vector<std::string> requiredExtensions = UTILS.ChangeType<std::string>(requiredExtensionsCStr);
			std::vector<std::string> supportedExtensions = GetSupportedExtensions();
			PrintStrings(requiredExtensions, "Required extensions");
			//PrintStrings(supportedExtensions, "Supported extensions");
			CheckExtentionSupport(supportedExtensions, requiredExtensions);

			createInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensionsCStr.size());
			createInfo.ppEnabledExtensionNames = requiredExtensionsCStr.data();

			VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
			if (m_EnableValidationLayers)
			{
				createInfo.enabledLayerCount = static_cast<uint32_t>(m_ValidationLayers.size());
				createInfo.ppEnabledLayerNames = m_ValidationLayers.data();

				PopulateDebugMessengerCreateInfo(debugCreateInfo);
				createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
			}
			else
			{
				createInfo.enabledLayerCount = 0;

				createInfo.pNext = nullptr;
			}

			// Create instance
			CHECK(vkCreateInstance(&createInfo, nullptr, &m_VkInstance), "vkCreateInstance failed");
		}

#pragma region VkExtensions
		std::vector<std::string> GetSupportedExtensions()
		{
			uint32_t extensionCount = 0;
			CHECK(vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr), "vkEnumerateInstanceExtensionProperties failed");
			std::vector<VkExtensionProperties> extensions{ extensionCount };
			CHECK(vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data()), "vkEnumerateInstanceExtensionProperties failed");

			std::vector<std::string> supportedExtensions{ extensionCount };

			for (uint32_t i = 0; i < extensionCount; i++)
			{
				supportedExtensions[i] = extensions[i].extensionName;
			}

			return std::move(supportedExtensions);
		}
		std::vector<const char*> GetRequiredExtensions()
		{
			// GLFW
			uint32_t glfwExtensionCount = 0;
			const char** glfwExtensions;
			glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

			if (glfwExtensions == nullptr)
				CORE_RTE("glfwGetRequiredInstanceExtensions failed");

			std::vector<const char*> requiredExtensions{ glfwExtensions, glfwExtensions + glfwExtensionCount };
			
			// Debug logging
			if (m_EnableValidationLayers)
				requiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);

			return std::move(requiredExtensions);
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
#pragma endregion VkExtensions

#pragma region VkValidationLayers
		void CheckValidationLayerSupport() const
		{
			// Get supported validation layers
			uint32_t supportedLayerCount = 0;
			CHECK(vkEnumerateInstanceLayerProperties(&supportedLayerCount, nullptr), "vkEnumerateInstanceLayerProperties failed");
			std::vector<VkLayerProperties> supportedLayers{ supportedLayerCount };
			CHECK(vkEnumerateInstanceLayerProperties(&supportedLayerCount, supportedLayers.data()), "vkEnumerateInstanceLayerProperties failed");

			// Check if all required layers are supported
			bool isEverythingSupported = true;

			for (const char* requiredLayerName : m_ValidationLayers)
			{
				bool layerFound = false;

				for (const VkLayerProperties& supportedLayer : supportedLayers)
				{
					if (strcmp(requiredLayerName, supportedLayer.layerName) == 0)
					{
						layerFound = true;
						break;
					}
				}

				if (!layerFound)
				{
					isEverythingSupported = false;
					CORE_LOGERROR(STR("Validation layer not supported: ") + requiredLayerName);
				}
			}

			if (isEverythingSupported)
				CORE_LOGI("All required validation layers are supported");
			else
				CORE_LOGCRITICAL("Not all required validation layers are supported");
		}
#pragma endregion VkValidationLayers

#pragma region DebugUtilsMessengerEXT
		static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity
			, VkDebugUtilsMessageTypeFlagsEXT messageType
			, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData
			, void* pUserData /*Ptr to Renderer*/)
		{
			std::string type{ string_VkDebugUtilsMessageTypeFlagsEXT(messageType) };
			type = std::string{ &type[28], &type[type.find("_BIT_EXT")] };

			std::string header{ "Validation layer(" + type + "):" };
			std::string message{ pCallbackData->pMessage };

			switch (messageSeverity)
			{
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
				CORE_LOGT(header);
				CORE_LOGT(message);
				CORE_LOGT("");
				break;
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
				CORE_LOGT(header);
				CORE_LOGT(message);
				CORE_LOGT("");
				break;
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
				CORE_LOGERROR(header);
				CORE_LOGERROR(message);
				CORE_LOGERROR("");
				break;
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
				CORE_LOGCRITICAL(header);
				CORE_LOGCRITICAL(message);
				CORE_LOGCRITICAL("");
				break;
			default:
				CORE_LOGCRITICAL("Unknown callback severity! Message: ");
				CORE_LOGCRITICAL(header);
				CORE_LOGCRITICAL(message);
				CORE_LOGCRITICAL("");
				break;
			}

			UNREFERENCED_PARAMETER(pUserData);

			return VK_FALSE;
		}

		static VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
		{
			auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
			if (func != nullptr)
			{
				return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
			}
			else
			{
				return VK_ERROR_EXTENSION_NOT_PRESENT;
			}
		}

		static void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
		{
			auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
			if (func != nullptr)
			{
				func(instance, debugMessenger, pAllocator);
			}
		}

		void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
		{
			createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
			createInfo.messageSeverity = /*VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |*/ /*VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |*/ VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
			createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
			createInfo.pfnUserCallback = DebugCallback;
			createInfo.pUserData = this;
		}

		void SetupDebugMessenger()
		{
			if (!m_EnableValidationLayers)
				return;

			VkDebugUtilsMessengerCreateInfoEXT createInfo;
			PopulateDebugMessengerCreateInfo(createInfo);

			CHECK(CreateDebugUtilsMessengerEXT(m_VkInstance, &createInfo, nullptr, &m_DebugMessenger), "Failed to set up debug messenger!");
		}

#pragma endregion DebugUtilsMessengerEXT

		static void PrintStrings(const std::vector<std::string>& vec, const std::string& description)
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

		VkDebugUtilsMessengerEXT m_DebugMessenger;

#ifdef NDEBUG
		const bool m_EnableValidationLayers = false;
#else
		const bool m_EnableValidationLayers = true;
#endif
		const std::vector<const char*> m_ValidationLayers = { "VK_LAYER_KHRONOS_validation" };
	};
}