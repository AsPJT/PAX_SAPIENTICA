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
#include <ctime>
#include <filesystem>
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

    // シミュレーション出力フォルダ用の時刻取得
    std::string calcDateTime() {
        static char buffer[20];
        std::time_t t = std::time(nullptr);
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d-%H-%M-%S", std::localtime(&t));
        return buffer;
    }

    class SettlementSimulator {
    public:
        using Agent = paxs::SettlementAgent;
        using Vector2 = paxs::Vector2<GridType>;

        explicit SettlementSimulator() = default;

        explicit SettlementSimulator(const std::string& map_list_path, const std::string& japan_provinces_path, const unsigned seed = 0) noexcept :
            environment(std::make_unique<Environment>(map_list_path)), gen(seed) {
            japan_provinces = std::make_unique<paxs::JapanProvinces>(japan_provinces_path);
            kanakuma_life_span.japan_provinces = japan_provinces.get();
        }
        /// @brief 環境を設定
        void setEnvironment(const std::string& map_list_path, const std::string& japan_provinces_path, /*const int z,*/ const unsigned seed = 0) noexcept {
            environment.reset();
            environment = std::make_unique<Environment>(map_list_path);

            gen = std::mt19937(seed);

            japan_provinces.reset();
            japan_provinces = std::make_unique<paxs::JapanProvinces>(japan_provinces_path);
            kanakuma_life_span.japan_provinces = japan_provinces.get();
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
                life_num1[kanakuma_life_span.setLifeSpan(true, 0, gen)] += 1;
                life_num2[kanakuma_life_span.setLifeSpan(true, 1, gen)] += 1;
            }
            life_ofs << "step\tyear\tfemale\tmale\n";
            for (int i = 0; i < SimulationConstants::getInstance()->steps_per_year * 100; ++i) {
                life_ofs << i << '\t' << i / SimulationConstants::getInstance()->steps_per_year << '\t' << life_num1[i] << '\t' << life_num2[i] << '\n';
            }
            return;
        }

        // 人口を計算
        void calcPop() {
            population_num = 0; // 人口数
            settlement_num = 0; // 集落数

            // 地名を描画
            for (const auto& settlement_grid : settlement_grids) {
                for (const auto& settlement : settlement_grid.second.cgetSettlements()) {
                    ++settlement_num; // 集落数を増加させる
                    population_num += settlement.getPopulation(); // 人口数を増加させる
                }
            }
        }
        // 結果の文字列を出力
        void outputResultString(std::ofstream& ofs_) const {
            ofs_ << "step_count" << '\t' << "settlement" << '\t' << "population" << '\t';
        }
        // 結果の最後の文字列を出力
        void outputResultLastString(std::ofstream& ofs_) const {
            ofs_ << "step_count" << '\n';
        }
        // 結果の地区名を出力
        void outputResultDistrictName(std::ofstream& ofs_, const std::size_t i_) const {
            ofs_ << japan_provinces->cgetDistrictList()[i_].name << '\t';
        }
        // 集落の初期化時にシミュレーション変数を出力
        void initResults() {
            // 出力
            const std::string str = "SimulationResults/" + calcDateTime();
            std::filesystem::create_directories(str);
            pop_ofs = std::ofstream(str + "/Population.txt");
            mtdna_ofs = std::ofstream(str + "/mtDNA.txt");
            snp_ofs = std::ofstream(str + "/SNP.txt");
            language_ofs = std::ofstream(str + "/Language.txt");
            live_ofs = std::ofstream(str + "/HabitableLand.txt");

            outputResultString(pop_ofs);
            outputResultString(mtdna_ofs);
            outputResultString(snp_ofs);
            outputResultString(language_ofs);
            for (std::size_t i = 0; i < max_number_of_districts; ++i) {
                outputResultDistrictName(pop_ofs, i);
                outputResultDistrictName(mtdna_ofs, i);
                outputResultDistrictName(snp_ofs, i);
                outputResultDistrictName(language_ofs, i);
            }
            outputResultLastString(pop_ofs);
            outputResultLastString(mtdna_ofs);
            outputResultLastString(snp_ofs);
            outputResultLastString(language_ofs);
        }

        /// @brief Initialize the simulator.
        /// @brief 集落の初期化
        /// @details 集落をクリアし、地域ごとに指定されたエージェント数になるようにランダムに配置する
        void init() {
            initResults();
            settlement_grids.clear();
            population_num = 0; // 人口数
            settlement_num = 0; // 集落数
            processing_time = 0.0;
            move_processing_time = 0.0;
            marriage_processing_time = 0.0;
            emigration_count = 0;
            step_count = 0;
            land_positions.clear();
            marriage_pos_list.clear();

            initRandomizeSettlements();
            randomizeSettlements(true, false /* 在地人 */, false /*青銅文化は持たない*/);
            calcPop(); // 人口を計算

            // 可住地の数を出力
            live_ofs << "district\thabitable_land\n";
            for (std::size_t i = 0; i < max_number_of_districts; ++i) {
                live_ofs << japan_provinces->cgetDistrictList()[i].name << '\t' << (*live_list)[i + 1].habitable_land_positions.size() << '\n';
            }
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
            const auto current_index = current_key.to(SettlementGridsType{});
            const auto target_index = target_key.to(SettlementGridsType{});

            // ターゲットの地域が登録されているか？
            auto it = settlement_grids.find(target_index);
            if (it != settlement_grids.end()) {
                // 登録されている場合はそのターゲット地域へ移動
                it->second.moveSettlementToThis(settlement_grids[current_index].getSettlement(id));
            }
            else {
                // 登録されていない場合は新しく地域を作成
                SettlementGrid settlement_grid = SettlementGrid(target_key * SimulationConstants::getInstance()->cell_group_length, environment, gen);
                settlement_grid.moveSettlementToThis(settlement_grids[current_index].getSettlement(id));
                settlement_grids[target_index] = settlement_grid;
            }
            settlement_grids[current_index].deleteSettlement(id);
        }

        /// @brief Execute the simulation for the one step.
        /// @brief シミュレーションを1ステップ実行する
        void step() noexcept {
            std::chrono::system_clock::time_point  start_time, end_time, move_time, m_start_time, m_end_time;
            start_time = std::chrono::system_clock::now(); // 計測開始

            // 指定したステップおきに出力する
            if (step_count % SimulationConstants::getInstance()->output_step_frequency == 0) {
                std::size_t pop_num = 0; // 人口数
                std::size_t sat_num = 0; // 集落数
                std::vector < std::vector<int>> mtdna_num(max_number_of_districts, std::vector<int>(256, 0)); // mtDNA 数
                std::size_t ryopop[max_number_of_districts]{};
                std::size_t ryoset[max_number_of_districts]{};
                double ryosnp[max_number_of_districts]{};
                double ryolanguage[max_number_of_districts]{};

                // 地名を描画
                for (const auto& agent : getSettlementGrids()) {
                    for (const auto& settlement : agent.second.cgetSettlements()) {
                        ++sat_num; // 集落数を増加させる
                        pop_num += settlement.getPopulation(); // 人口数を増加させる

                        const std::uint_least8_t ryo_id = environment->template getData<std::uint_least8_t>(SimulationConstants::getInstance()->district_key, settlement.getPosition());
                        if (ryo_id < max_number_of_districts) {
                            ryopop[ryo_id]+= settlement.getPopulation(); // 地区ごとに人口数を増加させる
                            ryosnp[ryo_id]+= settlement.getSNP(); // 地区ごとに SNP を増加させる
                            ryolanguage[ryo_id]+= settlement.getLanguage(); // 地区ごとに言語を増加させる
                            ++(ryoset[ryo_id]);

                            // mtDNA ごとにカウント
                            for (int popi = 0; popi < settlement.cgetAgents().size(); ++popi) {
                                mtdna_num[ryo_id][settlement.cgetAgents()[popi].cgetGenome().getMtDNA()] += 1;
                            }
                        }
                    }
                }
                pop_ofs << step_count << '\t' << sat_num << '\t' << pop_num << '\t';
                mtdna_ofs << step_count << '\t' << sat_num << '\t' << pop_num << '\t';
                snp_ofs << step_count << '\t' << sat_num << '\t' << pop_num << '\t';
                language_ofs << step_count << '\t' << sat_num << '\t' << pop_num << '\t';
                for (std::size_t i = 0; i < max_number_of_districts; ++i) {
                    ryosnp[i] /= static_cast<double>(ryoset[i]);
                    ryolanguage[i] /= static_cast<double>(ryoset[i]);
                    pop_ofs << ryopop[i] << '\t';
                    snp_ofs << ryosnp[i] << '\t';
                    language_ofs << ryolanguage[i] << '\t';
                    for (std::size_t j = 0; j < japan_provinces->getSizeMtDNA(); ++j) {
                        if (int(mtdna_num[i][j]) == 0) continue;
                        mtdna_ofs << japan_provinces->getMtDNA_Name(static_cast<std::uint_least8_t>(j)) << ':' << int(mtdna_num[i][j]) << '/';
                    }
                    mtdna_ofs << '\t';
                }
                pop_ofs << step_count << '\n';
                mtdna_ofs << step_count << '\n';
                snp_ofs << step_count << '\n';
                language_ofs << step_count << '\n';
            }

            std::vector<std::tuple<std::uint_least32_t, Vector2, Vector2>> move_list;
            for (auto& settlement_grid  : settlement_grids) {
                std::vector<Settlement>& settlements = settlement_grid.second.getSettlements();
                for (std::size_t i = 0; i < settlements.size(); ++i) {
                    // 集落の過去の位置情報を削除
                    settlements[i].clearOldPosition();

                    if (settlements[i].isMoved()) {
                        continue;
                    }

                    auto [target_id, current_key, target_key] = settlements[i].move(gen, japan_provinces->getDistrict(environment->template getData<std::uint_least8_t>(SimulationConstants::getInstance()->district_key, settlements[i].getPosition())));

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
                    settlement.preUpdate(kanakuma_life_span);
                }
            }
            // 前901年から稲作文化開始
            if (step_count >= SimulationConstants::getInstance()->immigration_start_steps &&
                step_count <= SimulationConstants::getInstance()->immigration_end_steps) {
                randomizeSettlements(false, true /* 渡来人 */, (step_count >= SimulationConstants::getInstance()->bronze_start_steps)/*青銅*/);
            }

            m_start_time = std::chrono::system_clock::now();  // 婚姻計測開始
            marriage_pos_list.clear();

            // 結婚の条件を満たすエージェントを取得
            std::vector<std::size_t> marriageable_female_index;
            // エージェントIDと集落IDのペアを作成
            std::vector<Marriage3> male_settlement_pair;

            // 近隣９グリッドの集落を取得
            std::vector<std::vector<Settlement>*> close_settlements_list;

            for (auto& settlement_grid : settlement_grids) {
                std::vector<Settlement>& settlements = settlement_grid.second.getSettlements();
                if (settlements.size() == 0) continue; // 集落が無い場合

                Vector2 grid_position = settlement_grid.second.getGridPosition();
                grid_position /= SimulationConstants::getInstance()->cell_group_length;

                close_settlements_list.clear();
                for (int i = -1; i <= 1; ++i) {
                    for (int j = -1; j <= 1; ++j) {
                        auto it = settlement_grids.find((grid_position + Vector2(i, j)).to(SettlementGridsType{}));
                        if (it != settlement_grids.end()) {
                            close_settlements_list.emplace_back(&(it->second.getSettlements()));
                        }
                    }
                }
                // 近隣９グリッドのシャッフル
                std::shuffle(close_settlements_list.begin(), close_settlements_list.end(), gen);
                for (auto& close_settlements : close_settlements_list) {
                    // 青銅交換
                    if (close_settlements->size() >= 2) {
                        std::uint_fast32_t bronze = (close_settlements->front().getBronze() + close_settlements->back().getBronze()) / 2;
                        close_settlements->front().setBronze(bronze);
                        close_settlements->back().setBronze(bronze);
                    }
                }
                for (auto& settlement : settlements) {
                    settlement.marriage(
                        close_settlements_list,
                        marriageable_female_index, male_settlement_pair,
                        marriage_pos_list
                    );
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

            calcPop();

            // 前901年から処理開始
            if (step_count >= SimulationConstants::getInstance()->immigration_start_steps &&
                step_count <= SimulationConstants::getInstance()->immigration_end_steps) {
                // 渡来数を増やす
                japan_provinces->update();
            }

            ++step_count;
            end_time = std::chrono::system_clock::now();  // 計測終了
            processing_time = static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() / 1000.0);
        }

        // 婚姻時に移動した位置一覧を取得する
        const std::vector<GridType4>& getMarriagePosList() const { return marriage_pos_list; }

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

        constexpr std::size_t cgetPopulationNum() const noexcept { return population_num; }
        constexpr std::size_t cgetSettlement() const noexcept { return settlement_num; }

    private:
        std::size_t population_num = 0; // 人口数
        std::size_t settlement_num = 0; // 集落数

        double processing_time = 0.0;
        double move_processing_time = 0.0;
        double marriage_processing_time = 0.0;

        std::unordered_map<SettlementGridsType, SettlementGrid> settlement_grids;
        std::shared_ptr<Environment> environment;

        std::unique_ptr<paxs::JapanProvinces> japan_provinces;

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
        std::unique_ptr<std::array<Live, max_number_of_districts + 1>> live_list;

        std::ofstream pop_ofs;
        std::ofstream mtdna_ofs;
        std::ofstream snp_ofs;
        std::ofstream language_ofs;
        std::ofstream live_ofs;
        // 婚姻時に移動前の位置と移動後の位置を記録
        std::vector<GridType4> marriage_pos_list{};

        /// @brief ()
        /// @brief 集落をランダムに配置する前の初期化処理
        bool initRandomizeSettlements() {
            std::cout << "Randomize settlements..." << std::endl;

            environment->getLandPositions(land_positions);

            live_list = std::unique_ptr<std::array<Live, max_number_of_districts + 1>>(new std::array<Live, max_number_of_districts + 1>());

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
                std::uint_least8_t district_id = environment->template getData<std::uint_least8_t>(SimulationConstants::getInstance()->district_key, position);
                if (district_id < max_number_of_districts) {
                    (*live_list)[district_id].emplaceBack(live_probability, land_position);
                }
            }
            return true;
        }

        /// @brief Randomly place settlements.
        /// @brief 集落をランダムに配置する
        void randomizeSettlements(
            bool is_ad200,
            bool is_farming, // 渡来人であるか？
            bool is_bronze // 青銅文化であるか？
        ) noexcept {
            // 地区 ID の最大値
            std::uint_least8_t district_id_max = 0;

            // 地区と人口のマップ
            std::unordered_map<std::uint_least8_t, std::uint_least32_t> district_population_map;
            for (auto& district : japan_provinces->cgetDistrictList()) {
                if (((is_ad200)? district.init_pop : district.immigrant) == 0) {
                    continue;
                }
                district_population_map[district.id] = ((is_ad200) ? district.init_pop : district.immigrant);
                // より地区 ID が大きい値を見つけたら上書き
                if (district.id > district_id_max) {
                    district_id_max = district.id;
                }
            }
            // 全人口
            int all_population = 0;
            if (step_count == 0) {
                // プログレスバー用に人口配置数を算出
                for (auto& district_population : district_population_map) {
                    all_population += static_cast<int>(district_population.second);
                }
            }
            int population_sum = 0;
            // より小さい値を選択
            district_id_max = (district_id_max < max_number_of_districts) ? district_id_max + 1 : max_number_of_districts;

            // 集落配置
            for (std::uint_least8_t district_id = 0; district_id < district_id_max; ++district_id) {

                Live live = (*live_list)[district_id];

                while (live.live_probabilities.size() > 0 && // 集落を配置し切るまで
                    district_population_map.find(district_id) != district_population_map.end() // 地区が残っている間
                    ) {
                    if (step_count == 0) {
                        StatusDisplayer::displayProgressBar(population_sum, all_population);
                    }

                    // 重みからインデックスを取得するための分布
                    std::discrete_distribution<> live_probability_dist(live.live_probabilities.begin(), live.live_probabilities.end());

                    const int live_probability_index = live_probability_dist(gen);
                    const Vector2 live_position = Vector2::from(live.habitable_land_positions[live_probability_index]);

                    // 地区ごとに人口が決められているので、人口に空きがあるかどうかを判定
                    // std::uint_least8_t district_id = environment->template getData<std::uint_least8_t>(SimulationConstants::getInstance()->district_key, live_position);

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
                    int settlement_population = std::uniform_int_distribution<>(district.settlement_pop_min, district.settlement_pop_max)(gen);
                    settlement_population = (std::min)(settlement_population, static_cast<int>(district_population_it->second));

                    // 集落をグリッドに配置
                    Vector2 grid_position = live_position / SimulationConstants::getInstance()->cell_group_length;
                    SettlementGridsType key = grid_position.to(SettlementGridsType{});
                    // グリッドが存在しない場合は作成
                    if (settlement_grids.find(key) == settlement_grids.end()) {
                        settlement_grids[key] = SettlementGrid(grid_position * SimulationConstants::getInstance()->cell_group_length, environment, gen);
                    }
                    // 集落を作成
                    Settlement settlement = Settlement(
                        UniqueIdentification<std::uint_least32_t>::generate(),
                        gen,
                        environment
                    );
                    // 青銅の持ち込み
                    if (is_bronze) settlement.setBronze(SimulationConstants::getInstance()->bronze);
                    settlement.setPosition(live_position);

                    // 渡来人込みの地区 ID
                    const std::uint_least8_t immigration_and_district_id = (is_farming) ? SimulationConstants::getInstance()->immigration_district_id/*toraijin*/ : district_id;
                    settlement.resizeAgents(settlement_population);
                    for (int i = 0; i < settlement_population; ++i) {
                        Genome genome = Genome::generateRandomSetMtDNA(gen, japan_provinces->getMtDNA(immigration_and_district_id, gen), japan_provinces->getSNP(immigration_and_district_id));
                        const AgeType set_lifespan = kanakuma_life_span.setLifeSpan((is_farming), genome.isMale(), gen);

                        AgeType age_value = 0;
                        if (set_lifespan > SimulationConstants::getInstance()->init_lifespan_grace_period) {
                            // 寿命の乱数分布
                            std::uniform_int_distribution<> lifespan_dist{ 0, static_cast<int>(set_lifespan - SimulationConstants::getInstance()->init_lifespan_grace_period) };
                            age_value = static_cast<AgeType>(lifespan_dist(gen));
                        }

                        settlement.setAgent(Agent(UniqueIdentification<std::uint_least32_t>::generate(),
                            age_value,
                            set_lifespan,
                            genome,
                            japan_provinces->getFarming(immigration_and_district_id),
                            japan_provinces->getHunterGatherer(immigration_and_district_id),
                            japan_provinces->getLanguage(immigration_and_district_id)
                        ), static_cast<std::size_t>(i));
                        if (is_farming) ++emigration_count; // 農耕カウント
                    }

                    // 地区の人口を減らす
                    district_population_it->second -= settlement_population;
                    if (district_population_it->second == 0) {
                        district_population_map.erase(district_population_it);
                    }
                    if (step_count == 0) {
                        population_sum += settlement_population;
                    }

                    // 集落をグリッドに配置
                    settlement_grids[key].addSettlement(settlement);
                    settlement_grids[key].addDistrictId(district_id);

                    live.live_probabilities[live_probability_index] = live.live_probabilities.back();
                    live.live_probabilities.pop_back();
                    live.habitable_land_positions[live_probability_index] = live.habitable_land_positions.back();
                    live.habitable_land_positions.pop_back();
                }
            }
            if (step_count == 0) {
                StatusDisplayer::displayProgressBar(all_population, all_population);
                std::cout << std::endl;
            }
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

                // 渡来人込みの地区 ID
                const std::uint_least8_t immigration_and_district_id = (is_farming) ? SimulationConstants::getInstance()->immigration_district_id/*toraijin*/ : district_id;
                for (auto& settlement : settlements) {
                    std::vector<Agent> agents(add_population);
                    for (int i = 0; i < add_population; ++i) {
                        Genome genome = Genome::generateRandomSetMtDNA(gen, japan_provinces->getMtDNA(immigration_and_district_id, gen), japan_provinces->getSNP(immigration_and_district_id));
                        const AgeType set_lifespan = kanakuma_life_span.setLifeSpan(is_farming, genome.isMale(), gen);

                        AgeType age_value = 0;
                        if (set_lifespan > SimulationConstants::getInstance()->init_lifespan_grace_period) {
                            // 寿命の乱数分布
                            std::uniform_int_distribution<> lifespan_dist{ 0, static_cast<int>(set_lifespan - SimulationConstants::getInstance()->init_lifespan_grace_period) };
                            age_value = static_cast<AgeType>(lifespan_dist(gen));
                        }

                        agents[i] = Agent(
                            UniqueIdentification<std::uint_least32_t>::generate(),
                            age_value,
                            set_lifespan,
                            genome,
                            japan_provinces->getFarming(immigration_and_district_id),
                            japan_provinces->getHunterGatherer(immigration_and_district_id),
                            japan_provinces->getLanguage(immigration_and_district_id)
                        );
                        if (is_farming) ++emigration_count; // 農耕カウント
                    }
                    settlement.addAgents(agents);
                    // 青銅の持ち込み
                    if (is_bronze) settlement.setBronze(SimulationConstants::getInstance()->bronze);
                }
            }

            if (step_count == 0) {
                std::cout << "Done." << std::endl;
            }
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
