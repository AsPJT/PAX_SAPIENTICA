#!/bin/bash
# vcpkg setup script for Ubuntu build environment
# This script is designed to run inside Docker container or CI environment

set -e

echo "========================================="
echo "PAX_SAPIENTICA vcpkg Setup for Ubuntu"
echo "========================================="

# Project paths
WORKSPACE_ROOT="/workspace"
VCPKG_DIR="${WORKSPACE_ROOT}/vcpkg"

# Check if vcpkg is already installed
if [ -d "$VCPKG_DIR" ] && [ -f "$VCPKG_DIR/vcpkg" ]; then
    echo "vcpkg is already installed at: $VCPKG_DIR"
    echo "Skipping vcpkg installation."
    exit 0
fi

# Install required system packages
echo ""
echo "Installing system dependencies..."

apt-get update
apt-get install -y \
    git \
    curl \
    zip \
    unzip \
    tar \
    cmake \
    ninja-build \
    pkg-config \
    build-essential \
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

# Clone vcpkg if not exists
if [ ! -d "$VCPKG_DIR" ]; then
    echo ""
    echo "Cloning vcpkg from GitHub..."
    cd "$WORKSPACE_ROOT"
    git clone https://github.com/microsoft/vcpkg.git
else
    echo "vcpkg directory exists, skipping clone."
fi

# Bootstrap vcpkg
echo ""
echo "Bootstrapping vcpkg..."
cd "$VCPKG_DIR"
./bootstrap-vcpkg.sh

# Install SFML using vcpkg.json manifest mode
echo ""
echo "Installing SFML 3.0+ and dependencies via vcpkg..."
cd "$WORKSPACE_ROOT"
"$VCPKG_DIR/vcpkg" install

echo ""
echo "========================================="
echo "vcpkg setup completed successfully!"
echo "========================================="
echo ""
