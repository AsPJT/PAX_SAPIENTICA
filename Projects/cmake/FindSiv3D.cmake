# FindSiv3D.cmake
# CMake module to find OpenSiv3D library
#
# This module defines:
#  Siv3D_FOUND - True if Siv3D is found
#  Siv3D_INCLUDE_DIRS - Include directories for Siv3D
#  Siv3D_LIBRARIES - Libraries to link against
#  Siv3D::Siv3D - Imported target for Siv3D

# Version to find (can be overridden)
if(NOT Siv3D_FIND_VERSION)
    set(Siv3D_FIND_VERSION "0.6.13")
endif()

# Try to find Siv3D installation
# Priority: SIV3D_ROOT_DIR (CMake var) > SIV3D_0_6_13 (env) > Default locations
find_path(Siv3D_ROOT_DIR
    NAMES include/Siv3D.hpp
    PATHS
        ${SIV3D_ROOT_DIR}
        $ENV{SIV3D_0_6_13}
        $ENV{SIV3D_DIR}
        "${CMAKE_CURRENT_SOURCE_DIR}/MapViewer/Windows/OpenSiv3D_${Siv3D_FIND_VERSION}"
        "C:/Siv3D"
    DOC "Siv3D root directory"
)

# Find include directory
find_path(Siv3D_INCLUDE_DIR
    NAMES Siv3D.hpp
    PATHS
        ${Siv3D_ROOT_DIR}/include
        $ENV{SIV3D_0_6_13}/include
        $ENV{SIV3D_DIR}/include
    DOC "Siv3D include directory"
)

# ThirdParty include directory is always in include/ThirdParty
set(Siv3D_THIRDPARTY_INCLUDE_DIR "${Siv3D_INCLUDE_DIR}/ThirdParty")

# Find library directory
if(WIN32)
    set(Siv3D_LIB_SUBDIR "lib/Windows")
elseif(APPLE)
    set(Siv3D_LIB_SUBDIR "lib/macOS")
elseif(UNIX)
    set(Siv3D_LIB_SUBDIR "lib/Linux")
endif()

# Find library files (Debug and Release)
if(MSVC)
    find_library(Siv3D_LIBRARY_DEBUG
        NAMES Siv3D_d Siv3D
        PATHS
            ${Siv3D_ROOT_DIR}/${Siv3D_LIB_SUBDIR}
            $ENV{SIV3D_0_6_13}/${Siv3D_LIB_SUBDIR}
            $ENV{SIV3D_DIR}/${Siv3D_LIB_SUBDIR}
    )

    find_library(Siv3D_LIBRARY_RELEASE
        NAMES Siv3D
        PATHS
            ${Siv3D_ROOT_DIR}/${Siv3D_LIB_SUBDIR}
            $ENV{SIV3D_0_6_13}/${Siv3D_LIB_SUBDIR}
            $ENV{SIV3D_DIR}/${Siv3D_LIB_SUBDIR}
    )
endif()

# Handle standard arguments
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Siv3D
    REQUIRED_VARS Siv3D_INCLUDE_DIR
    VERSION_VAR Siv3D_FIND_VERSION
    FAIL_MESSAGE "Could not find OpenSiv3D. Set SIV3D_ROOT_DIR or SIV3D_0_6_13 environment variable."
)

if(Siv3D_FOUND)
    set(Siv3D_INCLUDE_DIRS ${Siv3D_INCLUDE_DIR})

    # Add ThirdParty include directory if found
    if(Siv3D_THIRDPARTY_INCLUDE_DIR)
        list(APPEND Siv3D_INCLUDE_DIRS ${Siv3D_THIRDPARTY_INCLUDE_DIR})
    endif()

    # Create imported target
    if(NOT TARGET Siv3D::Siv3D)
        add_library(Siv3D::Siv3D INTERFACE IMPORTED)
        set_target_properties(Siv3D::Siv3D PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES "${Siv3D_INCLUDE_DIRS}"
        )

        # Set libraries based on build type using generator expressions
        if(Siv3D_LIBRARY_DEBUG AND Siv3D_LIBRARY_RELEASE)
            set_target_properties(Siv3D::Siv3D PROPERTIES
                INTERFACE_LINK_LIBRARIES "$<$<CONFIG:Debug>:${Siv3D_LIBRARY_DEBUG}>$<$<NOT:$<CONFIG:Debug>>:${Siv3D_LIBRARY_RELEASE}>"
            )
        elseif(Siv3D_LIBRARY_RELEASE)
            set_target_properties(Siv3D::Siv3D PROPERTIES
                INTERFACE_LINK_LIBRARIES "${Siv3D_LIBRARY_RELEASE}"
            )
        elseif(Siv3D_LIBRARY_DEBUG)
            set_target_properties(Siv3D::Siv3D PROPERTIES
                INTERFACE_LINK_LIBRARIES "${Siv3D_LIBRARY_DEBUG}"
            )
        endif()

        # Compile definitions and options are managed in CMakeLists.txt
    endif()

    mark_as_advanced(Siv3D_INCLUDE_DIR Siv3D_THIRDPARTY_INCLUDE_DIR Siv3D_LIBRARY_DEBUG Siv3D_LIBRARY_RELEASE)
endif()
