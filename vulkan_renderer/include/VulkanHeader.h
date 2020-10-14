#pragma once
#define VK_USE_PLATFORM_WIN32_KHR
#include "vulkan/vulkan.h"
#include "Exception.h"

#define VK_ASSERT_SUCCESSED(result) \
		if((result) != VK_SUCCESS){\
			THROW_EXCEPT("Assert "#result "Failed");\
		}

#define VK_DESTROY_CALLBACK(Device, VulkanObject) \
[device = Device](const Vk##VulkanObject* p##VulkanObject)\
{\
	if(*p##VulkanObject != VK_NULL_HANDLE)\
		vkDestroy##VulkanObject(Device, *p##VulkanObject, nullptr);\
	delete p##VulkanObject;\
}



