cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)
set(EXECUTABLE_OUTPUT_PATH ../../../Project/UnitTest/bin)
include_directories(../../Library)

add_executable(FileErrorLogTest source/FileErrorLogTest.cpp)
