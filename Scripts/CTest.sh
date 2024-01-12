#!/bin/bash

# Create build directory
ROOT_PATH=$(dirname $(dirname "$0"))
cmake -S${ROOT_PATH}/Projects -B${ROOT_PATH}/build

# Build the project
cmake --build ${ROOT_PATH}/build

# Change directory to build and execute ctest
cd ${ROOT_PATH}/build
ctest
