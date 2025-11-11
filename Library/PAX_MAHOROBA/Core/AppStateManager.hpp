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
#include <PAX_MAHOROBA/Rendering/FontSystem.hpp>

#include <PAX_SAPIENTICA/Calendar/Koyomi.hpp>
#include <PAX_SAPIENTICA/FeatureVisibilityManager.hpp>

#ifdef PAXS_USING_SIMULATOR
#include <PAX_MAHOROBA/Core/SimulationController.hpp>
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
        , simulation_controller_(event_bus)
#endif
        , visibility_manager_() {

        // MapViewportにEventBusを設定
        map_viewport_.setEventBus(&event_bus_);

        subscribeToEvents();

        // 初期状態を通知
        publishInitialState();
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
    SimulationManager& getSimulationManager() { return simulation_manager_; }

    /// @brief シミュレーションコントローラーを取得
    const SimulationController& getSimulationController() const { return simulation_controller_; }
#endif

    /// @brief 機能可視性マネージャーを取得（const版）
    const FeatureVisibilityManager& getVisibilityManager() const { return visibility_manager_; }

    /// @brief 機能可視性マネージャーを取得（mutable版）
    FeatureVisibilityManager& getVisibilityManager() { return visibility_manager_; }

    /// @brief 現在の言語インデックスを取得
    std::uint_least8_t getCurrentLanguageIndex() const { return current_language_index_; }

    // ============================================================================
    // 状態変更（イベント発行付き）
    // ============================================================================

    /// @brief 言語を設定
    /// @param index 言語インデックス
    /// @param language_key 言語キー（MurMur3ハッシュ値）
    void setLanguage(std::uint_least8_t index, std::uint_least32_t language_key) {
        if (current_language_index_ != index) {
            current_language_index_ = index;
            Fonts().setLanguage(static_cast<std::size_t>(index));
            Fonts().setLanguageByKey(language_key);
            event_bus_.publish(LanguageChangedEvent(index));
        }
    }

    /// @brief 時間再生制御コマンドを実行
    /// @param action 再生制御アクション
    void executeTimePlaybackControl(TimePlaybackControlEvent::Action action) {
        event_bus_.publish(TimePlaybackControlEvent(action));
    }

    /// @brief 日付移動コマンドを実行
    /// @param days 移動日数
    void executeDateNavigation(double days) {
        event_bus_.publish(DateNavigationEvent(days));
    }

    /// @brief 機能の可視性を設定
    /// @param key 機能キー
    /// @param visible 可視性
    void setFeatureVisibility(std::uint_least32_t key, bool visible) {
        if (visibility_manager_.setVisibility(key, visible)) {
            event_bus_.publish(FeatureVisibilityChangedEvent(key, visible));
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

    /// @brief シミュレーション初期化コマンドを実行
    /// @param model_name モデル名
    /// @param seed 乱数シード
    void executeSimulationInitialize(
        const std::string& model_name,
        unsigned int seed) {
        event_bus_.publish(SimulationInitializeCommandEvent(model_name, seed));
    }

    /// @brief シミュレーション入力データ再読み込みコマンドを実行
    /// @param model_name モデル名
    void executeReloadInputData(const std::string& model_name) {
        event_bus_.publish(ReloadInputDataCommandEvent(model_name));
    }

    /// @brief 人間データ初期化コマンドを実行
    /// @param model_name モデル名
    void executeInitHumanData(const std::string& model_name) {
        event_bus_.publish(InitHumanDataCommandEvent(model_name));
    }

    /// @brief シミュレーションクリアコマンドを実行
    /// @brief Execute simulation clear command
    void executeSimulationClear() {
        event_bus_.publish(SimulationClearCommandEvent());
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

            koyomi_.update();

            // シミュレーション実行制御（is_agent_updateがtrueの場合のみ）
            if (koyomi_.is_agent_update && simulation_manager_.isActive()) {
                // シミュレーションを1ステップ実行
                simulation_manager_.step();

                // シミュレーションステップ実行イベントを発行
                event_bus_.publish(SimulationStepExecutedEvent(
                    static_cast<std::uint_least32_t>(koyomi_.steps.cgetDay()),
                    static_cast<std::uint_least32_t>(simulation_manager_.getPopulation())
                ));
            }

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

        // SimulationControllerの更新（自動繰り返し実行制御）
        simulation_controller_.update(simulation_manager_, koyomi_, "");
    }
#endif

private:
    EventBus& event_bus_;

    // ドメインオブジェクト（AppStateManagerが所有）
    Koyomi koyomi_;
    MapViewport map_viewport_;
#ifdef PAXS_USING_SIMULATOR
    SimulationManager simulation_manager_;
    SimulationController simulation_controller_;
#endif
    FeatureVisibilityManager visibility_manager_;

    // 状態変数
    std::uint_least8_t current_language_index_ = 0;

    /// @brief 初期状態をイベントで通知
    /// @brief Publish initial state via events
    void publishInitialState() {
#ifdef PAXS_USING_SIMULATOR
        // Simulationの初期状態を通知
        event_bus_.publish(SimulationStateChangedEvent(
            SimulationState::Uninitialized, 0
        ));
#endif
    }

    /// @brief イベント購読を初期化
    void subscribeToEvents() {
        // 言語変更コマンドの購読
        event_bus_.subscribe<LanguageChangeCommandEvent>(
            [this](const LanguageChangeCommandEvent& event) {
                setLanguage(event.language_index, event.language_key);
            }
        );

        // 地物可視性変更コマンドの購読
        event_bus_.subscribe<FeatureVisibilityChangeCommandEvent>(
            [this](const FeatureVisibilityChangeCommandEvent& event) {
                setFeatureVisibility(event.feature_key, event.is_visible);
            }
        );

        // 時間再生制御コマンドの購読
        event_bus_.subscribe<TimePlaybackControlEvent>(
            [this](const TimePlaybackControlEvent& event) {
                handleTimePlaybackControl(event);
            }
        );

        // 日付移動コマンドの購読
        event_bus_.subscribe<DateNavigationEvent>(
            [this](const DateNavigationEvent& event) {
                handleDateNavigation(event);
            }
        );

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

        // シミュレーション初期化コマンドの購読
        event_bus_.subscribe<SimulationInitializeCommandEvent>(
            [this](const SimulationInitializeCommandEvent& event) {
                handleSimulationInitialize(event);
            }
        );

        // シミュレーション入力データ再読み込みコマンドの購読
        event_bus_.subscribe<ReloadInputDataCommandEvent>(
            [this](const ReloadInputDataCommandEvent& event) {
                handleReloadInputData(event);
            }
        );

        // 人間データ初期化コマンドの購読
        event_bus_.subscribe<InitHumanDataCommandEvent>(
            [this](const InitHumanDataCommandEvent& event) {
                handleInitHumanData(event);
            }
        );

        // シミュレーションクリアコマンドの購読
        event_bus_.subscribe<SimulationClearCommandEvent>(
            [this](const SimulationClearCommandEvent& event) {
                handleSimulationClear(event);
            }
        );
#endif
    }

    // ============================================================================
    // コマンドハンドラー（ドメインロジック実行）
    // ============================================================================

    /// @brief 時間再生制御コマンドを処理
    void handleTimePlaybackControl(const TimePlaybackControlEvent& event) {
        using Action = TimePlaybackControlEvent::Action;

#ifdef PAXS_USING_SIMULATOR
        // シミュレーション再生中は暦の逆再生・停止を無効化（シミュレーションを停止する）
        if (koyomi_.is_agent_update) {
            if (event.action == Action::Reverse || event.action == Action::Stop) {
                // シミュレーションを停止
                event_bus_.publish(SimulationStopCommandEvent());
                return;
            }
            // Forward の場合は何もしない（既にシミュレーション再生中）
            return;
        }
#endif

        switch (event.action) {
        case Action::Forward:
            koyomi_.move_forward_in_time = true;
            koyomi_.go_back_in_time = false;
            break;
        case Action::Reverse:
            koyomi_.move_forward_in_time = false;
            koyomi_.go_back_in_time = true;
            break;
        case Action::Stop:
            koyomi_.move_forward_in_time = false;
            koyomi_.go_back_in_time = false;
            break;
        }
    }

    /// @brief 日付移動コマンドを処理
    void handleDateNavigation(const DateNavigationEvent& event) {
        koyomi_.jdn.getDay() += event.days;
        koyomi_.calcDate();

        // 日付変更イベント発行
        auto gregorian_date = koyomi_.jdn.toGregorianCalendar();
        event_bus_.publish(DateChangedEvent(
            koyomi_.jdn.cgetDay(),
            gregorian_date.cgetYear(),
            gregorian_date.cgetMonth(),
            gregorian_date.cgetDay()
        ));
    }

#ifdef PAXS_USING_SIMULATOR
    /// @brief シミュレーション初期化コマンドを処理
    void handleSimulationInit(const SimulationInitCommandEvent& event) {
        (void)event;
        simulation_manager_.initSimulation();
        koyomi_.steps.setDay(0);
        // TODO: 下のコメントの正誤確認
        // Note: SimulationManagerにgetStartDate()がないため、
        // 実際の実装では適切な開始日を設定する必要があります

        // 状態変更イベント発行
        event_bus_.publish(SimulationStateChangedEvent(
            SimulationState::Stopped, 0
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
        koyomi_.is_agent_update = true;
        koyomi_.move_forward_in_time = true;

        // SimulationControllerに自動実行を開始（iterations > 1の場合）
        if (event.iterations > 1) {
            simulation_controller_.startAutoExecution(event.iterations, "");
        }

        // 状態変更イベント発行
        event_bus_.publish(SimulationStateChangedEvent(
            SimulationState::Playing,
            static_cast<std::uint_least32_t>(koyomi_.steps.cgetDay())
        ));
    }

    /// @brief シミュレーション一時停止コマンドを処理
    void handleSimulationPause(const SimulationPauseCommandEvent& event) {
        (void)event;
        koyomi_.move_forward_in_time = false;
        koyomi_.go_back_in_time = false;

        // 状態変更イベント発行
        event_bus_.publish(SimulationStateChangedEvent(
            SimulationState::Stopped,
            static_cast<std::uint_least32_t>(koyomi_.steps.cgetDay())
        ));
    }

    /// @brief シミュレーション停止コマンドを処理
    void handleSimulationStop(const SimulationStopCommandEvent& event) {
        (void)event;
        koyomi_.is_agent_update = false;
        koyomi_.move_forward_in_time = false;
        koyomi_.go_back_in_time = false;

        // 状態変更イベント発行
        event_bus_.publish(SimulationStateChangedEvent(
            SimulationState::Stopped,
            static_cast<std::uint_least32_t>(koyomi_.steps.cgetDay())
        ));
    }

    /// @brief シミュレーションステップ進行コマンドを処理
    void handleSimulationStep(const SimulationStepCommandEvent& event) {
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

    /// @brief シミュレーション初期化コマンドを処理
    void handleSimulationInitialize(const SimulationInitializeCommandEvent& event) {
        simulation_manager_.simulationInitialize(event.model_name, event.seed);

        // シミュレーション初期化
        simulation_manager_.initSimulation();
        koyomi_.steps.setDay(0);
        koyomi_.calcDate();
        koyomi_.is_agent_update = false;
        koyomi_.move_forward_in_time = false;
        koyomi_.go_back_in_time = false;

        // 状態変更イベント発行
        event_bus_.publish(SimulationStateChangedEvent(
            SimulationState::Stopped,
            static_cast<std::uint_least32_t>(koyomi_.steps.cgetDay())
        ));
    }

    /// @brief シミュレーション入力データ再読み込みコマンドを処理
    void handleReloadInputData(const ReloadInputDataCommandEvent& event) {
        simulation_manager_.reloadData(event.model_name);
    }

    /// @brief 人間データ初期化コマンドを処理
    void handleInitHumanData(const InitHumanDataCommandEvent& event) {
        simulation_manager_.initHumanData(event.model_name);
        koyomi_.steps.setDay(0);
        koyomi_.calcDate();

        // 状態変更イベント発行
        event_bus_.publish(SimulationStateChangedEvent(
            SimulationState::Stopped, 0
        ));

        auto gregorian_date = koyomi_.jdn.toGregorianCalendar();
        event_bus_.publish(DateChangedEvent(
            koyomi_.jdn.cgetDay(),
            gregorian_date.cgetYear(),
            gregorian_date.cgetMonth(),
            gregorian_date.cgetDay()
        ));
    }

    /// @brief シミュレーションクリアコマンドを処理
    /// @brief Handle simulation clear command
    void handleSimulationClear(const SimulationClearCommandEvent& /*event*/) {
        // シミュレーションを初期化前の状態に戻す
        simulation_manager_.clear();
        koyomi_.steps.setDay(0);
        koyomi_.calcDate();

        // 状態変更イベント発行（初期化前の状態に戻す）
        event_bus_.publish(SimulationStateChangedEvent(
            SimulationState::Uninitialized, 0
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
