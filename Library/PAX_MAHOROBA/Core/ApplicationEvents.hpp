/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_CORE_APPLICATION_EVENTS_HPP
#define PAX_MAHOROBA_CORE_APPLICATION_EVENTS_HPP

#include <cstdint>
#include <string>

#include <PAX_MAHOROBA/Core/EventBus.hpp>

namespace paxs {

// ========================================
// System Events
// ========================================

/// @brief ウィンドウサイズ変更イベント
/// @brief Window resized event
struct WindowResizedEvent : Event {
    const int new_width;
    const int new_height;

    WindowResizedEvent(int width, int height)
        : new_width(width), new_height(height) {}
};

/// @brief 言語設定変更イベント
/// @brief Language setting changed event
struct LanguageChangedEvent : Event {
    const std::uint_least8_t new_language;

    explicit LanguageChangedEvent(std::uint_least8_t language)
        : new_language(language) {}
};

// ========================================
// Calendar Events
// ========================================

/// @brief 日付変更イベント
/// @brief Date changed event
struct DateChangedEvent : Event {
    const double julian_day_number;
    const int new_year;
    const int new_month;
    const int new_day;

    DateChangedEvent(double jdn, int year, int month, int day)
        : julian_day_number(jdn), new_year(year), new_month(month), new_day(day) {}
};

/// @brief 時間速度変更イベント
/// @brief Time speed changed event
struct TimeSpeedChangedEvent : Event {
    const int new_speed;  // 例: 1, 2, 4, 8, 16

    explicit TimeSpeedChangedEvent(int speed)
        : new_speed(speed) {}
};

// ========================================
// Map Events
// ========================================

/// @brief ビューポート変更イベント
/// @brief Viewport changed event
struct ViewportChangedEvent : Event {
    const double new_center_x;
    const double new_center_y;
    const int new_zoom_level;

    ViewportChangedEvent(double center_x, double center_y, int zoom_level)
        : new_center_x(center_x), new_center_y(center_y), new_zoom_level(zoom_level) {}
};

/// @brief マップレイヤー表示切替イベント
/// @brief Map layer visibility changed event
struct MapLayerVisibilityChangedEvent : Event {
    const std::uint_least32_t layer_key;
    const bool is_visible;

    MapLayerVisibilityChangedEvent(std::uint_least32_t key, bool visible)
        : layer_key(key), is_visible(visible) {}
};

// ========================================
// Feature Visibility Events
// ========================================

/// @brief 地物表示切替イベント
/// @brief Feature visibility changed event
struct FeatureVisibilityChangedEvent : Event {
    const std::uint_least32_t feature_key;
    const bool is_visible;

    FeatureVisibilityChangedEvent(std::uint_least32_t key, bool visible)
        : feature_key(key), is_visible(visible) {}
};

/// @brief レンダーレイヤー表示切替イベント
/// @brief Render layer visibility changed event
struct RenderLayerVisibilityChangedEvent : Event {
    const std::uint_least32_t layer_key;
    const bool is_visible;

    RenderLayerVisibilityChangedEvent(std::uint_least32_t key, bool visible)
        : layer_key(key), is_visible(visible) {}
};

// ========================================
// Simulation Events
// ========================================

/// @brief シミュレーション状態変更イベント
/// @brief Simulation state changed event
struct SimulationStateChangedEvent : Event {
    enum class State {
        Stopped,
        Playing,
        Paused
    };

    const State new_state;
    const std::uint_least32_t current_step;

    SimulationStateChangedEvent(State state, std::uint_least32_t step)
        : new_state(state), current_step(step) {}
};

/// @brief シミュレーションステップ実行完了イベント
/// @brief Simulation step executed event
struct SimulationStepExecutedEvent : Event {
    const std::uint_least32_t current_step;
    const std::uint_least32_t total_population;

    SimulationStepExecutedEvent(std::uint_least32_t step, std::uint_least32_t population)
        : current_step(step), total_population(population) {}
};

// ========================================
// Command Events (UI → Domain)
// ========================================

/// @brief シミュレーション初期化コマンドイベント
/// @brief Simulation initialization command event
struct SimulationInitCommandEvent : Event {
    const std::string model_name;

    explicit SimulationInitCommandEvent(const std::string& name = "")
        : model_name(name) {}
};

/// @brief シミュレーション再生コマンドイベント
/// @brief Simulation play command event
struct SimulationPlayCommandEvent : Event {
    const int iterations;

