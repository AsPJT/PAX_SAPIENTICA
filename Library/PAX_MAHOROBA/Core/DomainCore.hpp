/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_CORE_DOMAIN_CORE_HPP
#define PAX_MAHOROBA_CORE_DOMAIN_CORE_HPP

#include <PAX_MAHOROBA/Map/MapViewport.hpp>

#include <PAX_SAPIENTICA/Calendar/Koyomi.hpp>

#ifdef PAXS_USING_SIMULATOR
#include <PAX_MAHOROBA/Core/SimulationController.hpp>
#include <PAX_SAPIENTICA/Simulation/Manager/SimulationManager.hpp>
#endif

namespace paxs {

/// @brief ドメインコアクラス
/// @brief Domain core class
class DomainCore {
public:
    DomainCore() = default;

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

    /// @brief ビューポート中心座標を設定（制約適用付き）
    void setViewportCenter(Vector2<double> center) {
        map_viewport_.setCenter(center);
        map_viewport_.applyConstraints();
    }

    /// @brief ビューポートサイズを設定（制約適用付き）
    void setViewportSize(double size) {
        map_viewport_.setSize(size);
        map_viewport_.applyConstraints();
    }

    /// @brief ビューポート変更を通知
    void notifyViewportChanged() {
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
    void navigateDate(double days) {
        koyomi_.jdn.addDays(days);
        koyomi_.calcDate();
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
    }

    /// @brief シミュレーション非同期初期化を開始
    /// @brief Start simulation async initialization
    /// @param model_name モデル名 / Model name
    /// @return 非同期ハンドル / Async handle
    LoadingHandle<bool> startSimulationInitializeAsync(const std::string& model_name) {
        return simulation_manager_.simulationInitializeAsync(model_name);
    }

    /// @brief シミュレーション初期化を完了（非同期版の後処理）
    /// @brief Complete simulation initialization (async version post-processing)
    void completeSimulationInitialization() {
        simulation_manager_.initSimulation();
        koyomi_.jdn.setDay(SimulationConstants::getInstance().start_julian_day);
        resetKoyomiToStoppedState();
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
    }

    /// @brief シミュレーションを一時停止
    /// @brief Pause simulation
    void pauseSimulation() {
        koyomi_.move_forward_in_time = false;
        koyomi_.go_back_in_time = false;
    }

    /// @brief シミュレーションを停止
    /// @brief Stop simulation
    void stopSimulation() {
        koyomi_.is_agent_update = false;
        koyomi_.move_forward_in_time = false;
        koyomi_.go_back_in_time = false;
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
    }

    /// @brief シミュレーションをクリア
    /// @brief Clear simulation
    void clearSimulation() {
        simulation_manager_.clear();
        resetKoyomiStepsAndDate();
    }

    /// @brief 暦を更新
    /// @brief Update Koyomi
    /// @details 暦の再生状態に応じて時間を進行・巻き戻す
    /// @details Advances or rewinds time according to playback state
    /// @details シミュレーションの再生時にはシミュレーションをステップ実行する
    /// @details Steps simulation when simulation playback is active
    /// @return 日付が変更された場合 true / True if date changed
    bool updateKoyomi() {
        if (koyomi_.move_forward_in_time || koyomi_.go_back_in_time) {
            const double old_jdn = koyomi_.jdn.getDay();

            koyomi_.update();

            // シミュレーション実行制御（is_agent_updateがtrueの場合のみ）
            if (koyomi_.is_agent_update && simulation_manager_.isActive()) {
                // シミュレーションを1ステップ実行
                simulation_manager_.step();
            }

            const double new_jdn = koyomi_.jdn.getDay();
            return old_jdn != new_jdn;
        }
        return false;
    }

    /// @brief SimulationControllerを更新
    /// @brief Update SimulationController
    /// @param output_path 出力パス / Output path
    void updateSimulationController(const std::string& output_path) {
        simulation_controller_.update(simulation_manager_, koyomi_, output_path);
    }

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

private:
    Koyomi koyomi_;
    MapViewport map_viewport_;
#ifdef PAXS_USING_SIMULATOR
    SimulationManager simulation_manager_;
    SimulationController simulation_controller_;
#endif
};

} // namespace paxs

#endif // !PAX_MAHOROBA_CORE_DOMAIN_CORE_HPP
