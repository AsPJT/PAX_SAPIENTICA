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

/// @brief ウィンドウサイズ変更イベント
struct WindowResizedEvent : Event {
    const int new_width;
    const int new_height;

    WindowResizedEvent(int width, int height)
        : new_width(width), new_height(height) {}
};

/// @brief 言語設定変更イベント（通知用）
struct LanguageChangedEvent : Event {
    const std::uint_least8_t new_language;

    explicit LanguageChangedEvent(std::uint_least8_t language)
        : new_language(language) {}
};

/// @brief 言語変更コマンドイベント（UI → Domain）
struct LanguageChangeCommandEvent : Event {
    const std::uint_least8_t language_index;
    const std::uint_least32_t language_key;

    LanguageChangeCommandEvent(std::uint_least8_t index, std::uint_least32_t key)
        : language_index(index), language_key(key) {}
};

/// @brief 日付変更イベント
struct DateChangedEvent : Event {
    const double julian_day_number;
    const int new_year;
    const int new_month;
    const int new_day;

    DateChangedEvent(double jdn, int year, int month, int day)
        : julian_day_number(jdn), new_year(year), new_month(month), new_day(day) {}
};

/// @brief 時間速度変更イベント
struct TimeSpeedChangedEvent : Event {
    const int new_speed;  // 例: 1, 2, 4, 8, 16

    explicit TimeSpeedChangedEvent(int speed)
        : new_speed(speed) {}
};

/// @brief ビューポート変更イベント
struct ViewportChangedEvent : Event {
    const double new_center_x;
    const double new_center_y;
    const int new_zoom_level;

    ViewportChangedEvent(double center_x, double center_y, int zoom_level)
        : new_center_x(center_x), new_center_y(center_y), new_zoom_level(zoom_level) {}
};

/// @brief マップレイヤー表示切替イベント
struct MapLayerVisibilityChangedEvent : Event {
    const std::uint_least32_t layer_key;
    const bool is_visible;

    MapLayerVisibilityChangedEvent(std::uint_least32_t key, bool visible)
        : layer_key(key), is_visible(visible) {}
};

/// @brief 地物表示切替イベント（通知用）
struct FeatureVisibilityChangedEvent : Event {
    const std::uint_least32_t feature_key;
    const bool is_visible;

    FeatureVisibilityChangedEvent(std::uint_least32_t key, bool visible)
        : feature_key(key), is_visible(visible) {}
};

/// @brief 地物可視性変更コマンドイベント（UI → Domain）
struct FeatureVisibilityChangeCommandEvent : Event {
    const std::uint_least32_t feature_key;
    const bool is_visible;

    FeatureVisibilityChangeCommandEvent(std::uint_least32_t key, bool visible)
        : feature_key(key), is_visible(visible) {}
};

/// @brief レンダーレイヤー表示切替イベント
struct RenderLayerVisibilityChangedEvent : Event {
    const std::uint_least32_t layer_key;
    const bool is_visible;

    RenderLayerVisibilityChangedEvent(std::uint_least32_t key, bool visible)
        : layer_key(key), is_visible(visible) {}
};

/// @brief Koyomi（暦）状態同期イベント
/// @details UIがKoyomiの最新状態を受け取るためのイベント
struct KoyomiStateSyncEvent : Event {
    const std::size_t date_list_size;  ///< カレンダーリストのサイズ / Size of calendar list
    const double current_jdn;          ///< 現在のユリウス日 / Current Julian Day Number

    KoyomiStateSyncEvent(std::size_t size, double jdn)
        : date_list_size(size), current_jdn(jdn) {}
};

/// @brief シミュレーション状態変更イベント
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

/// @brief SimulationManager状態同期イベント
/// @details UIがSimulationManagerの最新状態を受け取るためのイベント
struct SimulationManagerStateSyncEvent : Event {
    const bool is_active;              ///< シミュレーションが有効か / Whether simulation is active

    explicit SimulationManagerStateSyncEvent(bool active)
        : is_active(active) {}
};

