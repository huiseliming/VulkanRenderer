#include "VulkanRenderer.h"
#include <iostream>
#include <fmt/format.h>
#include "VulkanDevice.h"

namespace Renderer
{
	bool enableValidationLayers;
	std::vector<const char*> enabledInstanceExtensions = { VK_KHR_SURFACE_EXTENSION_NAME ,VK_KHR_WIN32_SURFACE_EXTENSION_NAME };
	std::vector<const char*> enabledInstanceValidationLayers = { };

	std::vector<std::unique_ptr<VulkanDevice>> devices;

	void Initialize(const char* applicationName)
	{
#ifdef NDEBUG
		enableValidationLayers = false;
#else
		enableValidationLayers = true;
#endif
		if (enableValidationLayers) {
			//如果启用验证层就启用debugutils扩展
			enabledInstanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
			enabledInstanceValidationLayers.push_back("VK_LAYER_KHRONOS_validation");

			uint32_t layerPropertyCount;
			std::vector<VkLayerProperties> layerProperties;
			VK_ASSERT_SUCCESSED(vkEnumerateInstanceLayerProperties(&layerPropertyCount, nullptr));
			layerProperties.resize(layerPropertyCount);
			VK_ASSERT_SUCCESSED(vkEnumerateInstanceLayerProperties(&layerPropertyCount, layerProperties.data()));

			for (auto& validationLayer : enabledInstanceValidationLayers)
			{
				bool validationLayerFound = false;
				for (auto& layerProperty : layerProperties)
					if (!strcmp(layerProperty.layerName, validationLayer))
						validationLayerFound = true;
				if (!validationLayerFound)
					THROW_VK_EXCEPT(fmt::format("Validation layer [{0:s}] no found!", validationLayer).c_str(), VkResult::VK_ERROR_UNKNOWN);
			}
			CreateInstance(applicationName, enabledInstanceExtensions, enabledInstanceValidationLayers);
			Debug::SetupDebugMessenger();
		}
		else
		{
			CreateInstance(applicationName, enabledInstanceExtensions, enabledInstanceValidationLayers);
		}
	}

	void Cleanup()
	{
		Debug::CleanupDebugMessenger();
		vkDestroyInstance(GetInstance(), nullptr);
	}

	VkInstance& GetInstance() 
	{ 
		static VkInstance instance;
		return instance; 
	}
	void CreateInstance(const char* applicationName, std::vector<const char*> enabledInstanceExtensions, std::vector<const char*> enabledInstanceValidationLayers)
	{
		//获取实例支持的扩展
		uint32_t extensionPropertyCount;
		std::vector<VkExtensionProperties> extensionProperties;
		VK_ASSERT_SUCCESSED(vkEnumerateInstanceExtensionProperties(nullptr, &extensionPropertyCount, nullptr));
		extensionProperties.resize(extensionPropertyCount);
		VK_ASSERT_SUCCESSED(vkEnumerateInstanceExtensionProperties(nullptr, &extensionPropertyCount, extensionProperties.data()));

		//检查实例是否支持扩展
		for (auto& enabledInstanceExtension : enabledInstanceExtensions)
		{
			bool extensionFound = false;
			for (auto& extensionProperty : extensionProperties)
				if (!strcmp(extensionProperty.extensionName, enabledInstanceExtension))
					extensionFound = true;
			if (!extensionFound)
				THROW_VK_EXCEPT(fmt::format("Extension [{0:s}] no found!", enabledInstanceExtension).c_str(), VkResult::VK_ERROR_UNKNOWN);
		}
		//构建应用信息
		VkApplicationInfo applicationInfo;
		applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		applicationInfo.pNext = nullptr;
		applicationInfo.pApplicationName = applicationName;
		applicationInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 0);
		applicationInfo.pEngineName = "No Engine";
		applicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		applicationInfo.apiVersion = VK_API_VERSION_1_0;

