cmake_minimum_required(VERSION 3.10)

project(GreyDawnEngine)

find_package(glfw3 CONFIG REQUIRED)
find_package(Vulkan REQUIRED) 
find_package(glm REQUIRED) 
find_package(fmt REQUIRED) 
find_package(assimp CONFIG REQUIRED)

if(NOT DEFINED CMAKE_BUILD_TYPE_INIT)
  set(CMAKE_BUILD_TYPE_INIT "Release")
  message(CMAKE_BUILD_TYPE "Release")
endif()

set(CMAKE_CXX_STANDARD 17)

set(ExecutableOutputPath ${CMAKE_BINARY_DIR}/bin CACHE INTERNAL "" FORCE)
set(LibraryOutputPath ${CMAKE_BINARY_DIR}/lib CACHE INTERNAL "" FORCE)
set(TestOutputPath ${EXECUTABLE_OUTPUT_PATH}/tests CACHE INTERNAL "" FORCE)
set(BINARY_OUTPUT_DIR ${CMAKE_BINARY_DIR}/bin CACHE INTERNAL "" FORCE)
set(LIBRARY_OUTPUT_DIR ${CMAKE_BINARY_DIR}/lib CACHE INTERNAL "" FORCE)

message("Current Build Setting")
message("BuildType:" ${CMAKE_BUILD_TYPE})
message("SourceDir:" ${CMAKE_SOURCE_DIR})
message("BinaryDir:" ${CMAKE_BINARY_DIR})
message("ModulePth:" ${CMAKE_MODULE_PATH})
message("BinaryDir:" ${BINARY_OUTPUT_DIR})
message("LibraryOutputDir:" ${LIBRARY_OUTPUT_DIR})

add_subdirectory(vulkan_renderer)
add_subdirectory(test)
