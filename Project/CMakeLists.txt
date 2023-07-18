cmake_minimum_required(VERSION 3.16)
project(PAX_SAPIENTICA)
set(CMAKE_CXX_STANDARD 20)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif()

get_filename_component(PARENT_DIR ${CMAKE_SOURCE_DIR} DIRECTORY)
add_definitions(-DPROJECT_ROOT_PATH=\"${PARENT_DIR}\")

add_subdirectory(./GeographicDataToSimulationGrid)
add_subdirectory(./IncludeTest)
add_subdirectory(./IntegrationTest)
add_subdirectory(./UnitTest)

