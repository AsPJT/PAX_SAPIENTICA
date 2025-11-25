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
    ${PROJECT_ROOT_DIR}/ExternalLibrary
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../../ExternalLibrary/googletest ${CMAKE_BINARY_DIR}/googletest)

set(TEST_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Source")

file(GLOB_RECURSE TEST_SOURCES "${TEST_SOURCE_DIR}/*.cpp")
foreach(TEST_SOURCE ${TEST_SOURCES})
    string(REPLACE "${TEST_SOURCE_DIR}/" "" TARGET_NAME ${TEST_SOURCE})
    string(REPLACE ".cpp" "" TARGET_NAME ${TARGET_NAME})
    string(REPLACE "/" "_" TARGET_NAME ${TARGET_NAME})
    string(REPLACE "${TEST_SOURCE_DIR}/" "Source/" TEST_SOURCE ${TEST_SOURCE})
    add_executable(${TARGET_NAME} ${TEST_SOURCE})
    target_link_libraries(${TARGET_NAME} gtest_main)
    add_test(
        NAME ${TARGET_NAME}
        COMMAND ${TARGET_NAME}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
    set_tests_properties(${TARGET_NAME} PROPERTIES LABELS "Unit")

    # Auto-detect PathTSV dependency by checking if test reads Config.tsv
    file(READ ${TEST_SOURCE} TEST_FILE_CONTENT)
    if(TEST_FILE_CONTENT MATCHES "Config\\.tsv" OR TEST_FILE_CONTENT MATCHES "AppConfig::getInstance")
        add_dependencies(${TARGET_NAME} GenerateConfigTSV)
        message(STATUS "Auto-detected Config.tsv dependency: ${TARGET_NAME}")
    endif()
endforeach()

# Generate Config.tsv with relative path to project root
paxs_generate_config_tsv()
