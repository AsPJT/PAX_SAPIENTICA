/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023 As Project
    [Production]	2023 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#include <gtest/gtest.h>

#include <PAX_SAPIENTICA/GeographicInformation/ConvertToInt.hpp>

TEST(ConvertToIntUnitTest, slope) {
    unsigned char expected = 100u;
    unsigned char actual = paxs::slopeF64ToLog2U8(5.07598309582116);
    ASSERT_EQ(expected, actual);
}

TEST(ConvertToIntUnitTest, elevation) {
    std::int_least16_t expected = -10000;
    std::int_least16_t actual = paxs::elevationF64ToLog2S16(-16.1257027576632);
    ASSERT_EQ(expected, actual);
}
