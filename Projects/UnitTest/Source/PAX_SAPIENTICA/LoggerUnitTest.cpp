/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#include <regex>
#include <string>

#include <gtest/gtest.h>

#include <PAX_SAPIENTICA/IO/File/FileSystem.hpp>
#include <PAX_SAPIENTICA/Utility/Logger.hpp>

TEST(LoggerUnitTest, currentDateTime) {
    const std::regex  e("\\d{4}-\\d{2}-\\d{2} \\d{2}:\\d{2}:\\d{2}");
    const std::string actual = paxs::Logger::currentDateTime();

    ASSERT_TRUE(std::regex_match(actual, e));
}
