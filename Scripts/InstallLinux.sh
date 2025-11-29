#!/bin/bash

# PAX SAPIENTICA Installation Script for Linux
# This script builds and installs PAX SAPIENTICA from source on Linux systems

set -e  # Exit on error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Get the root directory
ROOT_PATH=$(dirname $(dirname "$0"))
cd "${ROOT_PATH}"

echo -e "${GREEN}PAX SAPIENTICA Installation Script for Linux${NC}"
echo "=============================================="
echo ""

# Check if running on Linux
if [[ "$(uname)" != "Linux" ]]; then
    echo -e "${RED}Error: This script is for Linux only${NC}"
    exit 1
fi

# Check for vcpkg
VCPKG_ROOT="${ROOT_PATH}/vcpkg"
VCPKG_TOOLCHAIN="${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
USE_VCPKG=false

if [ -d "$VCPKG_ROOT" ] && [ -f "$VCPKG_ROOT/vcpkg" ]; then
    echo -e "${BLUE}vcpkg installation detected${NC}"
    USE_VCPKG=true
fi

# Check for required dependencies
echo "Checking dependencies..."
MISSING_DEPS=()

if ! command -v cmake &> /dev/null; then
    MISSING_DEPS+=("cmake")
fi

if ! command -v g++ &> /dev/null; then
    MISSING_DEPS+=("g++")
fi

# Check SFML availability
SFML_AVAILABLE=false

if [ "$USE_VCPKG" = true ]; then
    echo -e "${GREEN}Will use SFML from vcpkg${NC}"
    SFML_AVAILABLE=true
elif dpkg -l | grep -q libsfml-dev 2>/dev/null; then
    echo -e "${GREEN}System SFML found${NC}"
    SFML_AVAILABLE=true
elif pkg-config --exists sfml-graphics 2>/dev/null; then
    echo -e "${GREEN}SFML found via pkg-config${NC}"
    SFML_AVAILABLE=true
fi

if [ "$SFML_AVAILABLE" = false ]; then
    echo -e "${RED}SFML not found!${NC}"
    echo ""
    echo "You have two options:"
    echo ""
    echo "Option 1: Use vcpkg (recommended)"
    echo "  Run: ./Scripts/setup_vcpkg_linux.sh"
    echo "  Then run this script again"
    echo ""
    echo "Option 2: Install system SFML"
    echo "  For SFML 2.x: sudo apt-get install libsfml-dev"
    echo "  For SFML 3.x: Follow instructions in Projects/MapViewer/Ubuntu/SFML/README.md"
    echo ""
    exit 1
fi

if [ ${#MISSING_DEPS[@]} -ne 0 ]; then
    echo -e "${RED}Missing dependencies: ${MISSING_DEPS[*]}${NC}"
    echo ""
    echo "To install dependencies on Ubuntu/Debian, run:"
    echo "  sudo apt-get update"
    echo "  sudo apt-get install cmake g++"
    exit 1
fi

echo -e "${GREEN}All dependencies satisfied${NC}"
echo ""

# Ask for installation type
echo "Select installation type:"
echo "  1) User install (to ~/.local, no sudo required)"
echo "  2) System install (to /usr/local, requires sudo)"
echo "  3) Build DEB package only (for distribution)"
read -p "Enter choice [1-3]: " INSTALL_TYPE

case $INSTALL_TYPE in
    1)
        INSTALL_PREFIX="$HOME/.local"
        NEED_SUDO=false
        ;;
    2)
        INSTALL_PREFIX="/usr/local"
        NEED_SUDO=true
        ;;
    3)
        BUILD_DEB_ONLY=true
        ;;
    *)
        echo -e "${RED}Invalid choice${NC}"
        exit 1
        ;;
esac

BUILD_DIR_NAME="ProductionBuild"

# Clean previous build
if [ -d "${BUILD_DIR_NAME}" ]; then
    echo "Cleaning previous build..."
    rm -rf "${BUILD_DIR_NAME}"
fi

echo "Creating build directory..."
mkdir -p "${BUILD_DIR_NAME}"

