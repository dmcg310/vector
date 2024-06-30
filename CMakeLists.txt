cmake_minimum_required(VERSION 3.25)

project(vector VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-D_DEBUG)
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
    add_definitions(-D_RELEASE)
endif ()

# Enable verbose output
set(CMAKE_VERBOSE_MAKEFILE ON)

include(FetchContent)

FetchContent_Declare(
        imgui-glfw-glad-glm
        GIT_REPOSITORY https://github.com/cmmw/imgui-glfw-glad-glm.git
)
FetchContent_MakeAvailable(imgui-glfw-glad-glm)

# Add subdirectories
add_subdirectory(lib_window)
add_subdirectory(lib_application)
add_subdirectory(lib_log)
add_subdirectory(lib_renderer)
add_subdirectory(lib_imgui)
add_subdirectory(engine_src)