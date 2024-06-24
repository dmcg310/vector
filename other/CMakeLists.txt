# Required for ImGui

cmake_minimum_required(VERSION 3.10)
project(ImGui)

set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB IMGUI_SOURCES
    ${IMGUI_DIR}/*.cpp
    ${IMGUI_DIR}/example_glfw_opengl3/main.cpp
)

add_library(lib_imgui STATIC ${IMGUI_SOURCES})
target_include_directories(lib_imgui PUBLIC ${IMGUI_DIR} ${IMGUI_DIR}/backends ${IMGUI_DIR}/examples)
