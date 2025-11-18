/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com		https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#include <gtest/gtest.h>
#include <PAX_SAPIENTICA/Core/Type/Rect.hpp>

namespace paxs {

    // Basic construction tests
    TEST(RectTest, DefaultConstructor) {
        constexpr Rect<int> rect;
        EXPECT_EQ(rect.x(), 0);
        EXPECT_EQ(rect.y(), 0);
        EXPECT_EQ(rect.width(), 0);
        EXPECT_EQ(rect.height(), 0);
    }

    TEST(RectTest, ConstructorWithValues) {
        constexpr Rect<int> rect(10, 20, 30, 40);
        EXPECT_EQ(rect.x(), 10);
        EXPECT_EQ(rect.y(), 20);
        EXPECT_EQ(rect.width(), 30);
        EXPECT_EQ(rect.height(), 40);
    }

    TEST(RectTest, ConstructorWithVectors) {
        constexpr Vector2<int> pos(10, 20);
        constexpr Vector2<int> size(30, 40);
        constexpr Rect<int> rect(pos, size);
        EXPECT_EQ(rect.x(), 10);
        EXPECT_EQ(rect.y(), 20);
        EXPECT_EQ(rect.width(), 30);
        EXPECT_EQ(rect.height(), 40);
    }

    // Computed properties tests
    TEST(RectTest, EdgeProperties) {
        constexpr Rect<int> rect(10, 20, 30, 40);
        EXPECT_EQ(rect.left(), 10);
        EXPECT_EQ(rect.top(), 20);
        EXPECT_EQ(rect.right(), 40);   // 10 + 30
        EXPECT_EQ(rect.bottom(), 60);  // 20 + 40
    }

    TEST(RectTest, CornerProperties) {
        constexpr Rect<int> rect(10, 20, 30, 40);
        EXPECT_EQ(rect.topLeft().x, 10);
        EXPECT_EQ(rect.topLeft().y, 20);
        EXPECT_EQ(rect.topRight().x, 40);
        EXPECT_EQ(rect.topRight().y, 20);
        EXPECT_EQ(rect.bottomLeft().x, 10);
        EXPECT_EQ(rect.bottomLeft().y, 60);
        EXPECT_EQ(rect.bottomRight().x, 40);
        EXPECT_EQ(rect.bottomRight().y, 60);
    }

    TEST(RectTest, CenterProperty) {
        constexpr Rect<int> rect(10, 20, 30, 40);
        constexpr Vector2<int> center = rect.center();
        EXPECT_EQ(center.x, 25);  // 10 + 30/2
        EXPECT_EQ(center.y, 40);  // 20 + 40/2
    }

    // Contains tests
    TEST(RectTest, ContainsPointInside) {
        constexpr Rect<int> rect(10, 20, 30, 40);
        constexpr Vector2<int> point(25, 40);
        EXPECT_TRUE(rect.contains(point));
    }

    TEST(RectTest, ContainsPointOutside) {
        constexpr Rect<int> rect(10, 20, 30, 40);
        constexpr Vector2<int> point(5, 10);
        EXPECT_FALSE(rect.contains(point));
    }

    TEST(RectTest, ContainsPointOnEdge) {
        constexpr Rect<int> rect(10, 20, 30, 40);
        EXPECT_TRUE(rect.contains(10, 20));  // top-left corner
        EXPECT_TRUE(rect.contains(40, 60));  // bottom-right corner
    }

    TEST(RectTest, ContainsWithCoordinates) {
        constexpr Rect<int> rect(10, 20, 30, 40);
        EXPECT_TRUE(rect.contains(25, 40));
        EXPECT_FALSE(rect.contains(5, 10));
    }

    // Intersects tests
    TEST(RectTest, IntersectsOverlapping) {
        constexpr Rect<int> rect1(10, 10, 30, 30);
        constexpr Rect<int> rect2(20, 20, 30, 30);
        EXPECT_TRUE(rect1.intersects(rect2));
        EXPECT_TRUE(rect2.intersects(rect1));
    }

    TEST(RectTest, IntersectsSeparate) {
        constexpr Rect<int> rect1(10, 10, 20, 20);
        constexpr Rect<int> rect2(50, 50, 20, 20);
        EXPECT_FALSE(rect1.intersects(rect2));
        EXPECT_FALSE(rect2.intersects(rect1));
    }

    TEST(RectTest, IntersectsTouching) {
        constexpr Rect<int> rect1(10, 10, 20, 20);
        constexpr Rect<int> rect2(30, 10, 20, 20);
        // Touching edges should not intersect (right of rect1 == left of rect2)
        EXPECT_FALSE(rect1.intersects(rect2));
    }

