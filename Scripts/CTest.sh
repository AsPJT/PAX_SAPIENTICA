#!/bin/bash

# Create build directory
SCRIPT_DIR=$(cd $(dirname $0); pwd)
cmake -S${SCRIPT_DIR}/../Projects -B${SCRIPT_DIR}/../build

# Build the project
cmake --build ${SCRIPT_DIR}/../build

# Change directory to build and execute ctest
cd ${SCRIPT_DIR}/../build
ctest
