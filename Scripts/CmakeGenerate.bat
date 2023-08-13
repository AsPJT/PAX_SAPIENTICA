@echo off

SET SCRIPT_DIR=%~dp0

cmake -S"%SCRIPT_DIR%..\Project" -B"%SCRIPT_DIR%..\build"
