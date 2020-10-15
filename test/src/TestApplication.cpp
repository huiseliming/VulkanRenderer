#include "TestApplication.h"
#include <iostream>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "Singleton.h"
#include "Window.h"
#include "Exception.h"
#include "VulkanRenderer.h"
#include "VulkanDevice.h"
#include "VulkanSwapChain.h"
#include "VulkanPipeline.h"
#include "VulkanRenderPass.h"
#include "File.h"


Window g_window;
VulkanDevice g_device; 
VulkanSwapChain g_swapChain;
VulkanPipeline g_pipeline;
VulkanRenderPass g_renderPass;

void Application::Run()
{
	glfwInit();
	StartUp();
	MainLoop();
	Cleanup();
	glfwTerminate();
}

void Application::StartUp()
{
	Renderer::Initialize("TestApplication");
	g_window.Create();
	g_device.Create(g_window.GetSurfaceKHR(), { VK_KHR_SWAPCHAIN_EXTENSION_NAME });
	g_swapChain.Create(g_device, g_window.GetSurfaceKHR());
	//Create GraphicsPipeline
	auto fragShaderModule = g_device.CreateShaderModule(File::ReadFile("frag.spv"));
	auto vertShaderModule = g_device.CreateShaderModule(File::ReadFile("vert.spv"));
	g_pipeline.SetVertexShader(fragShaderModule);
	g_pipeline.SetFragmentShader(vertShaderModule);
	g_pipeline.SetVertexInputState(0, nullptr, 0, nullptr);
	g_pipeline.SetInputAssemblyState(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FALSE);
	VkViewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)g_swapChain.GetExtent().width;
	viewport.height = (float)g_swapChain.GetExtent().height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = g_swapChain.GetExtent();
	g_pipeline.SetViewportState(1, &viewport, 0, &scissor);
	g_pipeline.SetRasterizationState(VK_FALSE, VK_FALSE, VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT, VK_FRONT_FACE_COUNTER_CLOCKWISE, VK_FALSE, 0.0f, 0.0f, 0.0f, 1.0f);
	g_pipeline.SetMultisampleState(VK_SAMPLE_COUNT_1_BIT, VK_FALSE, 1.0f,nullptr, VK_FALSE, VK_FALSE);
	g_pipeline.SetDepthStencilState(VK_FALSE, VK_FALSE, VK_COMPARE_OP_LESS, VK_FALSE, VK_FALSE, {}, {}, 0.0f, 0.0f);
	VkPipelineColorBlendAttachmentState  colorBlendAttachmentState = {};
	colorBlendAttachmentState.blendEnable = VK_FALSE;
	colorBlendAttachmentState.srcColorBlendFactor= VK_BLEND_FACTOR_ONE;
	colorBlendAttachmentState.dstColorBlendFactor= VK_BLEND_FACTOR_ZERO;
	colorBlendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
	colorBlendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	colorBlendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD;
	colorBlendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	float blendConstants[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	g_pipeline.SetColorBlendState(VK_FALSE, VK_LOGIC_OP_COPY, 1, &colorBlendAttachmentState, blendConstants);
	std::vector<VkDynamicState> dynamicStates = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_LINE_WIDTH };
	g_pipeline.SetDynamicState(dynamicStates.size(), dynamicStates.data());
	g_pipeline.SetLayout(0, nullptr, 0, nullptr);
	g_renderPass.PushAttachmentDescription(0, g_swapChain.GetFormat(), VK_SAMPLE_COUNT_1_BIT, VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE, VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_ATTACHMENT_STORE_OP_DONT_CARE, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
	VkAttachmentReference colorAttachmentRef{};
	colorAttachmentRef.attachment = 0;
	colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	g_renderPass.PushSubpassDescription(0, VK_PIPELINE_BIND_POINT_GRAPHICS, 0, nullptr, 1, &colorAttachmentRef, nullptr, nullptr, 0, nullptr);
	g_renderPass.Create(g_device);
	g_pipeline.SetRenderPass(g_renderPass);
	g_pipeline.Create(g_device);
	vkDestroyShaderModule(g_device, fragShaderModule, nullptr);
	vkDestroyShaderModule(g_device, vertShaderModule, nullptr);

}

void Application::MainLoop()
{



	while (!g_window.ShouldClose())
	{
		g_window.PollEvents();
	}
}

void Application::Cleanup()
{
	g_pipeline.Destroy();
	g_swapChain.Destroy();
	g_device.Destroy();
	g_window.Destroy();
	Renderer::Cleanup();
}

int main(int argc, char* const argv[])\
{
	try
	{
		Singleton<Application>::GetInstance().Run();
	}
	catch (const std::exception& execption)
	{
		std::cout << "Catch std::exception " << std::endl << "[" << execption.what() << "]" << std::endl;
		return EXIT_FAILURE;
	}
	catch (...)
	{
		std::cout << "Catch unknow exception" << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}


















