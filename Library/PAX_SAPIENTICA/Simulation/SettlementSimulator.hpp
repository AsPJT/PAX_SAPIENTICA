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

#include <array>
#include <memory>
#include <random>
#include <unordered_map>
#include <vector>

#include <PAX_SAPIENTICA/Simulation/Environment.hpp>
#include <PAX_SAPIENTICA/Simulation/KanakumaLifeSpan.hpp>
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
            }
            catch (const std::runtime_error&) {
                Logger logger("Save/error_log.txt");
                const std::string message = "Failed to read Japan provinces TSV file: " + japan_provinces_path;
                logger.log(Logger::Level::PAX_ERROR, __FILE__, __LINE__, message);
                throw std::runtime_error(message);
            }

            // ランダムに移動確率を設定
            std::uniform_int_distribution<> move_probability_dist{ min_move_probability, max_move_probability };
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
        void run(const int step_count) {
            for (int i = 0; i < step_count; ++i) {
                std::cout << "Step: " << i + 1 << std::endl;
                try {
                    step();
                }
                catch (const std::exception& e) {
                    Logger logger("Save/error_log.txt");
                    logger.log(Logger::Level::PAX_ERROR, __FILE__, __LINE__, e.what());
                    throw e;
                }
            }
        }

        void moveSettlement(const std::uint_least32_t id, const Vector2 current_key, const Vector2 target_key) {
            try {
                // 集落グリッドの移動
                auto it = settlement_grids.find(target_key.toU64());
                if (it != settlement_grids.end()) {
                    it->second.moveSettlementToThis(settlement_grids[current_key.toU64()].getSettlement(id));
                }
                else {
                    SettlementGrid settlement_grid = SettlementGrid(target_key * grid_length, environment, gen);
                    settlement_grid.moveSettlementToThis(settlement_grids[current_key.toU64()].getSettlement(id));
                    settlement_grids[target_key.toU64()] = settlement_grid;
                }
                settlement_grids[current_key.toU64()].deleteSettlement(id);
            }
            catch (const std::exception& e) {
                Logger logger("Save/error_log.txt");
                logger.log(Logger::Level::PAX_ERROR, __FILE__, __LINE__, e.what());
                throw e;
            }
        }

        /// @brief Execute the simulation for the one step.
        /// @brief シミュレーションを1ステップ実行する
        void step() {
            try {
                std::vector<std::tuple<std::uint_least32_t, Vector2, Vector2>> move_list;
                for (auto& settlement_grid  : settlement_grids) {
                    std::vector<Settlement>& settlements = settlement_grid.second.getSettlements();
                    for (std::size_t i = 0; i < settlements.size(); ++i) {
                        if (settlements[i].isMoved()) {
                            continue;
                        }

                        auto [target_id, current_key, target_key] = settlements[i].move(gen, move_probability);

                        if (target_id != 0) {
                            move_list.emplace_back(target_id, current_key, target_key);
                        }
                    }
                }

                for (auto& move : move_list) {
                    moveSettlement(std::get<0>(move), std::get<1>(move), std::get<2>(move));
                }

                for (auto& settlement_grid : settlement_grids) {
                    for (auto& settlement : settlement_grid.second.getSettlements()) {
                        settlement.preUpdate();
                    }
                }

                auto delete_agent = [this](const std::uint_least64_t agent_id, const std::uint_least32_t settlement_id, const Vector2 key) {
                    auto it = settlement_grids.find(key.toU64());
                    if (it != settlement_grids.end()) {
                        it->second.deleteAgent(agent_id, settlement_id);
                    }
                    else {
                        Logger logger("Save/error_log.txt");
                        const std::string message = "Settlement grid not found. Key: " + std::to_string(key.x) + ", " + std::to_string(key.y);
                        logger.log(Logger::Level::PAX_ERROR, __FILE__, __LINE__, message);
                        throw std::runtime_error(message);
                    }
                };

                for (auto& settlement_grid : settlement_grids) {
                    std::vector<Settlement>& settlements = settlement_grid.second.getSettlements();
                    if (settlements.size() == 0) {
                        continue;
                    }

                    // 近隣8グリッドの集落を取得
                    std::vector<Settlement> close_settlements;
                    Vector2 grid_position = settlement_grid.second.getGridPosition();
                    grid_position /= grid_length;
                    for (int i = -1; i <= 1; ++i) {
                        for (int j = -1; j <= 1; ++j) {
                            auto it = settlement_grids.find((grid_position + Vector2(i, j)).toU64());
                            if (it != settlement_grids.end()) {
                                for (auto& settlement : it->second.getSettlements()) {
                                    close_settlements.emplace_back(settlement);
                                }
                            }
                        }
                    }

                    for (auto& settlement : settlements) {
                        settlement.marriage(close_settlements, delete_agent);
                    }
                }

                for (auto& settlement_grid : settlement_grids) {
                    for (auto& settlement : settlement_grid.second.getSettlements()) {
                        settlement.onUpdate();
                    }
                }

                for (auto& settlement_grid : settlement_grids) {
                    settlement_grid.second.checkSettlements();
                }

                for (auto& settlement_grid : settlement_grids) {
                    settlement_grid.second.divideSettlements();
                }
            }
            catch (const std::exception& e) {
                Logger logger("Save/error_log.txt");
                logger.log(Logger::Level::PAX_ERROR, __FILE__, __LINE__, e.what());
                throw e;
            }
        }

        /// @brief Get the agent list.
        /// @brief エージェントのリストを取得する
        constexpr std::unordered_map<std::uint_least64_t, SettlementGrid>&
            getSettlementGrids() noexcept { return settlement_grids; }
        /// @brief Get the agent list.
        /// @brief エージェントのリストを取得する
        constexpr const std::unordered_map<std::uint_least64_t, SettlementGrid>&
            cgetSettlementGrids() const noexcept { return settlement_grids; }

        /// @brief Export the simulation result to a file.
        /// @brief シミュレーション結果をファイルに出力する
        void exportResult(const std::string path) const {
            // 各令制国の人口を計算
            std::unordered_map<std::uint_least8_t, std::uint_least32_t> ryoseikoku_population_map;
            for (auto& settlement_grid : settlement_grids) {
                for (auto& settlement : settlement_grid.second->getSettlements()) {
                    std::uint_least8_t ryoseikoku_id = environment->template getData<std::uint_least8_t>(MurMur3::calcHash("gbank"), settlement.getPosition());
                    auto it = ryoseikoku_population_map.find(ryoseikoku_id);
                    if (it != ryoseikoku_population_map.end()) {
                        it->second += settlement.getPopulation();
                    } else {
                        ryoseikoku_population_map[ryoseikoku_id] = settlement.getPopulation();
                    }
                }
            }

            // 令制国id順にソート
            std::vector<std::pair<std::uint_least8_t, std::uint_least32_t>> ryoseikoku_population_list;
            ryoseikoku_population_list.reserve(ryoseikoku_population_map.size());
            for (auto& ryoseikoku_population : ryoseikoku_population_map) {
                ryoseikoku_population_list.push_back(ryoseikoku_population);
            }

            std::sort(ryoseikoku_population_list.begin(), ryoseikoku_population_list.end(), [](const std::pair<std::uint_least8_t, std::uint_least32_t>& a, const std::pair<std::uint_least8_t, std::uint_least32_t>& b) {
                return a.first < b.first;
            });

            // 計算した結果をファイルに出力
            std::ofstream ofs(path);
            if (!ofs) {
                Logger logger("Save/error_log.txt");
                const std::string message = "Failed to open file: " + path;
                logger.log(Logger::Level::PAX_ERROR, __FILE__, __LINE__, message);
                throw std::runtime_error(message);
            }

            ofs << "ID\tName\tPopulation" << std::endl;
            for (auto& ryoseikoku_population : ryoseikoku_population_map) {
                ofs << static_cast<int>(ryoseikoku_population.first) << "\t" << japan_provinces->cgetRyoseikoku(ryoseikoku_population.first).name << "\t" << ryoseikoku_population.second << std::endl;
            }

            ofs.close();
        }

        /// @brief Export the simulation initialization result to a file.
        /// @brief シミュレーションの初期化結果をファイルに出力する
        void exportInitResult(const std::string path) const {
            // ID, 座標, 人口をファイルに出力
            std::ofstream ofs(path);
            if (!ofs) {
                Logger logger("Save/error_log.txt");
                const std::string message = "Failed to open file: " + path;
                logger.log(Logger::Level::PAX_ERROR, __FILE__, __LINE__, message);
                throw std::runtime_error(message);
            }

            ofs << "ID\tX\tY\tPopulation" << std::endl;
            for (auto& settlement_grid : settlement_grids) {
                for (auto& settlement : settlement_grid.second.getSettlements()) {
                    ofs << settlement.getId() << "\t" << settlement.getPosition().x << "\t" << settlement.getPosition().y << "\t" << settlement.getPopulation() << std::endl;
                }
            }

            ofs.close();
        }

    private:
        std::unordered_map<std::uint_least64_t, SettlementGrid> settlement_grids;
        std::shared_ptr<Environment> environment;

        std::unique_ptr<paxs::JapanProvinces> japan_provinces;
        int move_probability = 0; // 移動確率

        std::mt19937 gen; // 乱数生成器
        std::uniform_int_distribution<> gender_dist{ 0, 1 }; // 性別の乱数分布

        KanakumaLifeSpan kanakuma_life_span;

        /// @brief Randomly place settlements.
        /// @brief 集落をランダムに配置する
        void randomizeSettlements() noexcept {
            std::cout << "Randomize settlements..." << std::endl;

            // 陸の位置のリストを取得
            std::vector<std::uint64_t> land_positions;
            environment->getLandPositions(land_positions);

            struct Live {
                // 可住地重みリスト
                std::vector<int> live_probabilities;
                // 可住地の位置
                std::vector<std::uint64_t> habitable_land_positions;

                void emplaceBack(const int live_probabilities_, const std::uint64_t habitable_land_positions_) {
                    live_probabilities.emplace_back(live_probabilities_);
                    habitable_land_positions.emplace_back(habitable_land_positions_);
                }
            };

            std::unique_ptr<std::array<Live, 90>> live_list(new std::array<Live, 90>());
            if (live_list.get() == nullptr) {
                std::cout << "Low memory" << std::endl;
                return; // 処理失敗
            }

            for (const std::uint64_t land_position : land_positions) {
                // 可住地かどうかを判定
                Vector2 position = Vector2::fromU64(land_position);
                if (!environment->isLive(position)) {
                    continue;
                }

                // 各陸地の可住地重みを計算
                std::uint_least8_t slope = environment->getSlope(position);
                int live_probability = 0;
                if (slope <= 90) {
                    live_probability += 9;
                }
                else if (slope <= 129) {
                    live_probability += 10;
                }
                else if (slope <= 163) {
                    live_probability += 4;
                }
                else {
                    continue;
                }

                // 令制国ごとに人口が決められているので、人口に空きがあるかどうかを判定
                std::uint_least8_t ryoseikoku_id = environment->template getData<std::uint_least8_t>(MurMur3::calcHash("gbank"), position);
                if (ryoseikoku_id < 90) {
                    (*live_list)[ryoseikoku_id].emplaceBack(live_probability, land_position);
                }
            }

            // 令制国と人口のマップ
            std::unordered_map<std::uint_least8_t, std::uint_least32_t> ryoseikoku_population_map;
            for (auto& ryoseikoku : japan_provinces->cgetRyoseikokuList()) {
                if (ryoseikoku.population_ad200 == 0) {
                    continue;
                }
                ryoseikoku_population_map[ryoseikoku.id] = ryoseikoku.population_ad200;
            }

            int all_population = 0;
            for (auto& ryoseikoku_population : ryoseikoku_population_map) {
                all_population += ryoseikoku_population.second;
            }
            std::cout << "All population: " << all_population << std::endl;
            int population_sum = 0;
            // 集落配置
            for (std::uint_least8_t ryoseikoku_id = 0; ryoseikoku_id < 90; ++ryoseikoku_id) {

                Live live = (*live_list)[ryoseikoku_id];

                while (live.live_probabilities.size() > 0 && // 集落を配置し切るまで
                    ryoseikoku_population_map.find(ryoseikoku_id) != ryoseikoku_population_map.end() // 令制国が残っている間
                    ) {
                    StatusDisplayer::displayProgressBar(population_sum, all_population);

                    // 重みからインデックスを取得するための分布
                    std::discrete_distribution<> live_probability_dist(live.live_probabilities.begin(), live.live_probabilities.end());

                    int live_probability_index = live_probability_dist(gen);
                    Vector2 live_position = Vector2::fromU64(live.habitable_land_positions[live_probability_index]);

                    // 令制国ごとに人口が決められているので、人口に空きがあるかどうかを判定
                    // std::uint_least8_t ryoseikoku_id = environment->template getData<std::uint_least8_t>(MurMur3::calcHash("gbank"), live_position);

                    auto ryoseikoku_population_it = ryoseikoku_population_map.find(ryoseikoku_id);
                    if (ryoseikoku_population_it == ryoseikoku_population_map.end()) {
                        live.live_probabilities.erase(live.live_probabilities.begin() + live_probability_index);
                        live.habitable_land_positions.erase(live.habitable_land_positions.begin() + live_probability_index);
                        continue;
                    }

                    // 配置する集落の人口を決定
                    paxs::Ryoseikoku ryoseikoku = japan_provinces->cgetRyoseikoku(ryoseikoku_id);
                    int settlement_population = std::uniform_int_distribution<>(ryoseikoku.settlement_population_min_ad200, ryoseikoku.settlement_population_max_ad200)(gen);
                    settlement_population = std::min(settlement_population, static_cast<int>(ryoseikoku_population_it->second));

                    // 集落をグリッドに配置
                    Vector2 grid_position = live_position / grid_length;
                    std::uint64_t key = grid_position.toU64();
                    // グリッドが存在しない場合は作成
                    if (settlement_grids.find(key) == settlement_grids.end()) {
                        settlement_grids[key] = SettlementGrid(grid_position * grid_length, environment, gen);
                    }
                    // 集落を作成
                    Settlement settlement = Settlement(
                        UniqueIdentification<std::uint_least32_t>::generate(),
                        gen,
                        environment
                    );
                    settlement.setPosition(live_position);

                    settlement.resizeAgents(settlement_population);
                    for (int i = 0; i < settlement_population; ++i) {
                        const std::uint_least8_t set_gender = static_cast<std::uint_least8_t>(gender_dist(gen));
                        const std::uint_least32_t set_lifespan = kanakuma_life_span.setLifeSpan(set_gender, gen);

                        if (set_lifespan == 0) continue;
                        std::uniform_int_distribution<> lifespan_dist{ 0, static_cast<int>(set_lifespan - 1) }; // 性別の乱数分布

                        settlement.setAgent(Agent(UniqueIdentification<std::uint_least64_t>::generate(),
                            0, // TODO: 名前ID
                            set_gender,
                            lifespan_dist(gen),
                            set_lifespan), static_cast<std::size_t>(i));
                    }

                    // 令制国の人口を減らす
                    ryoseikoku_population_it->second -= settlement_population;
                    if (ryoseikoku_population_it->second == 0) {
                        ryoseikoku_population_map.erase(ryoseikoku_population_it);
                    }
                    population_sum += settlement_population;

                    // 集落をグリッドに配置
                    settlement_grids[key].addSettlement(settlement);
                    settlement_grids[key].addRyoseikokuId(ryoseikoku_id);

                    live.live_probabilities.erase(live.live_probabilities.begin() + live_probability_index);
                    live.habitable_land_positions.erase(live.habitable_land_positions.begin() + live_probability_index);
                }
            }
            StatusDisplayer::displayProgressBar(all_population, all_population);
            std::cout << std::endl;

            // 令制国の人口が残っている場合は、ランダムに配置
            for (auto& ryoseikoku_population : ryoseikoku_population_map) {
                std::uint_least8_t ryoseikoku_id = ryoseikoku_population.first;
                int population = ryoseikoku_population.second;
                std::vector<Settlement> settlements;
                getSettlements(settlements, ryoseikoku_id);

                if (settlements.size() == 0) {
                    Logger logger("Save/error_log.txt");
                    const std::string message = "No settlement in ryoseikoku: " + std::to_string(ryoseikoku_id);
                    logger.log(Logger::Level::PAX_WARNING, __FILE__, __LINE__, message);
                    continue;
                }

                int add_population = population / settlements.size();

                for (auto& settlement : settlements) {
                    std::vector<Agent> agents;
                    for (int i = 0; i < add_population; ++i) {
                        const std::uint_least8_t set_gender = static_cast<std::uint_least8_t>(gender_dist(gen));
                        const std::uint_least32_t set_lifespan = kanakuma_life_span.setLifeSpan(set_gender, gen);

                        if (set_lifespan == 0) continue;
                        std::uniform_int_distribution<> lifespan_dist{ 0, static_cast<int>(set_lifespan - 1) }; // 性別の乱数分布

                        Agent agent = Agent(
                            UniqueIdentification<std::uint_least64_t>::generate(),
                            0, // TODO: 名前ID
                            set_gender,
                            lifespan_dist(gen),
                            set_lifespan
                        );
                        agents.emplace_back(agent);
                    }
                    settlement.addAgents(agents);
                }
            }

            std::cout << "Done." << std::endl;
        }

        /// @brief 指定した令制国のIDの集落グリッドを取得
        void getSettlementGrids(std::vector<SettlementGrid>& settlement_grids_, const std::uint_least8_t ryoseikoku_id_) noexcept {
            for (auto& settlement_grid : settlement_grids) {
                std::vector<std::uint_least8_t> ryoseikoku_ids = settlement_grid.second.getRyoseikokuIds();
                for (auto id : ryoseikoku_ids) {
                    if (id == ryoseikoku_id_) {
                        settlement_grids_.emplace_back(settlement_grid.second);
                        break;
                    }
                }
            }
        }

        /// @brief 指定した令制国のIDの集落を取得
        void getSettlements(std::vector<Settlement>& settlements, const std::uint_least8_t ryoseikoku_id_) noexcept {
            std::vector<SettlementGrid> settlement_grids_;
            getSettlementGrids(settlement_grids_, ryoseikoku_id_);
            for (auto& settlement_grid : settlement_grids_) {
                for (auto& settlement : settlement_grid.getSettlements()) {
                    settlements.emplace_back(settlement);
                }
            }
        }
    };
}

#endif // !PAX_SAPIENTICA_SETTLEMENT_SIMULATOR_HPP
