/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]      2023-2025 As Project
    [Production]    2023-2025 As Project
    [Contact Us]    wanotaitei@gmail.com         https://github.com/AsPJT/PAX_SAPIENTICA
    [License]       Distributed under the CC0 1.0. https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#include <gtest/gtest.h>

#include <PAX_MAHOROBA/Core/EventBus.hpp>
#include <PAX_MAHOROBA/Core/ApplicationEvents.hpp>

namespace paxs {

class EventBusTest : public ::testing::Test {
protected:
    void SetUp() override {
        // 各テスト前にEventBusをクリア
        EventBus::getInstance().clearForTesting();
    }

    void TearDown() override {
        // 各テスト後にEventBusをクリア
        EventBus::getInstance().clearForTesting();
    }
};

// Singleton パターンのテスト
TEST_F(EventBusTest, SingletonInstance) {
    EventBus& instance1 = EventBus::getInstance();
    EventBus& instance2 = EventBus::getInstance();
    EXPECT_EQ(&instance1, &instance2);
}

// イベントの購読と発行（即座に通知）
TEST_F(EventBusTest, PublishAndSubscribe) {
    int call_count = 0;
    int received_width = 0;
    int received_height = 0;

    // イベントハンドラーを購読
    EventBus::getInstance().subscribe<WindowResizedEvent>(
        [&](const WindowResizedEvent& event) {
            call_count++;
            received_width = event.new_width;
            received_height = event.new_height;
        }
    );

    // イベントを発行
    EventBus::getInstance().publish(WindowResizedEvent(1920, 1080));

    // ハンドラーが即座に呼ばれることを確認
    EXPECT_EQ(call_count, 1);
    EXPECT_EQ(received_width, 1920);
    EXPECT_EQ(received_height, 1080);
}

// 複数のハンドラーを購読
TEST_F(EventBusTest, MultipleSubscribers) {
    int call_count1 = 0;
    int call_count2 = 0;

    // 複数のハンドラーを購読
    EventBus::getInstance().subscribe<LanguageChangedEvent>(
        [&](const LanguageChangedEvent& event) { call_count1++; }
    );
    EventBus::getInstance().subscribe<LanguageChangedEvent>(
        [&](const LanguageChangedEvent& event) { call_count2++; }
    );

    // イベントを発行
    EventBus::getInstance().publish(LanguageChangedEvent(1));

    // 両方のハンドラーが呼ばれることを確認
    EXPECT_EQ(call_count1, 1);
    EXPECT_EQ(call_count2, 1);
}

// 異なるイベントタイプの分離
TEST_F(EventBusTest, EventTypeSeparation) {
    int window_call_count = 0;
    int language_call_count = 0;

    // 異なるイベントタイプのハンドラーを購読
    EventBus::getInstance().subscribe<WindowResizedEvent>(
        [&](const WindowResizedEvent& event) { window_call_count++; }
    );
    EventBus::getInstance().subscribe<LanguageChangedEvent>(
        [&](const LanguageChangedEvent& event) { language_call_count++; }
    );

    // WindowResizedEventを発行
    EventBus::getInstance().publish(WindowResizedEvent(800, 600));
    EXPECT_EQ(window_call_count, 1);
    EXPECT_EQ(language_call_count, 0);

    // LanguageChangedEventを発行
    EventBus::getInstance().publish(LanguageChangedEvent(0));
    EXPECT_EQ(window_call_count, 1);
    EXPECT_EQ(language_call_count, 1);
}

// イベントのキューイング（遅延処理）
TEST_F(EventBusTest, EnqueueAndProcessQueue) {
    int call_count = 0;

    EventBus::getInstance().subscribe<DateChangedEvent>(
        [&](const DateChangedEvent& event) { call_count++; }
    );

    // イベントをキューに追加（まだ処理されない）
    EventBus::getInstance().enqueue(DateChangedEvent(2460317.5, 2025, 1, 7));
    EXPECT_EQ(call_count, 0);
    EXPECT_EQ(EventBus::getInstance().getQueueSize(), 1);

    // キューを処理
    EventBus::getInstance().processQueue();
    EXPECT_EQ(call_count, 1);
    EXPECT_EQ(EventBus::getInstance().getQueueSize(), 0);
}

// 複数のイベントをキューイング
TEST_F(EventBusTest, MultipleQueuedEvents) {
    int call_count = 0;

    EventBus::getInstance().subscribe<TimeSpeedChangedEvent>(
        [&](const TimeSpeedChangedEvent& event) { call_count++; }
    );

    // 複数のイベントをキューに追加
    EventBus::getInstance().enqueue(TimeSpeedChangedEvent(1));
    EventBus::getInstance().enqueue(TimeSpeedChangedEvent(2));
    EventBus::getInstance().enqueue(TimeSpeedChangedEvent(4));
    EXPECT_EQ(EventBus::getInstance().getQueueSize(), 3);

    // キューを処理
    EventBus::getInstance().processQueue();
    EXPECT_EQ(call_count, 3);
    EXPECT_EQ(EventBus::getInstance().getQueueSize(), 0);
}

// 購読者数の取得
TEST_F(EventBusTest, GetSubscriberCount) {
    EXPECT_EQ(EventBus::getInstance().getSubscriberCount<ViewportChangedEvent>(), 0);

    EventBus::getInstance().subscribe<ViewportChangedEvent>(
        [](const ViewportChangedEvent& event) {}
    );
    EXPECT_EQ(EventBus::getInstance().getSubscriberCount<ViewportChangedEvent>(), 1);

    EventBus::getInstance().subscribe<ViewportChangedEvent>(
        [](const ViewportChangedEvent& event) {}
    );
    EXPECT_EQ(EventBus::getInstance().getSubscriberCount<ViewportChangedEvent>(), 2);
}

// 購読者がいない場合のイベント発行
TEST_F(EventBusTest, PublishWithoutSubscribers) {
    // 購読者がいない状態でイベントを発行しても例外が発生しないことを確認
    EXPECT_NO_THROW(EventBus::getInstance().publish(MapLayerVisibilityChangedEvent(123, true)));
}

// clearメソッドのテスト
TEST_F(EventBusTest, ClearMethod) {
    int call_count = 0;

    EventBus::getInstance().subscribe<FeatureVisibilityChangedEvent>(
        [&](const FeatureVisibilityChangedEvent& event) { call_count++; }
    );
    EventBus::getInstance().enqueue(FeatureVisibilityChangedEvent(12345, true));

    EXPECT_EQ(EventBus::getInstance().getSubscriberCount<FeatureVisibilityChangedEvent>(), 1);
    EXPECT_EQ(EventBus::getInstance().getQueueSize(), 1);

    // クリア
    EventBus::getInstance().clearForTesting();

    EXPECT_EQ(EventBus::getInstance().getSubscriberCount<FeatureVisibilityChangedEvent>(), 0);
    EXPECT_EQ(EventBus::getInstance().getQueueSize(), 0);

    // クリア後にイベントを発行してもハンドラーは呼ばれない
    EventBus::getInstance().publish(FeatureVisibilityChangedEvent(12345, false));
    EXPECT_EQ(call_count, 0);
}

// シミュレーションイベントのテスト
TEST_F(EventBusTest, SimulationStateChangedEvent) {
    SimulationStateChangedEvent::State received_state = SimulationStateChangedEvent::State::Stopped;

    EventBus::getInstance().subscribe<SimulationStateChangedEvent>(
        [&](const SimulationStateChangedEvent& event) {
            received_state = event.new_state;
        }
    );

    EventBus::getInstance().publish(
        SimulationStateChangedEvent(SimulationStateChangedEvent::State::Playing, 100)
    );
    EXPECT_EQ(received_state, SimulationStateChangedEvent::State::Playing);

    EventBus::getInstance().publish(
        SimulationStateChangedEvent(SimulationStateChangedEvent::State::Paused, 150)
    );
    EXPECT_EQ(received_state, SimulationStateChangedEvent::State::Paused);
}

// コマンドイベントのテスト
TEST_F(EventBusTest, CommandEvents) {
    int init_count = 0;
    int play_count = 0;
    int pause_count = 0;
    int stop_count = 0;
    int step_count = 0;

    EventBus::getInstance().subscribe<SimulationInitCommandEvent>(
        [&](const SimulationInitCommandEvent& event) { init_count++; }
    );
    EventBus::getInstance().subscribe<SimulationPlayCommandEvent>(
        [&](const SimulationPlayCommandEvent& event) { play_count++; }
    );
    EventBus::getInstance().subscribe<SimulationPauseCommandEvent>(
        [&](const SimulationPauseCommandEvent& event) { pause_count++; }
    );
    EventBus::getInstance().subscribe<SimulationStopCommandEvent>(
        [&](const SimulationStopCommandEvent& event) { stop_count++; }
    );
    EventBus::getInstance().subscribe<SimulationStepCommandEvent>(
        [&](const SimulationStepCommandEvent& event) { step_count++; }
    );

    // 各コマンドイベントを発行
    EventBus::getInstance().publish(SimulationInitCommandEvent());
    EventBus::getInstance().publish(SimulationPlayCommandEvent());
    EventBus::getInstance().publish(SimulationPauseCommandEvent());
    EventBus::getInstance().publish(SimulationStopCommandEvent());
    EventBus::getInstance().publish(SimulationStepCommandEvent());

    EXPECT_EQ(init_count, 1);
    EXPECT_EQ(play_count, 1);
    EXPECT_EQ(pause_count, 1);
    EXPECT_EQ(stop_count, 1);
    EXPECT_EQ(step_count, 1);
}

// UIイベントのテスト
TEST_F(EventBusTest, UIEvents) {
    std::string received_menu_id;
    std::string received_pulldown_id;
    std::size_t received_index = 0;
    std::string received_button_id;

    EventBus::getInstance().subscribe<MenuItemSelectedEvent>(
        [&](const MenuItemSelectedEvent& event) {
            received_menu_id = event.menu_id;
        }
    );
    EventBus::getInstance().subscribe<PulldownSelectionChangedEvent>(
        [&](const PulldownSelectionChangedEvent& event) {
            received_pulldown_id = event.pulldown_id;
            received_index = event.selected_index;
        }
    );
    EventBus::getInstance().subscribe<ButtonClickedEvent>(
        [&](const ButtonClickedEvent& event) {
            received_button_id = event.button_id;
        }
    );

    EventBus::getInstance().publish(MenuItemSelectedEvent("file_open"));
    EXPECT_EQ(received_menu_id, "file_open");

    EventBus::getInstance().publish(PulldownSelectionChangedEvent("language_select", 2));
    EXPECT_EQ(received_pulldown_id, "language_select");
    EXPECT_EQ(received_index, 2);

    EventBus::getInstance().publish(ButtonClickedEvent("apply_button"));
    EXPECT_EQ(received_button_id, "apply_button");
}

// データローディングイベントのテスト
TEST_F(EventBusTest, DataLoadingEvents) {
    std::string received_data_type;
    bool received_success = false;
    std::string received_layer_name;
    std::size_t received_feature_count = 0;

    EventBus::getInstance().subscribe<DataLoadingStartedEvent>(
        [&](const DataLoadingStartedEvent& event) {
            received_data_type = event.data_type;
        }
    );
    EventBus::getInstance().subscribe<DataLoadingCompletedEvent>(
        [&](const DataLoadingCompletedEvent& event) {
            received_data_type = event.data_type;
            received_success = event.success;
        }
    );
    EventBus::getInstance().subscribe<GeographicDataLoadedEvent>(
        [&](const GeographicDataLoadedEvent& event) {
            received_layer_name = event.layer_name;
            received_feature_count = event.feature_count;
        }
    );

    EventBus::getInstance().publish(DataLoadingStartedEvent("map_tiles"));
    EXPECT_EQ(received_data_type, "map_tiles");

    EventBus::getInstance().publish(DataLoadingCompletedEvent("map_tiles", true));
    EXPECT_EQ(received_data_type, "map_tiles");
    EXPECT_TRUE(received_success);

    EventBus::getInstance().publish(GeographicDataLoadedEvent("rivers", 1523));
    EXPECT_EQ(received_layer_name, "rivers");
    EXPECT_EQ(received_feature_count, 1523);
}

} // namespace paxs
