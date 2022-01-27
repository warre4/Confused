module;
#include "Core.h"
#include "Macros.h"
#include "Logger.h"

#include <string>
#include <cstring>
#include <vector>
#include <set>
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
	private:
		struct QueueFamilyIndices
		{
			std::optional<uint32_t> graphicsFamily;
			std::optional<uint32_t> presentFamily;

			[[nodiscard]] inline constexpr bool IsComplete() const noexcept
			{
				return graphicsFamily.has_value() && presentFamily.has_value();
			}
		};
		
		struct SwapChainSupportDetails
		{
			VkSurfaceCapabilitiesKHR capabilities;
			std::vector<VkSurfaceFormatKHR> formats;
			std::vector<VkPresentModeKHR> presentModes;
		};

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

		inline Window* GetWindow() const noexcept { return m_pWindow; }

		inline void SetWindow(Window* pWindow) noexcept { m_pWindow = pWindow; }

	private:

#pragma region Vulkan

#pragma region Initialize

		void InitializeVulkan()
		{
			// Instance
			CreateInstance();
			SetupDebugMessenger();

			// Surface
			CreateSurface();

			// Devices
			PickPhysicalDevice();
			CreateLogicalDevice();

			// Swap chain
			CreateSwapChain();
		}
		
		void CreateInstance()
		{
			// Validation layers
			if (m_EnableValidationLayers)
			{
				UTILS.PrintStrings(UTILS.ChangeType<std::string>(m_ValidationLayers), "Required validation layers", LOGGER, _LOGLEVEL trace);
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
			UTILS.PrintStrings(requiredExtensions, "Required extensions", LOGGER, _LOGLEVEL trace);
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
			CHECK(vkCreateInstance(&createInfo, nullptr, &m_Instance), "vkCreateInstance failed");
		}

#pragma region Surface

		void CreateSurface()
		{
			CHECK(m_pWindow->CreateSurface(m_Instance, &m_Surface), "Failed to create window surface!");
		}

#pragma endregion Surface
#pragma region Device

		void PickPhysicalDevice()
		{
			UTILS.PrintStrings(UTILS.ChangeType<std::string>(m_DeviceExtensions), "Required device extensions", LOGGER, _LOGLEVEL trace);

			uint32_t deviceCount = 0;
			CHECK(vkEnumeratePhysicalDevices(m_Instance, &deviceCount, nullptr), "vkEnumeratePhysicalDevices failed");

			if (deviceCount == 0)
				RTE("Failed to find GPUs with Vulkan support!");

			std::vector<VkPhysicalDevice> devices{ deviceCount };
			CHECK(vkEnumeratePhysicalDevices(m_Instance, &deviceCount, devices.data()), "vkEnumeratePhysicalDevices failed");

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
			const std::string deviceName{ STR(deviceProperties.deviceName) };
			LOGD("Vulkan will use " + deviceName);
			LOGI("All required device extensions are supported on " + deviceName);
		}
		// Unsuitable if return value is 0 (higher value is better)
		[[nodiscard]] uint32_t RateDeviceSuitability(VkPhysicalDevice device) const
		{
			QueueFamilyIndices indices = FindQueueFamilies(device);

			// Needs queue families set
			if (!indices.IsComplete())
				return 0;

			VkPhysicalDeviceProperties deviceProperties;
			vkGetPhysicalDeviceProperties(device, &deviceProperties);
			VkPhysicalDeviceFeatures deviceFeatures;
			vkGetPhysicalDeviceFeatures(device, &deviceFeatures);


			// Needs geometry shader
			if (!deviceFeatures.geometryShader)
				return 0;

			// Needs to support all required device extensions
			if (!CheckDeviceExtensionSupport(device))
				return 0;

			// Needs to support compatible swap chain
			SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(device);

			if (swapChainSupport.formats.empty() || swapChainSupport.presentModes.empty())
				return 0;

			// --------------------------------------------------------------------
			// Generate score
			// --------------------------------------------------------------------
			uint32_t score = deviceProperties.limits.maxImageDimension2D;

			if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
				score += 10'000;

			if (indices.graphicsFamily == indices.presentFamily)
				score += 1000;

			//LOGT(deviceProperties.deviceName + STR(" scored ") + std::to_string(score));

			return score;
		}
		bool CheckDeviceExtensionSupport(VkPhysicalDevice device) const
		{
			// Get supported device extensions
			uint32_t supportedExtensionCount;
			CHECK(vkEnumerateDeviceExtensionProperties(device, nullptr, &supportedExtensionCount, nullptr), "vkEnumerateDeviceExtensionProperties failed");

			std::vector<VkExtensionProperties> supportedExtensions{ supportedExtensionCount };
			CHECK(vkEnumerateDeviceExtensionProperties(device, nullptr, &supportedExtensionCount, supportedExtensions.data()), "vkEnumerateDeviceExtensionProperties failed");

			// Check if all required device extensions are supported
			std::set<std::string> requiredExtensions(m_DeviceExtensions.begin(), m_DeviceExtensions.end());

			for (const auto& supportedExtension : supportedExtensions)
				requiredExtensions.erase(supportedExtension.extensionName);

			return requiredExtensions.empty();
		}
		[[nodiscard]] QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device) const
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

				VkBool32 hasPresentSupport = false;
				CHECK(vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_Surface, &hasPresentSupport), "vkGetPhysicalDeviceSurfaceSupportKHR failed");

				if (hasPresentSupport)
					indices.presentFamily = i;

				if (indices.IsComplete())
					break;

				i++;
			}

			return std::move(indices);
		}

