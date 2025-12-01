#!/bin/bash

# Get absolute path to project root
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
ROOT_PATH="$(cd "${SCRIPT_DIR}/.." && pwd)"
cd "${ROOT_PATH}"
BUILD_DIR_NAME="ProductionBuild"

rm -rf "${BUILD_DIR_NAME}"
mkdir "${BUILD_DIR_NAME}"

cmake -S"Projects/MapViewer" -B"${BUILD_DIR_NAME}" -DCMAKE_BUILD_TYPE=Release -DPAXS_BUILD_TYPE=Production -DSFML_STATIC_LIBRARIES=TRUE -DCMAKE_TOOLCHAIN_FILE="${ROOT_PATH}/vcpkg/scripts/buildsystems/vcpkg.cmake" -DVCPKG_INSTALLED_DIR="${ROOT_PATH}/vcpkg_installed"
cmake --build "${BUILD_DIR_NAME}"

echo "Build completed."
if [[ "$OSTYPE" == "darwin"* ]]; then
    echo ".app bundle is located at: ${BUILD_DIR_NAME}/"
    echo ""
    echo "To create DMG after modifying the app bundle, run:"
    echo "./Scripts/CreateDMG.sh"
else
    echo "Binary is located at: ${BUILD_DIR_NAME}/"
fi
