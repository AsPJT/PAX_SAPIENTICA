/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#include <gtest/gtest.h>

#include <PAX_SAPIENTICA/Core/Math/Math.hpp>

TEST (MathUnitTest, pi) {
    const double expected = 3.1415926535897932384626433832795028841971;
    double actual = paxs::Math<double>::pi();
    ASSERT_EQ(expected, actual);
}

TEST (MathUnitTest, pi2) {
    const double expected = 6.2831853071795864769252867665590057683943;
    double actual = paxs::Math<double>::pi2();
    ASSERT_EQ(expected, actual);
}

TEST (MathUnitTest, piHalf) {
    const double expected = 1.5707963267948966192313216916397514420986;
    double actual = paxs::Math<double>::piHalf();
    ASSERT_EQ(expected, actual);
}

TEST (MathUnitTest, radToDeg) {
    const double expected = 57.2957795130823208767981548141051703324055;
    double actual = paxs::Math<double>::radToDeg();
    ASSERT_EQ(expected, actual);
    const double expected2 = 28.6478897565411604383990774070525851662028;
    double actual2 = paxs::Math<double>::radToDeg(0.5);
    ASSERT_EQ(expected2, actual2);
}

TEST (MathUnitTest, degToRad) {
    const double expected = 0.0174532925199432957692369076848861271344;
    double actual = paxs::Math<double>::degToRad();
    ASSERT_EQ(expected, actual);
    const double expected2 = 0.0087266462599716478846184538424430635672;
    double actual2 = paxs::Math<double>::degToRad(0.5);
    ASSERT_EQ(expected2, actual2);
}

