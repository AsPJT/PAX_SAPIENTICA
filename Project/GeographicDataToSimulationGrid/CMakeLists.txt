cmake_minimum_required(VERSION 3.16)
project(GeographicDataToSimulationGrid)

set(CMAKE_CXX_STANDARD 17)
include_directories(../../Library)

add_executable(GeographicDataToSimulationGrid Main.cpp)
