/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#include <gtest/gtest.h>

#include <PAX_GRAPHICA/Mouse.hpp>

// Test Mouse singleton instance
TEST(MouseUnitTest, SingletonInstance) {
    paxg::Mouse* mouse1 = paxg::Mouse::getInstance();
    paxg::Mouse* mouse2 = paxg::Mouse::getInstance();

    // Both should point to the same instance
    ASSERT_EQ(mouse1, mouse2);
    ASSERT_NE(mouse1, nullptr);
}

// Test initial mouse state
TEST(MouseUnitTest, InitialState) {
    paxg::Mouse* mouse = paxg::Mouse::getInstance();

    // Initial wheel rotation should be 0
    ASSERT_EQ(mouse->getWheelRotVol(), 0);

    // Initial left button state should be false
    ASSERT_FALSE(mouse->getLeft());
    ASSERT_FALSE(mouse->getLeftBefore1Frame());
}

// Test mouse position getters
TEST(MouseUnitTest, PositionGetters) {
    paxg::Mouse* mouse = paxg::Mouse::getInstance();

    // Position values should be accessible (exact values depend on platform)
    int x = mouse->getPosX();
    int y = mouse->getPosY();
    const paxs::Vector2<int> before = mouse->getPosBefore1Frame();

    // Just verify they return without crashing
    ASSERT_TRUE(x >= -1);  // -1 is initial value
    ASSERT_TRUE(y >= -1);
    ASSERT_TRUE(before.x >= 0);
    ASSERT_TRUE(before.y >= 0);
}

// Test mouse button state methods
TEST(MouseUnitTest, ButtonStateMethods) {
    paxg::Mouse* mouse = paxg::Mouse::getInstance();

    // Test upLeft (released)
    bool upLeft = mouse->upLeft();
    ASSERT_FALSE(upLeft);  // Initially should be false

    // Test downLeft (pressed)
    bool downLeft = mouse->downLeft();
    ASSERT_FALSE(downLeft);  // Initially should be false

    // Test pressedLeft2 (held for 2+ frames)
    bool pressedLeft2 = mouse->pressedLeft2();
    ASSERT_FALSE(pressedLeft2);  // Initially should be false
}

// Test SFML 3.0.0 tryGetPosition with std::optional
TEST(MouseUnitTest, TryGetPosition) {
    paxg::Mouse* mouse = paxg::Mouse::getInstance();

    auto posOpt = mouse->tryGetPosition();

#if defined(PAXS_USING_SIV3D) || defined(PAXS_USING_SFML)
    // For graphics libraries, should return a valid optional
    ASSERT_TRUE(posOpt.has_value());
    if (posOpt) {
        // Position should be accessible
        int x = posOpt->x();
        int y = posOpt->y();
        ASSERT_TRUE(x >= -1);
        ASSERT_TRUE(y >= -1);
    }
#elif defined(PAXS_USING_DXLIB)
    // DxLib may return nullopt if GetMousePoint fails
    // Just verify it doesn't crash
    if (posOpt) {
        ASSERT_TRUE(posOpt->x() >= 0);
        ASSERT_TRUE(posOpt->y() >= 0);
    }
#else
    // Null implementation always returns nullopt
    ASSERT_FALSE(posOpt.has_value());
#endif
}

// Test calledFirstEveryFrame doesn't crash
TEST(MouseUnitTest, CalledFirstEveryFrame) {
    paxg::Mouse* mouse = paxg::Mouse::getInstance();

    // This should not crash
    ASSERT_NO_THROW(mouse->calledFirstEveryFrame());

    // After calling, previous state should be updated
    const paxs::Vector2<int> before = mouse->getPosBefore1Frame();
    bool leftBefore = mouse->getLeftBefore1Frame();

    // Values should be accessible
    ASSERT_TRUE(before.x >= -1);
    ASSERT_TRUE(before.y >= -1);
    ASSERT_FALSE(leftBefore);  // Initially false
}

// Test std::optional usage pattern
TEST(MouseUnitTest, OptionalUsagePattern) {
    paxg::Mouse* mouse = paxg::Mouse::getInstance();

    if (auto pos = mouse->tryGetPosition()) {
        // If we have a position, use it
        ASSERT_TRUE(pos->x >= -1);
        ASSERT_TRUE(pos->y >= -1);
    } else {
        // If we don't have a position, handle gracefully
#if !defined(PAXS_USING_SIV3D) && !defined(PAXS_USING_SFML) && !defined(PAXS_USING_DXLIB)
        // Null implementation should reach here
        SUCCEED();
#endif
    }
}
