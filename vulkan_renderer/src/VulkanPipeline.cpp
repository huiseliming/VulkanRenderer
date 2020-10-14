#include "VulkanPipeline.h"
#include "VulkanDevice.h"

VulkanPipeline::VulkanPipeline()
{
}

VulkanPipeline::VulkanPipeline(VulkanDevice& device)
{
    Create(device);
}

VulkanPipeline::~VulkanPipeline()
{
}

void VulkanPipeline::Create(VulkanDevice& device)
{
    uint32_t index;
    uint8_t isNonzero = _BitScanReverse(reinterpret_cast<unsigned long*>(&index), m_shaderStagesMask);
    while (isNonzero)
        m_shaderStages.push_back(m_shaderStagesCache[index]);
    m_graphicsPipelineCreateInfo.stageCount = m_shaderStages.size();
    m_graphicsPipelineCreateInfo.pStages = m_shaderStages.data();
    VK_ASSERT_SUCCESSED(vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &m_graphicsPipelineCreateInfo, nullptr, &m_pipeline));
}

void VulkanPipeline::Destroy() 
{

}

void VulkanPipeline::SetShader(VkShaderStageFlagBits shaderStageFlag, VkShaderModule shaderModule)
{
    uint32_t index;
    uint8_t isNonzero = _BitScanReverse(reinterpret_cast<unsigned long*>(&index), shaderStageFlag);
    if (!isNonzero)
        THROW_VK_EXCEPT("shaderStageFlag is not set!", VkResult::VK_ERROR_UNKNOWN);
    m_shaderStagesMask |= shaderStageFlag;
    m_shaderStagesCache[index].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    m_shaderStagesCache[index].pNext = nullptr;
    m_shaderStagesCache[index].flags = 0;
    m_shaderStagesCache[index].stage = shaderStageFlag;
    m_shaderStagesCache[index].module = shaderModule;
    m_shaderStagesCache[index].pName = "main";
    m_shaderStagesCache[index].pSpecializationInfo = nullptr;
}

void VulkanPipeline::SetVertexShader(VkShaderModule shaderModule)
{
    SetShader(VK_SHADER_STAGE_VERTEX_BIT, shaderModule);
}

void VulkanPipeline::SetFragmentShader(VkShaderModule shaderModule)
{
    SetShader(VK_SHADER_STAGE_FRAGMENT_BIT, shaderModule);
}

void VulkanPipeline::SetVertexInputState(uint32_t bindingDescriptionCount, VkVertexInputBindingDescription* pBindingDescriptions, uint32_t attributeDescriptionCount, VkVertexInputAttributeDescription* pAttributeDescriptions)
{
    m_vertexInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    m_vertexInputState.pNext = nullptr;
    m_vertexInputState.flags = 0;
    m_vertexInputState.vertexBindingDescriptionCount = bindingDescriptionCount;
    m_vertexInputState.pVertexBindingDescriptions = pBindingDescriptions;
    m_vertexInputState.vertexAttributeDescriptionCount = attributeDescriptionCount;
    m_vertexInputState.pVertexAttributeDescriptions = pAttributeDescriptions;
}

void VulkanPipeline::SetInputAssemblyState(VkPrimitiveTopology topology, VkBool32 primitiveRestartEnable)
{
    m_inputAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    m_inputAssemblyState.pNext = nullptr;
    m_inputAssemblyState.flags = 0;
    m_inputAssemblyState.topology = topology;
    m_inputAssemblyState.primitiveRestartEnable = primitiveRestartEnable;
}

void VulkanPipeline::SetTessellationState(uint32_t patchControlPoints)
{
    m_tessellationState.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
    m_tessellationState.pNext = nullptr;
    m_tessellationState.flags = 0;
    m_tessellationState.patchControlPoints = patchControlPoints;
}

void VulkanPipeline::SetViewportState(uint32_t viewportCount, VkViewport* pViewports, uint32_t scissorCount, VkRect2D* pScissors)
{
    m_ViewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    m_ViewportState.pNext = nullptr;
    m_ViewportState.flags = 0;
    m_ViewportState.viewportCount = viewportCount;
    m_ViewportState.pViewports = pViewports;
    m_ViewportState.scissorCount = scissorCount;
    m_ViewportState.pScissors = pScissors;
}

