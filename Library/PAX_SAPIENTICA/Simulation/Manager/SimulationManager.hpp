/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]      2023-2025 As Project
    [Production]    2023-2025 As Project
    [Contact Us]    wanotaitei@gmail.com         https://github.com/AsPJT/PAX_SAPIENTICA
    [License]       Distributed under the CC0 1.0. https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SIMULATION_MANAGER_SIMULATION_MANAGER_HPP
#define PAX_SAPIENTICA_SIMULATION_MANAGER_SIMULATION_MANAGER_HPP

#include <memory>
#include <random>
#include <string>
#include <utility>

#include <PAX_SAPIENTICA/System/AppConfig.hpp>
#include <PAX_SAPIENTICA/Calendar/JulianDayNumber.hpp>
#include <PAX_SAPIENTICA/Simulation/Manager/SettlementSimulator.hpp>
#include <PAX_SAPIENTICA/Simulation/Config/SimulationConst.hpp>
#include <PAX_SAPIENTICA/IO/File/FileSystem.hpp>
#include <PAX_SAPIENTICA/Utility/Logger.hpp>
#include <PAX_SAPIENTICA/Utility/MurMur3.hpp>
#include <PAX_SAPIENTICA/Core/Utility/StringUtils.hpp>

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

        /// @brief モデル名からパスを生成
        /// @param model_name モデル名 / Model name
        /// @return <マップリストパス, 都道府県パス> / <map list path, provinces path>
        std::pair<std::string, std::string> generatePaths(const std::string& model_name) const {
            std::string map_list_path = AppConfig::getInstance()->getSettingPath(MurMur3::calcHash("SimulationXYZTiles"));
            std::string japan_provinces_path = AppConfig::getInstance()->getSettingPath(MurMur3::calcHash("SimulationProvincesPath"));

            paxs::StringUtils::replace(map_list_path, "Sample", model_name);
            paxs::StringUtils::replace(japan_provinces_path, "Sample", model_name);

            return {map_list_path, japan_provinces_path};
        }

        /// @brief シミュレーションを初期化
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

    public:
        SimulationManager() = default;
        ~SimulationManager() = default;

        // コピー・移動禁止
        SimulationManager(const SimulationManager&) = delete;
        SimulationManager& operator=(const SimulationManager&) = delete;
        SimulationManager(SimulationManager&&) = delete;
        SimulationManager& operator=(SimulationManager&&) = delete;

        /// @brief シミュレーションをクリア（初期化前の状態に戻す）
        void clear() {
            simulator_.reset();       // シミュレータを破棄
            is_initialized_ = false;  // 初期化フラグをfalseに
            current_model_name_.clear(); // モデル名をクリア
        }

        /// @brief シミュレーションを初期化（集落データをクリア）
        void initSimulation() {
            if (simulator_) {
                simulator_->init();
            }
        }

        /// @brief シミュレーションを1ステップ実行
        void step() {
            if (simulator_) {
                simulator_->step();
            }
        }

        /// @brief シミュレーションの入力データを再読み込み
        /// @param model_name モデル名（オプショナル） / Model name (
        void reloadInputData(const std::string& model_name = "") {
            if (simulator_) {
                simulator_->reloadInputData(model_name);
            }
        }

        /// @brief シミュレーションが初期化されているか
        /// @return 初期化されていればtrue / true if initialized
        bool isInitialized() const {
            return is_initialized_ && simulator_ != nullptr;
        }

        /// @brief シミュレーションが実行中か（有効か）
        /// @return 実行中であればtrue / true if active
        bool isActive() const {
            return simulator_ != nullptr;
        }

        /// @brief 現在のモデル名を取得
        /// @return モデル名 / Model name
        const std::string& getCurrentModelName() const {
            return current_model_name_;
        }

        // ========================================
        // ビジネスロジックAPI
        // Business Logic API
        // ========================================

        /// @brief 地理データを読み込んでシミュレーションを初期化
        /// @param model_name モデル名 / Model name
        /// @param seed 乱数シード（0の場合は自動生成） / Random seed (auto-generated if 0)
        void simulationInitialize(const std::string& model_name, std::uint_least32_t seed = 0) {
            SimulationConstants::getInstance(model_name)->init(model_name);

            auto [map_list_path, japan_provinces_path] = generatePaths(model_name);

            // パスの存在確認
            if (!FileSystem::exists(map_list_path)) {
                PAXS_WARNING("Model '" + model_name + "' does not exist (MapList not found: " + map_list_path + "). Available models: Sample, EpiJomon, Yaponesia, AynuMosir, Philippines, Aotearoa");
                return;
            }
            if (!FileSystem::exists(japan_provinces_path)) {
                PAXS_WARNING("Model '" + model_name + "' does not exist (Provinces path not found: " + japan_provinces_path + "). Available models: Sample, EpiJomon, Yaponesia, AynuMosir, Philippines, Aotearoa");
                return;
            }

            initialize(map_list_path, japan_provinces_path, seed, model_name);
        }

        /// @brief 人間データを初期化
        /// @param model_name モデル名 / Model name
        void initHumanData(const std::string& model_name) {
            // モデル名を更新
            current_model_name_ = model_name;
            // シミュレーションを初期化
            initSimulation();
        }

        /// @brief 入力データを再読み込み
        /// @param model_name モデル名 / Model name
        void reloadData(const std::string& model_name) {
            SimulationConstants::getInstance(model_name)->init(model_name);
            reloadInputData(model_name);
            current_model_name_ = model_name;
        }

        // ========================================
        // データ取得API（読み取り専用）
        // Data retrieval API (read-only)
        // ========================================

        /// @brief 総人口を取得
        /// @return 総人口 / Total population
        std::size_t getPopulation() const {
            return simulator_ ? simulator_->cgetPopulationNum() : 0;
        }

        /// @brief 集落数を取得
        /// @return 集落数 / Number of settlements
        std::size_t getSettlementCount() const {
            return simulator_ ? simulator_->cgetSettlement() : 0;
        }

        /// @brief 集落グリッドへのconst参照を取得（描画用）
        /// @return 集落グリッド / Settlement grids
        const auto& getSettlementGrids() const {
            static const paxs::UnorderedMap<SettlementGridsType, SettlementGrid> empty_map;
            return simulator_ ? simulator_->cgetSettlementGrids() : empty_map;
        }

        /// @brief 婚姻位置リストへのconst参照を取得（描画用）
        /// @return 婚姻位置リスト / Marriage position list
        const auto& getMarriagePositions() const {
            static const std::vector<GridType4> empty_vec;
            return simulator_ ? simulator_->getMarriagePosList() : empty_vec;
        }
    };

} // namespace paxs

#endif // !PAX_SAPIENTICA_SIMULATION_MANAGER_SIMULATION_MANAGER_HPP
