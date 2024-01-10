@echo off

cd /d "%~dp0\.."

git submodule sync
git submodule update --init --recursive

