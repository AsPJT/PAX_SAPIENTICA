cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

set(SOURCE_TSV_FILE "${CMAKE_CURRENT_SOURCE_DIR}/data/Config.tsv")
set(DESTINATION_DIR "${CMAKE_CURRENT_BINARY_DIR}")

add_custom_target(CopyTSV ALL
    COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_TSV_FILE} ${DESTINATION_DIR}
)
add_dependencies(SettlementSimulatorTest CopyTSV)
