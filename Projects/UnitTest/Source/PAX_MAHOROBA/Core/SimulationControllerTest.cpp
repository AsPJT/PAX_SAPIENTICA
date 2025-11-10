/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]      2023-2024 As Project
    [Production]    2023-2024 As Project
    [Contact Us]    wanotaitei@gmail.com            https://github.com/AsPJT/PAX_SAPIENTICA
    [License]       Distributed under the CC0 1.0.  https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#include <gtest/gtest.h>
#include <PAX_MAHOROBA/Core/EventBus.hpp>
#include <PAX_MAHOROBA/Core/SimulationController.hpp>

namespace paxs {

class SimulationControllerTest : public ::testing::Test {
protected:
    // EventBusはシングルトンなので参照を取得
    EventBus& event_bus_ = EventBus::getInstance();
    SimulationController controller_{event_bus_};
};

/// @brief 自動実行開始のテスト
/// @brief Test starting auto-execution
TEST_F(SimulationControllerTest, StartAutoExecution) {
    controller_.startAutoExecution(5, "TestModel");

    EXPECT_EQ(controller_.getRemainingIterations(), 5);
    EXPECT_TRUE(controller_.isAutoExecuting());
}

/// @brief 自動実行停止のテスト
/// @brief Test stopping auto-execution
TEST_F(SimulationControllerTest, StopAutoExecution) {
    controller_.startAutoExecution(5, "TestModel");
    controller_.stopAutoExecution();

    EXPECT_EQ(controller_.getRemainingIterations(), 0);
    EXPECT_FALSE(controller_.isAutoExecuting());
}

/// @brief 初期状態のテスト
/// @brief Test initial state
TEST_F(SimulationControllerTest, InitialState) {
    EXPECT_EQ(controller_.getRemainingIterations(), 0);
    EXPECT_FALSE(controller_.isAutoExecuting());
}

/// @brief 複数回の開始・停止のテスト
/// @brief Test multiple start-stop cycles
TEST_F(SimulationControllerTest, MultipleStartStopCycles) {
    // 1回目
    controller_.startAutoExecution(3, "Model1");
    EXPECT_EQ(controller_.getRemainingIterations(), 3);
    EXPECT_TRUE(controller_.isAutoExecuting());

    controller_.stopAutoExecution();
    EXPECT_EQ(controller_.getRemainingIterations(), 0);
    EXPECT_FALSE(controller_.isAutoExecuting());

    // 2回目
    controller_.startAutoExecution(7, "Model2");
    EXPECT_EQ(controller_.getRemainingIterations(), 7);
    EXPECT_TRUE(controller_.isAutoExecuting());

    controller_.stopAutoExecution();
    EXPECT_EQ(controller_.getRemainingIterations(), 0);
    EXPECT_FALSE(controller_.isAutoExecuting());
}

/// @brief SimulationStopCommandEventによる停止のテスト
/// @brief Test stopping via SimulationStopCommandEvent
TEST_F(SimulationControllerTest, StopViaEvent) {
    controller_.startAutoExecution(5, "TestModel");
    EXPECT_TRUE(controller_.isAutoExecuting());

    // SimulationStopCommandEventを発行
    event_bus_.publish(SimulationStopCommandEvent());

    // イベント処理（同期処理なので即座に反映される）
    EXPECT_EQ(controller_.getRemainingIterations(), 0);
    EXPECT_FALSE(controller_.isAutoExecuting());
}

/// @brief 異なるiterations値のテスト
/// @brief Test different iteration values
TEST_F(SimulationControllerTest, DifferentIterationValues) {
    // iterations = 1
    controller_.startAutoExecution(1, "TestModel");
    EXPECT_EQ(controller_.getRemainingIterations(), 1);

    controller_.stopAutoExecution();

    // iterations = 100
    controller_.startAutoExecution(100, "TestModel");
    EXPECT_EQ(controller_.getRemainingIterations(), 100);

    controller_.stopAutoExecution();

    // iterations = 0 (エッジケース)
    controller_.startAutoExecution(0, "TestModel");
    EXPECT_EQ(controller_.getRemainingIterations(), 0);
    // is_auto_executing_はtrueになる（updateで即座に停止するはず）
    EXPECT_TRUE(controller_.isAutoExecuting());
}

} // namespace paxs
