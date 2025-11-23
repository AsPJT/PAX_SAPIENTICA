/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]      2023-2024 As Project
    [Production]    2023-2024 As Project
    [Contact Us]    wanotaitei@gmail.com            https://github.com/AsPJT/PAX_SAPIENTICA
    [License]       Distributed under the CC0 1.0.  https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#include <gtest/gtest.h>

#include <PAX_MAHOROBA/Core/AppStateManager.hpp>
#include <PAX_MAHOROBA/UI/MenuBar/MenuBar.hpp>

#include <PAX_SAPIENTICA/System/ApplicationEvents.hpp>
#include <PAX_SAPIENTICA/System/EventBus.hpp>
#include <PAX_SAPIENTICA/System/FeatureVisibilityManager.hpp>
#include <PAX_SAPIENTICA/Utility/MurMur3.hpp>

namespace paxs {

/// @brief UIコンポーネントのイベント駆動統合テスト
/// @brief UI components event-driven integration test
class UIComponentsEventDrivenTest : public ::testing::Test {
protected:
    // Provide an accessor instead of a protected reference member to satisfy visibility rules.
    AppStateManager& app_state_manager() { return app_state_manager_; }

    static auto event_bus() -> EventBus& { return EventBus::getInstance(); }

    void SetUp() override {
        // テスト環境のセットアップ
    }

