/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_CORE_APP_STATE_MANAGER_HPP
#define PAX_MAHOROBA_CORE_APP_STATE_MANAGER_HPP

#include <cstdint>

#include <PAX_MAHOROBA/Map/MapViewport.hpp>
#include <PAX_MAHOROBA/Rendering/FontSystem.hpp>

#include <PAX_SAPIENTICA/Calendar/Koyomi.hpp>
#include <PAX_SAPIENTICA/System/ApplicationEvents.hpp>
#include <PAX_SAPIENTICA/System/EventBus.hpp>
#include <PAX_SAPIENTICA/System/FeatureVisibilityManager.hpp>

#ifdef PAXS_USING_SIMULATOR
#include <PAX_MAHOROBA/Core/SimulationController.hpp>
#include <PAX_SAPIENTICA/Simulation/Manager/SimulationManager.hpp>
#include <PAX_SAPIENTICA/System/Async/LoadingHandle.hpp>
#endif

namespace paxs {

/// @brief アプリケーション状態
/// @brief Application state
enum class AppState : std::uint8_t {
    Running,        ///< 通常実行中 / Normal running
    Loading         ///< ロード中 / Loading
};

/// @brief アプリケーション状態管理クラス
/// @brief Application state manager class
/// @details ドメインオブジェクトを集約管理し、状態変更時にイベントを発行
class AppStateManager {
public:
    explicit AppStateManager() {
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

    /// @brief アプリケーション状態を取得
    /// @brief Get application state
    /// @return アプリケーション状態 / Application state
    AppState getAppState() const { return app_state_; }

#ifdef PAXS_USING_SIMULATOR
    /// @brief ロードハンドルを取得（const版）
    /// @brief Get loading handle (const version)
    /// @return ロードハンドルへの参照 / Reference to loading handle
    const LoadingHandle<bool>& getLoadingHandle() const { return loading_handle_; }

    /// @brief ロード中かチェック
    /// @brief Check if loading
    /// @return ロード中ならtrue / True if loading
    bool isLoading() const {
        return app_state_ == AppState::Loading &&
               loading_handle_.isValid() &&
               !loading_handle_.isFinished();
    }
#endif

    // ============================================================================
    // 状態変更（イベント発行付き）
    // ============================================================================

    /// @brief 言語を設定
    /// @brief Set language
    /// @param language_key 言語キー（MurMur3ハッシュ値） / Language key (MurMur3 hash value)
    static bool setLanguage(std::uint_least32_t language_key) {
        const std::uint_least32_t current_key = Fonts().getSelectedLanguageKey();
        if (current_key != language_key) {
            bool changed = Fonts().setLanguageKey(language_key);
            if (changed) {
                paxs::EventBus::getInstance().publish(LanguageChangedEvent(language_key));
                return true;
            }
            PAXS_WARNING("[AppStateManager] setLanguage FAILED: Invalid language key: " + std::to_string(language_key));
        }
        return false;
    }

    /// @brief 時間再生制御コマンドを実行
    /// @param action 再生制御アクション
    static void executeTimePlaybackControl(TimePlaybackControlEvent::Action action) {
        paxs::EventBus::getInstance().publish(TimePlaybackControlEvent(action));
    }

    /// @brief 日付移動コマンドを実行
    /// @param days 移動日数
    static void executeDateNavigation(double days) {
        paxs::EventBus::getInstance().publish(DateNavigationEvent(days));
    }

    /// @brief 機能の可視性を設定
    /// @param key 機能キー
    /// @param visible 可視性
    void setFeatureVisibility(std::uint_least32_t key, bool visible) {
        if (visibility_manager_.setVisibility(key, visible)) {
            paxs::EventBus::getInstance().publish(FeatureVisibilityChangedEvent(key, visible));
        }
    }

    // ============================================================================
    // コマンド実行（UIから呼ばれる）
    // ============================================================================

#ifdef PAXS_USING_SIMULATOR
    /// @brief シミュレーション再生コマンドを実行
    /// @param iterations 繰り返し回数
    void executeSimulationPlay(int iterations = 1) const {
        paxs::EventBus::getInstance().publish(SimulationPlayCommandEvent(iterations));
    }

    /// @brief シミュレーション一時停止コマンドを実行
    void executeSimulationPause() const {
        paxs::EventBus::getInstance().publish(SimulationPauseCommandEvent());
    }

    /// @brief シミュレーション停止コマンドを実行
    void executeSimulationStop() const {
        paxs::EventBus::getInstance().publish(SimulationStopCommandEvent());
    }

    /// @brief シミュレーションステップ進行コマンドを実行
    /// @param steps ステップ数
    void executeSimulationStep(int steps = 1) const {
        paxs::EventBus::getInstance().publish(SimulationStepCommandEvent(steps));
    }

    /// @brief シミュレーション初期化コマンドを実行
    /// @param model_name モデル名
    void executeSimulationInitialize(const std::string& model_name) const {
        paxs::EventBus::getInstance().publish(SimulationInitializeCommandEvent(model_name));
    }

    /// @brief シミュレーション非同期初期化を開始
    /// @brief Start asynchronous simulation initialization
    /// @param model_name モデル名 / Model name
    void executeSimulationInitializeAsync(const std::string& model_name) {
        // すでにロード中の場合はキャンセル
        if (loading_handle_.isValid() && !loading_handle_.isFinished()) {
            loading_handle_.cancel();
            loading_handle_.join();
        }

        // アプリケーション状態をLoading に変更
        app_state_ = AppState::Loading;

        // 非同期初期化を開始
        loading_handle_ = simulation_manager_.simulationInitializeAsync(model_name);
    }

    /// @brief シミュレーション入力データ再読み込みコマンドを実行
    /// @param model_name モデル名
    void executeReloadInputData(const std::string& model_name) const {
        paxs::EventBus::getInstance().publish(ReloadInputDataCommandEvent(model_name));
    }

    /// @brief 人間データ初期化コマンドを実行
    /// @param model_name モデル名
    void executeInitHumanData(const std::string& model_name) const {
        paxs::EventBus::getInstance().publish(InitHumanDataCommandEvent(model_name));
    }

    /// @brief シミュレーションクリアコマンドを実行
    /// @brief Execute simulation clear command
    void executeSimulationClear() const {
        paxs::EventBus::getInstance().publish(SimulationClearCommandEvent());
    }
#endif

    // ============================================================================
    // 更新処理（メインループから呼ばれる）
    // ============================================================================

#ifdef PAXS_USING_SIMULATOR
    /// @brief 暦を更新
    /// @details 暦の再生状態に応じて時間を進行・巻き戻す
    /// @details シミュレーションの再生時にはシミュレーションをステップ実行する
    void updateKoyomi() {
        if (koyomi_.move_forward_in_time || koyomi_.go_back_in_time) {
            const double old_jdn = koyomi_.jdn.getDay();

            koyomi_.update();

            // シミュレーション実行制御（is_agent_updateがtrueの場合のみ）
            if (koyomi_.is_agent_update && simulation_manager_.isActive()) {
                // シミュレーションを1ステップ実行
                simulation_manager_.step();

                // シミュレーションステップ実行イベントを発行
                paxs::EventBus::getInstance().publish(SimulationStepExecutedEvent(
                    static_cast<std::uint_least32_t>(koyomi_.steps.getDay()),
                    static_cast<std::uint_least32_t>(simulation_manager_.getPopulation())
                ));
            }

            const double new_jdn = koyomi_.jdn.getDay();
            if (old_jdn != new_jdn) {
                publishDateChangedEvent();
            }
        }

        // SimulationControllerの更新（自動繰り返し実行制御）
        simulation_controller_.update(simulation_manager_, koyomi_, "");
    }

    /// @brief 非同期ロード状態を更新
    /// @brief Update async loading state
    void updateLoadingState() {
        // ロード中でない場合は何もしない
        if (app_state_ != AppState::Loading) {
            return;
        }

        // ロードが完了したかチェック
        if (loading_handle_.isValid() && loading_handle_.isFinished()) {
            // ロード結果を取得
            const bool* result = loading_handle_.getResult();

            if (result && *result) {
                // ロード成功 - 人間データを初期化
                simulation_manager_.initSimulation();

                // SimulationConstantsからstart_julian_dayを取得してKoyomiにセット
                koyomi_.jdn.setDay(SimulationConstants::getInstance().start_julian_day);
                resetKoyomiToStoppedState();

                // 状態変更イベント発行
                paxs::EventBus::getInstance().publish(SimulationStateChangedEvent(SimulationState::Stopped));
                publishDateChangedEvent();
            } else {
                // ロード失敗またはキャンセル
                if (loading_handle_.hasError()) {
                    PAXS_ERROR("Simulation initialization failed");
                }
            }

            // アプリケーション状態をRunningに戻す
            app_state_ = AppState::Running;
        }
    }
#endif

private:
    // ドメインオブジェクト（AppStateManagerが所有）
    Koyomi koyomi_;
    MapViewport map_viewport_;
#ifdef PAXS_USING_SIMULATOR
    SimulationManager simulation_manager_;
    SimulationController simulation_controller_;
    LoadingHandle<bool> loading_handle_;  ///< 非同期ロード用ハンドル / Loading handle for async operations
#endif
    FeatureVisibilityManager visibility_manager_;
    AppState app_state_ = AppState::Running;  ///< アプリケーション状態 / Application state

    /// @brief 初期状態をイベントで通知
    /// @brief Publish initial state via events
    void publishInitialState() {
#ifdef PAXS_USING_SIMULATOR
        // Simulationの初期状態を通知
        paxs::EventBus::getInstance().publish(SimulationStateChangedEvent(SimulationState::Uninitialized));
#endif
    }

    /// @brief イベント購読を初期化
    void subscribeToEvents() {
        paxs::EventBus& event_bus = paxs::EventBus::getInstance();

        // 言語変更コマンドの購読
        event_bus.subscribe<LanguageChangeCommandEvent>(
            [this](const LanguageChangeCommandEvent& event) -> void {
                setLanguage(event.language_key);
            }
        );

        // 地物可視性変更コマンドの購読
        event_bus.subscribe<FeatureVisibilityChangeCommandEvent>(
            [this](const FeatureVisibilityChangeCommandEvent& event) {
                setFeatureVisibility(event.feature_key, event.is_visible);
            }
        );

        // 時間再生制御コマンドの購読
        event_bus.subscribe<TimePlaybackControlEvent>(
            [this](const TimePlaybackControlEvent& event) {
                handleTimePlaybackControl(event);
            }
        );

        // 日付移動コマンドの購読
        event_bus.subscribe<DateNavigationEvent>(
            [this](const DateNavigationEvent& event) {
                handleDateNavigation(event);
            }
        );

#ifdef PAXS_USING_SIMULATOR
        // シミュレーション再生コマンドの購読
        event_bus.subscribe<SimulationPlayCommandEvent>(
            [this](const SimulationPlayCommandEvent& event) {
                handleSimulationPlay(event);
            }
        );

        // シミュレーション一時停止コマンドの購読
        event_bus.subscribe<SimulationPauseCommandEvent>(
            [this](const SimulationPauseCommandEvent& event) {
                handleSimulationPause(event);
            }
        );

        // シミュレーション停止コマンドの購読
        event_bus.subscribe<SimulationStopCommandEvent>(
            [this](const SimulationStopCommandEvent& event) {
                handleSimulationStop(event);
            }
        );

        // シミュレーションステップ進行コマンドの購読
        event_bus.subscribe<SimulationStepCommandEvent>(
            [this](const SimulationStepCommandEvent& event) {
                handleSimulationStep(event);
            }
        );

        // シミュレーション初期化コマンドの購読
        event_bus.subscribe<SimulationInitializeCommandEvent>(
            [this](const SimulationInitializeCommandEvent& event) {
                handleSimulationInitialize(event);
            }
        );

        // シミュレーション入力データ再読み込みコマンドの購読
        event_bus.subscribe<ReloadInputDataCommandEvent>(
            [this](const ReloadInputDataCommandEvent& event) {
                handleReloadInputData(event);
            }
        );

        // 人間データ初期化コマンドの購読
        event_bus.subscribe<InitHumanDataCommandEvent>(
            [this](const InitHumanDataCommandEvent& event) {
                handleInitHumanData(event);
            }
        );

        // シミュレーションクリアコマンドの購読
        event_bus.subscribe<SimulationClearCommandEvent>(
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
                paxs::EventBus::getInstance().publish(SimulationStopCommandEvent());
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

    /// @brief 日付変更イベントを発行
    /// @brief Publish date changed event
    void publishDateChangedEvent() const {
        auto gregorian_date = koyomi_.jdn.toGregorianCalendar();
        paxs::EventBus::getInstance().publish(DateChangedEvent(
            koyomi_.jdn.getDay(),
            gregorian_date.getYear(),
            gregorian_date.getMonth(),
            gregorian_date.getDay()
        ));
    }

#ifdef PAXS_USING_SIMULATOR
    /// @brief Koyomiのステップと日付をリセット
    /// @brief Reset Koyomi steps and date
    void resetKoyomiStepsAndDate() {
        koyomi_.steps.setDay(0);
        koyomi_.calcDate();
    }

    /// @brief Koyomiの状態をリセット（シミュレーション停止状態へ）
    /// @brief Reset Koyomi state to stopped simulation state
    void resetKoyomiToStoppedState() {
        resetKoyomiStepsAndDate();
        koyomi_.is_agent_update = false;
        koyomi_.move_forward_in_time = false;
        koyomi_.go_back_in_time = false;
    }
#endif

    /// @brief 日付移動コマンドを処理
    void handleDateNavigation(const DateNavigationEvent& event) {
        koyomi_.jdn.addDays(event.days);
        koyomi_.calcDate();
        publishDateChangedEvent();
    }

#ifdef PAXS_USING_SIMULATOR
    /// @brief シミュレーション初期化コマンドを処理
    void handleSimulationInitialize(const SimulationInitializeCommandEvent& event) {
        simulation_manager_.simulationInitialize(event.model_name);

        // シミュレーション初期化
        simulation_manager_.initSimulation();

        // SimulationConstantsからstart_julian_dayを取得してKoyomiにセット
        koyomi_.jdn.setDay(SimulationConstants::getInstance().start_julian_day);
        resetKoyomiToStoppedState();

        // 状態変更イベント発行
        paxs::EventBus::getInstance().publish(SimulationStateChangedEvent(SimulationState::Stopped));
        publishDateChangedEvent();
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
        paxs::EventBus::getInstance().publish(SimulationStateChangedEvent(SimulationState::Playing));
    }

    /// @brief シミュレーション一時停止コマンドを処理
    void handleSimulationPause(const SimulationPauseCommandEvent& event) {
        (void)event;
        koyomi_.move_forward_in_time = false;
        koyomi_.go_back_in_time = false;

        // 状態変更イベント発行
        paxs::EventBus::getInstance().publish(SimulationStateChangedEvent(SimulationState::Stopped));
    }

    /// @brief シミュレーション停止コマンドを処理
    void handleSimulationStop(const SimulationStopCommandEvent& event) {
        (void)event;
        koyomi_.is_agent_update = false;
        koyomi_.move_forward_in_time = false;
        koyomi_.go_back_in_time = false;

        // 状態変更イベント発行
        paxs::EventBus::getInstance().publish(SimulationStateChangedEvent(SimulationState::Stopped));
    }

    /// @brief シミュレーションステップ進行コマンドを処理
    void handleSimulationStep(const SimulationStepCommandEvent& event) {
        for (int i = 0; i < event.steps; ++i) {
            simulation_manager_.step();
            koyomi_.steps.setDay(koyomi_.steps.getDay() + 1);
        }
        koyomi_.jdn += event.steps;

        // 状態変更イベント発行
        paxs::EventBus::getInstance().publish(SimulationStepExecutedEvent(
            static_cast<std::uint_least32_t>(koyomi_.steps.getDay()),
            static_cast<std::uint_least32_t>(simulation_manager_.getPopulation())
        ));
        publishDateChangedEvent();
    }

    /// @brief シミュレーション入力データ再読み込みコマンドを処理
    void handleReloadInputData(const ReloadInputDataCommandEvent& event) {
        simulation_manager_.reloadData(event.model_name);
    }

    /// @brief 人間データ初期化コマンドを処理
    void handleInitHumanData(const InitHumanDataCommandEvent& event) {
        simulation_manager_.initHumanData(event.model_name);
        resetKoyomiToStoppedState();

        // 状態変更イベント発行
        paxs::EventBus::getInstance().publish(SimulationStateChangedEvent(SimulationState::Stopped));
        publishDateChangedEvent();
    }

    /// @brief シミュレーションクリアコマンドを処理
    /// @brief Handle simulation clear command
    void handleSimulationClear(const SimulationClearCommandEvent& /*event*/) {
        // シミュレーションを初期化前の状態に戻す
        simulation_manager_.clear();
        resetKoyomiStepsAndDate();

        // 状態変更イベント発行（初期化前の状態に戻す）
        paxs::EventBus::getInstance().publish(SimulationStateChangedEvent(SimulationState::Uninitialized));
        publishDateChangedEvent();
    }
#endif
};

} // namespace paxs

#endif // !PAX_MAHOROBA_CORE_APP_STATE_MANAGER_HPP
