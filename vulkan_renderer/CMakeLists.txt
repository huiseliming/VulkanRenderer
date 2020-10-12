cmake_minimum_required(VERSION 3.10)


#添加
file(GLOB_RECURSE VulkanRendererSourceFiles 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)
message(${VulkanRendererSourceFiles} )
file(GLOB_RECURSE VulkanRendererHeaderFiles 
    ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h
)
#message(${VulkanRendererHeaderFiles} )

add_library(vulkan_renderer STATIC  ${VulkanRendererSourceFiles})
message(${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(vulkan_renderer PUBLIC Vulkan::Vulkan)
target_link_libraries(vulkan_renderer PRIVATE glm)
#target_link_libraries(vulkan_renderer PRIVATE assimp::assimp)
target_link_libraries(vulkan_renderer PRIVATE fmt::fmt-header-only)

target_include_directories(vulkan_renderer PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
target_include_directories(vulkan_renderer PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src)

#SET_TARGET_PROPERTIES(vulkan_renderer PROPERTIES VERSION 1.0 SOVERSION 1)

INSTALL(TARGETS vulkan_renderer ARCHIVE DESTINATION lib)
INSTALL(FILES VulkanRendererHeaderFiles DESTINATION include/vulkan_renderer)









