cmake_minimum_required(VERSION 3.25)

project(engine VERSION 1.0.0)

message(STATUS "Starting engine CMakeLists.txt")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-D_DEBUG)
    message(STATUS "Debug build in engine")
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
    add_definitions(-D_RELEASE)
    message(STATUS "Release build in engine")
endif ()

# Enable verbose output
set(CMAKE_VERBOSE_MAKEFILE ON)

# Add the include directories for the dependencies
message(STATUS "Finding packages in engine")
find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

# Include directories for ImGui backends
include_directories(${VCPKG_INSTALLED_DIR}/x64-windows/include/imgui/backends)

# Add all source files
file(GLOB_RECURSE ENGINE_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/core/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/nodes/**/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/renderer/**/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/scene/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/core/window/imgui/imgui_manager.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/core/window/imgui/imgui_impl_glfw.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/core/window/imgui/imgui_impl_opengl3.cpp"
)

# Add all header files
file(GLOB_RECURSE ENGINE_HEADERS
        "${CMAKE_CURRENT_SOURCE_DIR}/core/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/nodes/**/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/renderer/**/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/scene/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)

message(STATUS "Source files: ${ENGINE_SOURCES}")
message(STATUS "Header files: ${ENGINE_HEADERS}")

# Create library
add_library(engine STATIC ${ENGINE_SOURCES} ${ENGINE_HEADERS})

# Link libraries
target_link_libraries(engine glad::glad glfw glm::glm imgui::imgui)

# Include directories for the engine
target_include_directories(engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

message(STATUS "Finished engine CMakeLists.txt")