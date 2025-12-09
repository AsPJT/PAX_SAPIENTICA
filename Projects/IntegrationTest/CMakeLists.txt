cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

set(PROJECT_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../..")

# Include helper functions for graphics libraries
include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/GraphicsLibraryHelpers.cmake)

include_directories(
    ${PROJECT_ROOT_DIR}/Library
    ${PROJECT_ROOT_DIR}/ExternalLibrary/googletest/googletest/include
)

# Generate Config.tsv with relative path to project root
paxs_generate_config_tsv()

add_executable(EnvironmentTest Source/EnvironmentTest.cpp)

# WindowTest for each graphics library
if(SFML_FOUND)
    add_executable(SFMLTest Source/SFMLTest.cpp)
    paxs_setup_sfml(SFMLTest)

    add_executable(SFMLWindowTest Source/WindowTest.cpp)
    paxs_setup_sfml(SFMLWindowTest DEFINE_MACRO)
    paxs_copy_generated_config_tsv(SFMLWindowTest)
endif()

if(WIN32 AND DxLib_FOUND)
    add_executable(DxLibWindowTest Source/WindowTest.cpp)
    paxs_setup_dxlib(DxLibWindowTest DEFINE_MACRO)
    paxs_copy_generated_config_tsv(DxLibWindowTest)
endif()

if(WIN32 AND Siv3D_FOUND)
    add_executable(Siv3DWindowTest Source/WindowTest.cpp)
    paxs_setup_siv3d(Siv3DWindowTest DEFINE_MACRO)
    paxs_copy_generated_config_tsv(Siv3DWindowTest)
endif()

add_executable(SimulatorTest Source/SimulatorTest.cpp)
target_compile_definitions(SimulatorTest PRIVATE PAX_SAPIENTICA_DEBUG)
paxs_copy_generated_config_tsv(SimulatorTest)

add_executable(SettlementSimulatorTest Source/SettlementSimulatorTest.cpp)
target_compile_definitions(SettlementSimulatorTest PRIVATE PAX_SAPIENTICA_DEBUG)
paxs_copy_generated_config_tsv(SettlementSimulatorTest)

add_executable(AppStateManagerTest AppStateManagerTest.cpp)
target_link_libraries(AppStateManagerTest PRIVATE gtest_main)

add_executable(UIComponentsEventDrivenTest Source/PAX_MAHOROBA/UI/UIComponentsEventDrivenTest.cpp)
target_link_libraries(UIComponentsEventDrivenTest PRIVATE gtest_main)
add_test(
    NAME UIComponentsEventDrivenTest
    COMMAND UIComponentsEventDrivenTest
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
set_tests_properties(UIComponentsEventDrivenTest PROPERTIES LABELS "Integration")

# FontDrawingTest for each graphics library
if(SFML_FOUND)
    add_executable(SFMLFontDrawingTest Source/FontDrawingTest.cpp)
    paxs_setup_sfml(SFMLFontDrawingTest DEFINE_MACRO)
    paxs_copy_generated_config_tsv(SFMLFontDrawingTest)
endif()

if(WIN32 AND DxLib_FOUND)
    add_executable(DxLibFontDrawingTest WIN32 Source/FontDrawingTest.cpp)
    paxs_setup_dxlib(DxLibFontDrawingTest DEFINE_MACRO)
    paxs_copy_generated_config_tsv(DxLibFontDrawingTest)
endif()

if(WIN32 AND Siv3D_FOUND)
    # Determine OS-specific directory name for Siv3D resources
    if(NOT DEFINED OS)
        if(WIN32)
            set(OS "Windows")
        endif()
    endif()

    set(SIV3D_RESOURCE_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/../MapViewer/${OS}/OpenSiv3D_0.6.13/App/Resource.rc"
    )

    add_executable(Siv3DFontDrawingTest WIN32 Source/FontDrawingTest.cpp ${SIV3D_RESOURCE_FILES})
    paxs_setup_siv3d(Siv3DFontDrawingTest DEFINE_MACRO)

    # Copy Siv3D resources (engine, dll folders) from MapViewer
    set(SIV3D_APP_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../MapViewer/${OS}/OpenSiv3D_0.6.13/App")
    add_custom_command(
        TARGET Siv3DFontDrawingTest POST_BUILD
        # Copy engine folder
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${SIV3D_APP_DIR}/engine"
            $<TARGET_FILE_DIR:Siv3DFontDrawingTest>/engine
        # Copy dll folder if exists
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${SIV3D_APP_DIR}/dll"
            $<TARGET_FILE_DIR:Siv3DFontDrawingTest>/dll
        COMMENT "Copying Siv3D resources for Siv3DFontDrawingTest..."
    )

    paxs_copy_generated_config_tsv(Siv3DFontDrawingTest)
endif()

