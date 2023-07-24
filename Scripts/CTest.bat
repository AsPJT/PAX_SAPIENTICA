@echo off

SET SCRIPT_DIR=%~dp0

:: Create build directory
cmake -S"%SCRIPT_DIR%..\Project" -B"%SCRIPT_DIR%..\build" -DCMAKE_BUILD_TYPE=Release

:: Build the project
cmake --build "%SCRIPT_DIR%..\build"

:: Change directory to build and execute ctest
cd "%SCRIPT_DIR%..\build"
ctest -C Release
