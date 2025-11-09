/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]      2023-2025 As Project
    [Production]    2023-2025 As Project
    [Contact Us]    wanotaitei@gmail.com         https://github.com/AsPJT/PAX_SAPIENTICA
    [License]       Distributed under the CC0 1.0. https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SIMULATION_MANAGER_HPP
#define PAX_SAPIENTICA_SIMULATION_MANAGER_HPP

#include <memory>
#include <random>
#include <string>

#include <PAX_SAPIENTICA/Calendar/JulianDayNumber.hpp>
#include <PAX_SAPIENTICA/Simulation/SettlementSimulator.hpp>

namespace paxs {

    /// @brief シミュレーション管理クラス
    /// @brief Simulation Manager Class
    /// @details SettlementSimulatorのファサードとして機能し、UIコンポーネントからの
    ///          直接アクセスを防ぐ。シミュレーションの初期化、実行、状態取得を管理する。
    ///          Acts as a facade for SettlementSimulator, preventing direct access from UI components.
    ///          Manages simulation initialization, execution, and state retrieval.
    class SimulationManager {
    private:
        /// @brief シミュレータインスタンス
        /// @brief Simulator instance
        std::unique_ptr<paxs::SettlementSimulator> simulator_;

        /// @brief シミュレーションが初期化されているか
        bool is_initialized_ = false;

        /// @brief 現在のシミュレーションモデル名
        std::string current_model_name_;

    public:
        SimulationManager() = default;
        ~SimulationManager() = default;

        // コピー・移動禁止
        SimulationManager(const SimulationManager&) = delete;
        SimulationManager& operator=(const SimulationManager&) = delete;
        SimulationManager(SimulationManager&&) = delete;
        SimulationManager& operator=(SimulationManager&&) = delete;

        /// @brief シミュレーションを初期化
        /// @brief Initialize simulation
        /// @param map_list_path マップリストファイルパス / Map list file path
        /// @param provinces_path 行政区画ファイルパス / Provinces file path
        /// @param seed 乱数シード（オプショナル） / Random seed (optional)
        /// @param model_name モデル名（オプショナル） / Model name (optional)
        void initialize(
            const std::string& map_list_path,
            const std::string& provinces_path,
            std::uint_least32_t seed = 0,
            const std::string& model_name = ""
        ) {
            // シードが0の場合はランダムに生成
            if (seed == 0) {
                std::random_device seed_gen;
                seed = seed_gen();
            }

            // 新しいシミュレータを作成
            simulator_ = std::make_unique<paxs::SettlementSimulator>(
                map_list_path,
                provinces_path,
                seed
            );

            current_model_name_ = model_name;
            is_initialized_ = true;
        }

        /// @brief シミュレーションをクリア（初期化前の状態に戻す）
        /// @brief Clear simulation (return to uninitialized state)
        void clear() {
            simulator_.reset();       // シミュレータを破棄
            is_initialized_ = false;  // 初期化フラグをfalseに
            current_model_name_.clear(); // モデル名をクリア
        }

        /// @brief シミュレーションを初期化（集落データをクリア）
        /// @brief Initialize simulation (clear settlement data)
        void initSimulation() {
            if (simulator_) {
                simulator_->init();
            }
        }

        /// @brief シミュレーションを1ステップ実行
        /// @brief Execute one step of simulation
        void step() {
            if (simulator_) {
                simulator_->step();
            }
        }

        /// @brief シミュレーションが初期化されているか
        /// @brief Check if simulation is initialized
        /// @return 初期化されていればtrue / true if initialized
        bool isInitialized() const {
            return is_initialized_ && simulator_ != nullptr;
        }

        /// @brief シミュレーションが実行中か（有効か）
        /// @brief Check if simulation is active
        /// @return 実行中であればtrue / true if active
        bool isActive() const {
            return simulator_ != nullptr;
        }

        /// @brief 現在のモデル名を取得
        /// @brief Get current model name
        /// @return モデル名 / Model name
        const std::string& getCurrentModelName() const {
            return current_model_name_;
        }

        // ========================================
        // データ取得API（読み取り専用）
        // Data retrieval API (read-only)
        // ========================================

        /// @brief 総人口を取得
        /// @brief Get total population
        /// @return 総人口 / Total population
        std::size_t getPopulation() const {
            return simulator_ ? simulator_->cgetPopulationNum() : 0;
        }

        /// @brief 集落数を取得
        /// @brief Get number of settlements
        /// @return 集落数 / Number of settlements
        std::size_t getSettlementCount() const {
            return simulator_ ? simulator_->cgetSettlement() : 0;
        }

        /// @brief 集落グリッドへのconst参照を取得（描画用）
        /// @brief Get const reference to settlement grids (for rendering)
        /// @return 集落グリッド / Settlement grids
        const auto& getSettlementGrids() const {
            static const paxs::UnorderedMap<SettlementGridsType, SettlementGrid> empty_map;
            return simulator_ ? simulator_->cgetSettlementGrids() : empty_map;
        }

        /// @brief 婚姻位置リストへのconst参照を取得（描画用）
        /// @brief Get const reference to marriage position list (for rendering)
        /// @return 婚姻位置リスト / Marriage position list
        const auto& getMarriagePositions() const {
            static const std::vector<GridType4> empty_vec;
            return simulator_ ? simulator_->getMarriagePosList() : empty_vec;
        }
    };

} // namespace paxs

#endif // !PAX_SAPIENTICA_SIMULATION_MANAGER_HPP
