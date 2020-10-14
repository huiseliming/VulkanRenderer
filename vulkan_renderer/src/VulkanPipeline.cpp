#include "..\include\VulkanPipeline.h"

VulkanPipeline::VulkanPipeline()
{
}

VulkanPipeline::~VulkanPipeline()
{
}

void VulkanPipeline::Create()
{

}

void VulkanPipeline::Destroy() 
{

}

void VulkanPipeline::SetVertexShader(VkShaderModule shaderModule)
{
    m_pipelineShaderStageCreateInfos[kVertexShaderStage].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    m_pipelineShaderStageCreateInfos[kVertexShaderStage].stage = VK_SHADER_STAGE_VERTEX_BIT;
    m_pipelineShaderStageCreateInfos[kVertexShaderStage].module = shaderModule;
    m_pipelineShaderStageCreateInfos[kVertexShaderStage].pName = "main";
    m_pipelineShaderStageCreateInfos[kVertexShaderStage].pSpecializationInfo = nullptr;
}

void VulkanPipeline::SetFragmentShader(VkShaderModule shaderModule)
{
    m_pipelineShaderStageCreateInfos[kFragmentShaderStage].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    m_pipelineShaderStageCreateInfos[kFragmentShaderStage].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    m_pipelineShaderStageCreateInfos[kFragmentShaderStage].module = shaderModule;
    m_pipelineShaderStageCreateInfos[kFragmentShaderStage].pName = "main";
    m_pipelineShaderStageCreateInfos[kFragmentShaderStage].pSpecializationInfo = nullptr;
}