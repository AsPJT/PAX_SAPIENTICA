/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023 As Project
    [Production]	2023 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#include <gtest/gtest.h>

#include <PAX_SAPIENTICA/Type/Vector2.hpp>

TEST (Vector2UnitTest, Constructor) {
    const paxs::Vector2<int> v1{};
    ASSERT_EQ(v1.x, 0);
    ASSERT_EQ(v1.y, 0);

    const paxs::Vector2 v2(1, 2);
    ASSERT_EQ(v2.x, 1);
    ASSERT_EQ(v2.y, 2);
}

TEST (Vector2UnitTest, zero) {
    const paxs::Vector2<int> expected(0, 0);
    const paxs::Vector2<int> actual = paxs::Vector2<int>::zero();
    ASSERT_EQ(expected, actual);
}

TEST (Vector2UnitTest, length) {
    const paxs::Vector2<int> v(3, 4);
    ASSERT_EQ(v.length(), 5);
}

TEST (Vector2UnitTest, distance) {
    const paxs::Vector2<int> v1(1, 2);
    const paxs::Vector2<int> v2(4, 6);
    ASSERT_EQ(v1.distance(v2), 5);
}

// 各種演算子のテスト
TEST (Vector2UnitTest, op) {
    const paxs::Vector2<int> v1(1, 2);
    const paxs::Vector2<int> v2(2, 4);
    const paxs::Vector2<int> v3(3, 6);
    ASSERT_EQ(v1 + v2, v3);
    ASSERT_EQ(v3 - v2, v1);
    ASSERT_EQ(v1 * 3, v3);
    ASSERT_EQ(v3 / 3, v1);
}
