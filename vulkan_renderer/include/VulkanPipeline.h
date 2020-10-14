#pragma once
#include <array>
#include <vector>
#include "VulkanHeader.h"

class VulkanDevice;

class VulkanPipeline 
{
public:
    VulkanPipeline();
    explicit VulkanPipeline(VulkanDevice& device);
    ~VulkanPipeline();

    void Create(VulkanDevice& device);
    void Destroy();

    void SetShader(VkShaderStageFlagBits shaderStageFlag,VkShaderModule shaderModule);
    void SetVertexShader(VkShaderModule shaderModule);
    void SetFragmentShader(VkShaderModule shaderModule);
    void SetVertexInputState(uint32_t bindingDescriptionCount, VkVertexInputBindingDescription* pBindingDescriptions, uint32_t attributeDescriptionCount, VkVertexInputAttributeDescription* pAttributeDescriptions);
    void SetInputAssemblyState(VkPrimitiveTopology topology, VkBool32 primitiveRestartEnable);

    void SetTessellationState(uint32_t patchControlPoints);
    void SetViewportState(uint32_t viewportCount, VkViewport* pViewports, uint32_t scissorCount, VkRect2D* pScissors);
    void SetRasterizationState(VkBool32 depthClampEnable, VkBool32 rasterizerDiscardEnable, VkPolygonMode polygonMode, VkCullModeFlags cullMode, VkFrontFace frontFace, VkBool32 depthBiasEnable, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor, float lineWidth);
    void SetMultisampleState(VkSampleCountFlagBits rasterizationSamples, VkBool32 sampleShadingEnable, float minSampleShading, const VkSampleMask* pSampleMask, VkBool32 alphaToCoverageEnable, VkBool32 alphaToOneEnable);

    void SetDepthStencilState(VkBool32 depthTestEnable, VkBool32 depthWriteEnable, VkCompareOp depthCompareOp, VkBool32 depthBoundsTestEnable, VkBool32 stencilTestEnable, VkStencilOpState front, VkStencilOpState back, float minDepthBounds, float maxDepthBounds);
    void SetColorBlendState(VkBool32 logicOpEnable, VkLogicOp logicOp, uint32_t attachmentCount, VkPipelineColorBlendAttachmentState* pAttachments, float blendConstants[4]);
    void SetDynamicState(uint32_t dynamicStateCount, VkDynamicState* pDynamicStates);

    //VkPipeline Get() { return pipeline_; }
private:

    VkPipeline m_pipeline = VK_NULL_HANDLE;
    VkGraphicsPipelineCreateInfo m_graphicsPipelineCreateInfo = {};
    uint32_t m_shaderStagesMask = 0;
    std::array<VkPipelineShaderStageCreateInfo, 32> m_shaderStagesCache = {};



    //VkStructureType                                  sType;
    //const void*                                      pNext;
    //VkPipelineCreateFlags                            flags;
    std::vector<VkPipelineShaderStageCreateInfo> m_shaderStages = {};
    VkPipelineVertexInputStateCreateInfo m_vertexInputState = {};
    VkPipelineInputAssemblyStateCreateInfo m_inputAssemblyState = {};
    VkPipelineTessellationStateCreateInfo m_tessellationState = {};
    VkPipelineViewportStateCreateInfo m_ViewportState = {};
    VkPipelineRasterizationStateCreateInfo m_rasterizationState = {};
    VkPipelineMultisampleStateCreateInfo m_multisampleState = {};
    VkPipelineDepthStencilStateCreateInfo m_depthStencilState = {};
    VkPipelineColorBlendStateCreateInfo m_colorBlendState = {};
    VkPipelineDynamicStateCreateInfo m_dynamicState = {};
    //VkPipelineLayout                                 layout;
    //VkRenderPass                                     renderPass;
    //uint32_t                                         subpass;
    //VkPipeline                                       basePipelineHandle;
    //int32_t                                          basePipelineIndex;


};























