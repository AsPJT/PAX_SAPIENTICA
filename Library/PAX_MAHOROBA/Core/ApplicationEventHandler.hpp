/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_CORE_APPLICATION_EVENT_HANDLER_HPP
#define PAX_MAHOROBA_CORE_APPLICATION_EVENT_HANDLER_HPP

#include <vector>

#include <PAX_MAHOROBA/Core/AppStateManager.hpp>

#include <PAX_SAPIENTICA/System/ApplicationEvents.hpp>
#include <PAX_SAPIENTICA/System/EventBus.hpp>

namespace paxs {

/// @brief アプリケーションレベルのイベントハンドラー
/// @brief Application-level event handler
/// @details AppStateManager のイベント購読を管理し、コマンドイベントをドメインロジックに変換
/// @details Manages AppStateManager's event subscriptions and translates command events to domain logic
class ApplicationEventHandler {
public:
    /// @brief コンストラクタ
    /// @brief Constructor
    /// @param app_state AppStateManager参照
    explicit ApplicationEventHandler(AppStateManager& app_state)
        : app_state_(app_state) {
        subscribeToEvents();
    }

    // コピー・ムーブ禁止（ScopedSubscriptionのため）
    ApplicationEventHandler(const ApplicationEventHandler&) = delete;
    ApplicationEventHandler& operator=(const ApplicationEventHandler&) = delete;
    ApplicationEventHandler(ApplicationEventHandler&&) = delete;
    ApplicationEventHandler& operator=(ApplicationEventHandler&&) = delete;

    /// @brief デストラクタ（ScopedSubscriptionが自動的に購読解除）
    ~ApplicationEventHandler() = default;

private:
    AppStateManager& app_state_;
    std::vector<ScopedSubscription> subscriptions_;

    /// @brief すべてのイベント購読を初期化
    /// @brief Initialize all event subscriptions
    void subscribeToEvents() {
        paxs::EventBus& event_bus = paxs::EventBus::getInstance();

        // 時間再生制御コマンドの購読
        subscriptions_.push_back(
            event_bus.subscribeScoped<TimePlaybackControlEvent>(
                [this](const TimePlaybackControlEvent& event) {
                    handleTimePlaybackControl(event);
                }
            )
        );

        // 日付移動コマンドの購読
        subscriptions_.push_back(
            event_bus.subscribeScoped<DateNavigationEvent>(
                [this](const DateNavigationEvent& event) {
                    handleDateNavigation(event);
                }
            )
        );

#ifdef PAXS_USING_SIMULATOR
        // シミュレーション再生コマンドの購読
        subscriptions_.push_back(
            event_bus.subscribeScoped<SimulationPlayCommandEvent>(
                [this](const SimulationPlayCommandEvent& event) {
                    handleSimulationPlay(event);
                }
            )
        );

        // シミュレーション一時停止コマンドの購読
        subscriptions_.push_back(
            event_bus.subscribeScoped<SimulationPauseCommandEvent>(
                [this](const SimulationPauseCommandEvent& event) {
                    handleSimulationPause(event);
                }
            )
        );

        // シミュレーション停止コマンドの購読
        subscriptions_.push_back(
            event_bus.subscribeScoped<SimulationStopCommandEvent>(
                [this](const SimulationStopCommandEvent& event) {
                    handleSimulationStop(event);
                }
            )
        );

        // シミュレーションステップ進行コマンドの購読
        subscriptions_.push_back(
            event_bus.subscribeScoped<SimulationStepCommandEvent>(
                [this](const SimulationStepCommandEvent& event) {
                    handleSimulationStep(event);
                }
            )
        );

        // シミュレーション初期化コマンドの購読（同期版）
        subscriptions_.push_back(
            event_bus.subscribeScoped<SimulationInitializeCommandEvent>(
                [this](const SimulationInitializeCommandEvent& event) {
                    handleSimulationInitialize(event);
                }
            )
        );

        // シミュレーション非同期初期化コマンドの購読
        subscriptions_.push_back(
            event_bus.subscribeScoped<SimulationInitializeAsyncCommandEvent>(
                [this](const SimulationInitializeAsyncCommandEvent& event) {
                    handleSimulationInitializeAsync(event);
                }
            )
        );

        // シミュレーション入力データ再読み込みコマンドの購読
        subscriptions_.push_back(
            event_bus.subscribeScoped<ReloadInputDataCommandEvent>(
                [this](const ReloadInputDataCommandEvent& event) {
                    handleReloadInputData(event);
                }
            )
        );

        // 人間データ初期化コマンドの購読
        subscriptions_.push_back(
            event_bus.subscribeScoped<InitHumanDataCommandEvent>(
                [this](const InitHumanDataCommandEvent& event) {
                    handleInitHumanData(event);
                }
            )
        );

        // シミュレーションクリアコマンドの購読
        subscriptions_.push_back(
            event_bus.subscribeScoped<SimulationClearCommandEvent>(
                [this](const SimulationClearCommandEvent& event) {
                    handleSimulationClear(event);
                }
            )
        );
#endif

        // 言語変更コマンドの購読
        subscriptions_.push_back(
            event_bus.subscribeScoped<LanguageChangeCommandEvent>(
                [this](const LanguageChangeCommandEvent& event) {
                    handleLanguageChange(event);
                }
            )
        );

        // 地物可視性変更コマンドの購読
        subscriptions_.push_back(
            event_bus.subscribeScoped<FeatureVisibilityChangeCommandEvent>(
                [this](const FeatureVisibilityChangeCommandEvent& event) {
                    handleFeatureVisibilityChange(event);
                }
            )
        );
    }

