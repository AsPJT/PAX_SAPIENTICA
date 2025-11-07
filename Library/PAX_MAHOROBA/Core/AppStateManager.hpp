/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_CORE_APP_STATE_MANAGER_HPP
#define PAX_MAHOROBA_CORE_APP_STATE_MANAGER_HPP

#include <PAX_MAHOROBA/Core/ApplicationEvents.hpp>
#include <PAX_MAHOROBA/Core/EventBus.hpp>
#include <PAX_MAHOROBA/Map/MapViewport.hpp>

#include <PAX_SAPIENTICA/Calendar/Koyomi.hpp>
#include <PAX_SAPIENTICA/FeatureVisibilityManager.hpp>

#ifdef PAXS_USING_SIMULATOR
#include <PAX_SAPIENTICA/Simulation/SimulationManager.hpp>
#endif

namespace paxs {

/// @brief アプリケーション状態管理クラス
/// @brief Application state manager class
/// @details ドメインオブジェクトを集約管理し、状態変更時にイベントを発行
class AppStateManager {
public:
    /// @brief コンストラクタ
    /// @param event_bus イベントバス
    explicit AppStateManager(EventBus& event_bus)
        : event_bus_(event_bus)
        , koyomi_()
        , map_viewport_()
#ifdef PAXS_USING_SIMULATOR
        , simulation_manager_()
#endif
        , visibility_manager_() {

        // MapViewportにEventBusを設定
        map_viewport_.setEventBus(&event_bus_);

        subscribeToEvents();
    }

    // ============================================================================
    // Getter（読み取り専用アクセス）
    // ============================================================================

    /// @brief 暦を取得
    const Koyomi& getKoyomi() const { return koyomi_; }
    Koyomi& getKoyomi() { return koyomi_; }

    /// @brief 地図ビューポートを取得
    const MapViewport& getMapViewport() const { return map_viewport_; }
    MapViewport& getMapViewport() { return map_viewport_; }

#ifdef PAXS_USING_SIMULATOR
    /// @brief シミュレーションマネージャーを取得
    const SimulationManager& getSimulationManager() const { return simulation_manager_; }
#endif

    /// @brief 機能可視性マネージャーを取得
    const FeatureVisibilityManager& getVisibilityManager() const { return visibility_manager_; }

    /// @brief 現在の言語インデックスを取得
    std::uint_least8_t getCurrentLanguageIndex() const { return current_language_index_; }

    // ============================================================================
    // 状態変更（イベント発行付き）
    // ============================================================================

    /// @brief 言語を設定
    /// @param index 言語インデックス
    void setLanguage(std::uint_least8_t index) {
        if (current_language_index_ != index) {
            current_language_index_ = index;
            event_bus_.publish(LanguageChangedEvent(index));
        }
    }

    /// @brief 機能の可視性を設定
    /// @param key 機能キー
    /// @param visible 可視性
    void setFeatureVisibility(std::uint_least32_t key, bool visible) {
        if (visibility_manager_.setVisibility(key, visible)) {
            event_bus_.publish(FeatureVisibilityChangedEvent(key, visible));
        }
    }

    /// @brief 地図レイヤーの可視性を設定
    /// @param key レイヤーキー
    /// @param visible 可視性
    void setMapLayerVisibility(std::uint_least32_t key, bool visible) {
        const bool changed = visibility_manager_.setVisibility(key, visible);
        if (changed) {
            event_bus_.publish(MapLayerVisibilityChangedEvent(key, visible));
        }
    }

    // ============================================================================
    // コマンド実行（UIから呼ばれる）
    // ============================================================================

#ifdef PAXS_USING_SIMULATOR
    /// @brief シミュレーション初期化コマンドを実行
    /// @param model_name モデル名
    void executeSimulationInit(const std::string& model_name = "") {
        event_bus_.publish(SimulationInitCommandEvent(model_name));
    }

    /// @brief シミュレーション再生コマンドを実行
    /// @param iterations 繰り返し回数
    void executeSimulationPlay(int iterations = 1) {
        event_bus_.publish(SimulationPlayCommandEvent(iterations));
    }

    /// @brief シミュレーション一時停止コマンドを実行
    void executeSimulationPause() {
        event_bus_.publish(SimulationPauseCommandEvent());
    }

    /// @brief シミュレーション停止コマンドを実行
    void executeSimulationStop() {
        event_bus_.publish(SimulationStopCommandEvent());
    }

    /// @brief シミュレーションステップ進行コマンドを実行
    /// @param steps ステップ数
    void executeSimulationStep(int steps = 1) {
        event_bus_.publish(SimulationStepCommandEvent(steps));
    }
#endif

    // ============================================================================
    // 更新処理（メインループから呼ばれる）
    // ============================================================================

#ifdef PAXS_USING_SIMULATOR
    /// @brief 暦を更新
    /// @details シミュレーションが実行中の場合、時間を進める
    void updateKoyomi() {
        if (koyomi_.move_forward_in_time || koyomi_.go_back_in_time) {
            const double old_jdn = koyomi_.jdn.cgetDay();

            koyomi_.update(simulation_manager_);

            const double new_jdn = koyomi_.jdn.cgetDay();
            if (old_jdn != new_jdn) {
                // グレゴリオ暦から年月日を取得
                auto gregorian_date = koyomi_.jdn.toGregorianCalendar();
                event_bus_.publish(DateChangedEvent(
                    new_jdn,
                    gregorian_date.cgetYear(),
                    gregorian_date.cgetMonth(),
                    gregorian_date.cgetDay()
                ));
            }
        }
    }
#endif

private:
    EventBus& event_bus_;

