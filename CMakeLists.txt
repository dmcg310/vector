cmake_minimum_required(VERSION 3.25)

project(vector VERSION 1.0.0)

message(STATUS "Starting top-level CMakeLists.txt")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-D_DEBUG)
    message(STATUS "Debug build")
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
    add_definitions(-D_RELEASE)
    message(STATUS "Release build")
endif ()

# Enable verbose output
set(CMAKE_VERBOSE_MAKEFILE ON)

# Add vcpkg integration
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
            CACHE STRING "")
    message(STATUS "Using vcpkg toolchain file: ${CMAKE_TOOLCHAIN_FILE}")
endif()

# Add subdirectories
message(STATUS "Adding engine subdirectory")
add_subdirectory(engine)

message(STATUS "Adding application subdirectory")
add_subdirectory(application)

message(STATUS "Finished top-level CMakeLists.txt")