    // ============================================================================
    // イベントハンドラー（AppStateManagerから移動）
    // ============================================================================

    /// @brief 時間再生制御コマンドを処理
    /// @brief Handle time playback control command
    void handleTimePlaybackControl(const TimePlaybackControlEvent& event) {
        using Action = TimePlaybackControlEvent::Action;

#ifdef PAXS_USING_SIMULATOR
        // シミュレーション再生中は暦の逆再生・停止を無効化（シミュレーションを停止する）
        if (app_state_.getKoyomi().is_agent_update) {
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
            app_state_.setTimePlayback(true, false);
            break;
        case Action::Reverse:
            app_state_.setTimePlayback(false, true);
            break;
        case Action::Stop:
            app_state_.setTimePlayback(false, false);
            break;
        }
    }

    /// @brief 日付移動コマンドを処理
    /// @brief Handle date navigation command
    void handleDateNavigation(const DateNavigationEvent& event) {
        app_state_.navigateDate(event.days);
    }

#ifdef PAXS_USING_SIMULATOR
    /// @brief シミュレーション初期化コマンドを処理（同期版）
    /// @brief Handle simulation initialization command (synchronous)
    void handleSimulationInitialize(const SimulationInitializeCommandEvent& event) {
        app_state_.initializeSimulation(event.model_name);
    }

    /// @brief シミュレーション非同期初期化コマンドを処理
    /// @brief Handle simulation async initialization command
    void handleSimulationInitializeAsync(const SimulationInitializeAsyncCommandEvent& event) {
        app_state_.initializeSimulationAsync(event.model_name);
    }

    /// @brief シミュレーション再生コマンドを処理
    /// @brief Handle simulation play command
    void handleSimulationPlay(const SimulationPlayCommandEvent& event) {
        app_state_.playSimulation(event.iterations);
    }

    /// @brief シミュレーション一時停止コマンドを処理
    /// @brief Handle simulation pause command
    void handleSimulationPause(const SimulationPauseCommandEvent& event) {
        (void)event;
        app_state_.pauseSimulation();
    }

    /// @brief シミュレーション停止コマンドを処理
    /// @brief Handle simulation stop command
    void handleSimulationStop(const SimulationStopCommandEvent& event) {
        (void)event;
        app_state_.stopSimulation();
    }

    /// @brief シミュレーションステップ進行コマンドを処理
    /// @brief Handle simulation step command
    void handleSimulationStep(const SimulationStepCommandEvent& event) {
        app_state_.stepSimulation(event.steps);
    }

    /// @brief シミュレーション入力データ再読み込みコマンドを処理
    /// @brief Handle reload input data command
    void handleReloadInputData(const ReloadInputDataCommandEvent& event) {
        app_state_.reloadInputData(event.model_name);
    }

    /// @brief 人間データ初期化コマンドを処理
    /// @brief Handle init human data command
    void handleInitHumanData(const InitHumanDataCommandEvent& event) {
        app_state_.initHumanData(event.model_name);
    }

    /// @brief シミュレーションクリアコマンドを処理
    /// @brief Handle simulation clear command
    void handleSimulationClear(const SimulationClearCommandEvent& /*event*/) {
        app_state_.clearSimulation();
    }
#endif

    /// @brief 言語変更コマンドを処理
    /// @brief Handle language change command
    void handleLanguageChange(const LanguageChangeCommandEvent& event) {
        AppStateManager::setLanguageKey(event.language_key);
    }

    /// @brief 地物可視性変更コマンドを処理
    /// @brief Handle feature visibility change command
    void handleFeatureVisibilityChange(const FeatureVisibilityChangeCommandEvent& event) {
        app_state_.setFeatureVisibility(event.feature_key, event.is_visible);
    }
};

} // namespace paxs

#endif // !PAX_MAHOROBA_CORE_APPLICATION_EVENT_HANDLER_HPP
