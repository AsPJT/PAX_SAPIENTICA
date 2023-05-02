cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)
set(EXECUTABLE_OUTPUT_PATH ../../Project/GeographicDataToSimulationGrid/bin)
include_directories(../../Library)
include_directories(include)
find_package(OpenCV REQUIRED)

# LandAndSea
add_executable(LandAndSea Main.cpp)
target_compile_definitions(LandAndSea PRIVATE LandAndSea)
target_link_libraries(LandAndSea ${OpenCV_LIBS})

# LandAndWaterConvert
add_executable(LandAndWaterConvert Main.cpp)
target_compile_definitions(LandAndWaterConvert PRIVATE LandAndWaterConvert)
target_link_libraries(LandAndWaterConvert ${OpenCV_LIBS})

# ImageCombine
add_executable(ImageCombine Main.cpp)
target_compile_definitions(ImageCombine PRIVATE ImageCombine)
target_link_libraries(ImageCombine ${OpenCV_LIBS})

# ImageSegment
add_executable(ImageSegment Main.cpp)
target_compile_definitions(ImageSegment PRIVATE ImageSegment)
target_link_libraries(ImageSegment ${OpenCV_LIBS})

# ColorCount
add_executable(ColorCount Main.cpp)
target_compile_definitions(ColorCount PRIVATE ColorCount)
target_link_libraries(ColorCount ${OpenCV_LIBS})

# AreaCalculate
add_executable(AreaCalculate Main.cpp)
target_compile_definitions(AreaCalculate PRIVATE AreaCalculate)
target_link_libraries(AreaCalculate ${OpenCV_LIBS})

# CreateSampleImages
add_executable(CreateSampleImages Main.cpp)
target_compile_definitions(CreateSampleImages PRIVATE CreateSampleImages)
target_link_libraries(CreateSampleImages ${OpenCV_LIBS})

include_directories(${OpenCV_INCLUDE_DIRS})