    explicit SimulationPlayCommandEvent(int iter = 1)
        : iterations(iter) {}
};

/// @brief シミュレーション一時停止コマンドイベント
/// @brief Simulation pause command event
struct SimulationPauseCommandEvent : Event {
    SimulationPauseCommandEvent() = default;
};

/// @brief シミュレーション停止コマンドイベント
/// @brief Simulation stop command event
struct SimulationStopCommandEvent : Event {
    SimulationStopCommandEvent() = default;
};

/// @brief シミュレーションステップ実行コマンドイベント
/// @brief Simulation step command event
struct SimulationStepCommandEvent : Event {
    const int steps;

    explicit SimulationStepCommandEvent(int step_count = 1)
        : steps(step_count) {}
};

/// @brief 地理データ読み込みコマンドイベント
/// @brief Load geographic data command event
struct LoadGeographicDataCommandEvent : Event {
    const std::string model_name;
    const std::string map_list_path;
    const std::string japan_provinces_path;
    const unsigned int seed;

    LoadGeographicDataCommandEvent(
        const std::string& model,
        const std::string& map_path,
        const std::string& provinces_path,
        unsigned int random_seed)
        : model_name(model)
        , map_list_path(map_path)
        , japan_provinces_path(provinces_path)
        , seed(random_seed) {}
};

/// @brief シミュレーション入力データ再読み込みコマンドイベント
/// @brief Reload simulation input data command event
struct ReloadInputDataCommandEvent : Event {
    const std::string model_name;

    explicit ReloadInputDataCommandEvent(const std::string& model)
        : model_name(model) {}
};

/// @brief 人間データ初期化コマンドイベント
/// @brief Initialize human data command event
struct InitHumanDataCommandEvent : Event {
    const std::string model_name;

    explicit InitHumanDataCommandEvent(const std::string& model)
        : model_name(model) {}
};

/// @brief 地理データ削除コマンドイベント
/// @brief Delete geographic data command event
struct DeleteGeographicDataCommandEvent : Event {
    const std::string map_list_path;
    const std::string japan_provinces_path;

    DeleteGeographicDataCommandEvent(
        const std::string& map_path,
        const std::string& provinces_path)
        : map_list_path(map_path)
        , japan_provinces_path(provinces_path) {}
};

// ========================================
// Time Control Events
// ========================================

/// @brief 時間再生制御イベント
/// @brief Time playback control event
struct TimePlaybackControlEvent : Event {
    enum class Action {
        Forward,   // 順再生
        Reverse,   // 逆再生
        Stop       // 停止
    };

    const Action action;

    explicit TimePlaybackControlEvent(Action a)
        : action(a) {}
};

/// @brief 日付移動イベント
/// @brief Date navigation event
struct DateNavigationEvent : Event {
    const double days;  // 移動日数（負数で過去、正数で未来）

    explicit DateNavigationEvent(double d)
        : days(d) {}
};

// ========================================
// UI Events
// ========================================

/// @brief メニューアイテム選択イベント
/// @brief Menu item selected event
struct MenuItemSelectedEvent : Event {
    const std::string menu_id;

    explicit MenuItemSelectedEvent(const std::string& id)
        : menu_id(id) {}
};

/// @brief プルダウン選択変更イベント
/// @brief Pulldown selection changed event
struct PulldownSelectionChangedEvent : Event {
    const std::string pulldown_id;
    const std::size_t selected_index;

    PulldownSelectionChangedEvent(const std::string& id, std::size_t index)
        : pulldown_id(id), selected_index(index) {}
};

/// @brief ボタンクリックイベント
/// @brief Button clicked event
struct ButtonClickedEvent : Event {
    const std::string button_id;

    explicit ButtonClickedEvent(const std::string& id)
        : button_id(id) {}
};

// ========================================
// Data Loading Events
// ========================================

/// @brief データ読込開始イベント
/// @brief Data loading started event
struct DataLoadingStartedEvent : Event {
    const std::string data_type;

    explicit DataLoadingStartedEvent(const std::string& type)
        : data_type(type) {}
};

/// @brief データ読込完了イベント
/// @brief Data loading completed event
struct DataLoadingCompletedEvent : Event {
    const std::string data_type;
    const bool success;

    DataLoadingCompletedEvent(const std::string& type, bool succeeded)
        : data_type(type), success(succeeded) {}
};

/// @brief 地理データ読込完了イベント
/// @brief Geographic data loaded event
struct GeographicDataLoadedEvent : Event {
    const std::string layer_name;
    const std::size_t feature_count;

    GeographicDataLoadedEvent(const std::string& name, std::size_t count)
        : layer_name(name), feature_count(count) {}
};

} // namespace paxs

#endif // !PAX_MAHOROBA_CORE_APPLICATION_EVENTS_HPP
