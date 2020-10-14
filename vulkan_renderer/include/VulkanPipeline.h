#pragma once
#include <array>
#include <vector>
#include "VulkanHeader.h"


class VulkanPipeline 
{
public:
    VulkanPipeline();
    ~VulkanPipeline();

    void Create();
    void Destroy();


    void SetVertexShader(VkShaderModule shaderModule);
    void SetFragmentShader(VkShaderModule shaderModule);
//void SetVertexInputState(uint32_t binding_description_count, VkVertexInputBindingDescription* binding_description_ptr, uint32_t attribute_description_count, VkVertexInputAttributeDescription* attribute_description_ptr);

    //VkPipeline Get() { return pipeline_; }
private:

    enum {
        kVertexShaderStage,
        kFragmentShaderStage,
        kMaxShaderStageTypeCount
    };

    VkGraphicsPipelineCreateInfo m_graphicsPipelineCreateInfo;

    std::array<VkPipelineShaderStageCreateInfo, kMaxShaderStageTypeCount> m_pipelineShaderStageCreateInfos;

    std::vector<VkPipelineShaderStageCreateInfo> m_pShaderStages;
    VkPipelineVertexInputStateCreateInfo m_vertexInputState;
    VkPipelineInputAssemblyStateCreateInfo m_inputAssemblyState;
    VkPipelineTessellationStateCreateInfo m_tessellationState;
    VkPipelineViewportStateCreateInfo m_ViewportState;
    VkPipelineRasterizationStateCreateInfo m_rasterizationState;
    VkPipelineMultisampleStateCreateInfo m_multisampleState;
    VkPipelineDepthStencilStateCreateInfo m_depthStencilState;
    VkPipelineColorBlendStateCreateInfo m_colorBlendState;
    VkPipelineDynamicStateCreateInfo m_dynamicState;



};























