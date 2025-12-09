#!/bin/bash
# Entrypoint script for Docker container
# This script provides flexible execution modes for the Ubuntu build environment

set -e

echo "========================================="
echo "PAX_SAPIENTICA Ubuntu Build Container"
echo "========================================="
echo ""

# Default command is to run the build script
if [ $# -eq 0 ]; then
    echo "Running default build process..."
    exec /workspace/Scripts/UbuntuBuild/Build.sh
else
    # Execute the provided command
    echo "Running custom command: $@"
    exec "$@"
fi
