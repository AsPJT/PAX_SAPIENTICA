/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]      2023-2025 As Project
    [Production]    2023-2025 As Project
    [Contact Us]    wanotaitei@gmail.com         https://github.com/AsPJT/PAX_SAPIENTICA
    [License]       Distributed under the CC0 1.0. https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#include <gtest/gtest.h>

#include <PAX_MAHOROBA/Core/AppStateManager.hpp>
#include <PAX_MAHOROBA/Core/ApplicationEvents.hpp>
#include <PAX_MAHOROBA/Core/EventBus.hpp>

namespace paxs {

class AppStateManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        event_bus_.clearForTesting();
    }

    void TearDown() override {
        event_bus_.clearForTesting();
    }

    EventBus& event_bus_ = EventBus::getInstance();
};

// ============================================================================
// 基本機能テスト
// ============================================================================

TEST_F(AppStateManagerTest, ConstructorInitializesObjects) {
    AppStateManager app_state{};

    // ドメインオブジェクトが初期化されていることを確認
    EXPECT_NO_THROW(app_state.getKoyomi());
    EXPECT_NO_THROW(app_state.getMapViewport());
    EXPECT_NO_THROW(app_state.getVisibilityManager());
}

TEST_F(AppStateManagerTest, GettersReturnConstReferences) {
    AppStateManager app_state{};

    // const参照が返されることを確認
    const Koyomi& koyomi = app_state.getKoyomi();
    const MapViewport& viewport = app_state.getMapViewport();
    const FeatureVisibilityManager& vis_manager = app_state.getVisibilityManager();

    // 使用できることを確認（コンパイルが通ればOK）
    SUCCEED();
}

TEST_F(AppStateManagerTest, GettersReturnNonConstReferences) {
    AppStateManager app_state{};

    // 非const参照が返されることを確認
    Koyomi& koyomi = app_state.getKoyomi();
    MapViewport& viewport = app_state.getMapViewport();

    // 使用できることを確認（コンパイルが通ればOK）
    SUCCEED();
}

// ============================================================================
// 言語設定テスト
// ============================================================================

TEST_F(AppStateManagerTest, SetLanguagePublishesEvent) {
    AppStateManager app_state{};

    std::uint_least8_t received_index = 255;

    event_bus_.subscribe<LanguageChangedEvent>(
        [&received_index](const LanguageChangedEvent& event) {
            received_index = event.new_language;
        }
    );

    app_state.setLanguage(5, 0); // 言語インデックス5、キーは0（テスト用ダミー値）

    EXPECT_EQ(received_index, 5);
    EXPECT_EQ(app_state.getCurrentLanguageIndex(), 5);
}

TEST_F(AppStateManagerTest, SetLanguageSameValueDoesNotPublish) {
    AppStateManager app_state{};
    app_state.setLanguage(3, 0); // 言語インデックス3、キーは0（テスト用ダミー値）

    int event_count = 0;

    event_bus_.subscribe<LanguageChangedEvent>(
        [&event_count](const LanguageChangedEvent& event) {
            event_count++;
        }
    );

    // 同じ値を設定（イベントは発行されない）
    app_state.setLanguage(3, 0); // 言語インデックス3、キーは0（テスト用ダミー値）

    EXPECT_EQ(event_count, 0);
}

// ============================================================================
// 機能可視性テスト
// ============================================================================

TEST_F(AppStateManagerTest, SetFeatureVisibilityPublishesEvent) {
    AppStateManager app_state{};

    std::uint_least32_t received_key = 0;
    bool received_visible = false;

    event_bus_.subscribe<FeatureVisibilityChangedEvent>(
        [&received_key, &received_visible](const FeatureVisibilityChangedEvent& event) {
            received_key = event.feature_key;
            received_visible = event.is_visible;
        }
    );

    const std::uint_least32_t test_key = 12345;
    app_state.setFeatureVisibility(test_key, true);

    EXPECT_EQ(received_key, test_key);
    EXPECT_TRUE(received_visible);
}

