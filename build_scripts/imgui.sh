#!/bin/bash

INITIAL_DIR=$(pwd)

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

# Change directory to lib_imgui and checkout the docking branch
cd ${IMGUI_DIR}
echo "Checking out docking branch..."
git checkout docking

# Move the temporary CMakeLists.txt to lib_imgui directory
cd ${INITIAL_DIR}
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