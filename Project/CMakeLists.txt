cmake_minimum_required(VERSION 3.16)
project(PAX_SAPIENTICA)
include(CTest)
set(CMAKE_CXX_STANDARD 20)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if(MSVC)
    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif()
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Od")
    elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O2")
    endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-long-long -pedantic")
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
    elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
    endif()
endif()

include_directories(${CMAKE_SOURCE_DIR}/../Library)

get_filename_component(PARENT_DIR ${CMAKE_SOURCE_DIR} DIRECTORY)
add_definitions(-DPROJECT_ROOT_PATH=\"${PARENT_DIR}\")

add_subdirectory(GeographicDataToSimulationGrid)
add_subdirectory(IncludeTest)
add_subdirectory(IntegrationTest)
add_subdirectory(UnitTest)

add_subdirectory(../ExternalLibrary/googletest ${CMAKE_BINARY_DIR}/googletest)

enable_testing()

