cmake_minimum_required(VERSION 3.16)
project(MapViewer)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../..")

include_directories(
    ${PROJECT_ROOT_DIR}/Library
    ${PROJECT_ROOT_DIR}/ExternalLibrary
)

set(SOURCE_TSV_FILE "${CMAKE_CURRENT_SOURCE_DIR}/${OS}/Config.tsv")
set(DESTINATION_DIR "${CMAKE_CURRENT_BINARY_DIR}")

set(PAXS_BUILD_TYPE "Development" CACHE STRING "Choose the type of build (Development or Production)")

if(PAXS_BUILD_TYPE STREQUAL "Development")
    set(BINARY_NAME "SFMLMapViewer")
elseif(PAXS_BUILD_TYPE STREQUAL "Production")
    set(BINARY_NAME "PAX_SAPIENTICA")
endif()

if(WIN32)
    set(CPP_FILE "${SFML_ROOT_DIR}/Main.cpp")
    add_executable(${BINARY_NAME} ${CPP_FILE})
    target_link_directories(${BINARY_NAME} PRIVATE ${SFML_ROOT_DIR}/lib)
    target_link_libraries(${BINARY_NAME} sfml-graphics.lib sfml-window.lib sfml-system.lib)
    set(SFML_DLLS
        "${SFML_ROOT_DIR}/sfml-graphics-2.dll"
        "${SFML_ROOT_DIR}/sfml-window-2.dll"
        "${SFML_ROOT_DIR}/sfml-system-2.dll"
    )
    add_custom_command(
        TARGET ${BINARY_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_TSV_FILE} ${DESTINATION_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy ${SFML_DLLS} ${DESTINATION_DIR}
    )
elseif(APPLE)
    set(CPP_FILE "${SFML_ROOT_DIR}/Main.cpp")
    add_executable(${BINARY_NAME} ${CPP_FILE})
    target_link_libraries(${BINARY_NAME} SFML::Graphics)
    add_custom_command(
        TARGET ${BINARY_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_TSV_FILE} ${DESTINATION_DIR}
    )

    if(PAXS_BUILD_TYPE STREQUAL "Production")
        set_target_properties(${BINARY_NAME} PROPERTIES
            INSTALL_RPATH "@executable_path/../Resources/SFML/lib"
            BUILD_WITH_INSTALL_RPATH TRUE
        )
    endif()
elseif(UNIX)
    set(CPP_FILE "${CMAKE_CURRENT_SOURCE_DIR}/${OS}/SFML/Main.cpp")
    add_executable(${BINARY_NAME} ${CPP_FILE})
    target_link_libraries(${BINARY_NAME} sfml-graphics)
    add_custom_command(
        TARGET ${BINARY_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_TSV_FILE} ${DESTINATION_DIR}
    )
endif()

target_compile_definitions(${BINARY_NAME} PRIVATE PAXS_USING_SFML)
if(PAXS_BUILD_TYPE STREQUAL "Development")
    target_compile_definitions(${BINARY_NAME} PRIVATE PAXS_DEVELOPMENT)
endif()

# if(PAXS_BUILD_TYPE STREQUAL "Development")
#     if(SFML_FOUND)
#     elseif(APPLE)
#     endif()
# elseif(PAXS_BUILD_TYPE STREQUAL "Production")
#     if(APPLE)
#     elseif(UNIX)
#         find_package(SFML 2.5 COMPONENTS graphics)

#         add_executable(${BINARY_NAME} ${OS}/SFML/Main.cpp)
#         target_link_libraries(${BINARY_NAME} sfml-graphics)

#         add_custom_command(
#             TARGET ${BINARY_NAME} POST_BUILD
#             COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_TSV_FILE} ${DESTINATION_DIR}
#         )

#         install(FILES ${DESTINATION_DIR}/Config.tsv DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
#         install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${OS}/Data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

#         # バージョンの読み込み
#         file(READ ${PROJECT_ROOT_DIR}/Library/PAX_SAPIENTICA/Version.hpp VERSION_HPP)
#         string(REGEX MATCH "PAX_SAPIENTICA_LIBRARY_MAJOR \\(([0-9]+)\\)" _ ${VERSION_HPP})
#         set(PAX_SAPIENTICA_LIBRARY_MAJOR ${CMAKE_MATCH_1})
#         string(REGEX MATCH "PAX_SAPIENTICA_LIBRARY_MINOR \\(([0-9]+)\\)" _ ${VERSION_HPP})
#         set(PAX_SAPIENTICA_LIBRARY_MINOR ${CMAKE_MATCH_1})
#         string(REGEX MATCH "PAX_SAPIENTICA_LIBRARY_PATCHLEVEL \\(([0-9]+)\\)" _ ${VERSION_HPP})
#         set(PAX_SAPIENTICA_LIBRARY_PATCHLEVEL ${CMAKE_MATCH_1})
#         set(PAX_SAPIENTICA_LIBRARY_VERSION ${PAX_SAPIENTICA_LIBRARY_MAJOR}.${PAX_SAPIENTICA_LIBRARY_MINOR}.${PAX_SAPIENTICA_LIBRARY_PATCHLEVEL})

#         set(CPACK_GENERATOR "DEB")
#         set(CPACK_PACKAGE_NAME "PAX_SAPIENTICA")
#         set(CPACK_PACKAGE_VERSION ${PAX_SAPIENTICA_LIBRARY_VERSION})
#         set(CPACK_DEBIAN_PACKAGE_MAINTAINER "PAX_SAPIENTICA")
#         set(CPACK_DEBIAN_PACKAGE_DEPENDS "libsfml-dev")

#         include(CPack)
#     endif()
# endif()
