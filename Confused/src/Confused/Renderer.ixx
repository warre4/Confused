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

struct QueueFamilyIndices
{
	std::optional<uint32_t> graphicsFamily;

	bool IsComplete()
	{
		return graphicsFamily.has_value();
	}
};

namespace Confused
{
	export class CONFUSED_API Renderer final : public Singleton<Renderer>
	{
	public:
		Renderer() = default;
		~Renderer() = default;

		void Initialize(Window* pWindow)
		{
			LOGT("Renderer initializing");

			SetWindow(pWindow);
			InitializeVulkan();
		}
		void Cleanup()
		{
			LOGT("Renderer cleaning up");

			CleanupVulkan();
		}

		void Render() const
		{

		}

		// Getters & Setters

		inline Window* GetWindow() { return m_pWindow; }

		inline void SetWindow(Window* pWindow) { m_pWindow = pWindow; }

	private:

#pragma region Vulkan

#pragma region Initialize

		void InitializeVulkan()
		{
			CreateInstance();
			SetupDebugMessenger();

			PickPhysicalDevice();
			CreateLogicalDevice();
		}
		
		void CreateInstance()
		{
			// Validation layers
			if (m_EnableValidationLayers)
			{
				UTILS.PrintStrings(UTILS.ChangeType<std::string>(m_ValidationLayers), "Required validation layers", LOGGER);
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
			UTILS.PrintStrings(requiredExtensions, "Required extensions", LOGGER);
			//UTILS.PrintStrings(supportedExtensions, "Supported extensions");
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

		void PickPhysicalDevice()
		{
			uint32_t deviceCount = 0;
			CHECK(vkEnumeratePhysicalDevices(m_VkInstance, &deviceCount, nullptr), "vkEnumeratePhysicalDevices failed");

			if (deviceCount == 0)
				RTE("Failed to find GPUs with Vulkan support!");

			std::vector<VkPhysicalDevice> devices{ deviceCount };
			CHECK(vkEnumeratePhysicalDevices(m_VkInstance, &deviceCount, devices.data()), "vkEnumeratePhysicalDevices failed");

			uint32_t bestScore = 0;
			uint32_t currScore;
			for (const auto& device : devices)
			{
				currScore = RateDeviceSuitability(device);
				if (currScore > bestScore)
				{
					m_PhysicalDevice = device;
					bestScore = currScore;
				}
			}

			if (m_PhysicalDevice == VK_NULL_HANDLE)
				RTE("Failed to find a suitable GPU!");

			// Log name of selected device
			VkPhysicalDeviceProperties deviceProperties{};
			vkGetPhysicalDeviceProperties(m_PhysicalDevice, &deviceProperties);
			LOGI("Vulkan will use " + STR(deviceProperties.deviceName));
		}
		uint32_t RateDeviceSuitability(VkPhysicalDevice device)
		{
			QueueFamilyIndices indices = FindQueueFamilies(device);

			// Needs queue family
			if (!indices.IsComplete())
				return 0;

			VkPhysicalDeviceProperties deviceProperties;
			VkPhysicalDeviceFeatures deviceFeatures;
			vkGetPhysicalDeviceProperties(device, &deviceProperties);
			vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

			// Needs geometry shader
			if (!deviceFeatures.geometryShader)
				return 0;

			// Generate score
			uint32_t score = 0;

			if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
				score += 4000;

			score += deviceProperties.limits.maxImageDimension2D;

			//LOGT(deviceProperties.deviceName + STR(" scored ") + std::to_string(score));

			return score;
		}

		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device)
		{
			QueueFamilyIndices indices;

			uint32_t queueFamilyCount = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

			std::vector<VkQueueFamilyProperties> queueFamilies{ queueFamilyCount };
			vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

			int i = 0;
			for (const auto& queueFamily : queueFamilies)
			{
				if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
					indices.graphicsFamily = i;

				if (indices.IsComplete())
					break;

				i++;
			}

			return indices;
		}

		void CreateLogicalDevice()
		{
			// Queues
			QueueFamilyIndices indices = FindQueueFamilies(m_PhysicalDevice);

			VkDeviceQueueCreateInfo queueCreateInfo{};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
			queueCreateInfo.queueCount = 1;

			float queuePriority = 1.f;
			queueCreateInfo.pQueuePriorities = &queuePriority;

			// Features
			VkPhysicalDeviceFeatures deviceFeatures{};

			// Create logical device
			VkDeviceCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

			createInfo.pQueueCreateInfos = &queueCreateInfo;
			createInfo.queueCreateInfoCount = 1;

			createInfo.pEnabledFeatures = &deviceFeatures;

			createInfo.enabledExtensionCount = 0;

			createInfo.enabledLayerCount = 0;

			CHECK(vkCreateDevice(m_PhysicalDevice, &createInfo, nullptr, &m_Device), "failed to create logical device!");

			vkGetDeviceQueue(m_Device, indices.graphicsFamily.value(), 0, &m_GraphicsQueue);
		}

#pragma endregion Initialize
#pragma region Cleanup

		void CleanupVulkan()
		{
			vkDestroyDevice(m_Device, nullptr);

			if (m_EnableValidationLayers)
				DestroyDebugUtilsMessengerEXT(m_VkInstance, m_DebugMessenger, nullptr);
			vkDestroyInstance(m_VkInstance, nullptr);
		}

#pragma endregion Cleanup

#pragma region Extensions

		[[nodiscard]] std::vector<std::string> GetSupportedExtensions()
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
		[[nodiscard]] std::vector<const char*> GetRequiredExtensions()
		{
			// GLFW
			uint32_t glfwExtensionCount = 0;
			const char** glfwExtensions;
			glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

			if (glfwExtensions == nullptr)
				RTE("glfwGetRequiredInstanceExtensions failed");

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
					LOGERROR("Extension not supported: " + requiredExt);
				}
			}

