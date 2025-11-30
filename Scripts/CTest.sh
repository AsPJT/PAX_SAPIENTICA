#!/bin/bash

# Create build directory
ROOT_PATH=$(dirname $(dirname "$0"))
cd "${ROOT_PATH}"
cmake -SProjects -Bbuild -DCMAKE_BUILD_TYPE=Release

# Build the project
cmake --build build

# Change directory to build and execute ctest
cd build
ctest --output-on-failure
