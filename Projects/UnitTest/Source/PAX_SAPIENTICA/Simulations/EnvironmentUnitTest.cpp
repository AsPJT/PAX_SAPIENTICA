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

using Vector2 = paxs::Vector2<paxs::GridType>;

class EnvironmentUnitTest : public ::testing::Test {
protected:
    paxs::Environment environment;

    void SetUp() override {
        if (CHANGE_DIR(PROJECT_ROOT_PATH) != 0) {
            std::cerr << "Failed to change directory to project root." << std::endl;
            exit(1);
        }

        const std::string root = PROJECT_ROOT_PATH;
        const std::string setting_file_path = root + "/Projects/UnitTest/Data/Simulations/UnitTestMapList.tsv";

        environment = paxs::Environment(setting_file_path);
    }
};

TEST_F (EnvironmentUnitTest, getData) {
    const paxs::Vector2 test_position(0, 0);
    std::uint_least8_t expected = 0;
    std::uint_least8_t actual = environment.getData<std::uint_least8_t>(paxs::MurMur3::calcHash("district"), test_position);
    EXPECT_EQ(expected, actual);
}

TEST_F (EnvironmentUnitTest, isLive) {
    const paxs::Vector2 test_position(0, 0);
    EXPECT_FALSE(environment.isLive(test_position));
}

TEST_F (EnvironmentUnitTest, getSlope) {
    const paxs::Vector2 test_position(0, 0);
    std::uint8_t expected = 0;
    std::uint8_t actual = environment.getSlope(test_position);
    EXPECT_EQ(expected, actual);
}

TEST_F (EnvironmentUnitTest, getElevation) {
    const paxs::Vector2 test_position(0, 0);
    std::int_least16_t expected = 0;
    std::int_least16_t actual = environment.getElevation(test_position);
    EXPECT_EQ(expected, actual);
}

TEST_F (EnvironmentUnitTest, isLand) {
    const paxs::Vector2 test_position(0, 0);
    EXPECT_FALSE(environment.isLand(test_position));
}

TEST_F (EnvironmentUnitTest, isWater) {
    const paxs::Vector2 test_position(0, 0);
    EXPECT_FALSE(environment.isWater(test_position));
}
