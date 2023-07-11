/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023 As Project
    [Production]	2023 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#include <gtest/gtest.h>

#include <PAX_SAPIENTICA/Simulation/Agent.hpp>
#include <PAX_SAPIENTICA/Simulation/Environment.hpp>

const std::string setting_file_path = "../data/Simulation/MapList.tsv";
paxs::Vector2<int> start_position(861, 350);
paxs::Vector2<int> end_position(950, 450);
paxs::Environment<int> environment(setting_file_path, start_position, end_position, 10);

TEST (EnvironmentUnitTest, getStartPosition) {
    EXPECT_EQ(environment.getStartPosition(), start_position);
}

TEST (EnvironmentUnitTest, getEndPosition) {
    EXPECT_EQ(environment.getEndPosition(), end_position);
}

TEST (EnvironmentUnitTest, getData) {
    std::uint_least8_t expected = 0;
    std::uint_least8_t actual = environment.getData<std::uint_least8_t>("gbank", start_position);
    EXPECT_EQ(expected, actual);
}

TEST (EnvironmentUnitTest, isLive) {
    EXPECT_FALSE(environment.isLive(start_position));
}

TEST (EnvironmentUnitTest, getSlope) {
    float expected = 0.0f;
    float actual = environment.getSlope(start_position);
    std::cout << "slope: " << actual << std::endl;
    EXPECT_EQ(expected, actual);
}

TEST (EnvironmentUnitTest, getElevation) {
    float expected = 0.0f;
    float actual = environment.getElevation(start_position);
    std::cout << "elevation: " << actual << std::endl;
    EXPECT_EQ(expected, actual);
}

TEST (EnvironmentUnitTest, isLand) {
    EXPECT_FALSE(environment.isLand(start_position));
}
