cmake_minimum_required(VERSION 3.16)
project(MapViewer)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../..")

# Include helper functions for graphics libraries
include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/GraphicsLibraryHelpers.cmake)

include_directories(
    ${PROJECT_ROOT_DIR}/Library
    ${PROJECT_ROOT_DIR}/ExternalLibrary
)

# Determine OS-specific directory name (inherit from parent if already set)
if(NOT DEFINED OS)
    if(APPLE)
        set(OS "macOS" CACHE STRING "Operating System name")
    elseif(UNIX)
        set(OS "Ubuntu" CACHE STRING "Operating System name")
    elseif(WIN32)
        set(OS "Windows" CACHE STRING "Operating System name")
    endif()
endif()

set(TSV_SOURCE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/${OS}/Config.tsv")
set(DESTINATION_DIR "${CMAKE_CURRENT_BINARY_DIR}")

set(PAXS_BUILD_TYPE "Development" CACHE STRING "Choose the type of build (Development or Production)")

# Find SFML if not already found (when building MapViewer directly)
if(NOT SFML_FOUND)
    # Add cmake module path
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cmake")

    # Set SFML_ROOT_DIR for project-local SFML installation
    if(NOT SFML_ROOT_DIR)
        set(SFML_LOCAL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/${OS}/SFML_3.0.0")
        if(EXISTS "${SFML_LOCAL_PATH}")
            set(SFML_ROOT_DIR "${SFML_LOCAL_PATH}")
            message(STATUS "Using project-local SFML: ${SFML_ROOT_DIR}")
        endif()
    endif()

    find_package(SFMLCustom QUIET)
endif()

if(PAXS_BUILD_TYPE STREQUAL "Development")
    set(BINARY_NAME "SFMLMapViewer")
elseif(PAXS_BUILD_TYPE STREQUAL "Production")
    set(BINARY_NAME "PAX_SAPIENTICA")
endif()

if(WIN32)
    set(CPP_FILE "${CMAKE_CURRENT_SOURCE_DIR}/${OS}/SFML_3.0.0/Main.cpp")
    add_executable(${BINARY_NAME} ${CPP_FILE})
    paxs_setup_sfml(${BINARY_NAME})
    paxs_copy_config_tsv(${BINARY_NAME} ${TSV_SOURCE_PATH})
elseif(APPLE)
    set(CPP_FILE "${CMAKE_CURRENT_SOURCE_DIR}/${OS}/SFML_3.0.0/Main.cpp")

    if(PAXS_BUILD_TYPE STREQUAL "Production")
        # Production: macOS .app bundle

        # Read version information
        include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/ReadVersion.cmake)
        read_pax_sapientica_version()

        add_executable(${BINARY_NAME} MACOSX_BUNDLE ${CPP_FILE})
        paxs_setup_common_options(${BINARY_NAME})

        set_target_properties(${BINARY_NAME} PROPERTIES
            MACOSX_BUNDLE TRUE
            OUTPUT_NAME "PAX SAPIENTICA"
            MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/${OS}/Info.plist.in"
            INSTALL_RPATH "@executable_path/../Frameworks"
            BUILD_WITH_INSTALL_RPATH TRUE
        )

        target_link_libraries(${BINARY_NAME} PRIVATE SFML::Graphics curl)

        # Copy SFML frameworks to the bundle
        if(SFML_FOUND)
            # Get SFML library paths
            get_target_property(SFML_GRAPHICS_LIB SFML::Graphics IMPORTED_LOCATION_RELEASE)
            if(NOT SFML_GRAPHICS_LIB)
                get_target_property(SFML_GRAPHICS_LIB SFML::Graphics IMPORTED_LOCATION)
            endif()

            if(SFML_GRAPHICS_LIB)
                get_filename_component(SFML_LIB_DIR ${SFML_GRAPHICS_LIB} DIRECTORY)
                set(APP_FRAMEWORKS_DIR "$<TARGET_BUNDLE_CONTENT_DIR:${BINARY_NAME}>/Frameworks")

                add_custom_command(
                    TARGET ${BINARY_NAME} POST_BUILD
                    # MacOS/にProdConfig.tsvをコピー
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                        "${CMAKE_CURRENT_SOURCE_DIR}/${OS}/ProdConfig.tsv"
                        "$<TARGET_FILE_DIR:${BINARY_NAME}>/Config.tsv"

                    # Resources/にappicon.icnsをコピー
                    COMMAND ${CMAKE_COMMAND} -E make_directory
                        "$<TARGET_BUNDLE_CONTENT_DIR:${BINARY_NAME}>/Resources"
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                        "${CMAKE_CURRENT_SOURCE_DIR}/${OS}/appicon.icns"
                        "$<TARGET_BUNDLE_CONTENT_DIR:${BINARY_NAME}>/Resources/appicon.icns"

                    # SFML の dylib をコピーして install_name_tool でパスを修正
                    COMMAND ${CMAKE_COMMAND} -E make_directory
                        ${APP_FRAMEWORKS_DIR}
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                        ${SFML_LIB_DIR}/libsfml-graphics.3.0.dylib
                        ${APP_FRAMEWORKS_DIR}/
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                        ${SFML_LIB_DIR}/libsfml-window.3.0.dylib
                        ${APP_FRAMEWORKS_DIR}/
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                        ${SFML_LIB_DIR}/libsfml-system.3.0.dylib
                        ${APP_FRAMEWORKS_DIR}/

                    COMMAND install_name_tool -change ${SFML_LIB_DIR}/libsfml-graphics.3.0.dylib @rpath/libsfml-graphics.3.0.dylib $<TARGET_FILE:${BINARY_NAME}>
                    COMMAND install_name_tool -change ${SFML_LIB_DIR}/libsfml-window.3.0.dylib   @rpath/libsfml-window.3.0.dylib   $<TARGET_FILE:${BINARY_NAME}>
                    COMMAND install_name_tool -change ${SFML_LIB_DIR}/libsfml-system.3.0.dylib   @rpath/libsfml-system.3.0.dylib   $<TARGET_FILE:${BINARY_NAME}>
                    COMMENT "Copying SFML dylibs to app bundle and fixing install names..."
                )
            endif()
        endif()
    else()
        # Development: standard executable
        add_executable(${BINARY_NAME} ${CPP_FILE})
        paxs_setup_sfml(${BINARY_NAME})
        paxs_copy_config_tsv(${BINARY_NAME} ${TSV_SOURCE_PATH})
    endif()
elseif(UNIX AND SFML_FOUND)
    set(CPP_FILE "${CMAKE_CURRENT_SOURCE_DIR}/${OS}/SFML/Main.cpp")
    add_executable(${BINARY_NAME} ${CPP_FILE})
    paxs_setup_sfml(${BINARY_NAME})
    paxs_copy_config_tsv(${BINARY_NAME} ${TSV_SOURCE_PATH})

    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Config.tsv DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${OS}/Data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

    # バージョンの読み込み
    include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/ReadVersion.cmake)
    read_pax_sapientica_version()

    set(CPACK_GENERATOR "DEB")
    set(CPACK_PACKAGE_NAME "PAX_SAPIENTICA")
    set(CPACK_PACKAGE_VERSION ${PAX_SAPIENTICA_LIBRARY_VERSION})
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "PAX_SAPIENTICA")
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "libsfml-dev")
endif()

