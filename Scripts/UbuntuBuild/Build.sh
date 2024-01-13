#!/bin/bash

apt-get update
apt-get install -y \
    build-essential \
    cmake \
    libsfml-dev

SCRIPT_PATH="/workspace/Scripts/ProductionBuild.sh"
chmod +x $SCRIPT_PATH
$SCRIPT_PATH

BUILD_PATH="/workspace/ProductionBuild"
cd $BUILD_PATH
cpack -G DEB
