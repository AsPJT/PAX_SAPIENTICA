@echo off

SET "SCRIPT_DIR=%~dp0"
cd /d "%SCRIPT_DIR%"

cmake -S"%SCRIPT_DIR%..\Project" -B"%SCRIPT_DIR%..\Project\build"
