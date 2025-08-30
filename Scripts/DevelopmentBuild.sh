#!/bin/bash

ROOT_PATH=$(dirname $(dirname "$0"))
cd "${ROOT_PATH}"
BUILD_DIR_NAME="DevelopmentBuild"

rm -rf "${BUILD_DIR_NAME}"
mkdir "${BUILD_DIR_NAME}"

cmake -S"Projects/MapViewer" -B"${BUILD_DIR_NAME}" -DCMAKE_BUILD_TYPE=Release
cmake --build "${BUILD_DIR_NAME}"
