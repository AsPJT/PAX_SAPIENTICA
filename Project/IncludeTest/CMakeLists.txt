cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)
set(EXECUTABLE_OUTPUT_PATH ../../Project/IncludeTest/bin)
include_directories(../../Library)

add_executable(LoggerIncludeTest source/LoggerIncludeTest.cpp)