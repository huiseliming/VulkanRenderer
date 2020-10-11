#pragma once
#define VK_USE_PLATFORM_WIN32_KHR
#include "vulkan/vulkan.h"

#define VK_ASSERT_SUCCESSED(result) \
		if((result) != VK_SUCCESS){\
			THROW_EXCEPT("Assert "#result "Failed");\
		}




