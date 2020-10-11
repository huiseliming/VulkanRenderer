#include "VulkanSwapChain.h"
#include "Exception.h"

VulkanSwapChain::VulkanSwapChain(VulkanDevice& device, VkSurfaceKHR surface)
{
	Create(device, surface);
}

VulkanSwapChain::~VulkanSwapChain()
{
	Destroy();
}

void VulkanSwapChain::Create(VulkanDevice& device, VkSurfaceKHR surface)
{
	device_ = device.Get();
	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR surface_capabilities;
		std::vector<VkSurfaceFormatKHR> surface_formats;
		std::vector<VkPresentModeKHR> present_modes;
	};

	SwapChainSupportDetails swap_chain_support_details;

	//从逻辑设备请求SwapChainSupportDetails内容
	VK_ASSERT_SUCCESSED(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device.GetPhysicalDevice(), surface, &swap_chain_support_details.surface_capabilities));

	uint32_t surface_format_count;
	VK_ASSERT_SUCCESSED(vkGetPhysicalDeviceSurfaceFormatsKHR(device.GetPhysicalDevice(), surface, &surface_format_count, nullptr));
	swap_chain_support_details.surface_formats.resize(surface_format_count);
	VK_ASSERT_SUCCESSED(vkGetPhysicalDeviceSurfaceFormatsKHR(device.GetPhysicalDevice(), surface, &surface_format_count, swap_chain_support_details.surface_formats.data()));

	uint32_t present_mode_count;
	VK_ASSERT_SUCCESSED(vkGetPhysicalDeviceSurfacePresentModesKHR(device.GetPhysicalDevice(), surface, &present_mode_count, nullptr));
	swap_chain_support_details.present_modes.resize(present_mode_count);
	VK_ASSERT_SUCCESSED(vkGetPhysicalDeviceSurfacePresentModesKHR(device.GetPhysicalDevice(), surface, &present_mode_count, swap_chain_support_details.present_modes.data()));

	//检查交换链支持
	if (!(!swap_chain_support_details.surface_formats.empty()) && (!swap_chain_support_details.present_modes.empty()))
		THROW_VK_EXCEPT("SwapChain not support!", VkResult::VK_ERROR_UNKNOWN);

	//设置表面色彩格式
	for (const auto& available_format : swap_chain_support_details.surface_formats) {
		if (available_format.format == VK_FORMAT_B8G8R8A8_SRGB && available_format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			surface_format_ = available_format;
			break;
		}
	}
	if (surface_format_.format == VK_FORMAT_UNDEFINED)
		surface_format_ = swap_chain_support_details.surface_formats[0];

	// 设置显示模式
	for (const auto& present_mode : swap_chain_support_details.present_modes) {
		if (present_mode == VK_PRESENT_MODE_MAILBOX_KHR) {
			present_mode_ = present_mode;
		}
	}
	if (present_mode_ != VK_PRESENT_MODE_MAILBOX_KHR)
		present_mode_ = VK_PRESENT_MODE_FIFO_KHR;

	// 设置长宽
	if (swap_chain_support_details.surface_capabilities.currentExtent.width != UINT32_MAX) {
		surface_extent_ = swap_chain_support_details.surface_capabilities.currentExtent;
	}
	else {
		VkExtent2D actual_extent = { 800, 600 };
		actual_extent.width = (std::max)(swap_chain_support_details.surface_capabilities.minImageExtent.width, (std::min)(swap_chain_support_details.surface_capabilities.maxImageExtent.width, actual_extent.width));
		actual_extent.height = (std::max)(swap_chain_support_details.surface_capabilities.minImageExtent.height, (std::min)(swap_chain_support_details.surface_capabilities.maxImageExtent.height, actual_extent.height));
		surface_extent_ = actual_extent;
	}
	// 构建交换链创建信息
	uint32_t image_count = swap_chain_support_details.surface_capabilities.minImageCount + 1;
	if (swap_chain_support_details.surface_capabilities.maxImageCount > 0 && image_count > swap_chain_support_details.surface_capabilities.maxImageCount)
		image_count = swap_chain_support_details.surface_capabilities.maxImageCount;
	VkSwapchainCreateInfoKHR swap_chain_create_info = {};
	swap_chain_create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swap_chain_create_info.surface = surface;
	swap_chain_create_info.minImageCount = image_count;
	swap_chain_create_info.imageFormat = surface_format_.format;
	swap_chain_create_info.imageColorSpace = surface_format_.colorSpace;
	swap_chain_create_info.imageExtent = surface_extent_;
	swap_chain_create_info.imageArrayLayers = 1;
	swap_chain_create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	uint32_t queue_family_indices_array[] = { device.GetGraphicsQueueFamily(), device.GetPresentQueueFamily() };

	if (device.GetGraphicsQueueFamily() != device.GetPresentQueueFamily()) {
		swap_chain_create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		swap_chain_create_info.queueFamilyIndexCount = 2;
		swap_chain_create_info.pQueueFamilyIndices = queue_family_indices_array;
	}
	else {
		swap_chain_create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swap_chain_create_info.queueFamilyIndexCount = 0; // Optional
		swap_chain_create_info.pQueueFamilyIndices = nullptr; // Optional
	}
	swap_chain_create_info.preTransform = swap_chain_support_details.surface_capabilities.currentTransform;
	swap_chain_create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swap_chain_create_info.presentMode = present_mode_;
	swap_chain_create_info.clipped = VK_TRUE;
	swap_chain_create_info.oldSwapchain = VK_NULL_HANDLE;
	//创建交换链
	VK_ASSERT_SUCCESSED(vkCreateSwapchainKHR(device_, &swap_chain_create_info, nullptr, &swap_chain_));

	VK_ASSERT_SUCCESSED(vkGetSwapchainImagesKHR(device_, swap_chain_, &image_count, nullptr));
	swap_chain_images_.resize(image_count);
	VK_ASSERT_SUCCESSED(vkGetSwapchainImagesKHR(device_, swap_chain_, &image_count, swap_chain_images_.data()));
	swap_chain_image_views_.resize(image_count);
	for (size_t i = 0; i < swap_chain_image_views_.size(); i++)
		swap_chain_image_views_[i] = device.CreateImageView(swap_chain_images_[i], surface_format_.format, VK_IMAGE_ASPECT_COLOR_BIT);
}

void VulkanSwapChain::Destroy()
{
	if (swap_chain_ != VK_NULL_HANDLE)
	{
		for (size_t i = 0; i < swap_chain_image_views_.size(); i++)
			if (swap_chain_image_views_[i] != VK_NULL_HANDLE)
				vkDestroyImageView(device_, swap_chain_image_views_[i], nullptr);
		vkDestroySwapchainKHR(device_, swap_chain_, nullptr);
	}
}

void VulkanSwapChain::SetAttachment(VkFormat depth_format)
{
	color_attachment_description.format = surface_format_.format;
	depth_attachment_description.format = depth_format;
}

