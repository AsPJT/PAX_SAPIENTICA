find_package(SFML 2.5 COMPONENTS graphics REQUIRED)

add_executable(EnvironmentTest source/EnvironmentTest.cpp)

add_executable(SFMLTest source/SFMLTest.cpp)
target_link_libraries(SFMLTest sfml-graphics)

add_executable(SimulatorTest source/SimulatorTest.cpp)
target_compile_definitions(SimulatorTest PRIVATE PAX_SAPIENTICA_DEBUG)
