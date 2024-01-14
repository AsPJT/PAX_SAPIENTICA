cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# os
if(APPLE)
    set(OS "macOS")
elseif(UNIX)
    set(OS "Ubuntu")
else()
    message(FATAL_ERROR "Unsupported OS")
endif()

set(PROJECT_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../..")

include_directories(
    ${PROJECT_ROOT_DIR}/Library
    ${PROJECT_ROOT_DIR}/ExternalLibrary
)

set(SOURCE_TSV_FILE "${CMAKE_CURRENT_SOURCE_DIR}/${OS}/Config.tsv")
set(DESTINATION_DIR "${CMAKE_CURRENT_BINARY_DIR}")

set(PAXS_BUILD_TYPE "Development" CACHE STRING "Choose the type of build (Development or Production)")

if(PAXS_BUILD_TYPE STREQUAL "Development")
    find_package(SFML 2.5 COMPONENTS graphics)
    if(SFML_FOUND)
        set(BINARY_NAME "SFMLMapviewer")
        add_executable(${BINARY_NAME} ${OS}/SFML_2.6.1/Main.cpp)
        target_link_libraries(${BINARY_NAME} sfml-graphics)
        add_definitions(-DPAXS_DEVELOPMENT)
        add_custom_command(
            TARGET ${BINARY_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_TSV_FILE} ${DESTINATION_DIR}
        )
    endif()
elseif(PAXS_BUILD_TYPE STREQUAL "Production")
    set(BINARY_NAME "PAX_SAPIENTICA")
    if(APPLE)
        set(SFML_INCLUDE_DIR "${OS}/SFML_2.6.1/include")
        set(SFML_LIBRARY_DIR "${OS}/SFML_2.6.1/lib")

        include_directories(${SFML_INCLUDE_DIR})

        add_executable(${BINARY_NAME} ${OS}/SFML_2.6.1/Main.cpp)

        target_link_directories(${BINARY_NAME} PRIVATE ${SFML_LIBRARY_DIR})
        target_link_libraries(${BINARY_NAME} sfml-graphics.2.6.1 sfml-window.2.6.1 sfml-system.2.6.1)
        add_custom_command(
            TARGET ${BINARY_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_TSV_FILE} ${DESTINATION_DIR}
        )

        set_target_properties(${BINARY_NAME} PROPERTIES
            INSTALL_RPATH "@executable_path/../Resources/SFML/lib"
            BUILD_WITH_INSTALL_RPATH TRUE
        )
    elseif(UNIX)
        find_package(SFML 2.5 COMPONENTS graphics)

        add_executable(${BINARY_NAME} ${OS}/SFML/Main.cpp)
        target_link_libraries(${BINARY_NAME} sfml-graphics)

        add_custom_command(
            TARGET ${BINARY_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_TSV_FILE} ${DESTINATION_DIR}
        )

        install(FILES ${DESTINATION_DIR}/Config.tsv DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
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

        include(CPack)
    endif()
endif()
