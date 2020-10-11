#pragma once
#include <optional>
#include <vector>
#include <memory>
#include "VulkanHeader.h"
#include "VulkanRenderer.h"

class VulkanDevice
{
public:
	VulkanDevice() = default;
	explicit VulkanDevice(VkSurfaceKHR surface, std::vector<const char*> enabledExtensions, std::vector<const char*>  enabledValidationLayers);
	~VulkanDevice();
	VulkanDevice(const VulkanDevice&) =delete;
	VulkanDevice& operator=(const VulkanDevice&) = delete;
	VulkanDevice(VulkanDevice&& other)
	{
		device = other.device;
		other.device = VK_NULL_HANDLE;
		physicalDevice = other.physicalDevice;
		graphicsQueue = other.graphicsQueue;
		presentQueue = other.presentQueue;
		queueFamilyIndices = other.queueFamilyIndices;
	}
	VulkanDevice& operator=(VulkanDevice&& other)
	{
		device = other.device;
		other.device = VK_NULL_HANDLE;
		physicalDevice = other.physicalDevice;
		graphicsQueue = other.graphicsQueue;
		presentQueue = other.presentQueue;
		queueFamilyIndices = other.queueFamilyIndices;
	}
	void Create(VkSurfaceKHR surface, std::vector<const char*> enabledExtensions, std::vector<const char*>  enabledValidationLayers = 
		Renderer::enabledInstanceValidationLayers);
	void Destroy();

	VkDevice Get() { return device; }
	VkDevice  GetDevice() { return Get(); }
	VkPhysicalDevice GetPhysicalDevice() { return physicalDevice; }

	uint32_t GetGraphicsQueueFamily() { return queueFamilyIndices.graphics.has_value() ? queueFamilyIndices.graphics.value() : 0xFFFFFFFF; }
	uint32_t GetComputeQueueFamily() { return queueFamilyIndices.compute.has_value() ? queueFamilyIndices.compute.value() : 0xFFFFFFFF; }
	uint32_t GetTransferQueueFamily() { return queueFamilyIndices.transfer.has_value() ? queueFamilyIndices.transfer.value() : 0xFFFFFFFF; }
	uint32_t GetPresentQueueFamily() { return queueFamilyIndices.present.has_value() ? queueFamilyIndices.present.value() : 0xFFFFFFFF; }

	VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags imageAspectFlags);
	std::shared_ptr<VkShaderModule> CreateShaderModule(const std::vector<char>& code);
	std::shared_ptr<VkPipelineLayout> CreatePipelineLayout(uint32_t setLayoutCount, VkDescriptorSetLayout* setLayout, uint32_t pushConstantRangeCount, VkPushConstantRange* pushConstantRanges);
	std::shared_ptr<VkRenderPass> CreateRenderPass(uint32_t attachmentCount, VkAttachmentDescription* attachments, uint32_t subpassCount, VkSubpassDescription* subpasses, uint32_t dependencyCount, VkSubpassDependency* dependencies);


	VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	VkFormat FindDepthFormat();





	bool IsPhysicalDeviceSuitable(VkPhysicalDevice physicalDevice);
public:
	static std::optional<uint32_t> GetQueueFamilyIndex(std::vector<VkQueueFamilyProperties>& queueFamilyProperties, VkQueueFlagBits queueFlags);

public:
	VkDevice device = VK_NULL_HANDLE;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkQueue graphicsQueue = VK_NULL_HANDLE;
	VkQueue presentQueue = VK_NULL_HANDLE;

	struct
	{
		std::optional<uint32_t> graphics;
		std::optional<uint32_t> compute;
		std::optional<uint32_t> transfer;
		std::optional<uint32_t> present;
	} queueFamilyIndices;


};