#pragma region SwapChain

		void CreateSwapChain()
		{
			// Choose properties
			SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(m_PhysicalDevice);

			VkSurfaceFormatKHR surfaceFormat = ChooseSwapSurfaceFormat(swapChainSupport.formats);
			VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);
			VkExtent2D extent = ChooseSwapExtent(swapChainSupport.capabilities);

			// Swap chain create info
			uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1; // +1 will improve performance
			if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
				imageCount = swapChainSupport.capabilities.maxImageCount;

			VkSwapchainCreateInfoKHR createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
			createInfo.surface = m_Surface;

			createInfo.minImageCount = imageCount;
			createInfo.imageFormat = surfaceFormat.format;
			createInfo.imageColorSpace = surfaceFormat.colorSpace;
			createInfo.imageExtent = extent;

			//INFO: 1  => default
			//INFO: 1+ => stereoscopic applications (like VR)
			createInfo.imageArrayLayers = 1;

			//INFO: VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT => directly render to swapchain
			//INFO: VK_IMAGE_USAGE_TRANSFER_DST_BIT     => allows rendering to serperate image first (needed for modifying image e.g. post-processing)
			createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

			// Queues
			QueueFamilyIndices indices = FindQueueFamilies(m_PhysicalDevice);
			uint32_t queueFamilyIndices[]{ indices.graphicsFamily.value(), indices.presentFamily.value() };

			if (indices.graphicsFamily != indices.presentFamily)
			{
				createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
				createInfo.queueFamilyIndexCount = 2;
				createInfo.pQueueFamilyIndices = queueFamilyIndices;
			}
			else
			{
				createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
				createInfo.queueFamilyIndexCount = 0;
				createInfo.pQueueFamilyIndices = nullptr;
			}

			//INFO: swapChainSupport.capabilities.currentTransform => no transform
			//INFO: anything else                                  => Allows for e.g. flipping or rotating the image
			createInfo.preTransform = swapChainSupport.capabilities.currentTransform;

			createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
			createInfo.presentMode = presentMode;
			createInfo.clipped = VK_TRUE;

			createInfo.oldSwapchain = VK_NULL_HANDLE;

			// Create swap chain
			CHECK(vkCreateSwapchainKHR(m_Device, &createInfo, nullptr, &m_SwapChain), "Failed to create swap chain!");

			// Adjust members
			CHECK(vkGetSwapchainImagesKHR(m_Device, m_SwapChain, &imageCount, nullptr), "vkGetSwapchainImagesKHR failed");
			m_SwapChainImages.resize(imageCount);
			CHECK(vkGetSwapchainImagesKHR(m_Device, m_SwapChain, &imageCount, m_SwapChainImages.data()), "vkGetSwapchainImagesKHR failed");

			m_SwapChainImageFormat = surfaceFormat.format;
			m_SwapChainExtent = extent;
		}

		[[nodiscard]] SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device) const
		{
			SwapChainSupportDetails details;

			// Capabilities
			CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_Surface, &details.capabilities), "vkGetPhysicalDeviceSurfaceCapabilitiesKHR failed");

			// Formats
			uint32_t formatCount;
			CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_Surface, &formatCount, nullptr), "vkGetPhysicalDeviceSurfaceFormatsKHR failed");

			if (formatCount != 0)
			{
				details.formats.resize(formatCount);
				CHECK(vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_Surface, &formatCount, details.formats.data()), "vkGetPhysicalDeviceSurfaceFormatsKHR failed");
			}

			// Present modes
			uint32_t presentModeCount;
			CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_Surface, &presentModeCount, nullptr), "vkGetPhysicalDeviceSurfacePresentModesKHR failed");

			if (presentModeCount != 0)
			{
				details.presentModes.resize(presentModeCount);
				CHECK(vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_Surface, &presentModeCount, details.presentModes.data()), "vkGetPhysicalDeviceSurfacePresentModesKHR failed");
			}

			return std::move(details);
		}

		[[nodiscard]] VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) const
		{
			for (const auto& availableFormat : availableFormats)
			{
				if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
				{
					return availableFormat;
				}
			}

			return availableFormats[0];
		}
		[[nodiscard]] VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) const
		{
			for (const auto& availablePresentMode : availablePresentModes)
			{
				if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
					return availablePresentMode;
			}

			return VK_PRESENT_MODE_FIFO_KHR;
		}
		[[nodiscard]] VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) const
		{
			if (capabilities.currentExtent.width != UINT32_MAX)
				return capabilities.currentExtent;

			int width, height;
			m_pWindow->GetFramebufferSize(&width, &height);

			VkExtent2D actualExtent{ static_cast<uint32_t>(width), static_cast<uint32_t>(height) };

			actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
			actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

			return actualExtent;
		}

