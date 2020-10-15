#pragma once
#include <vector>

#include "VulkanHeader.h"

class VulkanDevice;

class VulkanRenderPass
{
public:
    VulkanRenderPass();
    explicit VulkanRenderPass(VulkanDevice& device);
    ~VulkanRenderPass();

    operator VkRenderPass() { return m_renderPass; }

    void Create(VulkanDevice& device);
    void Destroy();

    VulkanRenderPass(const VulkanRenderPass& other) = delete;
    VulkanRenderPass(VulkanRenderPass&& other) = delete;
    VulkanRenderPass& operator=(const VulkanRenderPass& other) = delete;
    VulkanRenderPass& operator=(VulkanRenderPass&& other) = delete;

    void SetAttachmentDescriptions(uint32_t attachmentCount, VkAttachmentDescription* Attachments);
    void SetSubpasses(uint32_t subpassCount, VkSubpassDescription* subpasses);
    void SetDependencies(uint32_t dependencyCount, VkSubpassDependency* dependencies);

    void PushAttachmentDescription(VkAttachmentDescriptionFlags flags, VkFormat format, VkSampleCountFlagBits samples, VkAttachmentLoadOp loadOp, VkAttachmentStoreOp storeOp, VkAttachmentLoadOp stencilLoadOp, VkAttachmentStoreOp stencilStoreOp, VkImageLayout initialLayout, VkImageLayout finalLayout);
    void PushSubpassDescription(VkSubpassDescriptionFlags flags, VkPipelineBindPoint pipelineBindPoint, uint32_t inputAttachmentCount, const VkAttachmentReference* pInputAttachments, uint32_t colorAttachmentCount, const VkAttachmentReference* pColorAttachments, const VkAttachmentReference* pResolveAttachments, const VkAttachmentReference* pDepthStencilAttachment, uint32_t preserveAttachmentCount, const uint32_t* pPreserveAttachments);
    void PushSubpassDependency(uint32_t srcSubpass, uint32_t dstSubpass, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask, VkDependencyFlags dependencyFlags);

private:
    VulkanDevice* m_pDevice = nullptr;

    VkRenderPass m_renderPass = VK_NULL_HANDLE;
    VkRenderPassCreateInfo m_renderPassCreateInfo = {};

    std::vector<VkAttachmentDescription> m_attachmentDescriptions;
    std::vector<VkSubpassDescription> m_subpasses;
    std::vector<VkSubpassDependency> m_dependencies;

};







