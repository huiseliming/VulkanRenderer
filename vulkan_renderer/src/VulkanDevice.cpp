#include "VulkanDevice.h"
#include <fmt/format.h>
#include "Exception.h"
#include "VulkanRenderer.h"
VulkanDevice::VulkanDevice(VkSurfaceKHR surface, std::vector<const char*> enabledExtensions, std::vector<const char*>  enabledValidationLayers)
{
	Create(surface, enabledExtensions, enabledValidationLayers);
}
VulkanDevice::~VulkanDevice()
{
	if(device != VK_NULL_HANDLE)
		Destroy();
}

void VulkanDevice::Create(VkSurfaceKHR surface, std::vector<const char*> enabledExtensions, std::vector<const char*>  enabledValidationLayers)
{
	Renderer::enabledInstanceValidationLayers;
	assert(Renderer::GetInstance() != VK_NULL_HANDLE && "Renderer not initialize");
	//枚举设备
	uint32_t physicalDeviceCount;
	std::vector<VkPhysicalDevice> physicalDevices;
	VK_ASSERT_SUCCESSED(vkEnumeratePhysicalDevices(Renderer::GetInstance(), &physicalDeviceCount, nullptr));
	physicalDevices.resize(physicalDeviceCount);
	VK_ASSERT_SUCCESSED(vkEnumeratePhysicalDevices(Renderer::GetInstance(), &physicalDeviceCount, physicalDevices.data()));
	for (size_t i = 0; i < physicalDeviceCount; i++)
	{
		if (IsPhysicalDeviceSuitable(physicalDevices[i])) {
			physicalDevice = physicalDevices[i];
			break;
		}
	}
	if (physicalDevice == VK_NULL_HANDLE)
		THROW_VK_EXCEPT("Failed to find a suitable physical device", VkResult::VK_ERROR_UNKNOWN);

	// 获取图形队列的索引
	uint32_t queueFamilyPropertyCount;
	std::vector<VkQueueFamilyProperties> queueFamilyProperties;
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyPropertyCount, nullptr);
	queueFamilyProperties.resize(queueFamilyPropertyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyPropertyCount, queueFamilyProperties.data());

	queueFamilyIndices.graphics = GetQueueFamilyIndex(queueFamilyProperties, VK_QUEUE_GRAPHICS_BIT);
	queueFamilyIndices.compute = GetQueueFamilyIndex(queueFamilyProperties, VK_QUEUE_COMPUTE_BIT);
	queueFamilyIndices.transfer = GetQueueFamilyIndex(queueFamilyProperties, VK_QUEUE_TRANSFER_BIT);

	static float queuePriorities = 1.0f;
	std::vector<VkDeviceQueueCreateInfo> deviceQueuesCreateInfo;

	VkDeviceQueueCreateInfo deviceGraphicsQueuesCreateInfo; 
	deviceGraphicsQueuesCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	deviceGraphicsQueuesCreateInfo.pNext = nullptr;
	deviceGraphicsQueuesCreateInfo.flags = 0;
	deviceGraphicsQueuesCreateInfo.queueFamilyIndex = queueFamilyIndices.graphics.value();
	deviceGraphicsQueuesCreateInfo.queueCount = 1;
	deviceGraphicsQueuesCreateInfo.pQueuePriorities = &queuePriorities;
	deviceQueuesCreateInfo.push_back(deviceGraphicsQueuesCreateInfo);

	//检测显示支持
	VkBool32 presentSupported = false;
	VK_ASSERT_SUCCESSED(vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueFamilyIndices.graphics.value(), surface, &presentSupported));
	if (presentSupported)
	{
		//获取支持present的队列
		for (uint32_t i = 0; i < queueFamilyPropertyCount; i++)
		{
			VK_ASSERT_SUCCESSED(vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentSupported));
			if (presentSupported)
			{
				queueFamilyIndices.present = i;
				if (queueFamilyIndices.present.value() != queueFamilyIndices.graphics.value())
				{
					VkDeviceQueueCreateInfo devicePresentQueueCreateInfo;
					devicePresentQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
					devicePresentQueueCreateInfo.pNext = nullptr;
					devicePresentQueueCreateInfo.flags = 0;
					devicePresentQueueCreateInfo.queueFamilyIndex = queueFamilyIndices.present.value();
					devicePresentQueueCreateInfo.queueCount = 1;
					devicePresentQueueCreateInfo.pQueuePriorities = &queuePriorities;
					deviceQueuesCreateInfo.push_back(devicePresentQueueCreateInfo);
				}
				break;
			}
		}
	}
	else {
		queueFamilyIndices.present = queueFamilyIndices.graphics;
	}

	uint32_t extensionPropertyCount;
	std::vector<VkExtensionProperties> extensionProperties;
	VK_ASSERT_SUCCESSED(vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionPropertyCount, nullptr));
	extensionProperties.resize(extensionPropertyCount);
	VK_ASSERT_SUCCESSED(vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionPropertyCount, extensionProperties.data()));

	//检查设备是否支持扩展
	for (auto& enabledInstanceExtension : enabledExtensions)
	{
		bool extensionFound = false;
		for (auto& extensionProperty : extensionProperties)
			if (!strcmp(extensionProperty.extensionName, enabledInstanceExtension))
				extensionFound = true;
		if (!extensionFound)
			THROW_VK_EXCEPT(fmt::format("Extension [{0:s}] no found!", enabledInstanceExtension).c_str(), VkResult::VK_ERROR_UNKNOWN);
	}

	VkPhysicalDeviceFeatures physicalDeviceFeatures = {};
	physicalDeviceFeatures.samplerAnisotropy = VK_TRUE;

	VkDeviceCreateInfo deviceCreateInfo;
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.pNext = nullptr;
	deviceCreateInfo.flags = 0;
	deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(deviceQueuesCreateInfo.size());
	deviceCreateInfo.pQueueCreateInfos = deviceQueuesCreateInfo.data();
	deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(enabledValidationLayers.size());;
	deviceCreateInfo.ppEnabledLayerNames = enabledValidationLayers.data();
	deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(enabledExtensions.size());
	deviceCreateInfo.ppEnabledExtensionNames = enabledExtensions.data();
	deviceCreateInfo.pEnabledFeatures = &physicalDeviceFeatures;


	VK_ASSERT_SUCCESSED(vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &device));
	vkGetDeviceQueue(device, queueFamilyIndices.graphics.value(), 0, &graphicsQueue);

	if (queueFamilyIndices.graphics.value() != queueFamilyIndices.present.value())
		vkGetDeviceQueue(device, queueFamilyIndices.present.value(), 0, &presentQueue);
	else
		presentQueue = graphicsQueue;
}

