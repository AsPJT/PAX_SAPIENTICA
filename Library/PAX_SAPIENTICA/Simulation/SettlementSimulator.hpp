/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023 As Project
    [Production]	2023 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SETTLEMENT_SIMULATOR_HPP
#define PAX_SAPIENTICA_SETTLEMENT_SIMULATOR_HPP

/*##########################################################################################

##########################################################################################*/

#include <cstdint>

#include <memory>
#include <random>
#include <unordered_map>
#include <vector>

#include <PAX_SAPIENTICA/Simulation/Environment.hpp>
#include <PAX_SAPIENTICA/Simulation/JapanProvinces.hpp>
#include <PAX_SAPIENTICA/Simulation/Settlement.hpp>
#include <PAX_SAPIENTICA/Simulation/SettlementGrid.hpp>
#include <PAX_SAPIENTICA/Simulation/SimulationConst.hpp>
#include <PAX_SAPIENTICA/UniqueIdentification.hpp>

namespace paxs {

    template <typename GridType>
    class SettlementSimulator {
    public:
        using Agent = paxs::SettlementAgent<GridType>;
        using Environment = paxs::Environment<GridType>;
        using Settlement = paxs::Settlement<GridType>;
        using SettlementGrid = paxs::SettlementGrid<GridType>;
        using Vector2 = paxs::Vector2<GridType>;

        constexpr explicit SettlementSimulator() = default;

        explicit SettlementSimulator(const std::string& map_list_path, const std::string& japan_provinces_path, const Vector2& start_position, const Vector2& end_position, const int z, const unsigned seed = 0) :
            environment(std::make_unique<Environment>(map_list_path, start_position, end_position, z)), gen(seed) {
                if (z <= 0) {
                    Logger logger("Save/error_log.txt");
                    const std::string message = "Z must be greater than 0.";
                    logger.log(Logger::Level::PAX_ERROR, __FILE__, __LINE__, message);
                    throw std::runtime_error(message);
                }
                if (start_position.x < 0 || start_position.y < 0 || end_position.x < 0 || end_position.y < 0) {
                    Logger logger("Save/error_log.txt");
                    const std::string message = "Start position and end position must be greater than or equal to 0.";
                    logger.log(Logger::Level::PAX_ERROR, __FILE__, __LINE__, message);
                    throw std::runtime_error(message);
                }

                try {
                    japan_provinces = std::make_unique<paxs::JapanProvinces>(japan_provinces_path + "/JapanRegion.tsv", japan_provinces_path + "/Ryoseikoku.tsv");
                } catch (const std::runtime_error&) {
                    Logger logger("Save/error_log.txt");
                    const std::string message = "Failed to read Japan provinces TSV file: " + japan_provinces_path;
                    logger.log(Logger::Level::PAX_ERROR, __FILE__, __LINE__, message);
                    throw std::runtime_error(message);
                }

                // ランダムに移動確率を設定
                std::uniform_int_distribution<> move_probability_dist{min_move_probability, max_move_probability};
                move_probability = move_probability_dist(gen);
            }

        /// @brief Initialize the simulator.
        /// @brief 集落の初期化
        /// @details 集落をクリアし、地域ごとに指定されたエージェント数になるようにランダムに配置する
        void init() {
            settlement_grids.clear();
            randomizeSettlements();
        }

        /// @brief Run the simulation for the specified number of steps.
        /// @brief シミュレーションを指定されたステップ数だけ実行する
        void run(const int step_count) noexcept {
            for(int i = 0; i < step_count; ++i) {
                step();
            }
        }

        /// @brief Execute the simulation for the one step.
        /// @brief シミュレーションを1ステップ実行する
        void step() noexcept {
            for (auto& settlement_grid : settlement_grids) {
                auto settlement_grid_update = [this](const Vector2 current_key, const Vector2 target_key, std::uint_least32_t id)
                {
                    // 集落グリッドの移動
                    if (current_key != target_key) {
                        auto it = settlement_grids.find(target_key.toU64());
                        if (it != settlement_grids.end()) {
                            it->second->addSettlement(settlement_grids[current_key.toU64()]->getSettlement(id));
                        } else {
                            settlement_grids[target_key.toU64()] = std::make_shared<SettlementGrid>(target_key * grid_length, environment, gen());
                            settlement_grids[target_key.toU64()]->moveSettlementToThis(settlement_grids[current_key.toU64()]->getSettlement(id));
                        }
                        settlement_grids[current_key.toU64()]->deleteSettlement(id);
                    }
                };

                for (auto& settlement : settlement_grid.second->getSettlements()) {
                    settlement->move(gen, move_probability, settlement_grid_update);
                }
            }

            for (auto& settlement_grid : settlement_grids) {
                for (auto& settlement : settlement_grid.second->getSettlements()) {
                    settlement->preUpdate();
                }
            }

            for (auto& settlement_grid : settlement_grids) {
                // 近隣8グリッドの集落を取得
                std::vector<std::shared_ptr<Settlement>> settlements;
                Vector2 grid_position = settlement_grid.second->getGridPosition();
                grid_position /= grid_length;
                for (int i = -1; i <= 1; ++i) {
                    for (int j = -1; j <= 1; ++j) {
                        auto it = settlement_grids.find((grid_position + Vector2(i, j)).toU64());
                        if (it != settlement_grids.end()) {
                            for (auto& settlement : it->second->getSettlements()) {
                                settlements.push_back(settlement);
                            }
                        }
                    }
                }

                for (auto& settlement : settlement_grid.second->getSettlements()) {
                    settlement->marriage(settlements);
                }
            }

            for (auto& settlement_grid : settlement_grids) {
                for (auto& settlement : settlement_grid.second->getSettlements()) {
                    settlement->onUpdate();
                }
            }
        }

    private:
        std::unordered_map<std::uint_least64_t, std::shared_ptr<SettlementGrid>> settlement_grids;
        std::shared_ptr<Environment> environment;
        std::mt19937 gen; // 乱数生成器
        std::uniform_int_distribution<> gender_dist{0, 1}; // 性別の乱数分布
        std::uniform_int_distribution<> life_exp_dist{50, 100}; // 寿命の乱数分布
        std::unique_ptr<paxs::JapanProvinces> japan_provinces;
        int move_probability = 0; // 移動確率

        /// @brief Randomly place settlements.
        /// @brief 集落をランダムに配置する
        void randomizeSettlements() noexcept {
            // TODO: 日本の地方区分の人口数をもとに、集落をランダムに配置する

        }

    };
}

#endif // !PAX_SAPIENTICA_SETTLEMENT_SIMULATOR_HPP
