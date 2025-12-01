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
        set(OS "Linux" CACHE STRING "Operating System name")
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

    # First try to find SFML via vcpkg (standard CMake config)
    find_package(SFML 3 QUIET COMPONENTS Graphics Window System)

    # If not found via vcpkg, try custom finder (includes project-local SFML for Windows and legacy macOS)
    if(NOT SFML_FOUND)
        find_package(SFMLCustom QUIET)
    else()
        message(STATUS "Found SFML ${SFML_VERSION} via CMake config (vcpkg or system)")
    endif()
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
elseif(APPLE AND SFML_FOUND)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${OS}/SFML/Main.cpp")
        set(CPP_FILE "${CMAKE_CURRENT_SOURCE_DIR}/${OS}/SFML/Main.cpp")
    endif()

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

        # Get SFML library paths
        get_target_property(SFML_GRAPHICS_LIB SFML::Graphics IMPORTED_LOCATION_RELEASE)
        if(NOT SFML_GRAPHICS_LIB)
            get_target_property(SFML_GRAPHICS_LIB SFML::Graphics IMPORTED_LOCATION)
        endif()

        # Check if SFML is statically or dynamically linked
        set(SFML_IS_STATIC FALSE)
        if(SFML_GRAPHICS_LIB AND SFML_GRAPHICS_LIB MATCHES "\\.a$")
            set(SFML_IS_STATIC TRUE)
        elseif(SFML_STATIC_LIBRARIES)
            set(SFML_IS_STATIC TRUE)
        endif()

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
            COMMENT "Setting up app bundle resources..."
        )

        # Only copy SFML dylibs if using dynamic linking
        if(SFML_GRAPHICS_LIB AND NOT SFML_IS_STATIC)
            get_filename_component(SFML_LIB_DIR ${SFML_GRAPHICS_LIB} DIRECTORY)
            set(APP_FRAMEWORKS_DIR "$<TARGET_BUNDLE_CONTENT_DIR:${BINARY_NAME}>/Frameworks")

            add_custom_command(
                TARGET ${BINARY_NAME} POST_BUILD
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
        else()
            message(STATUS "Using static SFML libraries - no dylibs to copy")
        endif()

        # CPack settings for DMG generation
        set(CPACK_GENERATOR "DragNDrop")
        set(CPACK_PACKAGE_NAME "PAX_SAPIENTICA")
        set(CPACK_PACKAGE_VERSION ${PAX_SAPIENTICA_LIBRARY_VERSION})
        set(CPACK_PACKAGE_FILE_NAME "PAX_SAPIENTICA-${PAX_SAPIENTICA_LIBRARY_VERSION}")
        set(CPACK_PACKAGE_VENDOR "PAX SAPIENTICA Team")
        set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "PAX SAPIENTICA - Historical Simulation Viewer")
        set(CPACK_DMG_VOLUME_NAME "PAX SAPIENTICA ${PAX_SAPIENTICA_LIBRARY_VERSION}")
        set(CPACK_DMG_FORMAT "UDBZ")
        set(CPACK_DMG_BACKGROUND_IMAGE "${CMAKE_CURRENT_SOURCE_DIR}/${OS}/DMGBackground.png")
        set(CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/${OS}/appicon.icns")
        set(CPACK_SYSTEM_NAME "")

        # Note: CPACK_DMG_DS_STORE_SETUP_SCRIPT is disabled due to AppleScript compatibility issues
        # The DMG will use default layout with background image
        # set(CPACK_DMG_DS_STORE_SETUP_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/${OS}/DMGSetup.scpt")

        # Install the .app bundle
        install(TARGETS ${BINARY_NAME}
            BUNDLE DESTINATION .
        )

        # Include CPack module to enable DMG generation
        include(CPack)
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

    # バージョンの読み込み
    include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/ReadVersion.cmake)
    read_pax_sapientica_version()

    if(PAXS_BUILD_TYPE STREQUAL "Production")
        # Production build: Create DEB package with Data directory

        # Install binary to /usr/local/bin
        install(TARGETS ${BINARY_NAME}
            RUNTIME DESTINATION bin
        )

        # Install Config.tsv to /usr/local/share/pax_sapientica
        install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Config.tsv
            DESTINATION share/pax_sapientica
        )

        # Install Data directory if it exists
        if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${OS}/Data)
            install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${OS}/Data
                DESTINATION share/pax_sapientica
            )
        endif()

        # CPack settings for DEB package
        set(CPACK_GENERATOR "DEB")
        set(CPACK_PACKAGE_NAME "pax-sapientica")
        set(CPACK_PACKAGE_VERSION ${PAX_SAPIENTICA_LIBRARY_VERSION})
        set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "PAX SAPIENTICA - Historical Simulation Viewer")
        set(CPACK_PACKAGE_DESCRIPTION "PAX SAPIENTICA is a historical simulation and map viewer application.")
        set(CPACK_DEBIAN_PACKAGE_MAINTAINER "PAX SAPIENTICA Team")
        set(CPACK_DEBIAN_PACKAGE_DEPENDS "libsfml-graphics2.6 (>= 2.6.0), libsfml-window2.6 (>= 2.6.0), libsfml-system2.6 (>= 2.6.0)")
        set(CPACK_DEBIAN_PACKAGE_SECTION "graphics")
        set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
        set(CPACK_PACKAGE_CONTACT "https://github.com/guinpen/PAX_SAPIENTICA")

        # Include CPack module to enable package generation
        include(CPack)
    else()
        # Development build: Install to build directory for local testing
        install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Config.tsv
            DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
        )

        if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${OS}/Data)
            install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${OS}/Data
                DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
            )
        endif()
    endif()
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
