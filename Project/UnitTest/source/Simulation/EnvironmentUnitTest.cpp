/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#include <iostream>

#if defined(_WIN32) || defined(_WIN64)
    #include <direct.h>
    #define CHANGE_DIR _chdir
#else
    #include <unistd.h>
    #define CHANGE_DIR chdir
#endif

#include <gtest/gtest.h>

#include <PAX_SAPIENTICA/MurMur3.hpp>
#include <PAX_SAPIENTICA/Simulation/Agent.hpp>
#include <PAX_SAPIENTICA/Simulation/Environment.hpp>

class EnvironmentUnitTest : public ::testing::Test {
protected:
    paxs::Environment<int> environment;

    void SetUp() override {
        if (CHANGE_DIR(PROJECT_ROOT_PATH) != 0) {
            std::cerr << "Failed to change directory to project root." << std::endl;
            exit(1);
        }

        const std::string root = PROJECT_ROOT_PATH;
        const std::string setting_file_path = root + "/Project/UnitTest/data/Simulation/UnitTestMapList.tsv";

        const paxs::Vector2<int> start_position(850, 340);
        const paxs::Vector2<int> end_position(950, 450);

        environment = paxs::Environment<int>(setting_file_path, start_position, end_position, 10);
    }
};

TEST_F (EnvironmentUnitTest, getStartPosition) {
    const paxs::Vector2<int> start_position(850, 340);
    EXPECT_EQ(environment.getStartPosition(), start_position);
}

TEST_F (EnvironmentUnitTest, getEndPosition) {
    const paxs::Vector2<int> end_position(950, 450);
    EXPECT_EQ(environment.getEndPosition(), end_position);
}

TEST_F (EnvironmentUnitTest, getData) {
    const paxs::Vector2<int> test_position(0, 0);
    std::uint_least8_t expected = 0;
    std::uint_least8_t actual = environment.getData<std::uint_least8_t>(paxs::MurMur3::calcHash("gbank"), test_position);
    EXPECT_EQ(expected, actual);
}

TEST_F (EnvironmentUnitTest, isLive) {
    const paxs::Vector2<int> test_position(0, 0);
    EXPECT_FALSE(environment.isLive(test_position));
}

TEST_F (EnvironmentUnitTest, getSlope) {
    const paxs::Vector2<int> test_position(0, 0);
    std::uint8_t expected = 0;
    std::uint8_t actual = environment.getSlope(test_position);
    EXPECT_EQ(expected, actual);
}

TEST_F (EnvironmentUnitTest, getElevation) {
    const paxs::Vector2<int> test_position(0, 0);
    std::int_least16_t expected = 0;
    std::int_least16_t actual = environment.getElevation(test_position);
    EXPECT_EQ(expected, actual);
}

TEST_F (EnvironmentUnitTest, isLand) {
    const paxs::Vector2<int> test_position(0, 0);
    EXPECT_FALSE(environment.isLand(test_position));
}

TEST_F (EnvironmentUnitTest, isWater) {
    const paxs::Vector2<int> test_position(0, 0);
    EXPECT_FALSE(environment.isWater(test_position));
}
