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

#include <PAX_MAHOROBA/Core/DomainCore.hpp>
#include <PAX_MAHOROBA/Rendering/FontSystem.hpp>

#include <PAX_SAPIENTICA/Key/MenuBarKeys.hpp>
#include <PAX_SAPIENTICA/System/ApplicationEvents.hpp>
#include <PAX_SAPIENTICA/System/EventBus.hpp>
#include <PAX_SAPIENTICA/System/FeatureVisibilityManager.hpp>

#ifdef PAXS_USING_SIMULATOR
#include <PAX_SAPIENTICA/System/Async/LoadingHandle.hpp>
#endif

namespace paxs {

/// @brief アプリケーション状態
/// @brief Application state
enum class AppState : std::uint8_t {
    Initializing,   ///< 初期化中 / Initializing
    Running,        ///< 通常実行中 / Normal running
    Loading         ///< ロード中 / Loading
};

/// @brief アプリケーション状態管理クラス
/// @brief Application state manager class
/// @details ドメインオブジェクトを集約管理し、状態変更時にイベントを発行
class AppStateManager {
public:
    explicit AppStateManager() {
        // 機能可視性の初期化
        initializeFeatureVisibility();
        // 初期状態を通知
        publishInitialState();
    }

    // ============================================================================
    // Getter（読み取り専用アクセス）
    // ============================================================================

    const Koyomi& getKoyomi() const { return domain_core_.getKoyomi(); }
    const MapViewport& getMapViewport() const { return domain_core_.getMapViewport(); }
    /// @brief 地図ビューポートを取得（InputHandler専用）
    MapViewport& getMapViewportForInputHandler() { return domain_core_.getMapViewportForInputHandler(); }

#ifdef PAXS_USING_SIMULATOR
    const SimulationManager& getSimulationManager() const { return domain_core_.getSimulationManager(); }
    const SimulationController& getSimulationController() const { return domain_core_.getSimulationController(); }
#endif

    /// @brief 機能可視性マネージャーを取得（const版）
    const FeatureVisibilityManager& getVisibilityManager() const { return visibility_manager_; }

    /// @brief アプリケーション状態を取得
    /// @brief Get application state
    /// @return アプリケーション状態 / Application state
    AppState getAppState() const { return app_state_; }

    /// @brief 初期化完了時に呼ばれる
    /// @brief Called when initialization is complete
    void updateInitializationComplete() {
        app_state_ = AppState::Running;
    }

#ifdef PAXS_USING_SIMULATOR
    /// @brief ロードハンドルを取得
    const LoadingHandle<bool>& getLoadingHandle() const { return loading_handle_; }