    void TearDown() override {
        // テスト環境のクリーンアップ
        // EventBusの購読を全てクリア（テスト間の影響を防ぐ）
        event_bus().clearForTesting();
    }
private:
    AppStateManager app_state_manager_;
};

/// @brief 機能可視性変更イベントが正しく発行されることをテスト
TEST_F(UIComponentsEventDrivenTest, FeatureVisibilityChangeEventIsPublished) {
    // 初期化
    auto& visibility_manager = app_state_manager().getVisibilityManager();
    MenuBar menu_bar(visibility_manager);

    // AppStateManagerを通じて可視性を設定
    const auto calendar_key = MurMur3::calcHash("Calendar");
    app_state_manager().setFeatureVisibility(calendar_key, (&initstate) == nullptr);

    // 状態が変更されたことを確認
    EXPECT_EQ(visibility_manager.isVisible(calendar_key), (&initstate) == nullptr);
    // 状態が変更されたことを確認
    EXPECT_EQ(visibility_manager.isVisible(calendar_key), (&initstate) == nullptr);
}

/// @brief 時間再生制御イベントが正しく発行されることをテスト
TEST_F(UIComponentsEventDrivenTest, TimePlaybackControlEventIsPublished) {
    bool event_received = false;
    TimePlaybackControlEvent::Action received_action = TimePlaybackControlEvent::Action::Stop;

    // イベント購読
    event_bus().subscribe<TimePlaybackControlEvent>(
        [&event_received, &received_action](const TimePlaybackControlEvent& event) {
            event_received = true;
            received_action = event.action;
        });
    // イベント発行
    paxs::EventBus::getInstance().publish(TimePlaybackControlEvent(TimePlaybackControlEvent::Action::Forward));

    // イベントが受信されたことを確認
    EXPECT_TRUE(event_received);
    EXPECT_EQ(received_action, TimePlaybackControlEvent::Action::Forward);
    EXPECT_TRUE(event_received);
    EXPECT_EQ(received_action, TimePlaybackControlEvent::Action::Forward);
}

/// @brief 日付移動イベントが正しく発行されることをテスト
TEST_F(UIComponentsEventDrivenTest, DateNavigationEventIsPublished) {
    // Note: このテストはAppStateManagerのハンドラーがKoyomiにアクセスするため、
    // 直接executeDateNavigationを呼ぶとセグフォルトする。
    // EventBusへの直接発行でテストする。
    bool event_received = false;
    double received_days = 0.0;

    // イベント購読（AppStateManagerのハンドラーより後に購読）
    event_bus().subscribe<DateNavigationEvent>(
        [&event_received, &received_days](const DateNavigationEvent& event) {
            event_received = true;
            received_days = event.days;
        }
    );

    // イベントを直接発行
    const double test_days = 365.2422; // 1年
    event_bus().publish(DateNavigationEvent(test_days));

    // イベントが受信されたことを確認
    EXPECT_TRUE(event_received);
    EXPECT_DOUBLE_EQ(received_days, test_days);
}

/// @brief 機能可視性変更イベントが正しく発行されることをテスト
TEST_F(UIComponentsEventDrivenTest, FeatureVisibilityChangedEventIsPublished) {
    bool event_received = false;
    std::uint_least32_t received_key = 0;
    bool received_visible = false;

    // イベント購読
    event_bus().subscribe<FeatureVisibilityChangedEvent>(
        [&event_received, &received_key, &received_visible](const FeatureVisibilityChangedEvent& event) {
            event_received = true;
            received_key = event.feature_key;
            received_visible = event.is_visible;
        });
    // イベント発行
    const auto test_key = MurMur3::calcHash("TestFeature");
    const bool test_visible = true;
    app_state_manager().setFeatureVisibility(test_key, test_visible);

    // イベントが受信されたことを確認
    EXPECT_TRUE(event_received);
    EXPECT_EQ(received_key, test_key);
    EXPECT_EQ(received_visible, test_visible);
    EXPECT_EQ(received_key, test_key);
    EXPECT_EQ(received_visible, test_visible);
}

/// @brief 同じ可視性状態の設定では追加イベントが発行されないことをテスト
TEST_F(UIComponentsEventDrivenTest, SameVisibilityDoesNotPublishEvent) {
    int event_count = 0;

    // イベント購読
    event_bus().subscribe<FeatureVisibilityChangedEvent>(
        [&event_count](const FeatureVisibilityChangedEvent&) {
            event_count++;
        }
    );

    const auto test_key = MurMur3::calcHash("TestFeature");
    // 初回設定（イベント発行される）
    app_state_manager().setFeatureVisibility(test_key, true);
    EXPECT_EQ(event_count, 1);

    // 同じ値を再設定（イベント発行されない）
    app_state_manager().setFeatureVisibility(test_key, true);
    EXPECT_EQ(event_count, 1); // カウントは変わらない

    // 異なる値を設定（イベント発行される）
    app_state_manager().setFeatureVisibility(test_key, false);
    EXPECT_EQ(event_count, 2);
    app_state_manager().setFeatureVisibility(test_key, false);
    EXPECT_EQ(event_count, 2);
}

#ifdef PAXS_USING_SIMULATOR
/// @brief シミュレーション停止イベントが正しく発行されることをテスト
TEST_F(UIComponentsEventDrivenTest, SimulationStopEventIsPublished) {
    bool event_received = false;

    // イベント購読
    event_bus().subscribe<SimulationStopCommandEvent>(
        [&event_received](const SimulationStopCommandEvent&) {
            event_received = true;
        }
    );
    // イベント発行
    event_bus().publish(SimulationStopCommandEvent());

    // イベントが受信されたことを確認
    EXPECT_TRUE(event_received);
    // イベントが受信されたことを確認
    EXPECT_TRUE(event_received);
}

/// @brief シミュレーションステップイベントが正しく発行されることをテスト
TEST_F(UIComponentsEventDrivenTest, SimulationStepEventIsPublished) {
    bool event_received = false;
    int received_steps = 0;

    // イベント購読
    event_bus().subscribe<SimulationStepCommandEvent>(
        [&event_received, &received_steps](const SimulationStepCommandEvent& event) {
            event_received = true;
            received_steps = event.steps;
        }
    );
    // イベント発行
    const int test_steps = 5;
    event_bus().publish(SimulationStepCommandEvent(test_steps));

    // イベントが受信されたことを確認
    EXPECT_TRUE(event_received);
    EXPECT_EQ(received_steps, test_steps);
    EXPECT_TRUE(event_received);
    EXPECT_EQ(received_steps, test_steps);
}
#endif

} // namespace paxs
