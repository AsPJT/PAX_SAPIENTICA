/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]      2023-2025 As Project
    [Production]    2023-2025 As Project
    [Contact Us]    wanotaitei@gmail.com         https://github.com/AsPJT/PAX_SAPIENTICA
    [License]       Distributed under the CC0 1.0. https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#include <gtest/gtest.h>

#include <PAX_GRAPHICA/Triangle.hpp>

#include <PAX_SAPIENTICA/Core/Math/Math.hpp>

// Test default constructor
TEST(TriangleUnitTest, DefaultConstruction) {
    paxg::Triangle triangle;
    EXPECT_FLOAT_EQ(triangle.centerX(), 0.0f);
    EXPECT_FLOAT_EQ(triangle.centerY(), 0.0f);
    EXPECT_FLOAT_EQ(triangle.radius(), 0.0f);
    EXPECT_FLOAT_EQ(triangle.rotation(), 0.0f);
}

// Test constructor with parameters
TEST(TriangleUnitTest, ParameterizedConstruction) {
    paxg::Triangle triangle(100.0f, 200.0f, 50.0f, paxs::MathConstants<float>::pi());
    EXPECT_FLOAT_EQ(triangle.centerX(), 100.0f);
    EXPECT_FLOAT_EQ(triangle.centerY(), 200.0f);
    EXPECT_FLOAT_EQ(triangle.radius(), 50.0f);
    EXPECT_FLOAT_EQ(triangle.rotation(), paxs::MathConstants<float>::pi());
}

// Test Vec2f constructor
TEST(TriangleUnitTest, Vec2fConstruction) {
    paxs::Vector2<float> center(150.0f, 250.0f);
    paxg::Triangle triangle(center, 30.0f, 1.5708f);
    EXPECT_FLOAT_EQ(triangle.centerX(), 150.0f);
    EXPECT_FLOAT_EQ(triangle.centerY(), 250.0f);
    EXPECT_FLOAT_EQ(triangle.radius(), 30.0f);
    EXPECT_FLOAT_EQ(triangle.rotation(), 1.5708f);
}

// Test setters
TEST(TriangleUnitTest, Setters) {
    paxg::Triangle triangle;

    triangle.setCenterX(50.0f);
    EXPECT_FLOAT_EQ(triangle.centerX(), 50.0f);

    triangle.setCenterY(75.0f);
    EXPECT_FLOAT_EQ(triangle.centerY(), 75.0f);

    triangle.setCenter(100.0f, 150.0f);
    EXPECT_FLOAT_EQ(triangle.centerX(), 100.0f);
    EXPECT_FLOAT_EQ(triangle.centerY(), 150.0f);

    paxs::Vector2<float> newCenter(200.0f, 300.0f);
    triangle.setCenter(newCenter);
    EXPECT_FLOAT_EQ(triangle.centerX(), 200.0f);
    EXPECT_FLOAT_EQ(triangle.centerY(), 300.0f);

    triangle.setRadius(40.0f);
    EXPECT_FLOAT_EQ(triangle.radius(), 40.0f);

    triangle.setRotation(2.5f);
    EXPECT_FLOAT_EQ(triangle.rotation(), 2.5f);
}

// Test rotation values for different directions
TEST(TriangleUnitTest, RotationDirections) {
    const float pi = paxs::MathConstants<float>::pi();

    // Right-pointing triangle (0 radians)
    paxg::Triangle rightTriangle(100.0f, 100.0f, 20.0f, 0.0f);
    EXPECT_FLOAT_EQ(rightTriangle.rotation(), 0.0f);

    // Down-pointing triangle (π radians)
    paxg::Triangle downTriangle(100.0f, 100.0f, 20.0f, pi);
    EXPECT_NEAR(downTriangle.rotation(), pi, 0.0001f);

    // Left-pointing triangle (π radians)
    paxg::Triangle leftTriangle(100.0f, 100.0f, 20.0f, pi);
    EXPECT_NEAR(leftTriangle.rotation(), pi, 0.0001f);

    // Up-pointing triangle (3π/2 radians)
    paxg::Triangle upTriangle(100.0f, 100.0f, 20.0f, 3.0f * pi / 2.0f);
    EXPECT_NEAR(upTriangle.rotation(), 3.0f * pi / 2.0f, 0.0001f);
}

// Test constexpr construction
TEST(TriangleUnitTest, ConstexprConstruction) {
    constexpr paxg::Triangle triangle(10.0f, 20.0f, 5.0f, 1.0f);
    static_assert(triangle.centerX() == 10.0f, "centerX should be 10.0f");
    static_assert(triangle.centerY() == 20.0f, "centerY should be 20.0f");
    static_assert(triangle.radius() == 5.0f, "radius should be 5.0f");
    static_assert(triangle.rotation() == 1.0f, "rotation should be 1.0f");
}
