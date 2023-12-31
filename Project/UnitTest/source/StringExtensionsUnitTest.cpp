/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#include <gtest/gtest.h>

#include <PAX_SAPIENTICA/StringExtensions.hpp>

TEST (StringExtensionsUnitTest, split) {
    const std::string str = "Test split";
    const std::vector<std::string> expected = {
        "Test",
        "split"
    };
    const std::vector<std::string> actual = paxs::StringExtensions::split(str, ' ');
    ASSERT_EQ(expected, actual);
}

TEST (StringExtensionsUnitTest, replace) {
    std::string str = "Test replace";
    const std::string expected = "Test_replace";
    paxs::StringExtensions::replace(str, " ", "_");
    ASSERT_EQ(expected, str);
}

TEST (StringExtensionsUnitTest, replaceList) {
    std::string str = "Test replace";
    const std::string expected = "Test_replaced";
    const std::vector<std::string> fromList = {
        " ",
        "replace"
    };
    const std::vector<std::string> toList = {
        "_",
        "replaced"
    };
    paxs::StringExtensions::replaceList(str, fromList, toList);
    ASSERT_EQ(expected, str);
}

TEST (StringExtensionsUnitTest, tryToConvertStringToDouble) {
    const std::string str = "1.0";
    const double expected = 1.0;

    auto variant_value = paxs::StringExtensions::tryToConvertStringToDouble(str);

    if (std::holds_alternative<double>(variant_value)) {
        const double actual = std::get<double>(variant_value);
        ASSERT_EQ(expected, actual);
    } else {
        FAIL();
    }
}
