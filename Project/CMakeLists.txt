cmake_minimum_required(VERSION 3.16)
project(PAX_SAPIENTICA)
set(CMAKE_CXX_STANDARD 17)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif()

add_subdirectory(./GeographicDataToSimulationGrid)
add_subdirectory(./SimulationTest)