void VulkanDevice::Destroy()
{
	vkDestroyDevice(device, nullptr);
	device = VK_NULL_HANDLE;
}

VkImageView VulkanDevice::CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags imageAspectFlags)
{
	VkImageViewCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	createInfo.pNext = nullptr;
	createInfo.flags = 0;
	createInfo.image = image;
	createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	createInfo.format = format;
	createInfo.components;
	createInfo.subresourceRange.aspectMask = imageAspectFlags;
	createInfo.subresourceRange.baseMipLevel = 0;
	createInfo.subresourceRange.levelCount = 1;
	createInfo.subresourceRange.baseArrayLayer = 0;
	createInfo.subresourceRange.layerCount = 1;
	VkImageView imageView;
	VK_ASSERT_SUCCESSED(vkCreateImageView(device, &createInfo, nullptr, &imageView));
	return imageView;
}

std::shared_ptr<VkShaderModule> VulkanDevice::CreateShaderModule(const std::vector<char>& code)
{
	std::shared_ptr<VkShaderModule> pShaderModule = std::shared_ptr<VkShaderModule>(
		new VkShaderModule(),
		[this](const VkShaderModule* pShaderModule)
		{ 
			vkDestroyShaderModule(device, *pShaderModule, nullptr);
			delete pShaderModule;
		}
	);
	VkShaderModuleCreateInfo create_info{};
	create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	create_info.codeSize = code.size();
	create_info.pCode = reinterpret_cast<const uint32_t*>(code.data());
	VK_ASSERT_SUCCESSED(vkCreateShaderModule(device, &create_info, nullptr, pShaderModule.get()));
	return pShaderModule;
}