# DxLib target
if(WIN32 AND DxLib_FOUND)
    set(DXLIB_BINARY_NAME "DxLibMapViewer")
    set(DXLIB_CPP_FILE "${CMAKE_CURRENT_SOURCE_DIR}/${OS}/DxLib/Main.cpp")

    add_executable(${DXLIB_BINARY_NAME} WIN32 ${DXLIB_CPP_FILE})
    paxs_setup_dxlib(${DXLIB_BINARY_NAME})

    # Copy Config.tsv to build directory
    set(DXLIB_TSV_SOURCE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/${OS}/Config.tsv")
    paxs_copy_config_tsv(${DXLIB_BINARY_NAME} ${DXLIB_TSV_SOURCE_PATH})
endif()

# Siv3D target
if(WIN32 AND Siv3D_FOUND)
    set(SIV3D_BINARY_NAME "Siv3DMapViewer")
    set(SIV3D_CPP_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/${OS}/OpenSiv3D_0.6.13/Main.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/${OS}/OpenSiv3D_0.6.13/stdafx.cpp"
    )
    set(SIV3D_HEADER_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/${OS}/OpenSiv3D_0.6.13/stdafx.h"
    )
    set(SIV3D_RESOURCE_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/${OS}/OpenSiv3D_0.6.13/App/Resource.rc"
    )

    add_executable(${SIV3D_BINARY_NAME} WIN32 ${SIV3D_CPP_FILES} ${SIV3D_HEADER_FILES} ${SIV3D_RESOURCE_FILES})
    paxs_setup_siv3d(${SIV3D_BINARY_NAME})

    # Set up precompiled header (Siv3D-specific)
    if(MSVC)
        set_source_files_properties(
            "${CMAKE_CURRENT_SOURCE_DIR}/${OS}/OpenSiv3D_0.6.13/stdafx.cpp"
            PROPERTIES COMPILE_FLAGS "/Ycstdafx.h"
        )
        target_compile_options(${SIV3D_BINARY_NAME} PRIVATE /Yustdafx.h /FI"stdafx.h")
    endif()

    # Copy Siv3D resources to build directory
    set(SIV3D_APP_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${OS}/OpenSiv3D_0.6.13/App")
    paxs_copy_siv3d_resources(${SIV3D_BINARY_NAME} ${SIV3D_APP_DIR} ${TSV_SOURCE_PATH})
endif()

# Console MapViewer (macOS and Windows)
if(APPLE OR WIN32)
    set(CONSOLE_BINARY_NAME "ConsoleMapViewer")
    set(CONSOLE_CPP_FILE "${CMAKE_CURRENT_SOURCE_DIR}/${OS}/Console/Main.cpp")

    add_executable(${CONSOLE_BINARY_NAME} ${CONSOLE_CPP_FILE})
    paxs_setup_common_options(${CONSOLE_BINARY_NAME})
    paxs_copy_config_tsv(${CONSOLE_BINARY_NAME} ${TSV_SOURCE_PATH})
endif()