#pragma endregion SwapChain

		void CreateLogicalDevice()
		{
			QueueFamilyIndices indices = FindQueueFamilies(m_PhysicalDevice);

			// Queues
			std::vector<VkDeviceQueueCreateInfo> queueCreateInfos{};
			std::set<uint32_t> uniqueQueueFamilyIndices{ indices.graphicsFamily.value(), indices.presentFamily.value() };

			const float queuePriority = 1.f;
			for (const uint32_t queueFamilyIndex : uniqueQueueFamilyIndices)
			{
				// Make CreateInfo for every Family
				VkDeviceQueueCreateInfo queueCreateInfo{};
				queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
				queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
				queueCreateInfo.queueCount = 1;
				queueCreateInfo.pQueuePriorities = &queuePriority;

				queueCreateInfos.push_back(queueCreateInfo);
			}

			// Features
			VkPhysicalDeviceFeatures deviceFeatures{};

			// Create logical device
			VkDeviceCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

			createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
			createInfo.pQueueCreateInfos = queueCreateInfos.data();

			createInfo.pEnabledFeatures = &deviceFeatures;

			createInfo.enabledExtensionCount = static_cast<uint32_t>(m_DeviceExtensions.size());
			createInfo.ppEnabledExtensionNames = m_DeviceExtensions.data();

			createInfo.enabledLayerCount = 0;

			CHECK(vkCreateDevice(m_PhysicalDevice, &createInfo, nullptr, &m_Device), "Failed to create logical device!");

			vkGetDeviceQueue(m_Device, indices.graphicsFamily.value(), 0, &m_GraphicsQueue);
			vkGetDeviceQueue(m_Device, indices.presentFamily.value(), 0, &m_PresentQueue);
		}

#pragma endregion Device

#pragma endregion Initialize
#pragma region Cleanup

		void CleanupVulkan()
		{
			// Swap chain
			vkDestroySwapchainKHR(m_Device, m_SwapChain, nullptr);

			// Devices
			vkDestroyDevice(m_Device, nullptr);

			// Surface
			vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);

			// Instance
			if (m_EnableValidationLayers)
				DestroyDebugUtilsMessengerEXT(m_Instance, m_DebugMessenger, nullptr);
			vkDestroyInstance(m_Instance, nullptr);
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

			CHECK(CreateDebugUtilsMessengerEXT(m_Instance, &createInfo, nullptr, &m_DebugMessenger), "Failed to set up debug messenger!");
		}

#pragma endregion DebugUtilsMessengerEXT

#pragma endregion Vulkan

		// Variables

		Window* m_pWindow;

#pragma region Vulkan

		// Instance

		VkInstance m_Instance;
		VkDebugUtilsMessengerEXT m_DebugMessenger;

		// Surface

		VkSurfaceKHR m_Surface;

		// Devices

		VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
		VkDevice m_Device;
		VkQueue m_GraphicsQueue;
		VkQueue m_PresentQueue;

		// Swap chain

		VkSwapchainKHR m_SwapChain;
		std::vector<VkImage> m_SwapChainImages;
		VkFormat m_SwapChainImageFormat;
		VkExtent2D m_SwapChainExtent;

		// Validation layers / ...

#ifdef NDEBUG
		const bool m_EnableValidationLayers = false;
#else
		const bool m_EnableValidationLayers = true;
#endif
		const std::vector<const char*> m_ValidationLayers{ "VK_LAYER_KHRONOS_validation" };
		const std::vector<const char*> m_DeviceExtensions{ VK_KHR_SWAPCHAIN_EXTENSION_NAME };

#pragma endregion Vulkan
	};
}