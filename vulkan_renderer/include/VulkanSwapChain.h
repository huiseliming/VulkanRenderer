#pragma once
#include "VulkanDevice.h"





class VulkanSwapChain
{
public:
	VulkanSwapChain() = default;
	explicit VulkanSwapChain(VulkanDevice& device, VkSurfaceKHR surface);
	~VulkanSwapChain();

	VulkanSwapChain(const VulkanSwapChain& other) = delete;
	VulkanSwapChain(VulkanSwapChain&& other) = delete;
	VulkanSwapChain& operator=(const VulkanSwapChain& other) = delete;
	VulkanSwapChain& operator=(VulkanSwapChain&& other) = delete;

	void Create(VulkanDevice& device, VkSurfaceKHR surface);
	void Destroy();

	VkExtent2D GetExtent() { return surfaceExtent; }

public:
	VkDevice device;
	VkSwapchainKHR swapChainKHR;

	std::vector<VkImage> swapChainImages;
	std::vector<VkImageView> swapChainImageViews;
	std::vector<VkFramebuffer> swapChainFramebuffers;

	VkSurfaceCapabilitiesKHR surfaceCapabilityKHR;
	VkSurfaceFormatKHR surfaceFormatKHR = { VK_FORMAT_UNDEFINED, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
	VkPresentModeKHR presentModeKHR = VK_PRESENT_MODE_MAILBOX_KHR;
	VkExtent2D surfaceExtent;

};

