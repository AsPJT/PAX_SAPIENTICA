cmake_minimum_required(VERSION 3.16)
project(MapViewer)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../..")

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

    if(WIN32 OR APPLE)
        # Set SFML_ROOT_DIR for project-local SFML installation
        if(NOT SFML_ROOT_DIR)
            set(SFML_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${OS}/SFML_3.0.0")
        endif()
        find_package(SFMLCustom REQUIRED)
    elseif(UNIX)
        find_package(SFML COMPONENTS Graphics QUIET)
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
    target_link_libraries(${BINARY_NAME} PRIVATE SFML::Graphics)
    if(MSVC)
        target_compile_options(${BINARY_NAME} PRIVATE /utf-8 /bigobj)
    endif()

    # Copy SFML DLLs if available
    if(SFML_ROOT_DIR AND EXISTS "${SFML_ROOT_DIR}/bin")
        set(SFML_DLLS
            "${SFML_ROOT_DIR}/bin/sfml-graphics-d-3.dll"
            "${SFML_ROOT_DIR}/bin/sfml-window-d-3.dll"
            "${SFML_ROOT_DIR}/bin/sfml-system-d-3.dll"
        )
        add_custom_command(
            TARGET ${BINARY_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${TSV_SOURCE_PATH} $<TARGET_FILE_DIR:${BINARY_NAME}>
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SFML_DLLS} $<TARGET_FILE_DIR:${BINARY_NAME}>
            COMMENT "Copying SFML DLLs, Config.tsv..."
        )
    else()
        add_custom_command(
            TARGET ${BINARY_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${TSV_SOURCE_PATH} $<TARGET_FILE_DIR:${BINARY_NAME}>
            COMMENT "Copying Config.tsv for SFML..."
        )
    endif()
elseif(APPLE)
    set(CPP_FILE "${CMAKE_CURRENT_SOURCE_DIR}/${OS}/SFML_3.0.0/Main.cpp")

    if(PAXS_BUILD_TYPE STREQUAL "Production")
        # Production: macOS .app bundle

        # Read version information
        file(READ ${PROJECT_ROOT_DIR}/Library/PAX_SAPIENTICA/Version.hpp VERSION_HPP)
        string(REGEX MATCH "PAX_SAPIENTICA_LIBRARY_MAJOR \\(([0-9]+)\\)" _ ${VERSION_HPP})
        set(PAX_SAPIENTICA_LIBRARY_MAJOR ${CMAKE_MATCH_1})
        string(REGEX MATCH "PAX_SAPIENTICA_LIBRARY_MINOR \\(([0-9]+)\\)" _ ${VERSION_HPP})
        set(PAX_SAPIENTICA_LIBRARY_MINOR ${CMAKE_MATCH_1})
        string(REGEX MATCH "PAX_SAPIENTICA_LIBRARY_PATCHLEVEL \\(([0-9]+)\\)" _ ${VERSION_HPP})
        set(PAX_SAPIENTICA_LIBRARY_PATCHLEVEL ${CMAKE_MATCH_1})
        set(PAX_SAPIENTICA_LIBRARY_VERSION "${PAX_SAPIENTICA_LIBRARY_MAJOR}.${PAX_SAPIENTICA_LIBRARY_MINOR}.${PAX_SAPIENTICA_LIBRARY_PATCHLEVEL}")

        add_executable(${BINARY_NAME} MACOSX_BUNDLE ${CPP_FILE})

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
        target_link_libraries(${BINARY_NAME} SFML::Graphics curl)
        add_custom_command(
            TARGET ${BINARY_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${TSV_SOURCE_PATH} ${CMAKE_CURRENT_BINARY_DIR}
        )
    endif()
elseif(UNIX AND SFML_FOUND)
    set(CPP_FILE "${CMAKE_CURRENT_SOURCE_DIR}/${OS}/SFML/Main.cpp")
    add_executable(${BINARY_NAME} ${CPP_FILE})
    target_link_libraries(${BINARY_NAME} SFML::Graphics curl)
    add_custom_command(
        TARGET ${BINARY_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${TSV_SOURCE_PATH} ${CMAKE_CURRENT_BINARY_DIR}
    )

    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Config.tsv DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${OS}/Data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

    # バージョンの読み込み
    file(READ ${PROJECT_ROOT_DIR}/Library/PAX_SAPIENTICA/Version.hpp VERSION_HPP)
    string(REGEX MATCH "PAX_SAPIENTICA_LIBRARY_MAJOR \\(([0-9]+)\\)" _ ${VERSION_HPP})
    set(PAX_SAPIENTICA_LIBRARY_MAJOR ${CMAKE_MATCH_1})
    string(REGEX MATCH "PAX_SAPIENTICA_LIBRARY_MINOR \\(([0-9]+)\\)" _ ${VERSION_HPP})
    set(PAX_SAPIENTICA_LIBRARY_MINOR ${CMAKE_MATCH_1})
    string(REGEX MATCH "PAX_SAPIENTICA_LIBRARY_PATCHLEVEL \\(([0-9]+)\\)" _ ${VERSION_HPP})
    set(PAX_SAPIENTICA_LIBRARY_PATCHLEVEL ${CMAKE_MATCH_1})
    set(PAX_SAPIENTICA_LIBRARY_VERSION ${PAX_SAPIENTICA_LIBRARY_MAJOR}.${PAX_SAPIENTICA_LIBRARY_MINOR}.${PAX_SAPIENTICA_LIBRARY_PATCHLEVEL})

    set(CPACK_GENERATOR "DEB")
    set(CPACK_PACKAGE_NAME "PAX_SAPIENTICA")
    set(CPACK_PACKAGE_VERSION ${PAX_SAPIENTICA_LIBRARY_VERSION})
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "PAX_SAPIENTICA")
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "libsfml-dev")
endif()

if(SFML_FOUND)
    if(PAXS_BUILD_TYPE STREQUAL "Development")
        target_compile_definitions(${BINARY_NAME} PRIVATE PAXS_DEVELOPMENT)
    endif()
endif()

# DxLib target
if(WIN32 AND DxLib_FOUND)
    set(DXLIB_BINARY_NAME "DxLibMapViewer")
    set(DXLIB_CPP_FILE "${CMAKE_CURRENT_SOURCE_DIR}/${OS}/DxLib/Main.cpp")

    add_executable(${DXLIB_BINARY_NAME} WIN32 ${DXLIB_CPP_FILE})
    target_link_libraries(${DXLIB_BINARY_NAME} PRIVATE DxLib::DxLib)
    target_include_directories(${DXLIB_BINARY_NAME} PRIVATE
        ${PROJECT_ROOT_DIR}/Library
        ${PROJECT_ROOT_DIR}/ExternalLibrary
    )

    if(MSVC)
        target_compile_options(${DXLIB_BINARY_NAME} PRIVATE
            /utf-8
            /openmp
            /bigobj
        )
        # Use MultiThreaded runtime library
        set_property(TARGET ${DXLIB_BINARY_NAME} PROPERTY
            MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
        )
    endif()

    target_compile_definitions(${DXLIB_BINARY_NAME} PRIVATE PAXS_USING_DXLIB)
    if(PAXS_BUILD_TYPE STREQUAL "Development")
        target_compile_definitions(${DXLIB_BINARY_NAME} PRIVATE PAXS_DEVELOPMENT)
    endif()

    # Copy Config.tsv to build directory
    set(DXLIB_TSV_SOURCE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/${OS}/Config.tsv")
    if(EXISTS ${DXLIB_TSV_SOURCE_PATH})
        add_custom_command(
            TARGET ${DXLIB_BINARY_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${DXLIB_TSV_SOURCE_PATH} $<TARGET_FILE_DIR:${DXLIB_BINARY_NAME}>
            COMMENT "Copying Config.tsv for DxLib..."
        )
    endif()
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

    # Add Siv3D library directory for dependencies
    target_link_directories(${SIV3D_BINARY_NAME} PRIVATE "${Siv3D_ROOT_DIR}/lib/Windows")

    target_link_libraries(${SIV3D_BINARY_NAME} PRIVATE Siv3D::Siv3D)

    # Delay load DLLs as specified in original vcxproj
    if(MSVC)
        target_link_options(${SIV3D_BINARY_NAME} PRIVATE
            /DELAYLOAD:advapi32.dll
            /DELAYLOAD:crypt32.dll
            /DELAYLOAD:dwmapi.dll
            /DELAYLOAD:gdi32.dll
            /DELAYLOAD:imm32.dll
            /DELAYLOAD:ole32.dll
            /DELAYLOAD:oleaut32.dll
            /DELAYLOAD:opengl32.dll
            /DELAYLOAD:shell32.dll
            /DELAYLOAD:shlwapi.dll
            /DELAYLOAD:user32.dll
            /DELAYLOAD:winmm.dll
            /DELAYLOAD:ws2_32.dll
        )
    endif()

    target_include_directories(${SIV3D_BINARY_NAME} PRIVATE
        ${PROJECT_ROOT_DIR}/Library
        ${PROJECT_ROOT_DIR}/ExternalLibrary
    )
    # Get Siv3D include directories from the target
    get_target_property(SIV3D_INCLUDES Siv3D::Siv3D INTERFACE_INCLUDE_DIRECTORIES)
    target_include_directories(${SIV3D_BINARY_NAME} PRIVATE ${SIV3D_INCLUDES})

    if(MSVC)
        target_compile_options(${SIV3D_BINARY_NAME} PRIVATE
            /utf-8
            /openmp
            /std:c++latest
            /Zc:__cplusplus
            /bigobj
        )
        # Use MultiThreaded runtime library
        set_property(TARGET ${SIV3D_BINARY_NAME} PROPERTY
            MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
        )
        # Set C++ standard to latest
        set_property(TARGET ${SIV3D_BINARY_NAME} PROPERTY CXX_STANDARD 23)

        # Set up precompiled header
        set_source_files_properties(
            "${CMAKE_CURRENT_SOURCE_DIR}/${OS}/OpenSiv3D_0.6.13/stdafx.cpp"
            PROPERTIES COMPILE_FLAGS "/Ycstdafx.h"
        )
        target_compile_options(${SIV3D_BINARY_NAME} PRIVATE /Yustdafx.h /FI"stdafx.h")
    endif()

    # Define required definitions from vcxproj (PAXS_USING_SIV3D is defined in Main.cpp)
    target_compile_definitions(${SIV3D_BINARY_NAME} PRIVATE
        _WINDOWS
        _ENABLE_EXTENDED_ALIGNED_STORAGE
        _SILENCE_CXX20_CISO646_REMOVED_WARNING
        _SILENCE_ALL_CXX23_DEPRECATION_WARNINGS
        _SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS
    )
    if(PAXS_BUILD_TYPE STREQUAL "Development")
        target_compile_definitions(${SIV3D_BINARY_NAME} PRIVATE PAXS_DEVELOPMENT)
    endif()

    # Copy Siv3D resources to build directory
    set(SIV3D_APP_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${OS}/OpenSiv3D_0.6.13/App")

    add_custom_command(
        TARGET ${SIV3D_BINARY_NAME} POST_BUILD
        # Copy Config.tsv (use the Windows one, not the App one)
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${TSV_SOURCE_PATH}"
            $<TARGET_FILE_DIR:${SIV3D_BINARY_NAME}>
        # Copy engine folder
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${SIV3D_APP_DIR}/engine"
            $<TARGET_FILE_DIR:${SIV3D_BINARY_NAME}>/engine
        # Copy dll folder if exists
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${SIV3D_APP_DIR}/dll"
            $<TARGET_FILE_DIR:${SIV3D_BINARY_NAME}>/dll
        COMMENT "Copying Siv3D resources..."
    )
endif()

# Console MapViewer (macOS and Windows)
if(APPLE OR WIN32)
    set(CONSOLE_BINARY_NAME "ConsoleMapViewer")
    set(CONSOLE_CPP_FILE "${CMAKE_CURRENT_SOURCE_DIR}/${OS}/Console/Main.cpp")

    add_executable(${CONSOLE_BINARY_NAME} ${CONSOLE_CPP_FILE})

    # No GUI libraries needed for console version
    target_include_directories(${CONSOLE_BINARY_NAME} PRIVATE
        ${PROJECT_ROOT_DIR}/Library
        ${PROJECT_ROOT_DIR}/ExternalLibrary
    )

    if(MSVC)
        target_compile_options(${CONSOLE_BINARY_NAME} PRIVATE /utf-8 /bigobj)
    endif()

    if(PAXS_BUILD_TYPE STREQUAL "Development")
        target_compile_definitions(${CONSOLE_BINARY_NAME} PRIVATE PAXS_DEVELOPMENT)
    endif()

    # Copy Config.tsv to build directory
    add_custom_command(
        TARGET ${CONSOLE_BINARY_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${TSV_SOURCE_PATH} $<TARGET_FILE_DIR:${CONSOLE_BINARY_NAME}>
        COMMENT "Copying Config.tsv for ConsoleMapViewer..."
    )
endif()
