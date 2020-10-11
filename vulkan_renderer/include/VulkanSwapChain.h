#pragma once
#include "VulkanDevice.h"





class VulkanSwapChain
{
public:
	VulkanSwapChain() = default;
	explicit VulkanSwapChain(VulkanDevice& device, VkSurfaceKHR surface);
	~VulkanSwapChain();

	void Create(VulkanDevice& device, VkSurfaceKHR surface);
	void Destroy();

	VkExtent2D GetExtent() { return surface_extent_; }


	
	void SetAttachment(VkFormat depth_format);
	VkAttachmentReference color_attachment_ref = { 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };
	VkAttachmentDescription color_attachment_description = { 0, VK_FORMAT_UNDEFINED, VK_SAMPLE_COUNT_1_BIT,VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE, VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_ATTACHMENT_STORE_OP_DONT_CARE, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR };
	VkAttachmentReference depth_attachment_ref = { 1, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL };
	VkAttachmentDescription depth_attachment_description = { 0, VK_FORMAT_UNDEFINED, VK_SAMPLE_COUNT_1_BIT, VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_DONT_CARE, VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_ATTACHMENT_STORE_OP_DONT_CARE, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL };

public:
	VkDevice device_;

	VkSwapchainKHR swap_chain_;

	std::vector<VkImage> swap_chain_images_;
	std::vector<VkImageView> swap_chain_image_views_;
	std::vector<VkFramebuffer> swap_chain_framebuffers_;

	VkSurfaceCapabilitiesKHR surface_capability_;
	VkSurfaceFormatKHR surface_format_ = { VK_FORMAT_UNDEFINED, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
	VkPresentModeKHR present_mode_ = VK_PRESENT_MODE_MAILBOX_KHR;
	VkExtent2D surface_extent_;

};

