#!/bin/bash

# https://glad.dav1d.de/#language=c&specification=gl&api=gl%3D4.6&api=gles1%3Dnone&api=gles2%3Dnone&api=glsc2%3Dnone&profile=compatibility&loader=on

# --- Setup glad ---
EXTERNAL_DIR="external"
GLAD_DIR="${EXTERNAL_DIR}/glad"
INCLUDE_DIR="${GLAD_DIR}/include"
GLAD_SRC="${GLAD_DIR}/src/glad.c"
GLAD_CMAKE="${GLAD_DIR}/CMakeLists.txt"
GLAD_ZIP="${EXTERNAL_DIR}/glad.zip"

mkdir -p ${GLAD_DIR}
mkdir -p ${INCLUDE_DIR}

echo "Extracting glad..."
unzip -o ${GLAD_ZIP} -d ${GLAD_DIR}

echo "Creating CMakeLists.txt for glad..."
cat <<EOL > ${GLAD_CMAKE}
add_library(glad STATIC src/glad.c)
target_include_directories(glad PUBLIC include)
EOL

echo "Glad setup complete."

# --- Setup ImGui ---
IMGUI_DIR="lib_imgui"
IMGUI_REPO="https://github.com/ocornut/imgui.git"

if [ -d "${IMGUI_DIR}" ] && [ -f "${IMGUI_DIR}/.git" ]; then
    echo "Updating ImGui submodule..."
    git submodule update --init --recursive
else
    echo "Adding ImGui as a submodule..."
    git submodule add ${IMGUI_REPO} ${IMGUI_DIR}
    git submodule update --init --recursive
fi

echo "Creating CMakeLists.txt for ImGui..."
cat <<EOL > ${IMGUI_DIR}/CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(ImGui)

set(IMGUI_DIR \${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB IMGUI_SOURCES
    \${IMGUI_DIR}/*.cpp
    \${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    \${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp)

add_library(imgui STATIC \${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC \${IMGUI_DIR} \${IMGUI_DIR}/backends)
EOL

echo "Setup complete."

