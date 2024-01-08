#!/bin/bash

SCRIPT_DIR=$(dirname "$0")
cd "${SCRIPT_DIR}/.."

git submodule sync
git submodule update --init --recursive
