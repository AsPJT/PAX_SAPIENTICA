#!/bin/bash
# vcpkg setup script for Linux
# This script installs vcpkg and SFML 3.0.0+ for PAX_SAPIENTICA

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
VCPKG_DIR="$PROJECT_ROOT/vcpkg"

echo "========================================="
echo "PAX_SAPIENTICA vcpkg Setup for Linux"
echo "========================================="

# Check if vcpkg is already installed
if [ -d "$VCPKG_DIR" ]; then
    echo "vcpkg directory already exists at: $VCPKG_DIR"
    read -p "Do you want to reinstall vcpkg? (y/N): " -n 1 -r
    echo
    if [[ ! $REPLY =~ ^[Yy]$ ]]; then
        echo "Skipping vcpkg installation."
        echo "To use existing vcpkg, run: $VCPKG_DIR/vcpkg install"
        exit 0
    fi
    echo "Removing existing vcpkg directory..."
    rm -rf "$VCPKG_DIR"
fi

# Install required system packages
echo ""
echo "Installing system dependencies..."
echo "You may be prompted for your password (sudo required)."

sudo apt update

# SFML 3.0 dependencies for Ubuntu/Debian
sudo apt install -y \
    git \
    curl \
    zip \
    unzip \
    tar \
    cmake \
    ninja-build \
    pkg-config \
    libxrandr-dev \
    libxcursor-dev \
    libxi-dev \
    libudev-dev \
    libfreetype-dev \
    libflac-dev \
    libvorbis-dev \
    libgl1-mesa-dev \
    libegl1-mesa-dev \
    libdrm-dev \
    libgbm-dev

echo ""
echo "System dependencies installed successfully."

# Clone vcpkg
echo ""
echo "Cloning vcpkg from GitHub..."
cd "$PROJECT_ROOT"
git clone https://github.com/microsoft/vcpkg.git

# Bootstrap vcpkg
echo ""
echo "Bootstrapping vcpkg..."
cd "$VCPKG_DIR"
./bootstrap-vcpkg.sh

# Install SFML using vcpkg.json manifest mode
echo ""
echo "Installing SFML 3.0+ and dependencies via vcpkg..."
cd "$PROJECT_ROOT"
"$VCPKG_DIR/vcpkg" install

echo ""
echo "========================================="
echo "vcpkg setup completed successfully!"
echo "========================================="
echo ""
echo "Next steps:"
echo "1. Configure and build the project with CMake:"
echo "   cmake -B build -S Projects -DCMAKE_TOOLCHAIN_FILE=$VCPKG_DIR/scripts/buildsystems/vcpkg.cmake -DSFML_STATIC_LIBRARIES=TRUE -DVCPKG_INSTALLED_DIR=$PROJECT_ROOT/vcpkg_installed"
echo "   cmake --build build --target SFMLMapViewer"
echo ""
echo "2. Run the application:"
echo "   ./build/MapViewer/SFMLMapViewer"
echo ""
echo "Note: SFML_STATIC_LIBRARIES=TRUE is required to use static linking with vcpkg SFML."
echo "      Build and vcpkg_installed directories will be created at project root."
echo ""
