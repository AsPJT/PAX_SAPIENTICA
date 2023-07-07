cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)
set(EXECUTABLE_OUTPUT_PATH ../../Project/UnitTest/bin)
include_directories(../../Library)
include_directories(../../ExternalLibrary/googletest/googletest/include)

set(GOOGLETEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../ExternalLibrary/googletest)

add_subdirectory(${GOOGLETEST_DIR} ${CMAKE_BINARY_DIR}/googletest)

enable_testing()

add_executable(FileErrorLogTest source/FileErrorLogTest.cpp)
add_executable(FileUnitTest source/FileUnitTest.cpp)

target_link_libraries(FileUnitTest gtest_main)

add_test(NAME example_test COMMAND FileUnitTest)
