#!/bin/bash

# Create build directory
SCRIPT_DIR=$(cd $(dirname $0); pwd)
cmake -S${SCRIPT_DIR}/../Project -B${SCRIPT_DIR}/../Project/build

# Build the project
cmake --build ${SCRIPT_DIR}/../Project/build

# Change directory to build and execute ctest
cd ${SCRIPT_DIR}/../build
ctest
