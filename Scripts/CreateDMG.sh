#!/bin/bash

# Get absolute path to project root
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
ROOT_PATH="$(cd "${SCRIPT_DIR}/.." && pwd)"
cd "${ROOT_PATH}"
BUILD_DIR_NAME="ProductionBuild"

# Check if build directory exists
if [ ! -d "${BUILD_DIR_NAME}" ]; then
    echo "Error: ${BUILD_DIR_NAME} directory not found."
    echo "Please run ./Scripts/ProductionBuild.sh first."
    exit 1
fi

# Check if .app bundle exists
if [ ! -d "${BUILD_DIR_NAME}/PAX SAPIENTICA.app" ]; then
    echo "Error: PAX SAPIENTICA.app not found in ${BUILD_DIR_NAME}/"
    echo "Please run ./Scripts/ProductionBuild.sh first."
    exit 1
fi

echo "Creating DMG from .app bundle..."
echo "App bundle location: ${BUILD_DIR_NAME}/PAX SAPIENTICA.app"
echo ""
echo "If you need to modify the app bundle, press Ctrl+C now."
echo "Otherwise, DMG creation will start in 3 seconds..."
sleep 3

cd "${BUILD_DIR_NAME}"
cpack

if [ $? -eq 0 ]; then
    cd "${ROOT_PATH}"
    echo ""
    echo "✓ DMG file successfully created in ${BUILD_DIR_NAME}/"
    ls -lh "${BUILD_DIR_NAME}"/*.dmg
else
    cd "${ROOT_PATH}"
    echo ""
    echo "✗ Error: DMG creation failed."
    exit 1
fi
