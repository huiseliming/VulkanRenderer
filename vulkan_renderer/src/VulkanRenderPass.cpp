#include "VulkanRenderPass.h"
#include "VulkanDevice.h"

VulkanRenderPass::VulkanRenderPass()
{
	m_renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
}

VulkanRenderPass::VulkanRenderPass(VulkanDevice& device)
{
	VulkanRenderPass();
}

VulkanRenderPass::~VulkanRenderPass()
{
}

void VulkanRenderPass::Create(VulkanDevice& device)
{
	m_renderPassCreateInfo.pNext = nullptr;
	m_renderPassCreateInfo.flags = 0;
	m_renderPassCreateInfo.attachmentCount = static_cast<uint32_t>(m_attachmentDescriptions.size());
	m_renderPassCreateInfo.pAttachments = m_attachmentDescriptions.data();
	m_renderPassCreateInfo.subpassCount = static_cast<uint32_t>(m_subpasses.size());
	m_renderPassCreateInfo.pSubpasses = m_subpasses.data();
	m_renderPassCreateInfo.dependencyCount = static_cast<uint32_t>(m_dependencies.size());
	m_renderPassCreateInfo.pDependencies = m_dependencies.data();
	VK_ASSERT_SUCCESSED(vkCreateRenderPass(*m_pDevice, &m_renderPassCreateInfo, nullptr, &m_renderPass))
}

void VulkanRenderPass::Destroy()
{
}

void VulkanRenderPass::SetAttachmentDescriptions(uint32_t attachmentCount, VkAttachmentDescription* Attachments)
{
	m_attachmentDescriptions.clear();
	for (size_t i = 0; i < attachmentCount; i++)
		m_attachmentDescriptions.push_back(Attachments[i]);
}

void VulkanRenderPass::SetSubpasses(uint32_t subpassCount, VkSubpassDescription* subpasses)
{
	m_subpasses.clear();
	for (size_t i = 0; i < subpassCount; i++)
		m_subpasses.push_back(subpasses[i]);
}

void VulkanRenderPass::SetDependencies(uint32_t dependencyCount, VkSubpassDependency* dependencies)
{
	m_dependencies.clear();
	for (size_t i = 0; i < dependencyCount; i++)
		m_dependencies.push_back(dependencies[i]);
}

void VulkanRenderPass::PushAttachmentDescription(VkAttachmentDescriptionFlags flags, VkFormat format, VkSampleCountFlagBits samples, VkAttachmentLoadOp loadOp, VkAttachmentStoreOp storeOp, VkAttachmentLoadOp stencilLoadOp, VkAttachmentStoreOp stencilStoreOp, VkImageLayout initialLayout, VkImageLayout finalLayout)
{
	m_attachmentDescriptions.push_back({ flags, format, samples, loadOp, storeOp, stencilLoadOp, stencilStoreOp, initialLayout, finalLayout });
}

void VulkanRenderPass::PushSubpassDescription(VkSubpassDescriptionFlags flags = 0, VkPipelineBindPoint pipelineBindPoint = 0, uint32_t inputAttachmentCount = 0, const VkAttachmentReference* pInputAttachments = nullptr, uint32_t colorAttachmentCount = 0, const VkAttachmentReference* pColorAttachments = nullptr, const VkAttachmentReference* pResolveAttachments = nullptr, const VkAttachmentReference* pDepthStencilAttachment = nullptr, uint32_t preserveAttachmentCount = 0, const uint32_t* pPreserveAttachments = nullptr)
{
	m_subpasses.push_back({ flags, pipelineBindPoint, inputAttachmentCount, pInputAttachments, colorAttachmentCount, pColorAttachments, pResolveAttachments, pDepthStencilAttachment, preserveAttachmentCount, pPreserveAttachments });
}
void VulkanRenderPass::PushSubpassDependency(uint32_t srcSubpass, uint32_t dstSubpass, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask, VkDependencyFlags dependencyFlags)
{
	m_dependencies.push_back({ srcSubpass,  dstSubpass,  srcStageMask,  dstStageMask,  srcAccessMask,  dstAccessMask,  dependencyFlags });
}
