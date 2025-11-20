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
#include <PAX_SAPIENTICA/Key/MenuBarKeys.hpp>
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
        // 機能可視性の初期化
        initializeFeatureVisibility();
        // 初期状態を通知
        publishInitialState();
    }

    // ============================================================================
    // Getter（読み取り専用アクセス）
    // ============================================================================

    /// @brief 暦を取得
    const Koyomi& getKoyomi() const { return koyomi_; }

    /// @brief 地図ビューポートを取得
    const MapViewport& getMapViewport() const { return map_viewport_; }

    /// @brief 地図ビューポートを取得（InputHandler専用）
    MapViewport& getMapViewportForInputHandler() { return map_viewport_; }

#ifdef PAXS_USING_SIMULATOR
    /// @brief シミュレーションマネージャーを取得
    const SimulationManager& getSimulationManager() const { return simulation_manager_; }

    /// @brief シミュレーションコントローラーを取得
    const SimulationController& getSimulationController() const { return simulation_controller_; }
#endif

    /// @brief 機能可視性マネージャーを取得（const版）
    const FeatureVisibilityManager& getVisibilityManager() const { return visibility_manager_; }

    /// @brief アプリケーション状態を取得
    /// @brief Get application state
    /// @return アプリケーション状態 / Application state
    AppState getAppState() const { return app_state_; }

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
    /// @param x X座標 / X coordinate
    /// @param y Y座標 / Y coordinate
    void setViewportCenter(double x, double y) {
        map_viewport_.setCenter(x, y);
        map_viewport_.applyConstraints();
        map_viewport_.notifyViewportChanged();
    }

    /// @brief ビューポートサイズを設定（制約適用・通知付き）
    /// @brief Set viewport size with constraints and notification
    /// @param size サイズ / Size (zoom level)
    void setViewportSize(double size) {
        map_viewport_.setSize(size);
        map_viewport_.applyConstraints();
        map_viewport_.notifyViewportChanged();
    }

    /// @brief 時間再生の状態を設定
    /// @brief Set time playback state
    /// @param forward 順再生フラグ / Forward playback flag
    /// @param backward 逆再生フラグ / Backward playback flag
    void setTimePlayback(bool forward, bool backward) {
        koyomi_.move_forward_in_time = forward;
        koyomi_.go_back_in_time = backward;
    }

    /// @brief 日付を移動
    /// @brief Navigate date
    /// @param days 移動日数 / Days to navigate
    void navigateDate(double days) {
        koyomi_.jdn.addDays(days);
        koyomi_.calcDate();
        publishDateChangedEvent();
    }

#ifdef PAXS_USING_SIMULATOR
    /// @brief シミュレーションを初期化（同期版）
    /// @brief Initialize simulation (synchronous)
    /// @param model_name モデル名 / Model name
    void initializeSimulation(const std::string& model_name) {
        simulation_manager_.simulationInitialize(model_name);
        simulation_manager_.initSimulation();
        koyomi_.jdn.setDay(SimulationConstants::getInstance().start_julian_day);
        resetKoyomiToStoppedState();
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
        loading_handle_ = simulation_manager_.simulationInitializeAsync(model_name);
    }

    /// @brief シミュレーションを再生
    /// @brief Play simulation
    /// @param iterations 実行回数 / Number of iterations
    void playSimulation(int iterations) {
        koyomi_.is_agent_update = true;
        koyomi_.move_forward_in_time = true;
        if (iterations > 1) {
            simulation_controller_.startAutoExecution(iterations, "");
        }
        paxs::EventBus::getInstance().publish(SimulationStateChangedEvent(SimulationState::Playing));
    }

    /// @brief シミュレーションを一時停止
    /// @brief Pause simulation
    void pauseSimulation() {
        koyomi_.move_forward_in_time = false;
        koyomi_.go_back_in_time = false;
        paxs::EventBus::getInstance().publish(SimulationStateChangedEvent(SimulationState::Stopped));
    }

    /// @brief シミュレーションを停止
    /// @brief Stop simulation
    void stopSimulation() {
        koyomi_.is_agent_update = false;
        koyomi_.move_forward_in_time = false;
        koyomi_.go_back_in_time = false;
        paxs::EventBus::getInstance().publish(SimulationStateChangedEvent(SimulationState::Stopped));
    }

    /// @brief シミュレーションをステップ実行
    /// @brief Step simulation
    /// @param steps ステップ数 / Number of steps
    void stepSimulation(int steps) {
        for (int i = 0; i < steps; ++i) {
            simulation_manager_.step();
            koyomi_.steps.setDay(koyomi_.steps.getDay() + 1);
        }
        koyomi_.jdn += steps;
        paxs::EventBus::getInstance().publish(SimulationStepExecutedEvent(
            static_cast<std::uint_least32_t>(koyomi_.steps.getDay()),
            static_cast<std::uint_least32_t>(simulation_manager_.getPopulation())
        ));
        publishDateChangedEvent();
    }

    /// @brief 入力データを再読み込み
    /// @brief Reload input data
    /// @param model_name モデル名 / Model name
    void reloadInputData(const std::string& model_name) {
        simulation_manager_.reloadData(model_name);
    }

    /// @brief 人間データを初期化
    /// @brief Initialize human data
    /// @param model_name モデル名 / Model name
    void initHumanData(const std::string& model_name) {
        simulation_manager_.initHumanData(model_name);
        resetKoyomiToStoppedState();
        paxs::EventBus::getInstance().publish(SimulationStateChangedEvent(SimulationState::Stopped));
        publishDateChangedEvent();
    }

    /// @brief シミュレーションをクリア
    /// @brief Clear simulation
    void clearSimulation() {
        simulation_manager_.clear();
        resetKoyomiStepsAndDate();
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
    Koyomi koyomi_;
    MapViewport map_viewport_;
#ifdef PAXS_USING_SIMULATOR
    SimulationManager simulation_manager_;
    SimulationController simulation_controller_;
    LoadingHandle<bool> loading_handle_;  ///< 非同期ロード用ハンドル
#endif
    FeatureVisibilityManager visibility_manager_;
    AppState app_state_ = AppState::Running;

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
};

} // namespace paxs

#endif // !PAX_MAHOROBA_CORE_APP_STATE_MANAGER_HPP
