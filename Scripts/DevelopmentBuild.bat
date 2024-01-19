@echo off

cd /d "%~dp0\.."

SET BUILD_DIR_NAME=DevelopmentBuild

if exist "%BUILD_DIR_NAME%" rmdir /s /q "%BUILD_DIR_NAME%"
mkdir "%BUILD_DIR_NAME%"

cmake -S"Projects/MapViewer" -B"%BUILD_DIR_NAME%" -DCMAKE_BUILD_TYPE=Release -GNinja
cmake --build "%BUILD_DIR_NAME%"
