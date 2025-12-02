#!/bin/bash

# Get absolute path to project root
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
ROOT_PATH="$(cd "${SCRIPT_DIR}/.." && pwd)"
cd "${ROOT_PATH}"
BUILD_DIR_NAME="DevelopmentBuild"

rm -rf "${BUILD_DIR_NAME}"
mkdir "${BUILD_DIR_NAME}"

# Detect OS and set appropriate triplet
if [[ "$OSTYPE" == "darwin"* ]]; then
    # macOS
    if [ "$(uname -m)" = "arm64" ]; then
        VCPKG_TRIPLET="arm64-osx-static"
    else
        VCPKG_TRIPLET="x64-osx-static"
    fi

    cmake -S "Projects/MapViewer" -B "${BUILD_DIR_NAME}" \
      -DCMAKE_BUILD_TYPE=Release \
      -DSFML_STATIC_LIBRARIES=TRUE \
      -DCMAKE_TOOLCHAIN_FILE="${ROOT_PATH}/vcpkg/scripts/buildsystems/vcpkg.cmake" \
      -DVCPKG_INSTALLED_DIR="${ROOT_PATH}/vcpkg_installed" \
      -DVCPKG_OVERLAY_TRIPLETS="${ROOT_PATH}/Projects/cmake" \
      -DVCPKG_TARGET_TRIPLET="${VCPKG_TRIPLET}" \
      -DOPENGL_INCLUDE_DIR=/System/Library/Frameworks/OpenGL.framework \
      -DOPENGL_gl_LIBRARY=/System/Library/Frameworks/OpenGL.framework
else
    # Linux
    cmake -S "Projects/MapViewer" -B "${BUILD_DIR_NAME}" \
      -DCMAKE_BUILD_TYPE=Release \
      -DSFML_STATIC_LIBRARIES=TRUE \
      -DCMAKE_TOOLCHAIN_FILE="${ROOT_PATH}/vcpkg/scripts/buildsystems/vcpkg.cmake" \
      -DVCPKG_INSTALLED_DIR="${ROOT_PATH}/vcpkg_installed"
fi

cmake --build "${BUILD_DIR_NAME}"
