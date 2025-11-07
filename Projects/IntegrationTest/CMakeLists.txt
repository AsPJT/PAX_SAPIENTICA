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
    target_compile_definitions(WindowTest PRIVATE PAXS_USING_SFML)

    add_executable(SFMLTest Source/SFMLTest.cpp)
    if(WIN32 OR APPLE)
        target_link_libraries(SFMLTest PRIVATE SFML::Graphics)
        target_link_libraries(WindowTest PRIVATE SFML::Graphics)
    elseif(UNIX)
        target_link_libraries(SFMLTest PRIVATE sfml-graphics)
        target_link_libraries(WindowTest PRIVATE sfml-graphics)
    endif()

    if(WIN32)
        set(SFML_DLLS
            "${SFML_ROOT_DIR}/bin/sfml-graphics-d-3.dll"
            "${SFML_ROOT_DIR}/bin/sfml-window-d-3.dll"
            "${SFML_ROOT_DIR}/bin/sfml-system-d-3.dll"
        )
        add_custom_command(
            TARGET SFMLTest POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SFML_DLLS}  $<TARGET_FILE_DIR:SFMLTest>
        )
        add_custom_command(
            TARGET WindowTest POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SFML_DLLS} $<TARGET_FILE_DIR:WindowTest>
        )
    endif()
endif()

add_executable(SimulatorTest Source/SimulatorTest.cpp)
target_compile_definitions(SimulatorTest PRIVATE PAX_SAPIENTICA_DEBUG)

add_executable(SettlementSimulatorTest Source/SettlementSimulatorTest.cpp)
target_compile_definitions(SettlementSimulatorTest PRIVATE PAX_SAPIENTICA_DEBUG)

add_executable(AppStateManagerTest AppStateManagerTest.cpp)
target_link_libraries(AppStateManagerTest PRIVATE gtest_main)

add_custom_target(CopyTSV ALL
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/Data/Config.tsv" ${CMAKE_CURRENT_BINARY_DIR}
)
add_dependencies(SettlementSimulatorTest CopyTSV)
