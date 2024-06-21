#!/bin/bash

# https://glad.dav1d.de/#language=c&specification=gl&api=gl%3D4.6&api=gles1%3Dnone&api=gles2%3Dnone&api=glsc2%3Dnone&profile=compatibility&loader=on

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
