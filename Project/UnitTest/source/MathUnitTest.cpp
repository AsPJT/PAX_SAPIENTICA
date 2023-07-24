/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023 As Project
    [Production]	2023 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#include <gtest/gtest.h>

#include <PAX_SAPIENTICA/Math.hpp>

TEST (MathUnitTest, pi) {
    double expected = 3.1415926535897932384626433832795028841971;
    double actual = paxs::MathF64::pi();
    ASSERT_EQ(expected, actual);
}

TEST (MathUnitTest, pi2) {
    double expected = 6.2831853071795864769252867665590057683943;
    double actual = paxs::MathF64::pi2();
    ASSERT_EQ(expected, actual);
}

TEST (MathUnitTest, pi3) {
    double expected = 9.4247779607693797153879301492785086525914;
    double actual = paxs::MathF64::pi3();
    ASSERT_EQ(expected, actual);
}

TEST (MathUnitTest, pi4) {
    double expected = 12.5663706143591729538505735328380115367886;
    double actual = paxs::MathF64::pi4();
    ASSERT_EQ(expected, actual);
}

TEST (MathUnitTest, piHalf) {
    double expected = 1.5707963267948966192313216916397514420986;
    double actual = paxs::MathF64::piHalf();
    ASSERT_EQ(expected, actual);
}

TEST (MathUnitTest, radToDeg) {
    double expected = 57.2957795130823208767981548141051703324055;
    double actual = paxs::MathF64::radToDeg();
    ASSERT_EQ(expected, actual);
    double expected2 = 28.6478897565411604383990774070525851662028;
    double actual2 = paxs::MathF64::radToDeg(0.5);
    ASSERT_EQ(expected2, actual2);
}

TEST (MathUnitTest, degToRad) {
    double expected = 0.0174532925199432957692369076848861271344;
    double actual = paxs::MathF64::degToRad();
    ASSERT_EQ(expected, actual);
    double expected2 = 0.0087266462599716478846184538424430635672;
    double actual2 = paxs::MathF64::degToRad(0.5);
    ASSERT_EQ(expected2, actual2);
}

