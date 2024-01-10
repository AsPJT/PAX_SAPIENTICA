cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Library
    ${CMAKE_CURRENT_SOURCE_DIR}/../../ExternalLibrary
)

find_package(SFML 2.5 COMPONENTS graphics)
if(SFML_FOUND)
    add_executable(SFMLMapviewer CMake/SFML/Main.cpp)
    target_link_libraries(SFMLMapviewer sfml-graphics)

    set(SOURCE_TSV_FILE "${CMAKE_CURRENT_SOURCE_DIR}/CMake/Config.tsv")
    set(DESTINATION_DIR "${CMAKE_CURRENT_BINARY_DIR}")

    set(PAXS_BUILD_TYPE "Development" CACHE STRING "Choose the type of build (Development or Production)")
    # Devビルドの場合
    if(PAXS_BUILD_TYPE STREQUAL "Development")
        add_definitions(-DPAXS_DEVELOPMENT)
    endif()

    add_custom_command(
        TARGET SFMLMapviewer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_TSV_FILE} ${DESTINATION_DIR}
    )
endif()