std::shared_ptr<VkPipelineLayout> VulkanDevice::CreatePipelineLayout(uint32_t setLayoutCount, VkDescriptorSetLayout* setLayout, uint32_t pushConstantRangeCount, VkPushConstantRange* pushConstantRanges)
{
	VkPipelineLayoutCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	createInfo.setLayoutCount = setLayoutCount;
	createInfo.pSetLayouts = setLayout;
	createInfo.pushConstantRangeCount = pushConstantRangeCount;
	createInfo.pPushConstantRanges = pushConstantRanges;
	std::shared_ptr<VkPipelineLayout> pPipelineLayout = std::shared_ptr<VkPipelineLayout>(
		new VkPipelineLayout(),
		[this](VkPipelineLayout* pPipelineLayout)
		{
			vkDestroyPipelineLayout(device, *pPipelineLayout, nullptr);
			delete pPipelineLayout;
		}
	);
	VK_ASSERT_SUCCESSED(vkCreatePipelineLayout(device, &createInfo, nullptr, pPipelineLayout.get()));
	return pPipelineLayout;
}

std::shared_ptr<VkRenderPass> VulkanDevice::CreateRenderPass(uint32_t attachmentCount, VkAttachmentDescription* attachments, uint32_t subpassCount, VkSubpassDescription* subpasses, uint32_t dependencyCount, VkSubpassDependency* dependencies)
{
	std::shared_ptr<VkRenderPass> pRenderPass(new VkRenderPass,
		[this](VkRenderPass * pRenderPass)
		{
			vkDestroyRenderPass(device, *pRenderPass, nullptr);
			delete pRenderPass;
		}
	);
	VkRenderPassCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	createInfo.attachmentCount = attachmentCount;
	createInfo.pAttachments = attachments;
	createInfo.subpassCount = subpassCount;
	createInfo.pSubpasses = subpasses;
	createInfo.dependencyCount = dependencyCount;
	createInfo.pDependencies = dependencies;

	VK_ASSERT_SUCCESSED(vkCreateRenderPass(device, &createInfo, nullptr, pRenderPass.get()));
	return pRenderPass;
}

VkFormat VulkanDevice::FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
{
	for (VkFormat format : candidates) {
		VkFormatProperties formatProperties;
		vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &formatProperties);

		if (tiling == VK_IMAGE_TILING_LINEAR && (formatProperties.linearTilingFeatures & features) == features) {
			return format;
		}
		else if (tiling == VK_IMAGE_TILING_OPTIMAL && (formatProperties.optimalTilingFeatures & features) == features) {
			return format;
		}
	}
	THROW_VK_EXCEPT("Failed to find supported format!", VkResult::VK_ERROR_UNKNOWN);
}

VkFormat VulkanDevice::FindDepthFormat()
{
	return FindSupportedFormat(
		{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
		VK_IMAGE_TILING_OPTIMAL,
		VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
	);
}

bool VulkanDevice::IsPhysicalDeviceSuitable(VkPhysicalDevice physicalDevice)
{
	VkPhysicalDeviceProperties physicalDeviceProperties;
	vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties);

	VkPhysicalDeviceFeatures supportedFeatures;
	vkGetPhysicalDeviceFeatures(physicalDevice, &supportedFeatures);

	return supportedFeatures.samplerAnisotropy;
}


std::optional<uint32_t> VulkanDevice::GetQueueFamilyIndex(std::vector<VkQueueFamilyProperties>& queueFamilyProperties, VkQueueFlagBits queueFlags)
{
	// Dedicated queue for compute
	// Try to find a queue family index that supports compute but not graphics
	if (queueFlags & VK_QUEUE_COMPUTE_BIT)
	{
		for (uint32_t i = 0; i < static_cast<uint32_t>(queueFamilyProperties.size()); i++)
		{
			if ((queueFamilyProperties[i].queueFlags & queueFlags) && ((queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0))
			{
				return i;
			}
		}
	}

	// Dedicated queue for transfer
	// Try to find a queue family index that supports transfer but not graphics and compute
	if (queueFlags & VK_QUEUE_TRANSFER_BIT)
	{
		for (uint32_t i = 0; i < static_cast<uint32_t>(queueFamilyProperties.size()); i++)
		{
			if ((queueFamilyProperties[i].queueFlags & queueFlags) && ((queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0) && ((queueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT) == 0))
			{
				return i;
			}
		}
	}

	// For other queue types or if no separate compute queue is present, return the first one to support the requested flags
	for (uint32_t i = 0; i < static_cast<uint32_t>(queueFamilyProperties.size()); i++)
	{
		if (queueFamilyProperties[i].queueFlags & queueFlags)
		{
			return i;
		}
	}
	THROW_VK_EXCEPT("Failed to find queue family index", VkResult::VK_ERROR_UNKNOWN);
}