			if (isEverythingSupported)
				LOGI("All required extensions are supported");
			else
				LOGCRITICAL("Not all required extentions are supported");
		}

#pragma endregion Extensions
#pragma region ValidationLayers

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
					LOGERROR(STR("Validation layer not supported: ") + requiredLayerName);
				}
			}

			if (isEverythingSupported)
				LOGI("All required validation layers are supported");
			else
				LOGCRITICAL("Not all required validation layers are supported");
		}

#pragma endregion ValidationLayers
#pragma region DebugUtilsMessengerEXT

		static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity
			, VkDebugUtilsMessageTypeFlagsEXT messageType
			, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData
			, [[maybe_unused]] void* pUserData /*Ptr to Renderer*/)
		{
			std::string type{ string_VkDebugUtilsMessageTypeFlagsEXT(messageType) };
			type = std::string{ &type[28], &type[type.find("_BIT_EXT")] };

			std::string header{ "Validation layer(" + type + "):" };
			std::string message{ pCallbackData->pMessage };

			switch (messageSeverity)
			{
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
				LOGT(header);
				LOGT(message);
				LOGT("");
				break;
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
				LOGT(header);
				LOGT(message);
				LOGT("");
				break;
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
				LOGWARN(header);
				LOGWARN(message);
				LOGWARN("");
				break;
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
				LOGERROR(header);
				LOGERROR(message);
				LOGERROR("");
				break;
			default:
				LOGCRITICAL("Unknown callback severity! Message: ");
				LOGCRITICAL(header);
				LOGCRITICAL(message);
				LOGCRITICAL("");
				break;
			}

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

#pragma endregion Vulkan

		// Variables

		Window* m_pWindow;

		VkInstance m_VkInstance;
		VkDebugUtilsMessengerEXT m_DebugMessenger;
		VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
		VkDevice m_Device;
		VkQueue m_GraphicsQueue;

#ifdef NDEBUG
		const bool m_EnableValidationLayers = false;
#else
		const bool m_EnableValidationLayers = true;
#endif
		const std::vector<const char*> m_ValidationLayers{ "VK_LAYER_KHRONOS_validation" };
	};
}