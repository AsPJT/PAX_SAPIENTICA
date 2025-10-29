/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#include <gtest/gtest.h>

#include <PAX_SAPIENTICA/TouchStateManager.hpp>

TEST (TouchManagerUnitTest, init) {
    paxs::TouchManager touchManager;
    touchManager.init();
}

TEST (TouchManagerUnitTest, get) {
    paxs::TouchManager touchManager;
    touchManager.init();
    bool expected = true;
    bool actual = touchManager.get(true);
    ASSERT_EQ(expected, actual);
}
