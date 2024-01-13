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

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Library
    ${CMAKE_CURRENT_SOURCE_DIR}/../../ExternalLibrary
)

set(SOURCE_TSV_FILE "${CMAKE_CURRENT_SOURCE_DIR}/${OS}/Config.tsv")
set(DESTINATION_DIR "${CMAKE_CURRENT_BINARY_DIR}")

set(PAXS_BUILD_TYPE "Development" CACHE STRING "Choose the type of build (Development or Production)")

if(PAXS_BUILD_TYPE STREQUAL "Development")
    find_package(SFML 2.5 COMPONENTS graphics)
    if(SFML_FOUND)
        add_executable(SFMLMapviewer ${OS}/SFML_2.6.1/Main.cpp)
        target_link_libraries(SFMLMapviewer sfml-graphics)
        add_definitions(-DPAXS_DEVELOPMENT)
        add_custom_command(
            TARGET SFMLMapviewer POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_TSV_FILE} ${DESTINATION_DIR}
        )
    endif()
elseif(PAXS_BUILD_TYPE STREQUAL "Production")
    if(APPLE)
        set(SFML_INCLUDE_DIR "${OS}/SFML_2.6.1/include")
        set(SFML_LIBRARY_DIR "${OS}/SFML_2.6.1/lib")

        include_directories(${SFML_INCLUDE_DIR})

        add_executable(SFMLMapviewer ${OS}/SFML_2.6.1/Main.cpp)

        target_link_directories(SFMLMapviewer PRIVATE ${SFML_LIBRARY_DIR})
        target_link_libraries(SFMLMapviewer sfml-graphics.2.6.1 sfml-window.2.6.1 sfml-system.2.6.1)
        add_custom_command(
            TARGET SFMLMapviewer POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_TSV_FILE} ${DESTINATION_DIR}
        )

        set_target_properties(SFMLMapviewer PROPERTIES
            INSTALL_RPATH "@executable_path/../Resources/SFML/lib"
            BUILD_WITH_INSTALL_RPATH TRUE
        )
    elseif(UNIX)
        find_package(SFML 2.5 COMPONENTS graphics)

        add_executable(PAXS ${OS}/SFML/Main.cpp)
        target_link_libraries(PAXS sfml-graphics)

        add_custom_command(
            TARGET PAXS POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${SOURCE_TSV_FILE} ${DESTINATION_DIR}
        )

        install(FILES ${DESTINATION_DIR}/Config.tsv DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
        install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${OS}/Data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

        set(CPACK_GENERATOR "DEB")
        set(CPACK_PACKAGE_NAME "PAX SAPIENTICA")
        set(CPACK_PACKAGE_VERSION "1.0.0")
        set(CPACK_DEBIAN_PACKAGE_MAINTAINER "PAX SAPIENTICA")
        set(CPACK_DEBIAN_PACKAGE_DEPENDS "libsfml-dev")

        include(CPack)
    endif()
endif()
