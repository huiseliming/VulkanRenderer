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
#include "File.h"


VulkanDevice g_device; 
VulkanSwapChain g_swapChain;
Window g_window;


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
	auto fragShaderModule = g_device.CreateShaderModule(File::ReadFile("frag.spv"));
	auto vertShaderModule = g_device.CreateShaderModule(File::ReadFile("vert.spv"));

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


















