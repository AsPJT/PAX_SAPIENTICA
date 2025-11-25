cmake_minimum_required(VERSION 3.16)
project(PAX_SAPIENTICA)
include(CTest)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add custom CMake modules path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Set compiler warning flags
if(MSVC)
    add_compile_options(/W4)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wno-long-long -pedantic)
endif()

# Determine OS-specific directory name
if(NOT DEFINED OS)
    if(APPLE)
        set(OS "macOS" CACHE STRING "Operating System name")
    elseif(UNIX)
        set(OS "Ubuntu" CACHE STRING "Operating System name")
    elseif(WIN32)
        set(OS "Windows" CACHE STRING "Operating System name")
    else()
        message(FATAL_ERROR "Unsupported OS")
    endif()
endif()

# Find SFML using FindSFMLCustom for all platforms
find_package(SFMLCustom QUIET)
if(NOT SFML_FOUND)
    message(STATUS "SFML not found - MapViewer will not be built")
endif()

if(SFML_FOUND)
    if(SFML_INCLUDE_DIRS)
        include_directories(${SFML_INCLUDE_DIRS})
    endif()
else()
    message(WARNING "SFML not found. SFML-based targets will not be built.")
endif()

# Find DxLib (Windows only)
if(WIN32)
    find_package(DxLib)
    if(DxLib_FOUND)
        message(STATUS "DxLib found: ${DxLib_INCLUDE_DIRS}")
    else()
        message(STATUS "DxLib not found. Set DXLIB_ROOT_DIR to enable DxLib support.")
    endif()
endif()

# Find Siv3D (Windows only for now)
if(WIN32)
    find_package(Siv3D)
    if(Siv3D_FOUND)
        message(STATUS "Siv3D found: ${Siv3D_INCLUDE_DIRS}")
    else()
        message(STATUS "Siv3D not found. Set SIV3D_ROOT_DIR or SIV3D_0_6_13 environment variable to enable Siv3D support.")
    endif()
endif()

# Suppress warnings from external libraries (stb, etc.)
include_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/../ExternalLibrary)

add_subdirectory(IncludeTest)
add_subdirectory(IntegrationTest)
add_subdirectory(MapViewer)
add_subdirectory(UnitTest)
