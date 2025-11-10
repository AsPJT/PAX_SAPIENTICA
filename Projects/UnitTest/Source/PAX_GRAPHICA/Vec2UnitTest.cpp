/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#include <gtest/gtest.h>

#include <PAX_GRAPHICA/Vec2.hpp>

// Test Vec2<int> construction
TEST(Vec2UnitTest, IntConstruction) {
    paxg::Vec2i v1;
    ASSERT_EQ(v1.x(), 0);
    ASSERT_EQ(v1.y(), 0);

    paxg::Vec2i v2(10, 20);
    ASSERT_EQ(v2.x(), 10);
    ASSERT_EQ(v2.y(), 20);
}

// Test Vec2<float> construction
TEST(Vec2UnitTest, FloatConstruction) {
    paxg::Vec2f v1;
    ASSERT_FLOAT_EQ(v1.x(), 0.0f);
    ASSERT_FLOAT_EQ(v1.y(), 0.0f);

    paxg::Vec2f v2(3.14f, 2.71f);
    ASSERT_FLOAT_EQ(v2.x(), 3.14f);
    ASSERT_FLOAT_EQ(v2.y(), 2.71f);
}

// Test addition operator
TEST(Vec2UnitTest, AdditionOperator) {
    paxg::Vec2i v1(10, 20);
    paxg::Vec2i v2(5, 8);
    paxg::Vec2i result = v1 + v2;
    ASSERT_EQ(result.x(), 15);
    ASSERT_EQ(result.y(), 28);

    paxg::Vec2f vf1(1.5f, 2.5f);
    paxg::Vec2f vf2(0.5f, 1.0f);
    paxg::Vec2f resultf = vf1 + vf2;
    ASSERT_FLOAT_EQ(resultf.x(), 2.0f);
    ASSERT_FLOAT_EQ(resultf.y(), 3.5f);
}

// Test subtraction operator
TEST(Vec2UnitTest, SubtractionOperator) {
    paxg::Vec2i v1(10, 20);
    paxg::Vec2i v2(3, 5);
    paxg::Vec2i result = v1 - v2;
    ASSERT_EQ(result.x(), 7);
    ASSERT_EQ(result.y(), 15);

    paxg::Vec2f vf1(5.0f, 3.0f);
    paxg::Vec2f vf2(2.0f, 1.5f);
    paxg::Vec2f resultf = vf1 - vf2;
    ASSERT_FLOAT_EQ(resultf.x(), 3.0f);
    ASSERT_FLOAT_EQ(resultf.y(), 1.5f);
}

// Test += operator
TEST(Vec2UnitTest, AdditionAssignmentOperator) {
    paxg::Vec2i v(10, 20);
    v += paxg::Vec2i(5, 3);
    ASSERT_EQ(v.x(), 15);
    ASSERT_EQ(v.y(), 23);

    paxg::Vec2f vf(1.0f, 2.0f);
    vf += paxg::Vec2f(0.5f, 1.5f);
    ASSERT_FLOAT_EQ(vf.x(), 1.5f);
    ASSERT_FLOAT_EQ(vf.y(), 3.5f);
}

// Test -= operator
TEST(Vec2UnitTest, SubtractionAssignmentOperator) {
    paxg::Vec2i v(10, 20);
    v -= paxg::Vec2i(3, 8);
    ASSERT_EQ(v.x(), 7);
    ASSERT_EQ(v.y(), 12);

    paxg::Vec2f vf(5.0f, 3.0f);
    vf -= paxg::Vec2f(1.0f, 0.5f);
    ASSERT_FLOAT_EQ(vf.x(), 4.0f);
    ASSERT_FLOAT_EQ(vf.y(), 2.5f);
}

// Test equality operator
TEST(Vec2UnitTest, EqualityOperator) {
    paxg::Vec2i v1(10, 20);
    paxg::Vec2i v2(10, 20);
    paxg::Vec2i v3(11, 20);
    ASSERT_TRUE(v1 == v2);
    ASSERT_FALSE(v1 == v3);

    paxg::Vec2f vf1(1.0f, 2.0f);
    paxg::Vec2f vf2(1.0f, 2.0f);
    paxg::Vec2f vf3(1.0f, 2.1f);
    ASSERT_TRUE(vf1 == vf2);
    ASSERT_FALSE(vf1 == vf3);
}

// Test inequality operator
TEST(Vec2UnitTest, InequalityOperator) {
    paxg::Vec2i v1(10, 20);
    paxg::Vec2i v2(11, 20);
    paxg::Vec2i v3(10, 20);
    ASSERT_TRUE(v1 != v2);
    ASSERT_FALSE(v1 != v3);

    paxg::Vec2f vf1(1.0f, 2.0f);
    paxg::Vec2f vf2(1.1f, 2.0f);
    paxg::Vec2f vf3(1.0f, 2.0f);
    ASSERT_TRUE(vf1 != vf2);
    ASSERT_FALSE(vf1 != vf3);
}

// Test template instantiation
TEST(Vec2UnitTest, TypeAliases) {
    // Ensure Vec2i and Vec2f are properly instantiated
    paxg::Vec2i vi(1, 2);
    paxg::Vec2f vf(1.0f, 2.0f);

    ASSERT_EQ(sizeof(vi), sizeof(paxg::Vec2<int>));
    ASSERT_EQ(sizeof(vf), sizeof(paxg::Vec2<float>));
}

// Test zero vectors
TEST(Vec2UnitTest, ZeroVectors) {
    paxg::Vec2i vi;
    ASSERT_EQ(vi.x(), 0);
    ASSERT_EQ(vi.y(), 0);

    paxg::Vec2f vf;
    ASSERT_FLOAT_EQ(vf.x(), 0.0f);
    ASSERT_FLOAT_EQ(vf.y(), 0.0f);
}

// Test chain operations
TEST(Vec2UnitTest, ChainOperations) {
    paxg::Vec2i v(10, 20);
    v += paxg::Vec2i(5, 5);
    v -= paxg::Vec2i(3, 2);
    ASSERT_EQ(v.x(), 12);
    ASSERT_EQ(v.y(), 23);
}
