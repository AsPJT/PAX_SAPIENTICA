cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Library
    ${CMAKE_CURRENT_SOURCE_DIR}/../../ExternalLibrary/googletest/googletest/include
)

add_executable(EnvironmentTest source/EnvironmentTest.cpp)
add_executable(WindowTest source/WindowTest.cpp)

find_package(SFML 2.5 COMPONENTS graphics)
if(SFML_FOUND)
    add_executable(SFMLTest source/SFMLTest.cpp)
    target_link_libraries(SFMLTest sfml-graphics)
    add_definitions(-DPAXS_USING_SFML)
    target_link_libraries(WindowTest sfml-graphics sfml-window sfml-system)
endif()

add_executable(SimulatorTest source/SimulatorTest.cpp)
target_compile_definitions(SimulatorTest PRIVATE PAX_SAPIENTICA_DEBUG)

add_executable(SettlementSimulatorTest source/SettlementSimulatorTest.cpp)
target_compile_definitions(SettlementSimulatorTest PRIVATE PAX_SAPIENTICA_DEBUG)


