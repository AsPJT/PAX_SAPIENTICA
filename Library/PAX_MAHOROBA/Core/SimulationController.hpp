/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]      2023-2024 As Project
    [Production]    2023-2024 As Project
    [Contact Us]    wanotaitei@gmail.com            https://github.com/AsPJT/PAX_SAPIENTICA
    [License]       Distributed under the CC0 1.0.  https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SIMULATION_CONTROLLER_HPP
#define PAX_SAPIENTICA_SIMULATION_CONTROLLER_HPP

#include <cstddef>
#include <string>

#include <PAX_SAPIENTICA/Calendar/Koyomi.hpp>
#include <PAX_SAPIENTICA/EventSystem/ApplicationEvents.hpp>
#include <PAX_SAPIENTICA/EventSystem/EventBus.hpp>
#include <PAX_SAPIENTICA/Simulation/SimulationConst.hpp>
#include <PAX_SAPIENTICA/Simulation/SimulationManager.hpp>

namespace paxs {

/// @brief シミュレーションの自動実行制御を担当するクラス
/// @brief Simulation auto-execution controller
/// @details UIから分離されたビジネスロジック層
class SimulationController {
public:
    explicit SimulationController() {
        // シミュレーション停止イベントを購読
        EventBus::getInstance().subscribe<SimulationStopCommandEvent>(
            [this](const SimulationStopCommandEvent& /*event*/) {
                stopAutoExecution();
            }
        );
    }

    /// @brief シミュレーションの自動実行状態を更新
    /// @brief Update auto-execution state
    /// @param simulation_manager シミュレーションマネージャー
    /// @param koyomi 暦
    /// @param model_name モデル名
    void update(
        const SimulationManager& simulation_manager,
        const Koyomi& koyomi,
        const std::string& model_name
    ) {
        if (!is_auto_executing_) return;
        if (!simulation_manager.isActive()) return;

        // モデル名が変更された場合は total_steps を更新
        if (current_model_name_ != model_name) {
            current_model_name_ = model_name;
            const auto* constants = SimulationConstants::getInstance(model_name);
            total_steps_ = constants->total_steps;
        }

        // シミュレーション完了チェック
        if (isSimulationCompleted(koyomi)) {
            remaining_iterations_--;

            if (remaining_iterations_ > 0) {
                // 次のサイクルを開始
                startNextCycle();
            } else {
                // 全サイクル完了
                EventBus::getInstance().publish(SimulationStopCommandEvent());
                is_auto_executing_ = false;
                remaining_iterations_ = 0;
            }
        }
    }

    /// @brief 自動実行を開始
    /// @brief Start auto-execution
    /// @param iterations 実行回数
    /// @param model_name モデル名
    void startAutoExecution(int iterations, const std::string& model_name) {
        remaining_iterations_ = iterations;
        is_auto_executing_ = true;
        current_model_name_ = model_name;

        // total_stepsを取得
        const auto* constants = SimulationConstants::getInstance(model_name);
        total_steps_ = constants->total_steps;
    }

    /// @brief 自動実行を停止
    /// @brief Stop auto-execution
    void stopAutoExecution() {
        is_auto_executing_ = false;
        remaining_iterations_ = 0;
    }

    /// @brief 残り実行回数を取得
    /// @brief Get remaining iterations
    int getRemainingIterations() const { return remaining_iterations_; }

    /// @brief 自動実行中かどうか
    /// @brief Check if auto-execution is running
    bool isAutoExecuting() const { return is_auto_executing_; }

private:
    int remaining_iterations_ = 0;
    bool is_auto_executing_ = false;
    std::string current_model_name_;
    int total_steps_ = 0;

    /// @brief シミュレーションが完了したかチェック
    /// @brief Check if simulation is completed
    bool isSimulationCompleted(const Koyomi& koyomi) const {
        if (total_steps_ <= 0) return false;
        return koyomi.steps.cgetDay() >= static_cast<std::size_t>(total_steps_);
    }

    /// @brief 次の実行サイクルを開始
    /// @brief Start next execution cycle
    void startNextCycle() {
        paxs::EventBus& event_bus = paxs::EventBus::getInstance();
        // 人間データ初期化イベントを発行
        event_bus.publish(InitHumanDataCommandEvent(current_model_name_));

        // 再生イベントを発行
        event_bus.publish(SimulationPlayCommandEvent(remaining_iterations_));
    }
};

} // namespace paxs

#endif // !PAX_SAPIENTICA_SIMULATION_CONTROLLER_HPP
