# FindSFMLCustom.cmake
# CMake module to find SFML library with custom paths
#
# This module is a wrapper around the standard SFML CMake config files
# and adds support for finding SFML in custom project directories.
#
# This module defines:
#  SFML_FOUND - True if SFML is found
#  SFML_INCLUDE_DIRS - Include directories for SFML
#  SFML_LIBRARIES - Libraries to link against
#  SFML_ROOT_DIR - Root directory of SFML installation
#  SFML::Graphics, SFML::Window, etc. - Imported targets from SFML

# Determine OS-specific directory name
if(APPLE)
    set(SFML_OS_DIR "macOS")
elseif(UNIX)
    set(SFML_OS_DIR "Ubuntu")
elseif(WIN32)
    set(SFML_OS_DIR "Windows")
else()
    message(FATAL_ERROR "Unsupported OS for SFML detection")
endif()

# Set SFML root directory based on OS and priority:
# 1. User-specified SFML_ROOT_DIR (CMake variable)
# 2. Environment variable SFML_DIR
# 3. vcpkg or system-wide installation (handled by find_package) - preferred for macOS/Linux
# 4. Project-local SFML installation (Windows only, for backward compatibility)

if(NOT SFML_ROOT_DIR)
    if(DEFINED ENV{SFML_DIR})
        set(SFML_ROOT_DIR "$ENV{SFML_DIR}")
        message(STATUS "Using SFML from environment variable: ${SFML_ROOT_DIR}")
    elseif(WIN32)
        # Try project-local SFML 3.0.0 installation (Windows only)
        set(SFML_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/MapViewer/${SFML_OS_DIR}/SFML_3.0.0")
        if(EXISTS "${SFML_ROOT_DIR}")
            message(STATUS "Using project-local SFML: ${SFML_ROOT_DIR}")
        else()
            unset(SFML_ROOT_DIR)
        endif()
    endif()
    # Note: Linux and macOS now prefer vcpkg, with legacy fallback for macOS only
endif()

# Add SFML CMake config path to CMAKE_PREFIX_PATH
if(SFML_ROOT_DIR AND EXISTS "${SFML_ROOT_DIR}")
    if(EXISTS "${SFML_ROOT_DIR}/lib/cmake/SFML")
        list(APPEND CMAKE_PREFIX_PATH "${SFML_ROOT_DIR}/lib/cmake/SFML")
    endif()

    # Set include directory for later use
    if(EXISTS "${SFML_ROOT_DIR}/include")
        set(SFML_INCLUDE_DIRS "${SFML_ROOT_DIR}/include")
    endif()
endif()

# Find SFML package using its CMake config files
# First try version 3.0+ (may come from vcpkg or system)
find_package(SFML 3.0 COMPONENTS Graphics Window System QUIET)

# If SFML 3.0 is not found, try to find any version
if(NOT SFML_FOUND)
    find_package(SFML COMPONENTS Graphics Window System QUIET)
endif()

# Handle standard arguments
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SFMLCustom
    REQUIRED_VARS SFML_FOUND
    FAIL_MESSAGE "Could not find SFML. Install SFML system-wide or set SFML_ROOT_DIR to the SFML installation directory."
)

if(SFML_FOUND)
    # Set include directories if found
    if(NOT SFML_INCLUDE_DIRS AND SFML_ROOT_DIR)
        set(SFML_INCLUDE_DIRS "${SFML_ROOT_DIR}/include")
    endif()

    message(STATUS "SFML found: ${SFML_ROOT_DIR}")
    if(SFML_INCLUDE_DIRS)
        message(STATUS "SFML include directory: ${SFML_INCLUDE_DIRS}")
    endif()

    # Mark variables as advanced
    mark_as_advanced(SFML_ROOT_DIR SFML_INCLUDE_DIRS)
endif()
