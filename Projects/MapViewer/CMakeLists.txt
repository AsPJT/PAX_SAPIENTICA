cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Library
    ${CMAKE_CURRENT_SOURCE_DIR}/../../ExternalLibrary
)

find_package(SFML 2.5 COMPONENTS graphics)
if(SFML_FOUND)
    add_executable(SFMLMapviewer Windows/SFML/Main.cpp)
    target_link_libraries(SFMLMapviewer sfml-graphics)
endif()
