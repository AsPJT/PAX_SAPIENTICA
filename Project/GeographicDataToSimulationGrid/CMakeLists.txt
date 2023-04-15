cmake_minimum_required(VERSION 3.16)
project(GeographicDataToSimulationGrid)

set(CMAKE_CXX_STANDARD 17)
set(EXECUTABLE_OUTPUT_PATH ../Project/GeographicDataToSimulationGrid/bin)
include_directories(../../Library)
include_directories(include)
find_package(OpenCV REQUIRED)

# LandAndSea
add_executable(LandAndSea Main.cpp)
target_compile_definitions(LandAndSea PRIVATE LandAndSea)
target_link_libraries(LandAndSea ${OpenCV_LIBS})

# ImageCombine
add_executable(ImageCombine Main.cpp)
target_compile_definitions(ImageCombine PRIVATE ImageCombine)
target_link_libraries(ImageCombine ${OpenCV_LIBS})

# CreateSampleImages
add_executable(CreateSampleImages Main.cpp)
target_compile_definitions(CreateSampleImages PRIVATE CreateSampleImages)
target_link_libraries(CreateSampleImages ${OpenCV_LIBS})

include_directories(${OpenCV_INCLUDE_DIRS})