TEST_F(AppStateManagerTest, SetFeatureVisibilitySameValueDoesNotPublish) {
    AppStateManager app_state{};

    const std::uint_least32_t test_key = 12345;
    app_state.setFeatureVisibility(test_key, true);

    int event_count = 0;

    event_bus_.subscribe<FeatureVisibilityChangedEvent>(
        [&event_count](const FeatureVisibilityChangedEvent& event) {
            event_count++;
        }
    );

    // 同じ値を設定（イベントは発行されない）
    app_state.setFeatureVisibility(test_key, true);

    EXPECT_EQ(event_count, 0);
}

// ============================================================================
// MapViewport統合テスト
// ============================================================================

TEST_F(AppStateManagerTest, MapViewportSizeChangePublishesEvent) {
    AppStateManager app_state{};

    int event_count = 0;

    event_bus_.subscribe<ViewportChangedEvent>(
        [&event_count](const ViewportChangedEvent& event) {
            event_count++;
        }
    );

    // サイズ変更
    MapViewport& viewport = app_state.getMapViewport();
    viewport.setSize(20.0);
    viewport.notifyViewportChanged();

    // イベントが発行されることを確認
    EXPECT_EQ(event_count, 1);
}

#ifdef PAXS_USING_SIMULATOR
// ============================================================================
// シミュレーションコマンドテスト
// ============================================================================

TEST_F(AppStateManagerTest, SimulationCommandsAreHandled) {
    AppStateManager app_state{};

    int init_event_count = 0;
    int state_event_count = 0;

    event_bus_.subscribe<SimulationStateChangedEvent>(
        [&state_event_count](const SimulationStateChangedEvent& event) {
            state_event_count++;
        }
    );

    // 人間データ初期化コマンドを実行
    app_state.executeInitHumanData("test_model");

    // 状態変更イベントが発行されることを確認
    EXPECT_GE(state_event_count, 1);
}

TEST_F(AppStateManagerTest, SimulationPlayCommandPublishesEvent) {
    AppStateManager app_state{};

    int state_event_count = 0;
    SimulationStateChangedEvent::State received_state = SimulationStateChangedEvent::State::Stopped;

    event_bus_.subscribe<SimulationStateChangedEvent>(
        [&state_event_count, &received_state](const SimulationStateChangedEvent& event) {
            state_event_count++;
            received_state = event.new_state;
        }
    );

    // 人間データ初期化してから再生
    app_state.executeInitHumanData("test_model");
    state_event_count = 0;  // カウントをリセット

    app_state.executeSimulationPlay();

    EXPECT_EQ(state_event_count, 1);
    EXPECT_EQ(received_state, SimulationStateChangedEvent::State::Playing);
}

TEST_F(AppStateManagerTest, SimulationPauseCommandPublishesEvent) {
    AppStateManager app_state{};

    int state_event_count = 0;
    SimulationStateChangedEvent::State received_state = SimulationStateChangedEvent::State::Stopped;

    event_bus_.subscribe<SimulationStateChangedEvent>(
        [&state_event_count, &received_state](const SimulationStateChangedEvent& event) {
            state_event_count++;
            received_state = event.new_state;
        }
    );

    // 人間データ初期化して再生してから一時停止
    app_state.executeInitHumanData("test_model");
    app_state.executeSimulationPlay();
    state_event_count = 0;  // カウントをリセット

    app_state.executeSimulationPause();

    EXPECT_EQ(state_event_count, 1);
    EXPECT_EQ(received_state, SimulationStateChangedEvent::State::Stopped);
}

TEST_F(AppStateManagerTest, SimulationStopCommandPublishesEvent) {
    AppStateManager app_state{};

    int state_event_count = 0;
    SimulationStateChangedEvent::State received_state = SimulationStateChangedEvent::State::Playing;

    event_bus_.subscribe<SimulationStateChangedEvent>(
        [&state_event_count, &received_state](const SimulationStateChangedEvent& event) {
            state_event_count++;
            received_state = event.new_state;
        }
    );

    // 人間データ初期化して再生してから停止
    app_state.executeInitHumanData("test_model");
    app_state.executeSimulationPlay();
    state_event_count = 0;  // カウントをリセット

    app_state.executeSimulationStop();

    EXPECT_EQ(state_event_count, 1);
    EXPECT_EQ(received_state, SimulationStateChangedEvent::State::Stopped);
}
#endif // PAXS_USING_SIMULATOR

} // namespace paxs
