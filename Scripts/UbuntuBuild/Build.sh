#!/bin/bash
# Build script for Ubuntu environment (Docker container)
# This script sets up vcpkg, builds the project, and creates a DEB package

set -e

echo "========================================="
echo "PAX_SAPIENTICA Ubuntu Build"
echo "========================================="

WORKSPACE_ROOT="/workspace"
BUILD_DIR_NAME="ProductionBuild"

# Step 1: Setup vcpkg and install dependencies
echo ""
echo "Step 1: Setting up vcpkg and dependencies..."
SETUP_SCRIPT="${WORKSPACE_ROOT}/Scripts/UbuntuBuild/SetupVcpkg.sh"
chmod +x "$SETUP_SCRIPT"
"$SETUP_SCRIPT"

# Step 2: Build the project
echo ""
echo "Step 2: Building PAX_SAPIENTICA..."
cd "$WORKSPACE_ROOT"

# Remove old build directory
rm -rf "$BUILD_DIR_NAME"
mkdir "$BUILD_DIR_NAME"

# Set vcpkg toolchain file path
VCPKG_TOOLCHAIN_FILE="${WORKSPACE_ROOT}/vcpkg/scripts/buildsystems/vcpkg.cmake"

# Configure with CMake
cmake -S "Projects/MapViewer" -B "$BUILD_DIR_NAME" \
  -DCMAKE_BUILD_TYPE=Release \
  -DPAXS_BUILD_TYPE=Production \
  -DSFML_STATIC_LIBRARIES=TRUE \
  -DCMAKE_TOOLCHAIN_FILE="$VCPKG_TOOLCHAIN_FILE" \
  -DVCPKG_INSTALLED_DIR="${WORKSPACE_ROOT}/vcpkg_installed"

# Build
cmake --build "$BUILD_DIR_NAME"

echo ""
echo "Build completed successfully!"

# Step 3: Create DEB package
echo ""
echo "Step 3: Creating DEB package..."
cd "$BUILD_DIR_NAME"
cpack -G DEB

echo ""
echo "========================================="
echo "Build and packaging completed!"
echo "========================================="
echo ""
echo "DEB package location: ${WORKSPACE_ROOT}/${BUILD_DIR_NAME}/"
ls -lh "${WORKSPACE_ROOT}/${BUILD_DIR_NAME}/"*.deb 2>/dev/null || echo "Note: DEB package creation may have been skipped (check CMake configuration)"
