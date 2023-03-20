cmake_minimum_required(VERSION 3.16)
project(GeographicDataToSimulationGrid)

set(CMAKE_CXX_STANDARD 17)
set(EXECUTABLE_OUTPUT_PATH ../Project/GeographicDataToSimulationGrid/bin)
include_directories(../../Library)

add_definitions(-DLandAndSea)

add_executable(LandAndSea Main.cpp)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(LandAndSea ${OpenCV_LIBS})
