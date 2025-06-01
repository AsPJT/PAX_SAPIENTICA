cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Library
    ${CMAKE_CURRENT_SOURCE_DIR}/../../ExternalLibrary/googletest/googletest/include
)

add_executable(EnvironmentTest Source/EnvironmentTest.cpp)
add_executable(WindowTest Source/WindowTest.cpp)

if(SFML_FOUND)
    add_executable(SFMLTest Source/SFMLTest.cpp)
    if(WIN32 OR APPLE)
        target_link_libraries(SFMLTest PRIVATE SFML::Graphics SFML::Window SFML::System)
        target_link_libraries(WindowTest PRIVATE SFML::Graphics SFML::Window SFML::System)
    elseif(UNIX)
        target_link_libraries(SFMLTest PRIVATE sfml-graphics)
        target_link_libraries(WindowTest PRIVATE sfml-graphics sfml-window sfml-system)
    endif()
endif()

add_executable(SimulatorTest Source/SimulatorTest.cpp)
target_compile_definitions(SimulatorTest PRIVATE PAX_SAPIENTICA_DEBUG)

add_executable(SettlementSimulatorTest Source/SettlementSimulatorTest.cpp)
target_compile_definitions(SettlementSimulatorTest PRIVATE PAX_SAPIENTICA_DEBUG)

set(SOURCE_TSV_FILE "${CMAKE_CURRENT_SOURCE_DIR}/Data/Config.tsv")
set(DESTINATION_DIR "${CMAKE_CURRENT_BINARY_DIR}")

add_custom_target(CopyTSV ALL
    COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_TSV_FILE} ${DESTINATION_DIR}
)
add_dependencies(SettlementSimulatorTest CopyTSV)