/// @brief シミュレーションステップ実行完了イベント
struct SimulationStepExecutedEvent : Event {
    const std::uint_least32_t current_step;
    const std::uint_least32_t total_population;

    SimulationStepExecutedEvent(std::uint_least32_t step, std::uint_least32_t population)
        : current_step(step), total_population(population) {}
};

/// @brief 集落表示設定変更イベント
struct SettlementDisplayChangedEvent : Event {
    const std::size_t select_draw;
    const bool is_line;
    const bool is_arrow;

    SettlementDisplayChangedEvent(std::size_t draw, bool line, bool arrow)
        : select_draw(draw), is_line(line), is_arrow(arrow) {}
};

/// @brief シミュレーション初期化コマンドイベント
struct SimulationInitCommandEvent : Event {
    const std::string model_name;

    explicit SimulationInitCommandEvent(const std::string& name = "")
        : model_name(name) {}
};

/// @brief シミュレーション再生コマンドイベント
struct SimulationPlayCommandEvent : Event {
    const int iterations;

    explicit SimulationPlayCommandEvent(int iter = 1)
        : iterations(iter) {}
};

/// @brief シミュレーション一時停止コマンドイベント
struct SimulationPauseCommandEvent : Event {
    SimulationPauseCommandEvent() = default;
};

/// @brief シミュレーション停止コマンドイベント
struct SimulationStopCommandEvent : Event {
    SimulationStopCommandEvent() = default;
};

/// @brief シミュレーションステップ実行コマンドイベント
struct SimulationStepCommandEvent : Event {
    const int steps;

    explicit SimulationStepCommandEvent(int step_count = 1)
        : steps(step_count) {}
};

/// @brief 地理データ読み込みコマンドイベント
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
struct ReloadInputDataCommandEvent : Event {
    const std::string model_name;

    explicit ReloadInputDataCommandEvent(const std::string& model)
        : model_name(model) {}
};

/// @brief 人間データ初期化コマンドイベント
struct InitHumanDataCommandEvent : Event {
    const std::string model_name;

    explicit InitHumanDataCommandEvent(const std::string& model)
        : model_name(model) {}
};

/// @brief シミュレーションクリアコマンドイベント
struct SimulationClearCommandEvent : Event {
    SimulationClearCommandEvent() = default;
};

/// @brief 時間再生制御イベント
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
struct DateNavigationEvent : Event {
    const double days;  // 移動日数（負数で過去、正数で未来）

    explicit DateNavigationEvent(double d)
        : days(d) {}
};

/// @brief メニューアイテム選択イベント
struct MenuItemSelectedEvent : Event {
    const std::string menu_id;

    explicit MenuItemSelectedEvent(const std::string& id)
        : menu_id(id) {}
};

/// @brief プルダウン選択変更イベント
struct PulldownSelectionChangedEvent : Event {
    const std::string pulldown_id;
    const std::size_t selected_index;

    PulldownSelectionChangedEvent(const std::string& id, std::size_t index)
        : pulldown_id(id), selected_index(index) {}
};

/// @brief ボタンクリックイベント
struct ButtonClickedEvent : Event {
    const std::string button_id;

    explicit ButtonClickedEvent(const std::string& id)
        : button_id(id) {}
};

/// @brief データ読込開始イベント
struct DataLoadingStartedEvent : Event {
    const std::string data_type;

    explicit DataLoadingStartedEvent(const std::string& type)
        : data_type(type) {}
};

/// @brief データ読込完了イベント
struct DataLoadingCompletedEvent : Event {
    const std::string data_type;
    const bool success;

    DataLoadingCompletedEvent(const std::string& type, bool succeeded)
        : data_type(type), success(succeeded) {}
};

/// @brief 地理データ読込完了イベント
struct GeographicDataLoadedEvent : Event {
    const std::string layer_name;
    const std::size_t feature_count;

    GeographicDataLoadedEvent(const std::string& name, std::size_t count)
        : layer_name(name), feature_count(count) {}
};

} // namespace paxs

#endif // !PAX_MAHOROBA_CORE_APPLICATION_EVENTS_HPP