		//实例构建信息
		VkInstanceCreateInfo instanceCreateInfo;
		instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instanceCreateInfo.pNext = nullptr;
		instanceCreateInfo.flags = 0;
		instanceCreateInfo.pApplicationInfo = &applicationInfo;
		instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(enabledInstanceValidationLayers.size());
		instanceCreateInfo.ppEnabledLayerNames = enabledInstanceValidationLayers.data();
		instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(enabledInstanceExtensions.size());
		instanceCreateInfo.ppEnabledExtensionNames = enabledInstanceExtensions.data();

		VK_ASSERT_SUCCESSED(vkCreateInstance(&instanceCreateInfo, nullptr, &GetInstance()));
	}
	namespace Debug 
	{
		VkDebugUtilsMessengerEXT debugUtilsMessengerEXT;
		PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXT;
		PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT;

		static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
			VkDebugUtilsMessageTypeFlagsEXT message_type,
			const VkDebugUtilsMessengerCallbackDataEXT* callback_data_ptr,
			void* user_data_ptr) 
		{
			std::string message_severity_string;
			switch (message_severity)
			{
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
				message_severity_string = "verbose";
				break;
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
				message_severity_string = "info   ";
				break;
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
				message_severity_string = "warning";
				break;
			case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
				message_severity_string = "error  ";
				break;
			default:
				message_severity_string = "unknow ";
				break;
			}
			std::string message_type_string;
			switch (message_type)
			{
			case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
				message_type_string = "general    ";
				break;
			case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
				message_type_string = "validation ";
				break;
			case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
				message_type_string = "performance";
				break;
			default:
				message_type_string = "unknow     ";
				break;
			}

			std::cout << fmt::format("<{0:s}><{1:s}> {2:s} : {3:s}\n", message_severity_string, message_type_string, callback_data_ptr->pMessageIdName, callback_data_ptr->pMessage) << std::endl;

			return VK_FALSE;
		}

		void SetupDebugMessenger() 
		{
			// 获取vkCreateDebugUtilsMessengerEXT函数地址
			vkCreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(GetInstance(), "vkCreateDebugUtilsMessengerEXT");
			if (vkCreateDebugUtilsMessengerEXT == nullptr)
				THROW_VK_EXCEPT("ProcAddr[vkCreateDebugUtilsMessengerEXT] is nullptr, extension not present!", VkResult::VK_ERROR_UNKNOWN);

			// 获取vkDestroyDebugUtilsMessengerEXT函数地址
			vkDestroyDebugUtilsMessengerEXT = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(GetInstance(), "vkDestroyDebugUtilsMessengerEXT");
			if (vkDestroyDebugUtilsMessengerEXT == nullptr)
				THROW_VK_EXCEPT("ProcAddr[vkDestroyDebugUtilsMessengerEXT] is nullptr, extension not present!", VkResult::VK_ERROR_UNKNOWN);

			VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfoEXT = {};
			debugUtilsMessengerCreateInfoEXT.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
			debugUtilsMessengerCreateInfoEXT.pNext = nullptr;
			debugUtilsMessengerCreateInfoEXT.flags = 0;
			debugUtilsMessengerCreateInfoEXT.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT /*| VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT*/ | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
			debugUtilsMessengerCreateInfoEXT.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
			debugUtilsMessengerCreateInfoEXT.pfnUserCallback = DebugCallback;
			debugUtilsMessengerCreateInfoEXT.pUserData = nullptr;

			VK_ASSERT_SUCCESSED(vkCreateDebugUtilsMessengerEXT(GetInstance(), &debugUtilsMessengerCreateInfoEXT, nullptr, &debugUtilsMessengerEXT));
		}

		void CleanupDebugMessenger() 
		{
			if (debugUtilsMessengerEXT != VK_NULL_HANDLE)
				vkDestroyDebugUtilsMessengerEXT(GetInstance(), debugUtilsMessengerEXT, nullptr);
		}
	}
}