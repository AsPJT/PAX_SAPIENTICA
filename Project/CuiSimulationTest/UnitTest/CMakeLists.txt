add_executable(AgentTest AgentTest.cpp)
add_executable(SFMLTest SFMLTest.cpp)
find_package(SFML 2.5 COMPONENTS graphics REQUIRED)
target_link_libraries(SFMLTest sfml-graphics)