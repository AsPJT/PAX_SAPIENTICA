#!/bin/bash

SCRIPT_DIR=$(cd $(dirname $0); pwd)
cmake -S${SCRIPT_DIR}/../Projects -B${SCRIPT_DIR}/../build
