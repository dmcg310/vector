cmake_minimum_required(VERSION 3.25)

project(application)

message(STATUS "Starting application CMakeLists.txt")

# Add source files
file(GLOB_RECURSE APP_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
message(STATUS "Application source files: ${APP_SOURCES}")

# Create executable
add_executable(application ${APP_SOURCES})

# Link against the engine library
target_link_libraries(application engine)
message(STATUS "Linked application with engine")

# Include directories
target_include_directories(application PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/../engine
        ${CMAKE_CURRENT_SOURCE_DIR}/../engine/core
        ${CMAKE_CURRENT_SOURCE_DIR}/../engine/nodes
        ${CMAKE_CURRENT_SOURCE_DIR}/../engine/renderer
        ${CMAKE_CURRENT_SOURCE_DIR}/../engine/scene
)

message(STATUS "Include directories for application set")

message(STATUS "Finished application CMakeLists.txt")