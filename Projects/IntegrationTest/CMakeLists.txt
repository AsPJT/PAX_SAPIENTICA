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

