/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023 As Project
    [Production]	2023 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#include <gtest/gtest.h>

#include <PAX_SAPIENTICA/GeographicInformation/ConvertToInt.hpp>

TEST(ConvertToIntUnitTest, slope1) {
    unsigned char expected = 90u;
    unsigned char actual = paxs::slopeDegF64ToLog2U8(4.0);
    ASSERT_EQ(expected, actual);
}

TEST(ConvertToIntUnitTest, slope2) {
    unsigned char expected = 100u;
    unsigned char actual = paxs::slopeDegF64ToLog2U8(5.0);
    ASSERT_EQ(expected, actual);
}

TEST(ConvertToIntUnitTest, elevation1) {
    std::int_least16_t expected = -10000;
    std::int_least16_t actual = paxs::elevationF64ToLog2S16(-16.125);
    ASSERT_EQ(expected, actual);
}

TEST(ConvertToIntUnitTest, elevation2) {
    std::int_least16_t expected = 5000;
    std::int_least16_t actual = paxs::elevationF64ToLog2S16(3.138);
    ASSERT_EQ(expected, actual);
}

TEST(ConvertToIntUnitTest, slope3) {
    unsigned char expected = 100u;
    unsigned char actual = paxs::slopeDegF64ToLog2U8(paxs::slopeDegLog2U8ToF64(100));
    ASSERT_EQ(expected, actual);
}

TEST(ConvertToIntUnitTest, elevation3) {
    std::int_least16_t expected = -20000;
    std::int_least16_t actual = paxs::elevationF64ToLog2S16(paxs::elevationLog2S16ToF64(-20000));
    ASSERT_EQ(expected, actual);
}
