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
#include <PAX_SAPIENTICA/StatusDisplayer.hpp>
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
            std::cout << "Randomize settlements..." << std::endl;

            // 陸の位置のリストを取得
            std::vector<std::uint64_t> land_positions;
            environment->getLandPositions(land_positions);

            // 可住地重みリスト
            std::vector<int> live_probabilities;
            // 可住地の位置
            std::vector<std::uint64_t> habitable_land_positions;
            for (const std::uint64_t land_position : land_positions) {
                // 可住地かどうかを判定
                Vector2 position = Vector2::fromU64(land_position);
                if (!environment->isLive(position)) {
                    continue;
                }

                // 各陸地の可住地重みを計算
                float slope = environment->getSlope(position);
                int live_probability = 0;
                if (slope < 4) {
                    live_probability += 9;
                } else if (slope < 9.09) {
                    live_probability += 10;
                } else if (slope < 17.745) {
                    live_probability += 4;
                } else {
                    continue;
                }

                live_probabilities.push_back(live_probability);
                habitable_land_positions.push_back(land_position);
            }

            // 令制国と人口のマップ
            std::unordered_map<std::uint_least8_t, std::uint_least32_t> ryoseikoku_population_map;
            for (auto& ryoseikoku : japan_provinces->cgetRyoseikokuList()) {
                if (ryoseikoku.population == 0) {
                    continue;
                }
                ryoseikoku_population_map[ryoseikoku.id] = ryoseikoku.population;
            }

            int all_population = 0;
            for (auto& ryoseikoku_population : ryoseikoku_population_map) {
                all_population += ryoseikoku_population.second;
            }
            std::cout << "All population: " << all_population << std::endl;
            int population_sum = 0;
            // 集落配置
            while (live_probabilities.size() > 0 && // 集落を配置し切るまで
                ryoseikoku_population_map.size() > 0 // 令制国が残っている間
                ) {
                StatusDisplayer::displayProgressBar(population_sum, all_population);
                if (ryoseikoku_population_map.size() <= 10) {
                    // 残っている令制国を表示
                    std::cout << "Ryoseikoku: ";
                    for (auto& ryoseikoku_population : ryoseikoku_population_map) {
                        std::cout << ryoseikoku_population.first << " ";
                    }
                }

                // 重みからインデックスを取得するための分布
                std::discrete_distribution<> live_probability_dist(live_probabilities.begin(), live_probabilities.end());

                int live_probability_index = live_probability_dist(gen);
                Vector2 live_position = Vector2::fromU64(habitable_land_positions[live_probability_index]);

                // 令制国ごとに人口が決められているので、人口に空きがあるかどうかを判定
                std::uint_least8_t ryoseikoku_id = environment->template getData<std::uint_least8_t>("gbank", live_position);

                auto ryoseikoku_population_it = ryoseikoku_population_map.find(ryoseikoku_id);
                if (ryoseikoku_population_it == ryoseikoku_population_map.end()) {
                    live_probabilities.erase(live_probabilities.begin() + live_probability_index);
                    habitable_land_positions.erase(habitable_land_positions.begin() + live_probability_index);
                    continue;
                }

                // TODO: 配置する集落の人口を決定
                int settlement_population = 50;
                settlement_population = std::min(settlement_population, static_cast<int>(ryoseikoku_population_it->second));

                // 集落をグリッドに配置
                std::uint64_t key = (live_position / grid_length).toU64();
                // グリッドが存在しない場合は作成
                if (settlement_grids.find(key) == settlement_grids.end()) {
                    settlement_grids[key] = std::make_shared<SettlementGrid>(live_position, environment, gen());
                }
                // 集落を作成
                std::shared_ptr<Settlement> settlement = std::make_shared<Settlement>(
                    UniqueIdentification<std::uint_least32_t>::generate(),
                    gen(),
                    environment
                );
                settlement->setPosition(live_position);

                std::vector<std::shared_ptr<Agent>> agents;
                agents.reserve(settlement_population);
                for (int i = 0; i < settlement_population; ++i) {
                    std::shared_ptr<Agent> agent = std::make_shared<Agent>(
                        UniqueIdentification<std::uint_least64_t>::generate(),
                    0, // TODO: 名前ID
                    static_cast<std::uint_least8_t>(gender_dist(gen)),
                    0,
                    static_cast<std::uint_least8_t>(life_exp_dist(gen)),
                    environment
                    );
                    agents.push_back(agent);
                }
                settlement->setAgents(agents);

                // 令制国の人口を減らす
                ryoseikoku_population_it->second -= settlement_population;
                if (ryoseikoku_population_it->second == 0) {
                    ryoseikoku_population_map.erase(ryoseikoku_population_it);
                }
                population_sum += settlement_population;

                // 集落をグリッドに配置
                settlement_grids[key]->addSettlement(settlement);
                settlement_grids[key]->addRyoseikokuId(ryoseikoku_id);

                live_probabilities.erase(live_probabilities.begin() + live_probability_index);
                habitable_land_positions.erase(habitable_land_positions.begin() + live_probability_index);
            }

            StatusDisplayer::displayProgressBar(all_population, all_population);
            std::cout << std::endl;

            // 令制国の人口が残っている場合は、ランダムに配置
            for (auto& ryoseikoku_population : ryoseikoku_population_map) {
                std::uint_least8_t ryoseikoku_id = ryoseikoku_population.first;
                int population = ryoseikoku_population.second;
                std::vector<std::shared_ptr<Settlement>> settlements;
                getSettlements(settlements, ryoseikoku_id);

                int add_population = population / settlements.size();

                for (auto& settlement : settlements) {
                    std::vector<std::shared_ptr<Agent>> agents;
                    for (int i = 0; i < add_population; ++i) {
                        std::shared_ptr<Agent> agent = std::make_shared<Agent>(
                            UniqueIdentification<std::uint_least64_t>::generate(),
                        0, // TODO: 名前ID
                        static_cast<std::uint_least8_t>(gender_dist(gen)),
                        0,
                        static_cast<std::uint_least8_t>(life_exp_dist(gen)),
                        environment
                        );
                        agents.push_back(agent);
                    }
                    settlement->addAgents(agents);
                }
            }
        }

        /// @brief 指定した令制国のIDの集落グリッドを取得
        void getSettlementGrids(std::vector<std::shared_ptr<SettlementGrid>>& settlement_grids, const std::uint_least8_t ryoseikoku_id) noexcept {
            for (auto& settlement_grid : settlement_grids) {
                for (auto id : settlement_grid->getRyoseikokuIds()) {
                    if (id == ryoseikoku_id) {
                        settlement_grids.push_back(settlement_grid);
                        break;
                    }
                }
            }
        }

        /// @brief 指定した令制国のIDの集落を取得
        void getSettlements(std::vector<std::shared_ptr<Settlement>>& settlements, const std::uint_least8_t ryoseikoku_id) noexcept {
            std::vector<std::shared_ptr<SettlementGrid>> settlement_grids;
            getSettlementGrids(settlement_grids, ryoseikoku_id);
            for (auto& settlement_grid : settlement_grids) {
                for (auto& settlement : settlement_grid->getSettlements()) {
                    settlements.push_back(settlement);
                }
            }
        }
    };
}

#endif // !PAX_SAPIENTICA_SETTLEMENT_SIMULATOR_HPP
