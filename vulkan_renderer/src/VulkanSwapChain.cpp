#include "VulkanSwapChain.h"

VulkanSwapChain::VulkanSwapChain(VulkanDevice& device, VkSurfaceKHR surfaceKHR)
{
	Create(device, surfaceKHR);
}

VulkanSwapChain::~VulkanSwapChain()
{
	Destroy();
}

void VulkanSwapChain::Create(VulkanDevice& device, VkSurfaceKHR surfaceKHR)
{
	this->device = device;
	//从逻辑设备请求SwapChainSupportDetails内容
	SwapChainSupportDetail swapChainSupportDetail = device.GetSwapChainSupportDetail(surfaceKHR);

	//检查交换链支持
	if (!(!swapChainSupportDetail.surfaceFormatsKHR.empty()) && (!swapChainSupportDetail.presentModesKHR.empty()))
		THROW_VK_EXCEPT("SwapChain not support!", VkResult::VK_ERROR_UNKNOWN);

	//设置表面色彩格式
	for (const auto& availableFormat : swapChainSupportDetail.surfaceFormatsKHR) {
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			surfaceFormatKHR = availableFormat;
			break;
		}
	}
	if (surfaceFormatKHR.format == VK_FORMAT_UNDEFINED)
		surfaceFormatKHR = swapChainSupportDetail.surfaceFormatsKHR[0];

	// 设置显示模式
	for (const auto& presentMode : swapChainSupportDetail.presentModesKHR) {
		if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
			presentModeKHR = presentMode;
		}
	}
	if (presentModeKHR != VK_PRESENT_MODE_MAILBOX_KHR)
		presentModeKHR = VK_PRESENT_MODE_FIFO_KHR;

	// 设置长宽
	if (swapChainSupportDetail.surfaceCapabilitiesKHR.currentExtent.width != UINT32_MAX) {
		surfaceExtent = swapChainSupportDetail.surfaceCapabilitiesKHR.currentExtent;
	}
	else {
		VkExtent2D actualExtent = { 800, 600 };
		actualExtent.width = (std::max)(swapChainSupportDetail.surfaceCapabilitiesKHR.minImageExtent.width, (std::min)(swapChainSupportDetail.surfaceCapabilitiesKHR.maxImageExtent.width, actualExtent.width));
		actualExtent.height = (std::max)(swapChainSupportDetail.surfaceCapabilitiesKHR.minImageExtent.height, (std::min)(swapChainSupportDetail.surfaceCapabilitiesKHR.maxImageExtent.height, actualExtent.height));
		surfaceExtent = actualExtent;
	}
	// 构建交换链创建信息
	uint32_t imageCount = swapChainSupportDetail.surfaceCapabilitiesKHR.minImageCount + 1;
	if (swapChainSupportDetail.surfaceCapabilitiesKHR.maxImageCount > 0 && imageCount > swapChainSupportDetail.surfaceCapabilitiesKHR.maxImageCount)
		imageCount = swapChainSupportDetail.surfaceCapabilitiesKHR.maxImageCount;
	VkSwapchainCreateInfoKHR swapchainCreateInfoKHR = {};
	swapchainCreateInfoKHR.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchainCreateInfoKHR.surface = surfaceKHR;
	swapchainCreateInfoKHR.minImageCount = imageCount;
	swapchainCreateInfoKHR.imageFormat = surfaceFormatKHR.format;
	swapchainCreateInfoKHR.imageColorSpace = surfaceFormatKHR.colorSpace;
	swapchainCreateInfoKHR.imageExtent = surfaceExtent;
	swapchainCreateInfoKHR.imageArrayLayers = 1;
	swapchainCreateInfoKHR.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	uint32_t queueFamilyIndicesArray[] = { device.GetGraphicsQueueFamily(), device.GetPresentQueueFamily() };

	if (device.GetGraphicsQueueFamily() != device.GetPresentQueueFamily()) {
		swapchainCreateInfoKHR.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		swapchainCreateInfoKHR.queueFamilyIndexCount = 2;
		swapchainCreateInfoKHR.pQueueFamilyIndices = queueFamilyIndicesArray;
	}
	else {
		swapchainCreateInfoKHR.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapchainCreateInfoKHR.queueFamilyIndexCount = 0; // Optional
		swapchainCreateInfoKHR.pQueueFamilyIndices = nullptr; // Optional
	}
	swapchainCreateInfoKHR.preTransform = swapChainSupportDetail.surfaceCapabilitiesKHR.currentTransform;
	swapchainCreateInfoKHR.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapchainCreateInfoKHR.presentMode = presentModeKHR;
	swapchainCreateInfoKHR.clipped = VK_TRUE;
	swapchainCreateInfoKHR.oldSwapchain = VK_NULL_HANDLE;
	//创建交换链
	VK_ASSERT_SUCCESSED(vkCreateSwapchainKHR(device, &swapchainCreateInfoKHR, nullptr, &swapChainKHR));

	VK_ASSERT_SUCCESSED(vkGetSwapchainImagesKHR(device, swapChainKHR, &imageCount, nullptr));
	swapChainImages.resize(imageCount);
	VK_ASSERT_SUCCESSED(vkGetSwapchainImagesKHR(device, swapChainKHR, &imageCount, swapChainImages.data()));
	swapChainImageViews.resize(imageCount);
	for (size_t i = 0; i < swapChainImageViews.size(); i++)
		swapChainImageViews[i] = device.CreateImageView(swapChainImages[i], surfaceFormatKHR.format, VK_IMAGE_ASPECT_COLOR_BIT);
}

void VulkanSwapChain::Destroy()
{
	if (swapChainKHR != VK_NULL_HANDLE)
	{
		for (size_t i = 0; i < swapChainImageViews.size(); i++)
			if (swapChainImageViews[i] != VK_NULL_HANDLE){
				vkDestroyImageView(device, swapChainImageViews[i], nullptr);
				swapChainImageViews[i] = VK_NULL_HANDLE;
			}
		vkDestroySwapchainKHR(device, swapChainKHR, nullptr);
		swapChainKHR = VK_NULL_HANDLE;
	}
}