    /// @brief ロード中かチェック
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
    static bool setLanguageKey(std::uint_least32_t language_key) {
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

    /// @brief 機能の可視性を設定
    /// @param key 機能キー
    /// @param visible 可視性
    void setFeatureVisibility(std::uint_least32_t key, bool visible) {
        if (visibility_manager_.setVisibility(key, visible)) {
            paxs::EventBus::getInstance().publish(FeatureVisibilityChangedEvent(key, visible));
        }
    }

    /// @brief ビューポート中心座標を設定（制約適用・通知付き）
    /// @brief Set viewport center with constraints and notification
    /// @param center 中心座標 / Center position
    void setViewportCenter(Vector2<double> center) {
        domain_core_.setViewportCenter(center);
        domain_core_.notifyViewportChanged();
    }

    /// @brief ビューポートサイズを設定（制約適用・通知付き）
    /// @brief Set viewport size with constraints and notification
    /// @param size サイズ / Size (zoom level)
    void setViewportSize(double size) {
        domain_core_.setViewportSize(size);
        domain_core_.notifyViewportChanged();
    }

    /// @brief 時間再生の状態を設定
    /// @brief Set time playback state
    /// @param forward 順再生フラグ / Forward playback flag
    /// @param backward 逆再生フラグ / Backward playback flag
    void setTimePlayback(bool forward, bool backward) {
        domain_core_.setTimePlayback(forward, backward);
    }

    /// @brief 日付を移動
    /// @brief Navigate date
    /// @param days 移動日数 / Days to navigate
    void navigateDate(double days) {
        domain_core_.navigateDate(days);
        publishDateChangedEvent();
    }

#ifdef PAXS_USING_SIMULATOR
    /// @brief シミュレーションを初期化（同期版）
    /// @brief Initialize simulation (synchronous)
    /// @param model_name モデル名 / Model name
    void initializeSimulation(const std::string& model_name) {
        domain_core_.initializeSimulation(model_name);
        paxs::EventBus::getInstance().publish(SimulationStateChangedEvent(SimulationState::Stopped));
        publishDateChangedEvent();
    }

    /// @brief シミュレーションを非同期初期化
    /// @brief Initialize simulation asynchronously
    /// @param model_name モデル名 / Model name
    void initializeSimulationAsync(const std::string& model_name) {
        if (loading_handle_.isValid() && !loading_handle_.isFinished()) {
            loading_handle_.cancel();
            loading_handle_.join();
        }
        app_state_ = AppState::Loading;
        loading_handle_ = domain_core_.startSimulationInitializeAsync(model_name);
    }

    /// @brief シミュレーションを再生
    /// @brief Play simulation
    /// @param iterations 実行回数 / Number of iterations
    void playSimulation(int iterations) {
        domain_core_.playSimulation(iterations);
        paxs::EventBus::getInstance().publish(SimulationStateChangedEvent(SimulationState::Playing));
    }

    /// @brief シミュレーションを一時停止
    /// @brief Pause simulation
    void pauseSimulation() {
        domain_core_.pauseSimulation();
        paxs::EventBus::getInstance().publish(SimulationStateChangedEvent(SimulationState::Stopped));
    }

    /// @brief シミュレーションを停止
    /// @brief Stop simulation
    void stopSimulation() {
        domain_core_.stopSimulation();
        paxs::EventBus::getInstance().publish(SimulationStateChangedEvent(SimulationState::Stopped));
    }

    /// @brief シミュレーションをステップ実行
    /// @brief Step simulation
    /// @param steps ステップ数 / Number of steps
    void stepSimulation(int steps) {
        domain_core_.stepSimulation(steps);
        paxs::EventBus::getInstance().publish(SimulationStepExecutedEvent(
            static_cast<std::uint_least32_t>(domain_core_.getKoyomi().steps.getDay()),
            static_cast<std::uint_least32_t>(domain_core_.getSimulationManager().getPopulation())
        ));
        publishDateChangedEvent();
    }

    /// @brief 入力データを再読み込み
    /// @brief Reload input data
    /// @param model_name モデル名 / Model name
    void reloadInputData(const std::string& model_name) {
        domain_core_.reloadInputData(model_name);
    }

    /// @brief 人間データを初期化
    /// @brief Initialize human data
    /// @param model_name モデル名 / Model name
    void initHumanData(const std::string& model_name) {
        domain_core_.initHumanData(model_name);
        paxs::EventBus::getInstance().publish(SimulationStateChangedEvent(SimulationState::Stopped));
        publishDateChangedEvent();
    }

    /// @brief シミュレーションをクリア
    /// @brief Clear simulation
    void clearSimulation() {
        domain_core_.clearSimulation();
        paxs::EventBus::getInstance().publish(SimulationStateChangedEvent(SimulationState::Uninitialized));
        publishDateChangedEvent();
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
        bool date_changed = domain_core_.updateKoyomi();

        if (date_changed) {
            // シミュレーションステップ実行イベントを発行
            paxs::EventBus::getInstance().publish(SimulationStepExecutedEvent(
                static_cast<std::uint_least32_t>(domain_core_.getKoyomi().steps.getDay()),
                static_cast<std::uint_least32_t>(domain_core_.getSimulationManager().getPopulation())
            ));
            publishDateChangedEvent();
        }

        // SimulationControllerの更新（自動繰り返し実行制御）
        domain_core_.updateSimulationController("");
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
                domain_core_.completeSimulationInitialization();

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
    DomainCore domain_core_;
#ifdef PAXS_USING_SIMULATOR
    LoadingHandle<bool> loading_handle_;
#endif
    FeatureVisibilityManager visibility_manager_;
    AppState app_state_ = AppState::Initializing;

    /// @brief 機能可視性の初期値を設定
    /// @brief Initialize feature visibility default values
    void initializeFeatureVisibility() {
        // View メニューの初期値
        visibility_manager_.emplace(ViewMenu::calendar, true);
        visibility_manager_.emplace(ViewMenu::map, true);
        visibility_manager_.emplace(ViewMenu::ui, true);
        visibility_manager_.emplace(ViewMenu::simulation, true);
        visibility_manager_.emplace(ViewMenu::person, true);
        visibility_manager_.emplace(ViewMenu::license, false);
        visibility_manager_.emplace(ViewMenu::debug, false);
        visibility_manager_.emplace(ViewMenu::view_3d, false);

        // Map レイヤーメニューの初期値
        visibility_manager_.emplace(MapLayersMenu::land_and_water, false);
        visibility_manager_.emplace(MapLayersMenu::soil, false);
        visibility_manager_.emplace(MapLayersMenu::ryosei_line, true);
        visibility_manager_.emplace(MapLayersMenu::slope, true);
        visibility_manager_.emplace(MapLayersMenu::line1, false);
    }

    /// @brief 初期状態をイベントで通知
    /// @brief Publish initial state via events
    void publishInitialState() {
#ifdef PAXS_USING_SIMULATOR
        // Simulationの初期状態を通知
        paxs::EventBus::getInstance().publish(SimulationStateChangedEvent(SimulationState::Uninitialized));
#endif
    }

    // ============================================================================
    // ヘルパーメソッド
    // ============================================================================

    /// @brief 日付変更イベントを発行
    /// @brief Publish date changed event
    void publishDateChangedEvent() const {
        const auto& koyomi = domain_core_.getKoyomi();
        auto gregorian_date = koyomi.jdn.toGregorianCalendar();
        paxs::EventBus::getInstance().publish(DateChangedEvent(
            koyomi.jdn.getDay(),
            gregorian_date.getYear(),
            gregorian_date.getMonth(),
            gregorian_date.getDay()
        ));
    }
};

} // namespace paxs

#endif // !PAX_MAHOROBA_CORE_APP_STATE_MANAGER_HPP
