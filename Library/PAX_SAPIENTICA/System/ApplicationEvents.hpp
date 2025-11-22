/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SYSTEM_APPLICATION_EVENTS_HPP
#define PAX_SAPIENTICA_SYSTEM_APPLICATION_EVENTS_HPP

#include <cstdint>
#include <string>
#include <utility>

#include <PAX_SAPIENTICA/Core/Type/Vector2.hpp>
#include <PAX_SAPIENTICA/Simulation/Config/SimulationState.hpp>
#include <PAX_SAPIENTICA/System/EventBus.hpp>

namespace paxs {

/// @brief ウィンドウサイズ変更イベント
struct WindowResizedEvent : Event {
    const Vector2<int> new_size;

    explicit WindowResizedEvent(Vector2<int> size)
        : new_size(size) {}
};

/// @brief UIレイアウト変更イベント
/// @brief UI layout changed event (triggered after layout recalculation)
struct UILayoutChangedEvent : Event {
    UILayoutChangedEvent() = default;
};

/// @brief マップビューポートのドラッグ開始イベント
/// @brief Map viewport drag started event (fired once when drag movement actually begins)
struct MapViewportDragStartedEvent : Event {
    MapViewportDragStartedEvent() = default;
};

/// @brief 言語設定変更イベント（通知用）
/// @brief Language changed event (for notification)
/// @details キーのみで言語を識別（インデックスは Pulldown 内部の実装詳細）
/// @details Language is identified by key only (index is an implementation detail inside Pulldown)
struct LanguageChangedEvent : Event {
    const std::uint_least32_t language_key;

    explicit LanguageChangedEvent(std::uint_least32_t key)
        : language_key(key) {}
};

/// @brief 言語変更コマンドイベント（UI → Domain）
/// @brief Language change command event (UI → Domain)
/// @details キーのみで言語を識別
/// @details Language is identified by key only
struct LanguageChangeCommandEvent : Event {
    const std::uint_least32_t language_key;

    explicit LanguageChangeCommandEvent(std::uint_least32_t key)
        : language_key(key) {}
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

/// @brief ビューポート変更イベント
struct ViewportChangedEvent : Event {
    const Vector2<double> new_center;
    const int new_zoom_level;

    ViewportChangedEvent(Vector2<double> center, int zoom_level)
        : new_center(center), new_zoom_level(zoom_level) {}
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

/// @brief シミュレーション状態変更イベント
struct SimulationStateChangedEvent : Event {
    const SimulationState new_state;

    SimulationStateChangedEvent(SimulationState state)
        : new_state(state) {}
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

/// @brief シミュレーション初期化コマンドイベント（同期版）
struct SimulationInitializeCommandEvent : Event {
    const std::string model_name;

    explicit SimulationInitializeCommandEvent(const std::string& model)
        : model_name(model) {}
};

/// @brief シミュレーション非同期初期化コマンドイベント
/// @brief Simulation async initialization command event
struct SimulationInitializeAsyncCommandEvent : Event {
    const std::string model_name;

    explicit SimulationInitializeAsyncCommandEvent(const std::string& model)
        : model_name(model) {}
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
    enum class Action : std::uint8_t {
        Forward,   // 順再生
        Reverse,   // 逆再生
        Stop       // 停止
    };

    const Action action;

    explicit TimePlaybackControlEvent(Action action_type)
        : action(action_type) {}
};

/// @brief 日付移動イベント
struct DateNavigationEvent : Event {
    const double days;  // 移動日数（負数で過去、正数で未来）

    explicit DateNavigationEvent(double day_count)
        : days(day_count) {}
};

/// @brief ログイベント（Logger → UI通知）
/// @brief Log event (Logger → UI notification)
///
/// Logger.hppから発行され、DebugLayerが購読します。
/// デバッグビルド時のみログ内容を画面に表示するために使用されます。
struct LogEvent : Event {
    /// @brief ログレベル
    /// @brief Log level
    enum class Level : std::uint8_t {
        Info,       ///< 情報メッセージ
        Warning,    ///< 警告メッセージ
        Error       ///< エラーメッセージ
    };

    Level level;                // ログレベル
    std::string message;        // ログメッセージ
    std::string filename;       // ファイル名
    int line;                   // 行番号
    std::string timestamp;      // タイムスタンプ

    LogEvent(Level lvl,
             std::string  msg,
             std::string  file,
             int line_number,
             std::string  time_stamp)
        : level(lvl)
        , message(std::move(msg))
        , filename(std::move(file))
        , line(line_number)
        , timestamp(std::move(time_stamp)) {}
};

} // namespace paxs

#endif // !PAX_SAPIENTICA_SYSTEM_APPLICATION_EVENTS_HPP
