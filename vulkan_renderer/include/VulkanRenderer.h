#pragma once
#include <vector>
#include "VulkanHeader.h"
namespace Renderer 
{
	extern std::vector<const char*> enabledInstanceExtensions;
	extern std::vector<const char*> enabledInstanceValidationLayers;

	void Initialize(const char* applicationName);
	void Cleanup();

	VkInstance& GetInstance();

	void CreateInstance(const char* applicationName, std::vector<const char*> enabledInstanceExtensions, std::vector<const char*> enabledInstanceValidationLayers);

	namespace Debug {
		void SetupDebugMessenger();
		void CleanupDebugMessenger();
	}

}













