# FindDxLib.cmake
# CMake module to find DxLib library
#
# This module defines:
#  DxLib_FOUND - True if DxLib is found
#  DxLib_INCLUDE_DIRS - Include directories for DxLib
#  DxLib_LIBRARIES - Libraries to link against
#  DxLib::DxLib - Imported target for DxLib

# Try to find DxLib installation
# Priority: DXLIB_ROOT_DIR (CMake var) > DXLIB_DIR (env) > Default locations
find_path(DxLib_ROOT_DIR
    NAMES DxLib.h
    PATHS
        ${DXLIB_ROOT_DIR}
        $ENV{DXLIB_DIR}
        "C:/DxLib"
        "${CMAKE_CURRENT_SOURCE_DIR}/MapViewer/Windows/DxLib_SDK"
    DOC "DxLib root directory"
)

# Find include directory
find_path(DxLib_INCLUDE_DIR
    NAMES DxLib.h
    PATHS
        ${DxLib_ROOT_DIR}
        ${DXLIB_ROOT_DIR}
        $ENV{DXLIB_DIR}
        "C:/DxLib"
    DOC "DxLib include directory"
)

# Find library files (Debug and Release)
if(MSVC)
    # Visual Studio uses different library names for Debug/Release
    find_library(DxLib_LIBRARY_DEBUG
        NAMES DxLib_d DxLib
        PATHS
            ${DxLib_ROOT_DIR}
            ${DXLIB_ROOT_DIR}
            $ENV{DXLIB_DIR}
            "C:/DxLib"
        PATH_SUFFIXES lib lib/x64
    )

    find_library(DxLib_LIBRARY_RELEASE
        NAMES DxLib
        PATHS
            ${DxLib_ROOT_DIR}
            ${DXLIB_ROOT_DIR}
            $ENV{DXLIB_DIR}
            "C:/DxLib"
        PATH_SUFFIXES lib lib/x64
    )
endif()

# Handle standard arguments
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(DxLib
    REQUIRED_VARS DxLib_INCLUDE_DIR
    FAIL_MESSAGE "Could not find DxLib. Set DXLIB_ROOT_DIR to the DxLib installation directory."
)

if(DxLib_FOUND)
    set(DxLib_INCLUDE_DIRS ${DxLib_INCLUDE_DIR})

    # Create imported target
    if(NOT TARGET DxLib::DxLib)
        add_library(DxLib::DxLib INTERFACE IMPORTED)
        set_target_properties(DxLib::DxLib PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${DxLib_INCLUDE_DIRS}"
        )

        # Set libraries based on build type using generator expressions
        if(DxLib_LIBRARY_DEBUG AND DxLib_LIBRARY_RELEASE)
            set_target_properties(DxLib::DxLib PROPERTIES
                INTERFACE_LINK_LIBRARIES "$<$<CONFIG:Debug>:${DxLib_LIBRARY_DEBUG}>$<$<NOT:$<CONFIG:Debug>>:${DxLib_LIBRARY_RELEASE}>"
            )
        elseif(DxLib_LIBRARY_RELEASE)
            set_target_properties(DxLib::DxLib PROPERTIES
                INTERFACE_LINK_LIBRARIES "${DxLib_LIBRARY_RELEASE}"
            )
        elseif(DxLib_LIBRARY_DEBUG)
            set_target_properties(DxLib::DxLib PROPERTIES
                INTERFACE_LINK_LIBRARIES "${DxLib_LIBRARY_DEBUG}"
            )
        endif()

        # Add compile definitions
        set_target_properties(DxLib::DxLib PROPERTIES
            INTERFACE_COMPILE_DEFINITIONS "PAXS_USING_DXLIB"
        )
    endif()

    mark_as_advanced(DxLib_INCLUDE_DIR DxLib_LIBRARY_DEBUG DxLib_LIBRARY_RELEASE)
endif()
