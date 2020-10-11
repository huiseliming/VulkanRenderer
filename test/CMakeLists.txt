



#添加
file(GLOB_RECURSE TestApplicationSourceFiles 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
    )

add_executable(TestApplication ${TestApplicationSourceFiles})

target_include_directories(TestApplication PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src)

target_link_libraries(TestApplication PRIVATE vulkan_renderer)
target_link_libraries(TestApplication PRIVATE glfw)


