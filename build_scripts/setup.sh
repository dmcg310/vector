#!/bin/bash

# https://glad.dav1d.de/#language=c&specification=gl&api=gl%3D4.6&api=gles1%3Dnone&api=gles2%3Dnone&api=glsc2%3Dnone&profile=compatibility&loader=on

INITIAL_DIR=$(pwd)

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
OTHER_DIR="${INITIAL_DIR}/other"
TEMP_CMAKE_FILE="${OTHER_DIR}/CMakeLists.txt"

if [ -d "${IMGUI_DIR}" ] && [ -f "${IMGUI_DIR}/.git" ]; then
    echo "Updating ImGui submodule..."
    git submodule update --init --recursive
else
    echo "Adding ImGui as a submodule..."
    git submodule add ${IMGUI_REPO} ${IMGUI_DIR}
    git submodule update --init --recursive
fi

echo "Creating CMakeLists.txt for ImGui..."
if [ ! -f "${TEMP_CMAKE_FILE}" ]; then
    echo "Error: Temporary CMakeLists.txt file does not exist."
    exit 1
fi

cp ${TEMP_CMAKE_FILE} ${IMGUI_DIR}/CMakeLists.txt
if [ $? -ne 0 ]; then
    echo "Error: Failed to move CMakeLists.txt for ImGui."
    exit 1
fi

echo "ImGui Setup complete."

echo "Setup complete."

