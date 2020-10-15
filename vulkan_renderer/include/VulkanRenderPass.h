#pragma once
#include "VulkanHeader.h"

class VulkanDevice;

class VulkanRenderPass
{
public:
    VulkanRenderPass();
    explicit VulkanRenderPass(VulkanDevice& device);
    ~VulkanRenderPass();

    void Create(VulkanDevice& device);
    void Destroy();

    VulkanRenderPass(const VulkanRenderPass& other) = delete;
    VulkanRenderPass(VulkanRenderPass&& other) = delete;
    VulkanRenderPass& operator=(const VulkanRenderPass& other) = delete;
    VulkanRenderPass& operator=(VulkanRenderPass&& other) = delete;

private:
    VulkanDevice* m_pDevice;

};









