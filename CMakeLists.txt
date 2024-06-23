cmake_minimum_required(VERSION 3.10)
project(vector)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_definitions(-D_DEBUG)
endif()

# set the policy for finding OpenGL to prefer GLVND if available
if(POLICY CMP0072)
  cmake_policy(SET CMP0072 NEW)
endif()

add_subdirectory(lib_window)
add_subdirectory(lib_application)
add_subdirectory(lib_log)
add_subdirectory(lib_imgui)
add_subdirectory(lib_renderer)
add_subdirectory(engine_src)
add_subdirectory(${CMAKE_SOURCE_DIR}/external/glad)

if (WIN32)
  add_subdirectory(${CMAKE_SOURCE_DIR}/external/glfw)
ENDIF()