# Prepare CMake arguments
CMAKE_ARGS=(
    -S"Projects/MapViewer"
    -B"${BUILD_DIR_NAME}"
    -DCMAKE_BUILD_TYPE=Release
    -DPAXS_BUILD_TYPE=Production
)

# Add vcpkg toolchain if available
if [ "$USE_VCPKG" = true ]; then
    # Convert to absolute path
    VCPKG_TOOLCHAIN_ABS="$(cd "$(dirname "$VCPKG_TOOLCHAIN")" && pwd)/$(basename "$VCPKG_TOOLCHAIN")"
    VCPKG_MANIFEST_DIR_ABS="${ROOT_PATH}/Projects"
    echo -e "${BLUE}Using vcpkg toolchain: ${VCPKG_TOOLCHAIN_ABS}${NC}"
    CMAKE_ARGS+=(-DCMAKE_TOOLCHAIN_FILE="${VCPKG_TOOLCHAIN_ABS}")
    CMAKE_ARGS+=(-DVCPKG_MANIFEST_DIR="${VCPKG_MANIFEST_DIR_ABS}")
fi

if [ "$BUILD_DEB_ONLY" = true ]; then
    # Build DEB package
    echo ""
    echo -e "${YELLOW}Configuring build for DEB package...${NC}"
    cmake "${CMAKE_ARGS[@]}"

    echo ""
    echo -e "${YELLOW}Building...${NC}"
    cmake --build "${BUILD_DIR_NAME}" --parallel $(nproc)

    echo ""
    echo -e "${YELLOW}Creating DEB package...${NC}"
    cd "${BUILD_DIR_NAME}"
    cpack -G DEB

    # Get the actual package name
    DEB_FILE=$(ls -1 pax-sapientica*.deb 2>/dev/null | head -1)

    cd "${ROOT_PATH}"

    echo ""
    echo -e "${GREEN}DEB package created successfully!${NC}"
    if [ -n "$DEB_FILE" ]; then
        echo "Package: ${BUILD_DIR_NAME}/${DEB_FILE}"
    else
        echo "Package location: ${BUILD_DIR_NAME}/"
    fi
    echo ""
    echo "To install the package, run:"
    if [ -n "$DEB_FILE" ]; then
        echo "  sudo dpkg -i ${BUILD_DIR_NAME}/${DEB_FILE}"
    else
        echo "  sudo dpkg -i ${BUILD_DIR_NAME}/pax-sapientica-*.deb"
    fi
    echo "  sudo apt-get install -f  # Install any missing dependencies"
else
    # Build and install
    echo ""
    echo -e "${YELLOW}Configuring build...${NC}"
    CMAKE_ARGS+=(-DCMAKE_INSTALL_PREFIX="${INSTALL_PREFIX}")
    cmake "${CMAKE_ARGS[@]}"

    echo ""
    echo -e "${YELLOW}Building...${NC}"
    cmake --build "${BUILD_DIR_NAME}" --parallel $(nproc)

    echo ""
    echo -e "${YELLOW}Installing...${NC}"
    if [ "$NEED_SUDO" = true ]; then
        sudo cmake --install "${BUILD_DIR_NAME}"
    else
        cmake --install "${BUILD_DIR_NAME}"
    fi

    echo ""
    echo -e "${GREEN}Installation completed successfully!${NC}"
    echo ""
    echo "Binary installed to: ${INSTALL_PREFIX}/bin/PAX_SAPIENTICA"
    echo "Data installed to: ${INSTALL_PREFIX}/share/pax_sapientica/"
    echo ""

    if [ "$INSTALL_TYPE" = "1" ]; then
        # Check if ~/.local/bin is in PATH
        if [[ ":$PATH:" != *":$HOME/.local/bin:"* ]]; then
            echo -e "${YELLOW}Warning: $HOME/.local/bin is not in your PATH${NC}"
            echo "Add the following line to your ~/.bashrc or ~/.profile:"
            echo "  export PATH=\"\$HOME/.local/bin:\$PATH\""
            echo ""
        fi
    fi

    echo "To run PAX SAPIENTICA, execute:"
    echo "  PAX_SAPIENTICA"
fi

echo ""
echo "Done!"
