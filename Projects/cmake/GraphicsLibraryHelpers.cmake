# GraphicsLibraryHelpers.cmake
# Helper functions for setting up graphics libraries (SFML, DxLib, Siv3D)

# Set common options for all PAX_SAPIENTICA targets
function(paxs_setup_common_options TARGET_NAME)
    if(NOT DEFINED PROJECT_ROOT_DIR)
        set(PROJECT_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../.." PARENT_SCOPE)
    endif()

    target_include_directories(${TARGET_NAME} PRIVATE
        ${PROJECT_ROOT_DIR}/Library
        ${PROJECT_ROOT_DIR}/ExternalLibrary
    )

    if(MSVC)
        target_compile_options(${TARGET_NAME} PRIVATE /utf-8 /bigobj)
    endif()

    if(PAXS_BUILD_TYPE STREQUAL "Development")
        target_compile_definitions(${TARGET_NAME} PRIVATE PAXS_DEVELOPMENT)
    endif()
endfunction()

# Setup SFML for a target
# Usage: paxs_setup_sfml(target_name [DEFINE_MACRO])
#   DEFINE_MACRO: Optional. If specified, defines PAXS_USING_SFML via CMake.
#                 Use this only for PAX_GRAPHICA test code where the library
#                 needs to be selected at build time. Normal code should
#                 define PAXS_USING_SFML in the source file itself.
function(paxs_setup_sfml TARGET_NAME)
    paxs_setup_common_options(${TARGET_NAME})

    target_link_libraries(${TARGET_NAME} PRIVATE SFML::Graphics)

    # macOS and Ubuntu need curl
    if(APPLE OR (UNIX AND NOT WIN32))
        find_package(CURL REQUIRED)
        target_link_libraries(${TARGET_NAME} PRIVATE CURL::libcurl)
    endif()

    # Windows: Copy SFML DLLs
    if(WIN32 AND SFML_ROOT_DIR AND EXISTS "${SFML_ROOT_DIR}/bin")
        set(SFML_DLLS
            "${SFML_ROOT_DIR}/bin/sfml-graphics$<$<CONFIG:Debug>:-d>-3.dll"
            "${SFML_ROOT_DIR}/bin/sfml-window$<$<CONFIG:Debug>:-d>-3.dll"
            "${SFML_ROOT_DIR}/bin/sfml-system$<$<CONFIG:Debug>:-d>-3.dll"
        )
        add_custom_command(
            TARGET ${TARGET_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SFML_DLLS} $<TARGET_FILE_DIR:${TARGET_NAME}>
            COMMENT "Copying SFML DLLs for ${TARGET_NAME}..."
        )
    endif()

    # Optional: Define PAXS_USING_SFML if DEFINE_MACRO argument is provided
    set(OPTIONS DEFINE_MACRO)
    cmake_parse_arguments(ARG "${OPTIONS}" "" "" ${ARGN})
    if(ARG_DEFINE_MACRO)
        target_compile_definitions(${TARGET_NAME} PRIVATE PAXS_USING_SFML)
    endif()
endfunction()

# Setup DxLib for a target
# Usage: paxs_setup_dxlib(target_name [DEFINE_MACRO])
#   DEFINE_MACRO: Optional. If specified, defines PAXS_USING_DXLIB via CMake.
function(paxs_setup_dxlib TARGET_NAME)
    paxs_setup_common_options(${TARGET_NAME})

    target_link_libraries(${TARGET_NAME} PRIVATE DxLib::DxLib)

    if(MSVC)
        target_compile_options(${TARGET_NAME} PRIVATE /openmp)
        set_property(TARGET ${TARGET_NAME} PROPERTY
            MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
        )
    endif()

    # Optional: Define PAXS_USING_DXLIB if DEFINE_MACRO argument is provided
    set(OPTIONS DEFINE_MACRO)
    cmake_parse_arguments(ARG "${OPTIONS}" "" "" ${ARGN})
    if(ARG_DEFINE_MACRO)
        target_compile_definitions(${TARGET_NAME} PRIVATE PAXS_USING_DXLIB)
    endif()
endfunction()

# Setup Siv3D for a target
# Usage: paxs_setup_siv3d(target_name [DEFINE_MACRO])
#   DEFINE_MACRO: Optional. If specified, defines PAXS_USING_SIV3D via CMake.
function(paxs_setup_siv3d TARGET_NAME)
    paxs_setup_common_options(${TARGET_NAME})

    target_link_libraries(${TARGET_NAME} PRIVATE Siv3D::Siv3D)
    target_link_directories(${TARGET_NAME} PRIVATE "${Siv3D_ROOT_DIR}/lib/Windows")

    # Get Siv3D include directories from the target
    get_target_property(SIV3D_INCLUDES Siv3D::Siv3D INTERFACE_INCLUDE_DIRECTORIES)
    target_include_directories(${TARGET_NAME} PRIVATE ${SIV3D_INCLUDES})

    if(MSVC)
        target_compile_options(${TARGET_NAME} PRIVATE
            /openmp
            /std:c++latest
            /Zc:__cplusplus
        )
        set_property(TARGET ${TARGET_NAME} PROPERTY
            MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
        )
        set_property(TARGET ${TARGET_NAME} PROPERTY CXX_STANDARD 23)
    endif()

    # Define required definitions from vcxproj
    target_compile_definitions(${TARGET_NAME} PRIVATE
        _WINDOWS
        _ENABLE_EXTENDED_ALIGNED_STORAGE
        _SILENCE_CXX20_CISO646_REMOVED_WARNING
        _SILENCE_ALL_CXX23_DEPRECATION_WARNINGS
        _SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS
    )

    # Optional: Define PAXS_USING_SIV3D if DEFINE_MACRO argument is provided
    set(OPTIONS DEFINE_MACRO)
    cmake_parse_arguments(ARG "${OPTIONS}" "" "" ${ARGN})
    if(ARG_DEFINE_MACRO)
        target_compile_definitions(${TARGET_NAME} PRIVATE PAXS_USING_SIV3D)
    endif()

    # Delay load DLLs as specified in original vcxproj
    if(MSVC)
        target_link_options(${TARGET_NAME} PRIVATE
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
endfunction()

# Copy Config.tsv to target build directory
function(paxs_copy_config_tsv TARGET_NAME TSV_PATH)
    if(EXISTS ${TSV_PATH})
        add_custom_command(
            TARGET ${TARGET_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${TSV_PATH} $<TARGET_FILE_DIR:${TARGET_NAME}>
            COMMENT "Copying Config.tsv for ${TARGET_NAME}..."
        )
    else()
        message(WARNING "Config.tsv not found at ${TSV_PATH}")
    endif()
endfunction()

# Copy Siv3D resources to target build directory
function(paxs_copy_siv3d_resources TARGET_NAME SIV3D_APP_DIR CONFIG_TSV_PATH)
    add_custom_command(
        TARGET ${TARGET_NAME} POST_BUILD
        # Copy Config.tsv
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CONFIG_TSV_PATH}"
            $<TARGET_FILE_DIR:${TARGET_NAME}>
        # Copy engine folder
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${SIV3D_APP_DIR}/engine"
            $<TARGET_FILE_DIR:${TARGET_NAME}>/engine
        # Copy dll folder if exists
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${SIV3D_APP_DIR}/dll"
            $<TARGET_FILE_DIR:${TARGET_NAME}>/dll
        COMMENT "Copying Siv3D resources for ${TARGET_NAME}..."
    )
endfunction()

# Generate Config.tsv with relative path to project root
# This is useful for tests that need to locate asset files relative to the build directory
# Usage: paxs_generate_config_tsv()
#   Creates a Config.tsv in CMAKE_BINARY_DIR with asset_file pointing to project root
#   Can be called multiple times - only generates the target once
function(paxs_generate_config_tsv)
    # Check if target already exists
    if(TARGET GenerateConfigTSV)
        return()
    endif()

    if(NOT DEFINED PROJECT_ROOT_DIR)
        set(PROJECT_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../..")
    endif()

    # Calculate relative path from CMAKE_BINARY_DIR to project root
    file(RELATIVE_PATH PROJECT_ROOT_RELATIVE_PATH
        "${CMAKE_BINARY_DIR}"
        "${PROJECT_ROOT_DIR}"
    )

    set(PATH_TSV_FILE "${CMAKE_BINARY_DIR}/Config.tsv")
    set(TSV_CONTENT "key\tvalue\nasset_file\t${PROJECT_ROOT_RELATIVE_PATH}\n")

    file(WRITE ${PATH_TSV_FILE} ${TSV_CONTENT})

    add_custom_target(GenerateConfigTSV ALL
        DEPENDS ${PATH_TSV_FILE}
    )

    # Export the target name to parent scope
    set(PAXS_CONFIG_TSV_TARGET "GenerateConfigTSV" PARENT_SCOPE)
endfunction()

# Copy generated Config.tsv from CMAKE_BINARY_DIR to target directory
# Usage: paxs_copy_generated_config_tsv(target_name)
#   Generates a Config.tsv with the correct relative path for the target's directory
function(paxs_copy_generated_config_tsv TARGET_NAME)
    if(NOT DEFINED PROJECT_ROOT_DIR)
        set(PROJECT_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../..")
    endif()

    # Calculate relative path from target's output directory to project root
    # Note: We use CMAKE_CURRENT_BINARY_DIR as a proxy since we can't know
    # the exact target directory at configure time
    file(RELATIVE_PATH TARGET_ROOT_RELATIVE_PATH
        "${CMAKE_CURRENT_BINARY_DIR}"
        "${PROJECT_ROOT_DIR}"
    )

    # Create a temporary file with the correct content
    set(TEMP_TSV_FILE "${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}_Config.tsv")
    file(WRITE ${TEMP_TSV_FILE} "key\tvalue\nasset_file\t${TARGET_ROOT_RELATIVE_PATH}\n")

    # Copy the temporary file to the target directory
    add_custom_command(
        TARGET ${TARGET_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${TEMP_TSV_FILE}"
            $<TARGET_FILE_DIR:${TARGET_NAME}>/Config.tsv
        COMMENT "Copying Config.tsv to ${TARGET_NAME}..."
    )
    add_dependencies(${TARGET_NAME} GenerateConfigTSV)
endfunction()
