find_package(SFML 2.5 COMPONENTS graphics REQUIRED)

add_executable(AgentTest source/AgentTest.cpp)

add_executable(EnvironmentTest source/EnvironmentTest.cpp)

add_executable(SFMLTest source/SFMLTest.cpp)
target_link_libraries(SFMLTest sfml-graphics)

add_executable(SimulationTest source/SimulationTest.cpp)
target_link_libraries(SimulationTest sfml-graphics)