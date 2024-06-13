/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SETTLEMENT_SIMULATOR_HPP
#define PAX_SAPIENTICA_SETTLEMENT_SIMULATOR_HPP

/*##########################################################################################

##########################################################################################*/

#include <cstdint>

#include <array>
#include <chrono>
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

    class SettlementSimulator {
    public:
        using Agent = paxs::SettlementAgent;
        using Vector2 = paxs::Vector2<GridType>;

        explicit SettlementSimulator() = default;

        explicit SettlementSimulator(const std::string& map_list_path, const std::string& japan_provinces_path, const unsigned seed = 0) noexcept :
            environment(std::make_unique<Environment>(map_list_path)), gen(seed) {
            japan_provinces = std::make_unique<paxs::JapanProvinces>(japan_provinces_path);

            // ランダムに移動確率を設定
            std::uniform_int_distribution<> move_probability_dist{ SimulationConstants::getInstance()->min_move_probability, SimulationConstants::getInstance()->max_move_probability };
            move_probability = move_probability_dist(gen);
        }
        /// @brief 環境を設定
        void setEnvironment(const std::string& map_list_path, const std::string& japan_provinces_path, const int z, const unsigned seed = 0) noexcept {
            environment.reset();
            environment = std::make_unique<Environment>(map_list_path);

            gen = std::mt19937(seed);

            japan_provinces.reset();
            japan_provinces = std::make_unique<paxs::JapanProvinces>(japan_provinces_path);

            // ランダムに移動確率を設定
            std::uniform_int_distribution<> move_probability_dist{ SimulationConstants::getInstance()->min_move_probability, SimulationConstants::getInstance()->max_move_probability };
            move_probability = move_probability_dist(gen);
        }

        /// @brief
        /// @brief 渡来数の取得
        std::uint_least64_t emigrationSize() {
            return emigration_count;
        }

        /// @brief
        /// @brief 生命表を取得
        void outputLifeSpan(const int loop_num_) {
            std::ofstream life_ofs = std::ofstream("life.txt");
            std::vector<int> life_num1(SimulationConstants::getInstance()->steps_per_year * 100, 0);
            std::vector<int> life_num2(SimulationConstants::getInstance()->steps_per_year * 100, 0);
            for (int i = 0; i < loop_num_; ++i) {
                life_num1[kanakuma_life_span.setLifeSpan(0, gen)] += 1;
                life_num2[kanakuma_life_span.setLifeSpan(1, gen)] += 1;
            }
            life_ofs << "step\tyear\tfemale\tmale\n";
            for (int i = 0; i < SimulationConstants::getInstance()->steps_per_year * 100; ++i) {
                life_ofs << i << '\t' << i / SimulationConstants::getInstance()->steps_per_year << '\t' << life_num1[i] << '\t' << life_num2[i] << '\n';
            }
            return;
        }

        /// @brief Initialize the simulator.
        /// @brief 集落の初期化
        /// @details 集落をクリアし、地域ごとに指定されたエージェント数になるようにランダムに配置する
        void init() {
            settlement_grids.clear();
            initRandomizeSettlements();
            randomizeSettlements(0, 0, 255, 0/*縄文人は SNP:0*/);
        }

        /// @brief Run the simulation for the specified number of steps.
        /// @brief シミュレーションを指定されたステップ数だけ実行する
        void run(const int step_count_) noexcept {
            for (int i = 0; i < step_count_; ++i) {
                std::cout << "Step: " << i + 1 << std::endl;
                step();
            }
        }

        void moveSettlement(const std::uint_least32_t id, const Vector2 current_key, const Vector2 target_key) noexcept {
            auto it = settlement_grids.find(target_key.to<SettlementGridsType>());
            if (it != settlement_grids.end()) {
                it->second.moveSettlementToThis(settlement_grids[current_key.to<SettlementGridsType>()].getSettlement(id));
            }
            else {
                SettlementGrid settlement_grid = SettlementGrid(target_key * SimulationConstants::getInstance()->grid_length, environment, gen);
                settlement_grid.moveSettlementToThis(settlement_grids[current_key.to<SettlementGridsType>()].getSettlement(id));
                settlement_grids[target_key.to<SettlementGridsType>()] = settlement_grid;
            }
            settlement_grids[current_key.to<SettlementGridsType>()].deleteSettlement(id);
        }

        /// @brief Execute the simulation for the one step.
        /// @brief シミュレーションを1ステップ実行する
        void step() noexcept {
            std::chrono::system_clock::time_point  start_time, end_time, move_time, m_start_time, m_end_time;
            start_time = std::chrono::system_clock::now(); // 計測開始

            if (step_count % 120 == 0) {
                std::size_t pop_num = 0; // 人口数
                std::size_t sat_num = 0; // 集落数
                std::vector < std::vector<int>> mtdna_num(90, std::vector<int>(256, 0)); // mtDNA 数
                std::size_t ryopop[90]{};
                std::size_t ryoset[90]{};
                double ryosnp[90]{};

                // 地名を描画
                for (const auto& agent : getSettlementGrids()) {
                    for (const auto& settlement : agent.second.cgetSettlements()) {
                        ++sat_num; // 集落数を増加させる
                        pop_num += settlement.getPopulation(); // 人口数を増加させる

                        const std::uint_least8_t ryo_id = environment->template getData<std::uint_least8_t>(MurMur3::calcHash("gbank"), settlement.getPosition());
                        if (ryo_id < 90) {
                            ryopop[ryo_id]+= settlement.getPopulation(); // 地区ごとに人口数を増加させる
                            ryosnp[ryo_id]+= settlement.getSNP(); // 地区ごとに SNP を増加させる
                            ++(ryoset[ryo_id]);

                            // mtDNA ごとにカウント
                            for (int popi = 0; popi < settlement.cgetAgents().size(); ++popi) {
                                mtdna_num[ryo_id][settlement.cgetAgents()[popi].cgetGenome().getMtDNA()] += 1;
                            }
                        }
                    }
                }
                pop_ofs << step_count << '\t' << sat_num << '\t' << pop_num << '\t';
                pop_mtdna_ofs << step_count << '\t' << sat_num << '\t' << pop_num << '\t';
                pop_snp_ofs << step_count << '\t' << sat_num << '\t' << pop_num << '\t';
                for (int i = 0; i < 90; ++i) {
                    ryosnp[i] /= static_cast<double>(ryoset[i]);
                    pop_ofs << ryopop[i] << '\t';
                    pop_snp_ofs << ryosnp[i] << '\t';
                    for (int j = 0; j < 27; ++j) {
                        if (int(mtdna_num[i][j]) == 0) continue;
                        pop_mtdna_ofs << japan_provinces->getMtDNA_Name(j) << ':' << int(mtdna_num[i][j]) << '/';
                    }
                    pop_mtdna_ofs << '\t';
                }
                pop_ofs << step_count << '\n';
                pop_mtdna_ofs << step_count << '\n';
                pop_snp_ofs << step_count << '\n';
            }

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
            move_time = std::chrono::system_clock::now(); // 移動
            move_processing_time = static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(move_time - start_time).count() / 1000.0);

            for (auto& settlement_grid : settlement_grids) {
                for (auto& settlement : settlement_grid.second.getSettlements()) {
                    settlement.preUpdate(kanakuma_life_span, emigration_count);
                }
            }
            // 前901年から稲作文化開始
            if (step_count > SimulationConstants::getInstance()->steps_per_year * 200) {
                randomizeSettlements(1, 255, 0, 255/*渡来人は SNP:255*/);
            }

            m_start_time = std::chrono::system_clock::now();  // 婚姻計測開始

            auto delete_agent = [this](const std::uint_least32_t agent_id, const std::uint_least32_t settlement_id, const Vector2 key) {
                auto it = settlement_grids.find(key.to<SettlementGridsType>());
                if (it != settlement_grids.end()) {
                    it->second.deleteAgent(agent_id, settlement_id);
                }
                else {
                    PAXS_ERROR("Settlement grid not found. Key: " + std::to_string(key.x) + ", " + std::to_string(key.y));
                }
            };

            auto add_agent = [this](const paxs::SettlementAgent agent_, const std::uint_least32_t settlement_id, const Vector2 key) {
                auto it = settlement_grids.find(key.to<SettlementGridsType>());
                if (it != settlement_grids.end()) {
                    it->second.addAgent(agent_, settlement_id);
                }
                else {
                    PAXS_ERROR("Settlement grid not found. Key: " + std::to_string(key.x) + ", " + std::to_string(key.y));
                }
                };

            for (auto& settlement_grid : settlement_grids) {
                std::vector<Settlement>& settlements = settlement_grid.second.getSettlements();
                if (settlements.size() == 0) {
                    continue;
                }

                // 近隣8グリッドの集落を取得
                std::vector<Settlement*> close_settlements;
                Vector2 grid_position = settlement_grid.second.getGridPosition();
                grid_position /= SimulationConstants::getInstance()->grid_length;
                for (int i = -1; i <= 1; ++i) {
                    for (int j = -1; j <= 1; ++j) {
                        auto it = settlement_grids.find((grid_position + Vector2(i, j)).to<SettlementGridsType>());
                        if (it != settlement_grids.end()) {
                            for (auto& settlement : it->second.getSettlements()) {
                                close_settlements.emplace_back(&settlement);
                            }
                        }
                    }
                }

                for (auto& settlement : settlements) {
                    settlement.marriage(close_settlements, add_agent, delete_agent);
                }
            }

            m_end_time = std::chrono::system_clock::now();  // 婚姻計測終了
            marriage_processing_time = static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(m_end_time - m_start_time).count() / 1000.0);

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

            ++step_count;
            end_time = std::chrono::system_clock::now();  // 計測終了
            processing_time = static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() / 1000.0);
        }

        /// @brief Get the agent list.
        /// @brief エージェントのリストを取得する
        constexpr std::unordered_map<SettlementGridsType, SettlementGrid>&
            getSettlementGrids() noexcept { return settlement_grids; }
        /// @brief Get the agent list.
        /// @brief エージェントのリストを取得する
        constexpr const std::unordered_map<SettlementGridsType, SettlementGrid>&
            cgetSettlementGrids() const noexcept { return settlement_grids; }

        /// @brief Get processing_time.
        /// @brief 処理時間を取得する
        constexpr double cgetProcessingTime() const noexcept { return processing_time; }
        /// @brief Get processing_time.
        /// @brief 処理時間を取得する
        constexpr double cgetMoveProcessingTime() const noexcept { return move_processing_time; }
        /// @brief Get processing_time.
        /// @brief 処理時間を取得する
        constexpr double cgetMarriageProcessingTime() const noexcept { return marriage_processing_time; }

    private:
        double processing_time = 0.0;
        double move_processing_time = 0.0;
        double marriage_processing_time = 0.0;

        std::unordered_map<SettlementGridsType, SettlementGrid> settlement_grids;
        std::shared_ptr<Environment> environment;

        std::unique_ptr<paxs::JapanProvinces> japan_provinces;
        int move_probability = 0; // 移動確率

        std::mt19937 gen; // 乱数生成器

        KanakumaLifeSpan kanakuma_life_span;

        std::uint_least64_t emigration_count = 0;
        std::uint_least64_t step_count = 0;

        // 陸の位置のリストを取得
        std::vector<DataGridsType> land_positions;

        struct Live {
            // 可住地重みリスト
            std::vector<int> live_probabilities;
            // 可住地の位置
            std::vector<DataGridsType> habitable_land_positions;

            void emplaceBack(const int live_probabilities_, const DataGridsType habitable_land_positions_) {
                live_probabilities.emplace_back(live_probabilities_);
                habitable_land_positions.emplace_back(habitable_land_positions_);
            }
        };
        std::unique_ptr<std::array<Live, max_number_of_districts>> live_list;

        std::ofstream pop_ofs = std::ofstream("pop.txt");
        std::ofstream pop_mtdna_ofs = std::ofstream("pop_mtdna.txt");
        std::ofstream pop_snp_ofs = std::ofstream("pop_snp.txt");

        /// @brief ()
        /// @brief 集落をランダムに配置する前の初期化処理
        bool initRandomizeSettlements() {
            std::cout << "Randomize settlements..." << std::endl;

            environment->getLandPositions(land_positions);

            live_list = std::unique_ptr<std::array<Live, max_number_of_districts>>(new std::array<Live, max_number_of_districts>());

            if (live_list.get() == nullptr) {
                std::cout << "Low memory" << std::endl;
                return false; // 処理失敗
            }

            for (const DataGridsType land_position : land_positions) {
                // 可住地かどうかを判定
                Vector2 position = Vector2::from(land_position);
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

                // 地区ごとに人口が決められているので、人口に空きがあるかどうかを判定
                std::uint_least8_t district_id = environment->template getData<std::uint_least8_t>(MurMur3::calcHash("gbank"), position);
                if (district_id < max_number_of_districts) {
                    (*live_list)[district_id].emplaceBack(live_probability, land_position);
                }
            }
            return true;
        }

        /// @brief Randomly place settlements.
        /// @brief 集落をランダムに配置する
        void randomizeSettlements(
            std::size_t ad200,
            std::uint_least8_t farming,
            std::uint_least8_t hunter_gatherer,
            std::uint_least8_t snp_
        ) noexcept {

            // 地区と人口のマップ
            std::unordered_map<std::uint_least8_t, std::uint_least32_t> district_population_map;
            for (auto& district : japan_provinces->cgetDistrictList()) {
                if (district.population[ad200/*ad200*/] == 0) {
                    continue;
                }
                district_population_map[district.id] = district.population[ad200/*ad200*/];
            }

            int all_population = 0;
            for (auto& district_population : district_population_map) {
                all_population += district_population.second;
            }
            int population_sum = 0;
            // 集落配置
            for (std::uint_least8_t district_id = 0; district_id < max_number_of_districts; ++district_id) {

                Live live = (*live_list)[district_id];

                while (live.live_probabilities.size() > 0 && // 集落を配置し切るまで
                    district_population_map.find(district_id) != district_population_map.end() // 地区が残っている間
                    ) {
                    //StatusDisplayer::displayProgressBar(population_sum, all_population);

                    // 重みからインデックスを取得するための分布
                    std::discrete_distribution<> live_probability_dist(live.live_probabilities.begin(), live.live_probabilities.end());

                    const int live_probability_index = live_probability_dist(gen);
                    const Vector2 live_position = Vector2::from(live.habitable_land_positions[live_probability_index]);

                    // 地区ごとに人口が決められているので、人口に空きがあるかどうかを判定
                    // std::uint_least8_t district_id = environment->template getData<std::uint_least8_t>(MurMur3::calcHash("gbank"), live_position);

                    auto district_population_it = district_population_map.find(district_id);
                    if (district_population_it == district_population_map.end()) {
                        live.live_probabilities[live_probability_index] = live.live_probabilities.back();
                        live.live_probabilities.pop_back();
                        live.habitable_land_positions[live_probability_index] = live.habitable_land_positions.back();
                        live.habitable_land_positions.pop_back();
                        continue;
                    }

                    // 配置する集落の人口を決定
                    paxs::District district = japan_provinces->cgetDistrict(district_id);
                    int settlement_population = std::uniform_int_distribution<>(district.settlement_population_min_ad200, district.settlement_population_max_ad200)(gen);
                    settlement_population = (std::min)(settlement_population, static_cast<int>(district_population_it->second));

                    // 集落をグリッドに配置
                    Vector2 grid_position = live_position / SimulationConstants::getInstance()->grid_length;
                    SettlementGridsType key = grid_position.to<SettlementGridsType>();
                    // グリッドが存在しない場合は作成
                    if (settlement_grids.find(key) == settlement_grids.end()) {
                        settlement_grids[key] = SettlementGrid(grid_position * SimulationConstants::getInstance()->grid_length, environment, gen);
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
                        Genome genome = Genome::generateRandomSetMtDNA(gen, japan_provinces->getMtDNA((farming>0)? 73/*toraijin*/ : district_id, gen), snp_);
                        const std::uint_least32_t set_lifespan = kanakuma_life_span.setLifeSpan(genome.getGender(), gen);

                        int age_value = 0;
                        if (set_lifespan > 180) {
                            std::uniform_int_distribution<> lifespan_dist{ 0, static_cast<int>(set_lifespan - 180) }; // 性別の乱数分布
                            age_value = lifespan_dist(gen);
                        }

                        settlement.setAgent(Agent(UniqueIdentification<std::uint_least32_t>::generate(),
                            //0, // TODO: 名前ID
                            age_value,
                            set_lifespan,
                            genome,
                            farming, // ((gen() % 2) == 0) ? agent.cgetFarming() : agent.cgetPartnerFarming(),
                            hunter_gatherer // ((gen() % 2) == 0) ? agent.cgetHunterGatherer() : agent.cgetPartnerHunterGatherer()
                        ), static_cast<std::size_t>(i));
                        if (farming > 0) ++emigration_count; // 農耕カウント
                    }

                    // 地区の人口を減らす
                    district_population_it->second -= settlement_population;
                    if (district_population_it->second == 0) {
                        district_population_map.erase(district_population_it);
                    }
                    population_sum += settlement_population;

                    // 集落をグリッドに配置
                    settlement_grids[key].addSettlement(settlement);
                    settlement_grids[key].addDistrictId(district_id);

                    live.live_probabilities[live_probability_index] = live.live_probabilities.back();
                    live.live_probabilities.pop_back();
                    live.habitable_land_positions[live_probability_index] = live.habitable_land_positions.back();
                    live.habitable_land_positions.pop_back();
                }
            }
            //StatusDisplayer::displayProgressBar(all_population, all_population);
            //std::cout << std::endl;

            // 地区の人口が残っている場合は、ランダムに配置
            for (auto& district_population : district_population_map) {
                std::uint_least8_t district_id = district_population.first;
                int population = district_population.second;
                std::vector<Settlement> settlements;
                getSettlements(settlements, district_id);

                if (settlements.size() == 0) {
                    PAXS_WARNING("Settlements not found. District ID: " + std::to_string(district_id));
                    continue;
                }

                const int add_population = population / static_cast<int>(settlements.size());

                for (auto& settlement : settlements) {
                    std::vector<Agent> agents(add_population);
                    for (int i = 0; i < add_population; ++i) {
                        Genome genome = Genome::generateRandomSetMtDNA(gen, japan_provinces->getMtDNA((farming > 0) ? 73/*toraijin*/ : district_id, gen), snp_);
                        const std::uint_least32_t set_lifespan = kanakuma_life_span.setLifeSpan(genome.getGender(), gen);

                        int age_value = 0;
                        if (set_lifespan > 180) {
                            std::uniform_int_distribution<> lifespan_dist{ 0, static_cast<int>(set_lifespan - 180) }; // 性別の乱数分布
                            age_value = lifespan_dist(gen);
                        }

                        agents[i] = Agent(
                            UniqueIdentification<std::uint_least32_t>::generate(),
                            //0, // TODO: 名前ID
                            age_value,
                            set_lifespan,
                            genome,
                            farming, // ((gen() % 2) == 0) ? agent.cgetFarming() : agent.cgetPartnerFarming(),
                            hunter_gatherer // ((gen() % 2) == 0) ? agent.cgetHunterGatherer() : agent.cgetPartnerHunterGatherer()
                        );
                        if (farming > 0) ++emigration_count; // 農耕カウント
                    }
                    settlement.addAgents(agents);
                }
            }

            //std::cout << "Done." << std::endl;
        }

        /// @brief 指定した地区のIDの集落グリッドを取得
        void getSettlementGrids(std::vector<SettlementGrid>& settlement_grids_, const std::uint_least8_t district_id_) noexcept {
            for (auto& settlement_grid : settlement_grids) {
                std::vector<std::uint_least8_t> district_ids = settlement_grid.second.getDistrictIds();
                for (auto id : district_ids) {
                    if (id == district_id_) {
                        settlement_grids_.emplace_back(settlement_grid.second);
                        break;
                    }
                }
            }
        }

        /// @brief 指定した地区のIDの集落を取得
        void getSettlements(std::vector<Settlement>& settlements, const std::uint_least8_t district_id_) noexcept {
            std::vector<SettlementGrid> settlement_grids_;
            getSettlementGrids(settlement_grids_, district_id_);
            for (auto& settlement_grid : settlement_grids_) {
                for (auto& settlement : settlement_grid.getSettlements()) {
                    settlements.emplace_back(settlement);
                }
            }
        }
    };
}

#endif // !PAX_SAPIENTICA_SETTLEMENT_SIMULATOR_HPP