void VulkanPipeline::SetRasterizationState(VkBool32 depthClampEnable, VkBool32 rasterizerDiscardEnable, VkPolygonMode polygonMode, VkCullModeFlags cullMode, VkFrontFace frontFace, VkBool32 depthBiasEnable, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor, float lineWidth)
{
    m_rasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    m_rasterizationState.pNext = nullptr;
    m_rasterizationState.flags = 0;
    m_rasterizationState.depthClampEnable = depthClampEnable;
    m_rasterizationState.rasterizerDiscardEnable = rasterizerDiscardEnable;
    m_rasterizationState.polygonMode = polygonMode;
    m_rasterizationState.cullMode = cullMode;
    m_rasterizationState.frontFace = frontFace;
    m_rasterizationState.depthBiasEnable = depthBiasEnable;
    m_rasterizationState.depthBiasConstantFactor = depthBiasConstantFactor;
    m_rasterizationState.depthBiasClamp = depthBiasClamp;
    m_rasterizationState.depthBiasSlopeFactor = depthBiasSlopeFactor;
    m_rasterizationState.lineWidth = lineWidth;
}

void VulkanPipeline::SetMultisampleState(VkSampleCountFlagBits rasterizationSamples, VkBool32 sampleShadingEnable, float minSampleShading, const VkSampleMask* pSampleMask, VkBool32 alphaToCoverageEnable, VkBool32 alphaToOneEnable)
{
    m_multisampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    m_multisampleState.pNext = nullptr;
    m_multisampleState.flags = 0;
    m_multisampleState.rasterizationSamples = rasterizationSamples;
    m_multisampleState.sampleShadingEnable = sampleShadingEnable;
    m_multisampleState.minSampleShading = minSampleShading;
    m_multisampleState.pSampleMask = pSampleMask;
    m_multisampleState.alphaToCoverageEnable = alphaToCoverageEnable;
    m_multisampleState.alphaToOneEnable = alphaToOneEnable;
}

void VulkanPipeline::SetDepthStencilState(VkBool32 depthTestEnable, VkBool32 depthWriteEnable, VkCompareOp depthCompareOp, VkBool32 depthBoundsTestEnable, VkBool32 stencilTestEnable, VkStencilOpState front, VkStencilOpState back, float minDepthBounds, float maxDepthBounds)
{
    m_depthStencilState.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    m_depthStencilState.pNext = nullptr;
    m_depthStencilState.flags = 0;
    m_depthStencilState.depthTestEnable = depthTestEnable;
    m_depthStencilState.depthWriteEnable = depthWriteEnable;
    m_depthStencilState.depthCompareOp = depthCompareOp;
    m_depthStencilState.depthBoundsTestEnable = depthBoundsTestEnable;
    m_depthStencilState.stencilTestEnable = stencilTestEnable;
    m_depthStencilState.front = front;
    m_depthStencilState.back = back;
    m_depthStencilState.minDepthBounds = minDepthBounds;
    m_depthStencilState.maxDepthBounds = maxDepthBounds;
}

void VulkanPipeline::SetColorBlendState(VkBool32 logicOpEnable, VkLogicOp logicOp, uint32_t attachmentCount, VkPipelineColorBlendAttachmentState* pAttachments, float blendConstants[4])
{
    m_colorBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    m_colorBlendState.pNext = nullptr;
    m_colorBlendState.flags = 0;
    m_colorBlendState.logicOpEnable = logicOpEnable;
    m_colorBlendState.logicOp = logicOp;
    m_colorBlendState.attachmentCount = attachmentCount;
    m_colorBlendState.pAttachments = pAttachments;
    m_colorBlendState.blendConstants[0] = blendConstants[0];
    m_colorBlendState.blendConstants[1] = blendConstants[1];
    m_colorBlendState.blendConstants[2] = blendConstants[2];
    m_colorBlendState.blendConstants[3] = blendConstants[3];
}

void VulkanPipeline::SetDynamicState(uint32_t dynamicStateCount, VkDynamicState* pDynamicStates)
{
    m_dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    m_dynamicState.pNext = nullptr;
    m_dynamicState.flags = 0;
    m_dynamicState.dynamicStateCount = dynamicStateCount;
    m_dynamicState.pDynamicStates = pDynamicStates;
}








