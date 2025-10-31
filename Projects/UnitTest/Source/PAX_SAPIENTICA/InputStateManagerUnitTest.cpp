/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#include <gtest/gtest.h>

#include <PAX_SAPIENTICA/InputStateManager.hpp>

TEST (InputStateManagerUnitTest, init) {
    paxs::InputStateManager inputManager;
    inputManager.init();
}

TEST (InputStateManagerUnitTest, get) {
    paxs::InputStateManager inputManager;
    inputManager.init();
    bool expected = true;
    bool actual = inputManager.get(true);
    ASSERT_EQ(expected, actual);
}

TEST (InputStateManagerUnitTest, preventDuplicateProcessing) {
    paxs::InputStateManager inputManager;
    inputManager.init();

    // 最初の入力はtrueを返す
    ASSERT_TRUE(inputManager.get(true));

    // 同じ入力状態が続く場合はfalseを返す（二重処理防止）
    ASSERT_FALSE(inputManager.get(true));
    ASSERT_FALSE(inputManager.get(true));
}
