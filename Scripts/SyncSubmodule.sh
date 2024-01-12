#!/bin/bash

ROOT_PATH=$(dirname $(dirname "$0"))
cd "${ROOT_PATH}"

git submodule sync
git submodule update --init --recursive
