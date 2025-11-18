/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#include <gtest/gtest.h>

#include <PAX_SAPIENTICA/Geography/Coordinate/Projection.hpp>

const paxs::MercatorDeg mercatorDeg({0.0, 0.0});

TEST (MapProjectionUnitTest, toEquirectangularRadY) {
    double expected = 0.0;
    double actual = mercatorDeg.toEquirectangularRadY();
    ASSERT_EQ(expected, actual);
}