    TEST(RectTest, IntersectsContained) {
        constexpr Rect<int> rect1(10, 10, 50, 50);
        constexpr Rect<int> rect2(20, 20, 10, 10);
        EXPECT_TRUE(rect1.intersects(rect2));
        EXPECT_TRUE(rect2.intersects(rect1));
    }

    // Expanded tests
    TEST(RectTest, ExpandedPositive) {
        constexpr Rect<int> rect(10, 20, 30, 40);
        constexpr Rect<int> expanded = rect.expanded(5);
        EXPECT_EQ(expanded.x(), 5);   // 10 - 5
        EXPECT_EQ(expanded.y(), 15);  // 20 - 5
        EXPECT_EQ(expanded.width(), 40);   // 30 + 5*2
        EXPECT_EQ(expanded.height(), 50);  // 40 + 5*2
    }

    TEST(RectTest, ExpandedNegative) {
        constexpr Rect<int> rect(10, 20, 30, 40);
        constexpr Rect<int> shrunk = rect.expanded(-5);
        EXPECT_EQ(shrunk.x(), 15);  // 10 - (-5)
        EXPECT_EQ(shrunk.y(), 25);  // 20 - (-5)
        EXPECT_EQ(shrunk.width(), 20);   // 30 + (-5)*2
        EXPECT_EQ(shrunk.height(), 30);  // 40 + (-5)*2
    }

    // FromCenter tests
    TEST(RectTest, FromCenter) {
        constexpr Vector2<int> center(25, 40);
        constexpr Vector2<int> size(30, 40);
        constexpr Rect<int> rect = Rect<int>::fromCenter(center, size);
        EXPECT_EQ(rect.x(), 10);   // 25 - 30/2
        EXPECT_EQ(rect.y(), 20);   // 40 - 40/2
        EXPECT_EQ(rect.width(), 30);
        EXPECT_EQ(rect.height(), 40);
        EXPECT_EQ(rect.center().x, 25);
        EXPECT_EQ(rect.center().y, 40);
    }

    // FromPoints tests
    TEST(RectTest, FromPoints) {
        constexpr Vector2<int> p1(10, 20);
        constexpr Vector2<int> p2(40, 60);
        constexpr Rect<int> rect = Rect<int>::fromPoints(p1, p2);
        EXPECT_EQ(rect.x(), 10);
        EXPECT_EQ(rect.y(), 20);
        EXPECT_EQ(rect.width(), 30);
        EXPECT_EQ(rect.height(), 40);
    }

    TEST(RectTest, FromPointsReversed) {
        constexpr Vector2<int> p1(40, 60);
        constexpr Vector2<int> p2(10, 20);
        constexpr Rect<int> rect = Rect<int>::fromPoints(p1, p2);
        EXPECT_EQ(rect.x(), 10);
        EXPECT_EQ(rect.y(), 20);
        EXPECT_EQ(rect.width(), 30);
        EXPECT_EQ(rect.height(), 40);
    }

    // Equality tests
    TEST(RectTest, Equality) {
        constexpr Rect<int> rect1(10, 20, 30, 40);
        constexpr Rect<int> rect2(10, 20, 30, 40);
        EXPECT_TRUE(rect1 == rect2);
        EXPECT_FALSE(rect1 != rect2);
    }

    TEST(RectTest, Inequality) {
        constexpr Rect<int> rect1(10, 20, 30, 40);
        constexpr Rect<int> rect2(10, 20, 30, 50);
        EXPECT_FALSE(rect1 == rect2);
        EXPECT_TRUE(rect1 != rect2);
    }

    // Type alias tests
    TEST(RectTest, FloatType) {
        constexpr Rect<float> rect(10.5f, 20.5f, 30.5f, 40.5f);
        EXPECT_FLOAT_EQ(rect.x(), 10.5f);
        EXPECT_FLOAT_EQ(rect.y(), 20.5f);
        EXPECT_FLOAT_EQ(rect.width(), 30.5f);
        EXPECT_FLOAT_EQ(rect.height(), 40.5f);
    }

    TEST(RectTest, DoubleType) {
        constexpr Rect<double> rect(10.5, 20.5, 30.5, 40.5);
        EXPECT_DOUBLE_EQ(rect.x(), 10.5);
        EXPECT_DOUBLE_EQ(rect.y(), 20.5);
        EXPECT_DOUBLE_EQ(rect.width(), 30.5);
        EXPECT_DOUBLE_EQ(rect.height(), 40.5);
    }

    // Constexpr tests
    TEST(RectTest, ConstexprEvaluation) {
        constexpr Rect<int> rect(10, 20, 30, 40);
        constexpr Vector2<int> center = rect.center();
        constexpr bool contains = rect.contains(25, 40);
        constexpr Rect<int> expanded = rect.expanded(5);

        EXPECT_EQ(center.x, 25);
        EXPECT_TRUE(contains);
        EXPECT_EQ(expanded.width(), 40);
    }

} // namespace paxs
