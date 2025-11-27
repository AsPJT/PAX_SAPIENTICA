/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#include <gtest/gtest.h>

#include <PAX_SAPIENTICA/Utility/MurMur3.hpp>

TEST(MurMur3UnitTest, aiueo) {
    std::uint_least32_t expected = 3387496686u;
    std::uint_least32_t actual = paxs::MurMur3::calcHash("aiueo");
    ASSERT_EQ(expected, actual);
    std::uint_least32_t actual2 = paxs::MurMur3::calcHash(5, "aiueo");
    ASSERT_EQ(expected, actual2);
}

TEST(MurMur3UnitTest, abcdefghijklmnopqrstuvwxyz) {
    std::uint_least32_t expected = 2739798893u;
    std::uint_least32_t actual = paxs::MurMur3::calcHash("abcdefghijklmnopqrstuvwxyz");
    ASSERT_EQ(expected, actual);
    std::uint_least32_t actual2 = paxs::MurMur3::calcHash(26, "abcdefghijklmnopqrstuvwxyz");
    ASSERT_EQ(expected, actual2);
}
