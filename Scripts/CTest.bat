@echo off

SET SCRIPT_DIR=%~dp0

:: Create build directory
cmake -S"%SCRIPT_DIR%..\Project" -B"%SCRIPT_DIR%..\Project\build"

:: Build the project
cmake --build "%SCRIPT_DIR%..\Project\build"

:: Change directory to build and execute ctest
cd "%SCRIPT_DIR%..\build"
ctest