    // ドメインオブジェクト（AppStateManagerが所有）
    Koyomi koyomi_;
    MapViewport map_viewport_;
#ifdef PAXS_USING_SIMULATOR
    SimulationManager simulation_manager_;
#endif
    FeatureVisibilityManager visibility_manager_;

    // 状態変数
    std::uint_least8_t current_language_index_ = 0;

    /// @brief イベント購読を初期化
    void subscribeToEvents() {
#ifdef PAXS_USING_SIMULATOR
        // シミュレーション初期化コマンドの購読
        event_bus_.subscribe<SimulationInitCommandEvent>(
            [this](const SimulationInitCommandEvent& event) {
                handleSimulationInit(event);
            }
        );

        // シミュレーション再生コマンドの購読
        event_bus_.subscribe<SimulationPlayCommandEvent>(
            [this](const SimulationPlayCommandEvent& event) {
                handleSimulationPlay(event);
            }
        );

        // シミュレーション一時停止コマンドの購読
        event_bus_.subscribe<SimulationPauseCommandEvent>(
            [this](const SimulationPauseCommandEvent& event) {
                handleSimulationPause(event);
            }
        );

        // シミュレーション停止コマンドの購読
        event_bus_.subscribe<SimulationStopCommandEvent>(
            [this](const SimulationStopCommandEvent& event) {
                handleSimulationStop(event);
            }
        );

        // シミュレーションステップ進行コマンドの購読
        event_bus_.subscribe<SimulationStepCommandEvent>(
            [this](const SimulationStepCommandEvent& event) {
                handleSimulationStep(event);
            }
        );
#endif
    }

    // ============================================================================
    // コマンドハンドラー（ドメインロジック実行）
    // ============================================================================

#ifdef PAXS_USING_SIMULATOR
    /// @brief シミュレーション初期化コマンドを処理
    void handleSimulationInit(const SimulationInitCommandEvent& event) {
        // ドメインロジック実行
        simulation_manager_.initSimulation();
        koyomi_.steps.setDay(0);
        // Note: SimulationManagerにgetStartDate()がないため、
        // 実際の実装では適切な開始日を設定する必要があります

        // 状態変更イベント発行
        event_bus_.publish(SimulationStateChangedEvent(
            SimulationStateChangedEvent::State::Stopped, 0
        ));

        auto gregorian_date = koyomi_.jdn.toGregorianCalendar();
        event_bus_.publish(DateChangedEvent(
            koyomi_.jdn.cgetDay(),
            gregorian_date.cgetYear(),
            gregorian_date.cgetMonth(),
            gregorian_date.cgetDay()
        ));
    }

    /// @brief シミュレーション再生コマンドを処理
    void handleSimulationPlay(const SimulationPlayCommandEvent& event) {
        // ドメインロジック実行
        koyomi_.is_agent_update = true;
        koyomi_.move_forward_in_time = true;

        // 状態変更イベント発行
        event_bus_.publish(SimulationStateChangedEvent(
            SimulationStateChangedEvent::State::Playing,
            static_cast<std::uint_least32_t>(koyomi_.steps.cgetDay())
        ));
    }

    /// @brief シミュレーション一時停止コマンドを処理
    void handleSimulationPause(const SimulationPauseCommandEvent& event) {
        // ドメインロジック実行
        koyomi_.move_forward_in_time = false;
        koyomi_.go_back_in_time = false;

        // 状態変更イベント発行
        event_bus_.publish(SimulationStateChangedEvent(
            SimulationStateChangedEvent::State::Paused,
            static_cast<std::uint_least32_t>(koyomi_.steps.cgetDay())
        ));
    }

    /// @brief シミュレーション停止コマンドを処理
    void handleSimulationStop(const SimulationStopCommandEvent& event) {
        // ドメインロジック実行
        koyomi_.is_agent_update = false;
        koyomi_.move_forward_in_time = false;
        koyomi_.go_back_in_time = false;

        // 状態変更イベント発行
        event_bus_.publish(SimulationStateChangedEvent(
            SimulationStateChangedEvent::State::Stopped,
            static_cast<std::uint_least32_t>(koyomi_.steps.cgetDay())
        ));
    }

    /// @brief シミュレーションステップ進行コマンドを処理
    void handleSimulationStep(const SimulationStepCommandEvent& event) {
        // ドメインロジック実行
        for (int i = 0; i < event.steps; ++i) {
            simulation_manager_.step();
            koyomi_.steps.setDay(koyomi_.steps.cgetDay() + 1);
        }
        koyomi_.jdn += event.steps;

        // 状態変更イベント発行
        event_bus_.publish(SimulationStepExecutedEvent(
            static_cast<std::uint_least32_t>(koyomi_.steps.cgetDay()),
            static_cast<std::uint_least32_t>(simulation_manager_.getPopulation())
        ));

        auto gregorian_date = koyomi_.jdn.toGregorianCalendar();
        event_bus_.publish(DateChangedEvent(
            koyomi_.jdn.cgetDay(),
            gregorian_date.cgetYear(),
            gregorian_date.cgetMonth(),
            gregorian_date.cgetDay()
        ));
    }
#endif
};

} // namespace paxs

#endif // !PAX_MAHOROBA_CORE_APP_STATE_MANAGER_HPP
