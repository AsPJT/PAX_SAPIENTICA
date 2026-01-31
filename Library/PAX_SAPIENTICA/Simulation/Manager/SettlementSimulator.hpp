/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SIMULATION_MANAGER_SETTLEMENT_SIMULATOR_HPP
#define PAX_SAPIENTICA_SIMULATION_MANAGER_SETTLEMENT_SIMULATOR_HPP

#include <algorithm>
#include <array>
#include <cstdint>
#include <memory>
#include <random>
#include <utility> // for std::pair
#include <vector>

#include <PAX_SAPIENTICA/IO/File/FileSystem.hpp>
#include <PAX_SAPIENTICA/Interface/ConsoleProgressReporter.hpp>
#include <PAX_SAPIENTICA/Interface/IProgressReporter.hpp>
#include <PAX_SAPIENTICA/Simulation/Config/Environment.hpp>
#include <PAX_SAPIENTICA/Simulation/Config/JapanProvinces.hpp>
#include <PAX_SAPIENTICA/Simulation/Config/KanakumaLifeSpan.hpp>
#include <PAX_SAPIENTICA/Simulation/Config/SimulationConst.hpp>
#include <PAX_SAPIENTICA/Simulation/Entity/Settlement.hpp>
#include <PAX_SAPIENTICA/Simulation/Entity/SettlementGrid.hpp>
#include <PAX_SAPIENTICA/Simulation/Output/SimulationResultWriter.hpp>
#include <PAX_SAPIENTICA/Utility/StatusDisplayer.hpp>
#include <PAX_SAPIENTICA/Utility/TimeUtils.hpp>
#include <PAX_SAPIENTICA/Utility/UniqueIdentification.hpp>

namespace paxs {

    // シミュレーション出力フォルダ用の時刻取得
    std::string calcDateTime() {
        return TimeUtils::getFilenameSafeDateTime();
    }

    class SettlementSimulator {
    public:
        using Agent = paxs::SettlementAgent;
        using Vector2 = paxs::Vector2<GridType>;
        // 青銅交換リストの型定義: <開始位置, 終了位置>
        using BronzeShareList = std::vector<std::pair<paxs::Vector2<int>, paxs::Vector2<int>>>;

        explicit SettlementSimulator() = default;

        explicit SettlementSimulator(const std::string& map_list_path, const std::string& japan_provinces_path, const unsigned seed = 0) noexcept :
            environment(std::make_unique<Environment>(map_list_path)), gen(seed) {
            japan_provinces = std::make_unique<paxs::JapanProvinces>(japan_provinces_path);
        }
        /// @brief 環境を設定
        /// @brief Set environment
        /// @param map_list_path マップリストファイルパス / Map list file path
        /// @param japan_provinces_path 行政区画ファイルパス / Provinces file path
        /// @param seed 乱数シード / Random seed
        void setEnvironment(
            const std::string& map_list_path,
            const std::string& japan_provinces_path,
            const unsigned seed = 0
        ) noexcept {
            if (progress_reporter_) {
                progress_reporter_->reportProgress(0.0f, "Starting environment setup...");
            }

            environment.reset();

            if (progress_reporter_) {
                progress_reporter_->reportProgress(0.05f, "Loading environment data...");
            }

            // Environment読み込み（進捗: 5% - 80%）
            environment = std::make_unique<Environment>();
            environment->setProgressReporter(progress_reporter_);
            environment->loadFromFile(map_list_path);

            if (progress_reporter_) {
                progress_reporter_->reportProgress(0.8f, "Initializing random generator...");
            }

            gen = std::mt19937(seed);

            if (progress_reporter_) {
                progress_reporter_->reportProgress(0.85f, "Loading provinces data...");
            }

            japan_provinces.reset();

            japan_provinces = std::make_unique<paxs::JapanProvinces>(japan_provinces_path);

            if (progress_reporter_) {
                progress_reporter_->reportProgress(1.0f, "Environment setup complete");
            }
        }

        /// @brief シミュレーション入力データを再読み込み
        /// @brief Reload simulation input data
        /// @param model_name モデル名 / Model name
        static void reloadInputData(const std::string& model_name = "Sample") {
            // SimulationConstantsのシングルトンインスタンスを取得
            auto& constants = SimulationConstants::getInstance(model_name);

            // 各種データを再読み込み
            constants.init(model_name);
            constants.inputLifeSpan(model_name);
            constants.inputMarriage(model_name);
            constants.inputChildbearing(model_name);
        }

        /// @brief 渡来数の取得
        std::uint_least64_t getMigrationCount() const {
            return migration_count;
        }

        /// @brief 合計特殊出生率(TFR)を計算
        /// @brief Calculate Total Fertility Rate
        /// @return TFR値 / TFR value
        double calculateTotalFertilityRate() const {
            // 15歳から49歳までの女性人口をカウント
            std::size_t childbearing_age_female_count = 0;

            // 全集落を走査して出産可能年齢の女性人口を集計
            for (const auto& settlement_grid : settlement_grids) {
                for (const auto& settlement : settlement_grid.second.cgetSettlements()) {
                    for (const auto& agent : settlement.cgetAgents()) {
                        if (!agent.isMale()) {
                            const std::size_t age_in_years = agent.getAgeSizeT();
                            // 15歳から49歳の女性をカウント
                            if (age_in_years >= SimulationConstants::getInstance().childbearing_age_min &&
                                age_in_years < SimulationConstants::getInstance().childbearing_age_max) {
                                childbearing_age_female_count++;
                            }
                        }
                    }
                }
            }

            // 出産可能年齢の女性がいない場合は0を返す
            if (childbearing_age_female_count == 0) {
                return 0.0;
            }

            // 直近1年間の出生数を取得（循環バッファの合計）
            std::size_t total_births_last_year = 0;
            for (const auto& count : birth_count_history_) {
                total_births_last_year += count;
            }

            // TFR = (年間出生数 / 出産可能年齢の女性人口) × 出産可能年齢の年数
            const double births_per_woman = static_cast<double>(total_births_last_year) /
                static_cast<double>(childbearing_age_female_count);

            // 出産可能年齢の範囲（通常は35年：15歳から49歳）
            const std::size_t childbearing_age_range =
                SimulationConstants::getInstance().childbearing_age_max -
                SimulationConstants::getInstance().childbearing_age_min;

            // TFRを計算
            const double tfr = births_per_woman * static_cast<double>(childbearing_age_range);

            return tfr;
        }

        /// @brief
        /// @brief 生命表を取得
        void outputLifeSpan(const int loop_num_) {
            std::ofstream life_ofs = std::ofstream("life.txt");
            std::vector<int> life_num1(SimulationConstants::getInstance().steps_per_year * 100, 0);
            std::vector<int> life_num2(SimulationConstants::getInstance().steps_per_year * 100, 0);
            for (int i = 0; i < loop_num_; ++i) {
                life_num1[kanakuma_life_span.setLifeSpan(true, 0, gen)] += 1;
                life_num2[kanakuma_life_span.setLifeSpan(true, 1, gen)] += 1;
            }
            life_ofs << "step\tyear\tfemale\tmale\n";
            for (int i = 0; i < SimulationConstants::getInstance().steps_per_year * 100; ++i) {
                life_ofs << i << '\t' << i / SimulationConstants::getInstance().steps_per_year << '\t' << life_num1[i] << '\t' << life_num2[i] << '\n';
            }
        }

        // 人口を計算（キャッシュ対応版）
        void calcPop() {
            if (population_cache_valid) {
                population_num = cached_population_num;
                settlement_num = cached_settlement_num;
                return;
            }

            cached_population_num = 0;
            cached_settlement_num = 0;

            for (const auto& settlement_grid : settlement_grids) {
                for (const auto& settlement : settlement_grid.second.cgetSettlements()) {
                    ++cached_settlement_num;
                    cached_population_num += settlement.getPopulation();
                }
            }

            population_num = cached_population_num;
            settlement_num = cached_settlement_num;
            population_cache_valid = true;
        }

        /// @brief キャッシュを無効化（人口変動時に呼び出す）
        /// @brief Invalidate cache (call when population changes)
        void invalidatePopulationCache() noexcept {
            population_cache_valid = false;
        }

        // 結果の文字列を出力
        static void outputResultString(std::ofstream& ofs_) {
            ofs_ << "step_count" << '\t' << "settlement" << '\t' << "population" << '\t';
        }
        // 結果の最後の文字列を出力
        static void outputResultLastString(std::ofstream& ofs_) {
            ofs_ << "step_count" << '\n';
        }
        // 結果の地区名を出力
        void outputResultDistrictName(std::ofstream& ofs_, const std::size_t i_) const {
            ofs_ << japan_provinces->getDistrictList()[i_].name << '\t';
        }
        // 集落の初期化時にシミュレーション変数を出力
        void initResults() {
            result_writer_ = std::make_unique<SimulationResultWriter>();
            result_writer_->initialize(SimulationConstants::getInstance().output_directory_name, japan_provinces->getDistrictList());
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
            migration_count = 0;
            step_count = 0;
            land_positions.clear();
            marriage_pos_list.clear();
            bronze_share_list.clear();

            // 出生数履歴の初期化（1年分のステップ数）
            birth_count_history_.assign(SimulationConstants::getInstance().steps_per_year, 0);
            birth_history_index_ = 0;

            initRandomizeSettlements();
            randomizeSettlements(true, false /* 在地人 */, false /*青銅文化は持たない*/);
            calcPop(); // 人口を計算

            // 毎回取得すると重い＆読みづらいので参照をとる
            const auto& dist_list = japan_provinces->getDistrictList();
            const auto& lives = *live_list;

            // 可住地の数を出力
            for (std::size_t i = 1;
                i < max_number_of_districts && i < dist_list.size() && (i + 1) < lives.size();
                ++i) {

                result_writer_->writeHabitableLand(
                    dist_list[i].name,
                    lives[i + 1].habitable_land_positions.size()
                );
            }
        }

        /// @brief Run the simulation for the specified number of steps.
        /// @brief シミュレーションを指定されたステップ数だけ実行する
        void run(const int step_count_) noexcept {
            for (int i = 0; i < step_count_; ++i) {
                if (progress_reporter_) {
                    progress_reporter_->reportProgress(static_cast<float>(i) / static_cast<float>(step_count_), "Step: " + std::to_string(i + 1));
                }
                else {
                    std::cout << "Step: " << i + 1 << std::endl;
                }
                step();
            }
        }

        void moveSettlement(const std::uint_least32_t settlement_id, const Vector2 current_key, const Vector2 target_key) noexcept {
            const auto current_index = current_key.to(SettlementGridsType{});
            const auto target_index = target_key.to(SettlementGridsType{});

            // ターゲットの地域が登録されているか？
            auto* const target_ptr = settlement_grids.try_get(target_index);
            auto* const current_ptr = settlement_grids.try_get(current_index);
            if (current_ptr == nullptr) {
                return; // current_indexが存在しない場合は何もしない
            }

            if (target_ptr != nullptr) {
                // 登録されている場合はそのターゲット地域へ移動
                target_ptr->moveSettlementToThis(current_ptr->getSettlement(settlement_id));
            }
            else {
                // 登録されていない場合は新しく地域を作成
                SettlementGrid settlement_grid = SettlementGrid(target_key * SimulationConstants::getInstance().cell_group_length, environment, gen);
                settlement_grid.moveSettlementToThis(current_ptr->getSettlement(settlement_id));
                settlement_grids.emplace(target_index, std::move(settlement_grid));
            }
            current_ptr->deleteSettlement(settlement_id);
        }

        /// @brief Execute the simulation for the one step.
        /// @brief シミュレーションを1ステップ実行する
        void step() noexcept {
            // パフォーマンス計測開始（TimeUtilsを使用）
            const auto start_time = TimeUtils::now();

            // 指定したステップおきに出力する
            if (step_count % SimulationConstants::getInstance().output_step_frequency == 0) {
                std::size_t pop_num = 0; // 人口数
                std::size_t sat_num = 0; // 集落数
                std::vector < std::vector<int>> mtdna_num(max_number_of_districts, std::vector<int>(256, 0)); // mtDNA 数
                std::vector < std::vector<int>> ydna_num(max_number_of_districts, std::vector<int>(256, 0)); // Y-DNA 数
                std::vector < std::vector<int>> language_num(max_number_of_districts, std::vector<int>(256, 0)); // 言語 数
                std::vector<std::vector<int>> pottery_num(max_number_of_districts, std::vector<int>(8, 0)); // 土器系統 数
                std::size_t ryopop[max_number_of_districts]{};
                std::size_t ryoset[max_number_of_districts]{};
                double ryosnp[max_number_of_districts]{};
                double ryolanguage[max_number_of_districts]{};

                std::array<std::uint_least32_t, 10> region_pop{};
                std::array<std::uint_least32_t, 10> region_set{};
                std::array<std::uint_least32_t, 10> region_snp{};
                std::array<std::uint_least32_t, 10> region_language{};
                std::vector<std::vector<int>> mtdna_region_num(10, std::vector<int>(256, 0)); // mtDNA 数
                std::vector<std::vector<int>> ydna_region_num(10, std::vector<int>(256, 0)); // Y-DNA 数
                std::vector<std::vector<int>> language_region_num(10, std::vector<int>(256, 0)); // 言語 数
                std::vector<std::vector<int>> pottery_region_num(10, std::vector<int>(8, 0)); // 土器系統 数

                // 地名を描画
                for (const auto& agent : getSettlementGrids()) {
                    for (const auto& settlement : agent.second.cgetSettlements()) {
                        ++sat_num; // 集落数を増加させる
                        pop_num += settlement.getPopulation(); // 人口数を増加させる

                        const std::uint_least8_t ryo_id = environment->template getData<std::uint_least8_t>(SimulationConstants::getInstance().district_key, settlement.getPosition());
                        if (ryo_id < max_number_of_districts) {
                            ryopop[ryo_id] += settlement.getPopulation(); // 地区ごとに人口数を増加させる
                            ryosnp[ryo_id] += settlement.getSNP(); // 地区ごとに SNP を増加させる
                            ryolanguage[ryo_id] += settlement.getLanguage(); // 地区ごとに言語を増加させる
                            ++(ryoset[ryo_id]);

                            // 地域区分
                            const std::uint_least8_t region_id = japan_provinces->getJapanRegionId(ryo_id);

                            // O(n) で統計を一括取得
                            const auto stats = settlement.getStatistics();

                            // mtDNA カウントの集計
                            for (const auto& mtdna_pair : stats.mtdna_counts) {
                                mtdna_num[ryo_id][mtdna_pair.first] += mtdna_pair.second;
                                if (region_id < 10) mtdna_region_num[region_id][mtdna_pair.first] += mtdna_pair.second;
                            }

                            // Y-DNA カウントの集計 (男性のみ)
                            for (const auto& ydna_pair : stats.ydna_counts) {
                                ydna_num[ryo_id][ydna_pair.first] += ydna_pair.second;
                                if (region_id < 10) ydna_region_num[region_id][ydna_pair.first] += ydna_pair.second;
                            }

                            // 言語カウントの集計
                            for (const auto& lang_pair : stats.language_counts) {
                                language_num[ryo_id][lang_pair.first] += lang_pair.second;
                                if (region_id < 10) language_region_num[region_id][lang_pair.first] += lang_pair.second;
                            }

                            // 土器系統カウントの集計
                            for (const auto& pottery_pair : stats.pottery_counts) {
                                pottery_num[ryo_id][pottery_pair.first] += pottery_pair.second;
                                if (region_id < 10) pottery_region_num[region_id][pottery_pair.first] += pottery_pair.second;
                            }
                        }
                    }
                }

                // Prepare district statistics
                paxs::StepStatistics stats;
                stats.step_count = step_count;
                stats.settlement_count = sat_num;
                stats.population_count = pop_num;
                stats.get_mtdna_name = [this](std::uint_least8_t id) { return japan_provinces->getMtDNA_Name(id); };
                stats.get_ydna_name = [this](std::uint_least8_t id) { return japan_provinces->getYDNA_Name(id); };
                stats.get_language_name = [this](std::uint_least8_t id) { return japan_provinces->getLanguageName(id); };
                stats.get_pottery_name = [](std::uint_least8_t id) { return std::string(1, static_cast<char>('A' + id)); };

                // Aggregate region stats and prepare district stats
                for (std::size_t i = 1; i < max_number_of_districts; ++i) {
                    // 地域区分
                    const std::uint_least8_t region_id = static_cast<std::uint_least8_t>(japan_provinces->getJapanRegionId(static_cast<std::uint_least8_t>(i)));
                    if (region_id < 10) {
                        region_pop[region_id] += static_cast<std::uint_least32_t>(ryopop[i]);
                        region_set[region_id] += static_cast<std::uint_least32_t>(ryoset[i]);
                        region_snp[region_id] += static_cast<std::uint_least32_t>(ryosnp[i]);
                        region_language[region_id] += static_cast<std::uint_least32_t>(ryolanguage[i]);
                    }

                    // Prepare district statistics
                    paxs::DistrictStatistics district_stat;
                    district_stat.population = ryopop[i];
                    district_stat.snp_avg = ryosnp[i] / static_cast<double>(ryoset[i]);
                    district_stat.language_avg = ryolanguage[i] / static_cast<double>(ryoset[i]);

                    for (std::size_t j = 0; j < japan_provinces->getSizeMtDNA(); ++j) {
                        if (int(mtdna_num[i][j]) > 0) {
                            district_stat.mtdna_counts[static_cast<std::uint_least8_t>(j)] = int(mtdna_num[i][j]);
                        }
                    }
                    for (std::size_t j = 0; j < japan_provinces->getSizeYDNA(); ++j) {
                        if (int(ydna_num[i][j]) > 0) {
                            district_stat.ydna_counts[static_cast<std::uint_least8_t>(j)] = int(ydna_num[i][j]);
                        }
                    }
                    for (std::size_t j = 0; j < japan_provinces->getSizeLanguage(); ++j) {
                        if (int(language_num[i][j]) > 0) {
                            district_stat.language_counts[static_cast<std::uint_least8_t>(j)] = int(language_num[i][j]);
                        }
                    }
                    for (std::size_t j = 0; j < pottery_num[i].size(); ++j) {
                        if (pottery_num[i][j] > 0) {
                            district_stat.pottery_counts[static_cast<std::uint_least8_t>(j)] = pottery_num[i][j];
                        }
                    }
                    stats.district_stats.push_back(district_stat);
                }

                // Prepare region statistics
                for (std::size_t region_id = 0; region_id < 10; ++region_id) {
                    paxs::DistrictStatistics region_stat;
                    region_stat.population = region_pop[region_id];
                    region_stat.snp_avg = static_cast<double>(region_snp[region_id]) / static_cast<double>(region_set[region_id]);
                    region_stat.language_avg = static_cast<double>(region_language[region_id]) / static_cast<double>(region_set[region_id]);

                    for (std::size_t j = 0; j < japan_provinces->getSizeMtDNA(); ++j) {
                        if (int(mtdna_region_num[region_id][j]) > 0) {
                            region_stat.mtdna_counts[static_cast<std::uint_least8_t>(j)] = int(mtdna_region_num[region_id][j]);
                        }
                    }
                    for (std::size_t j = 0; j < japan_provinces->getSizeYDNA(); ++j) {
                        if (int(ydna_region_num[region_id][j]) > 0) {
                            region_stat.ydna_counts[static_cast<std::uint_least8_t>(j)] = int(ydna_region_num[region_id][j]);
                        }
                    }
                    for (std::size_t j = 0; j < japan_provinces->getSizeLanguage(); ++j) {
                        if (int(language_region_num[region_id][j]) > 0) {
                            region_stat.language_counts[static_cast<std::uint_least8_t>(j)] = int(language_region_num[region_id][j]);
                        }
                    }
                    for (std::size_t j = 0; j < pottery_region_num[region_id].size(); ++j) {
                        if (pottery_region_num[region_id][j] > 0) {
                            region_stat.pottery_counts[static_cast<std::uint_least8_t>(j)] = pottery_region_num[region_id][j];
                        }
                    }
                    stats.region_stats.push_back(region_stat);
                }

                // Write all statistics in one call
                result_writer_->writeStepStatistics(stats);
            }

            std::vector<std::tuple<std::uint_least32_t, Vector2, Vector2>> move_list;
            for (auto& settlement_grid : settlement_grids) {
                std::vector<Settlement>& settlements = settlement_grid.second.getSettlements();
                for (std::size_t i = 0; i < settlements.size(); ++i) {
                    // 集落の過去の位置情報を削除
                    settlements[i].clearOldPosition();

                    if (settlements[i].isMoved()) {
                        continue;
                    }

                    auto [target_id, current_key, target_key] = settlements[i].move(
                        gen,
                        japan_provinces->getDistrict(environment->template getData<std::uint_least8_t>(SimulationConstants::getInstance().district_key, settlements[i].getPosition())),
                        japan_provinces.get()
                    );

                    if (target_id != 0) {
                        move_list.emplace_back(target_id, current_key, target_key);
                    }
                }
            }

            for (auto& move : move_list) {
                moveSettlement(std::get<0>(move), std::get<1>(move), std::get<2>(move));
            }
            const auto move_time = TimeUtils::now(); // 移動計測
            move_processing_time = TimeUtils::getDifferenceSeconds(start_time, move_time);

            // このステップでの出生数をカウント（preUpdate前の人口）
            std::size_t population_before_birth = 0;
            invalidatePopulationCache();
            for (const auto& settlement_grid : settlement_grids) {
                for (const auto& settlement : settlement_grid.second.cgetSettlements()) {
                    population_before_birth += settlement.getPopulation();
                }
            }

            for (auto& settlement_grid : settlement_grids) {
                for (auto& settlement : settlement_grid.second.getSettlements()) {
                    settlement.preUpdate(kanakuma_life_span, japan_provinces.get());
                }
            }

            // このステップでの出生数をカウント（preUpdate後の人口増加分）
            std::size_t population_after_birth = 0;
            for (const auto& settlement_grid : settlement_grids) {
                for (const auto& settlement : settlement_grid.second.cgetSettlements()) {
                    population_after_birth += settlement.getPopulation();
                }
            }

            // 出生数を記録（循環バッファ）
            const std::size_t births_this_step = (population_after_birth > population_before_birth) ?
                (population_after_birth - population_before_birth) : 0;
            birth_count_history_[birth_history_index_] = births_this_step;
            birth_history_index_ = (birth_history_index_ + 1) % SimulationConstants::getInstance().steps_per_year;

            // 渡来期間
            if (SimulationConstants::getInstance().immigration_step_interval > 0 &&
                step_count >= SimulationConstants::getInstance().immigration_start_steps &&
                step_count <= SimulationConstants::getInstance().immigration_end_steps &&
                (step_count - SimulationConstants::getInstance().immigration_start_steps) % SimulationConstants::getInstance().immigration_step_interval == 0) {
                randomizeSettlements(false, true /* 渡来人 */, (step_count >= SimulationConstants::getInstance().bronze_start_steps)/*青銅*/);
            }

            const auto m_start_time = TimeUtils::now();  // 婚姻計測開始
            marriage_pos_list.clear();
            bronze_share_list.clear(); // 青銅交換リストのクリア

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
                grid_position /= SimulationConstants::getInstance().cell_group_length;

                close_settlements_list.clear();
                for (int i = -1; i <= 1; ++i) {
                    for (int j = -1; j <= 1; ++j) {
                        if (auto* const ptr = settlement_grids.try_get((grid_position + Vector2(i, j)).to(SettlementGridsType{}))) {
                            close_settlements_list.emplace_back(&(ptr->getSettlements()));
                        }
                    }
                }
                for (auto& close_settlements : close_settlements_list) {
                    if (close_settlements->size() < 2) continue;

                    // 参照をキャッシュ
                    Settlement& first = close_settlements->front();
                    Settlement& last = close_settlements->back();

                    const std::uint_fast32_t bronze = (first.getBronze() + last.getBronze()) / 2;
                    first.setBronze(bronze);
                    last.setBronze(bronze);

                    bronze_share_list.emplace_back(first.getPosition(), last.getPosition());
                }
                for (auto& settlement : settlements) {
                    settlement.marriage(
                        close_settlements_list,
                        marriage_pos_list,
                        japan_provinces.get()
                    );
                }
            }

            const auto m_end_time = TimeUtils::now();  // 婚姻計測終了
            marriage_processing_time = TimeUtils::getDifferenceSeconds(m_start_time, m_end_time);

            for (auto& settlement_grid : settlement_grids) {
                for (auto& settlement : settlement_grid.second.getSettlements()) {
                    settlement.death();
                    settlement.setIsMoved(false);
                }
            }
            invalidatePopulationCache();
            // 集落の削除処理と集落の分割処理
            for (auto& settlement_grid : settlement_grids) {
                settlement_grid.second.checkSettlements();
                invalidatePopulationCache();
                settlement_grid.second.divideSettlements();
            }

            calcPop();

            if (SimulationConstants::getInstance().pottery_transmission_interval_steps > 0 &&
                (step_count % SimulationConstants::getInstance().pottery_transmission_interval_steps) == 0) {
                for (auto& settlement_grid : settlement_grids) {
                    for (auto& settlement : settlement_grid.second.getSettlements()) {
                        settlement.propagatePottery();
                    }
                }
            }

            // 渡来期間
            if (SimulationConstants::getInstance().immigration_step_interval > 0 &&
                step_count >= SimulationConstants::getInstance().immigration_start_steps &&
                step_count <= SimulationConstants::getInstance().immigration_end_steps &&
                (step_count - SimulationConstants::getInstance().immigration_start_steps) % SimulationConstants::getInstance().immigration_step_interval == 0) {
                // 渡来数を増やす
                japan_provinces->update();
            }
            ++step_count; // ステップ数を増やす
            const auto end_time = TimeUtils::now();  // 計測終了
            processing_time = TimeUtils::getDifferenceSeconds(start_time, end_time);
        }

        // 婚姻時に移動した位置一覧を取得する
        const std::vector<GridType4>& getMarriagePosList() const { return marriage_pos_list; }

        // 青銅交換の位置一覧を取得する
        const BronzeShareList& getBronzeShareList() const { return bronze_share_list; }

        /// @brief Get the agent list.
        /// @brief エージェントのリストを取得する
        constexpr paxs::UnorderedMap<SettlementGridsType, SettlementGrid>&
            getSettlementGrids() noexcept { return settlement_grids; }
        /// @brief Get the agent list.
        /// @brief エージェントのリストを取得する
        constexpr const paxs::UnorderedMap<SettlementGridsType, SettlementGrid>&
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

        /// @brief 進捗報告インターフェースを設定
        /// @brief Set progress reporter
        void setProgressReporter(IProgressReporter* reporter) {
            progress_reporter_ = reporter;
        }

    private:
        std::size_t population_num = 0; // 人口数
        std::size_t settlement_num = 0; // 集落数

        // キャッシュ管理用のメンバ追加
        mutable std::size_t cached_population_num = 0;
        mutable std::size_t cached_settlement_num = 0;
        mutable bool population_cache_valid = false;

        double processing_time = 0.0;
        double move_processing_time = 0.0;
        double marriage_processing_time = 0.0;

        paxs::UnorderedMap<SettlementGridsType, SettlementGrid> settlement_grids;
        std::shared_ptr<Environment> environment;

        // 地区ID → 集落グリッドキーのマップを追加
        std::unordered_map<std::uint_least8_t, std::vector<SettlementGridsType>> district_to_grids_;

        std::unique_ptr<paxs::JapanProvinces> japan_provinces;

        std::mt19937 gen; // 乱数生成器（メインスレッド用）

        // スレッドローカルな乱数生成器（OpenMP用）
#ifdef _OPENMP
        std::vector<std::mt19937> thread_local_gens_;
#endif

        KanakumaLifeSpan kanakuma_life_span;

        std::uint_least64_t migration_count = 0;
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

        // シミュレーション結果出力クラス
        std::unique_ptr<SimulationResultWriter> result_writer_;

        // 婚姻時に移動前の位置と移動後の位置を記録
        std::vector<GridType4> marriage_pos_list;

        // 青銅交換の位置を記録
        BronzeShareList bronze_share_list;

        // 進捗報告インターフェース
        IProgressReporter* progress_reporter_ = nullptr;

        // 直近1年間（steps_per_year分）の出生数を記録する循環バッファ
        std::vector<std::size_t> birth_count_history_;
        std::size_t birth_history_index_ = 0;

        /// @brief スレッドローカル乱数生成器を初期化
        /// @brief Initialize thread-local random generators
        void initThreadLocalGenerators() noexcept {
#ifdef _OPENMP
            const int num_threads = omp_get_max_threads();
            thread_local_gens_.clear();
            thread_local_gens_.reserve(num_threads);

            // 各スレッドに異なるシードを設定
            std::seed_seq seed_seq{ gen(), gen(), gen(), gen() };
            std::vector<std::uint32_t> seeds(num_threads);
            seed_seq.generate(seeds.begin(), seeds.end());

            for (int i = 0; i < num_threads; ++i) {
                thread_local_gens_.emplace_back(seeds[i]);
            }
#endif
        }

        /// @brief スレッド安全な乱数生成器を取得
        /// @brief Get thread-safe random generator
        /// @return スレッドローカルな乱数生成器の参照
        std::mt19937& getThreadLocalGen() noexcept {
#ifdef _OPENMP
            const int thread_id = omp_get_thread_num();
            return thread_local_gens_[thread_id];
#else
            return gen;
#endif
        }

        /// @brief エージェントをランダムに生成するヘルパーメソッド（スレッドセーフ版）
        /// @brief Create a random agent helper method (thread-safe)
        /// @param is_farming 農耕民かどうか / Is farming
        /// @param district_id 地区ID / District ID
        /// @param thread_gen スレッドローカルな乱数生成器 / Thread-local random generator
        /// @return 生成されたエージェント / Generated agent
        Agent createRandomAgentThreadSafe(bool is_farming, std::uint_least8_t district_id, std::mt19937& thread_gen) {
            const std::uint_least8_t mtdna_value = japan_provinces->getMtDNA(district_id, thread_gen);
            const std::uint_least8_t ydna_value = japan_provinces->getYDNA(district_id, thread_gen);
            Genome genome = Genome::generateRandomSetMtDNA(
                thread_gen,
                mtdna_value,
                ydna_value,
                static_cast<std::uint_least8_t>(japan_provinces->getSNP(district_id)),
                japan_provinces->getLanguage(district_id, thread_gen)
            );
            applyImmigrantGenderBias(genome, is_farming, thread_gen, ydna_value);

            const std::uint_least8_t rice_type = selectRiceType(is_farming);
            const AgeType set_lifespan = kanakuma_life_span.setLifeSpan(
                rice_type == SettlementAgent::RiceTypePaddy, genome.isMale(), thread_gen);

            const std::uint_least8_t pottery_lineage = selectPotteryLineage(is_farming, district_id);
            const std::uint_least8_t pottery_make =
                (pottery_lineage > 0 && isPotteryGenderAllowed(genome, SimulationConstants::getInstance().pottery_make_mode)) ? 1 : 0;

            AgeType age_value = 0;
            if (set_lifespan > SimulationConstants::getInstance().init_lifespan_grace_period) {
                std::uniform_int_distribution<> lifespan_dist{
                    0, static_cast<int>(set_lifespan - SimulationConstants::getInstance().init_lifespan_grace_period)
                };
                age_value = static_cast<AgeType>(lifespan_dist(thread_gen));
            }

            return Agent(
                UniqueIdentification<std::uint_least32_t>::generate(),
                age_value,
                set_lifespan,
                genome,
                static_cast<std::uint_least8_t>(japan_provinces->getFarming(district_id)),
                static_cast<std::uint_least8_t>(japan_provinces->getHunterGatherer(district_id)),
                static_cast<std::uint_least8_t>(japan_provinces->getLanguage(district_id)),
                pottery_make,
                pottery_lineage,
                rice_type
            );
        }

        /// @brief エージェントをランダムに生成するヘルパーメソッド
        /// @brief Create a random agent helper method
        /// @param is_farming 農耕民かどうか / Is farming
        /// @param district_id 地区ID / District ID
        /// @return 生成されたエージェント / Generated agent
        Agent createRandomAgent(bool is_farming, std::uint_least8_t district_id) {
            return createRandomAgentThreadSafe(is_farming, district_id, gen);
        }

        bool isPotteryGenderAllowed(const Genome& genome, const std::uint_least8_t mode) const noexcept {
            // 土器作り対象の性別判定（女性/男性/男女）
            if (mode == 1) return genome.isFemale();
            if (mode == 2) return genome.isMale();
            if (mode == 3) return true;
            return false;
        }

        std::uint_least8_t selectRiceType(const bool is_farming) const noexcept {
            // 農耕でなければ稲作なし
            if (!is_farming) return SettlementAgent::RiceTypeNone;
            // 水田稲作開始ステップ以降は水田稲作
            if (SimulationConstants::getInstance().rice_paddy_start_steps > 0 &&
                step_count >= SimulationConstants::getInstance().rice_paddy_start_steps) {
                return SettlementAgent::RiceTypePaddy;
            }
            // それ以前はイネ栽培
            return SettlementAgent::RiceTypeIne;
        }

        std::uint_least8_t selectPotteryLineage(const bool is_farming) const noexcept {
            return selectPotteryLineage(is_farming, 0);
        }

        std::uint_least8_t selectPotteryLineage(const bool is_farming, const std::uint_least8_t district_id = 0) const noexcept {
            // 在地人は地区ごとの初期系統を利用
            if (!is_farming) return japan_provinces->getPotteryLineageInitial(district_id);
            // 渡来人は期間に応じて系統を切り替える
            if (SimulationConstants::getInstance().pottery_lineage_change_step > 0 &&
                step_count >= SimulationConstants::getInstance().pottery_lineage_change_step) {
                return SimulationConstants::getInstance().pottery_lineage_immigrant_after;
            }
            // 変更前の渡来系統
            return SimulationConstants::getInstance().pottery_lineage_immigrant_before;
        }

        void applyImmigrantGenderBias(Genome& genome, const bool is_farming, std::mt19937& thread_gen, const std::uint_least8_t ydna_value) const noexcept {
            // 農耕文化以外は性別操作しない
            if (!is_farming) return;
            const double male_probability = SimulationConstants::getInstance().immigrant_male_probability;
            // 男性のみ/女性のみのケースを先に処理
            if (male_probability <= 0.0) {
                genome.setYDNA(0);
                return;
            }
            if (male_probability >= 1.0) {
                genome.setYDNA(ydna_value);
                return;
            }
            // 確率で男性/女性を決める
            const bool is_male = (SimulationConstants::getInstance().random_dist(thread_gen) < male_probability);
            genome.setYDNA(is_male ? ydna_value : 0);
        }

        /// @brief ()
        /// @brief 集落をランダムに配置する前の初期化処理
        bool initRandomizeSettlements() {
            if (progress_reporter_ != nullptr) {
                progress_reporter_->startProgress(0, "Randomize settlements...");
            }
            else {
                std::cout << "Randomize settlements..." << std::endl;
            }

            environment->getLandPositions(land_positions);

            live_list = std::unique_ptr<std::array<Live, max_number_of_districts + 1>>(new std::array<Live, max_number_of_districts + 1>());

            if (live_list.get() == nullptr) {
                if (progress_reporter_ != nullptr) {
                    progress_reporter_->reportProgress(0.0f, "Low memory");
                }
                else {
                    std::cout << "Low memory" << std::endl;
                }
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
                std::uint_least8_t district_id = environment->template getData<std::uint_least8_t>(SimulationConstants::getInstance().district_key, position);
                if (district_id < max_number_of_districts) {
                    (*live_list)[district_id].emplaceBack(live_probability, land_position);
                }
            }

            // スレッドローカル乱数生成器を初期化
            initThreadLocalGenerators();

            return true;
        }

        /// @brief Randomly place settlements (OpenMP parallelized version - Further optimized).
        /// @brief 集落をランダムに配置する（OpenMP並列化版 - さらなる最適化）
        void randomizeSettlements(
            bool is_ad200,
            bool is_farming,
            bool is_bronze
        ) noexcept {
            std::uint_least8_t district_id_max = 0;

            paxs::UnorderedMap<std::uint_least8_t, std::uint_least32_t> district_population_map;
            for (const auto& district : japan_provinces->getDistrictList()) {
                if (((is_ad200) ? district.init_pop : district.immigrant) == 0) {
                    continue;
                }
                district_population_map.emplace(district.id, ((is_ad200) ? district.init_pop : district.immigrant));
                district_id_max = (std::max)(district.id, district_id_max);
            }

            int all_population = 0;
            if (step_count == 0) {
                for (auto& district_population : district_population_map) {
                    all_population += static_cast<int>(district_population.second);
                }
            }

            district_id_max = (district_id_max < max_number_of_districts) ? district_id_max + 1 : max_number_of_districts;

            // 地区情報を事前キャッシュ
            std::vector<paxs::District> district_cache(district_id_max);
            for (std::uint_least8_t i = 0; i < district_id_max; ++i) {
                district_cache[i] = japan_provinces->getDistrict(i);
            }

            // 並列処理用の地区リストを作成
            std::vector<std::uint_least8_t> district_ids_to_process;
            district_ids_to_process.reserve(district_id_max);
            for (std::uint_least8_t district_id = 0; district_id < district_id_max; ++district_id) {
                Live& live = (*live_list)[district_id];
                if (live.live_probabilities.size() > 0 && district_population_map.contains(district_id)) {
                    district_ids_to_process.push_back(district_id);
                }
            }

            // 最適化1: 地区ごとの処理結果構造体を改良
            struct DistrictProcessingResult {
                std::vector<Settlement> settlements;
                std::vector<std::pair<SettlementGridsType, Vector2>> grid_positions;
                std::vector<std::uint_least8_t> district_ids; // ✅ 追加: 地区IDリスト
                int population_processed = 0;
                std::uint_least64_t local_migration_count = 0;

                // メモリを事前確保
                void reserve(std::size_t estimated_settlements) {
                    settlements.reserve(estimated_settlements);
                    grid_positions.reserve(estimated_settlements);
                    district_ids.reserve(estimated_settlements);
                }
            };

            std::vector<DistrictProcessingResult> district_results(district_ids_to_process.size());

            // 最適化2: 各地区の推定集落数を事前計算
#ifdef _OPENMP
#pragma omp parallel for schedule(static)
#endif
            for (int idx = 0; idx < static_cast<int>(district_ids_to_process.size()); ++idx) {
                const std::uint_least8_t district_id = district_ids_to_process[idx];
                auto* const district_population_ptr = district_population_map.try_get(district_id);
                if (district_population_ptr != nullptr) {
                    paxs::District& district = district_cache[district_id];
                    const std::size_t estimated_settlements =
                        (*district_population_ptr) / ((district.settlement_pop_min + district.settlement_pop_max) / 2) + 5;
                    district_results[idx].reserve(estimated_settlements);
                }
            }

            // 最適化3: 並列処理（動的スケジューリング）
#ifdef _OPENMP
#pragma omp parallel for schedule(dynamic, 2)
#endif
            for (int idx = 0; idx < static_cast<int>(district_ids_to_process.size()); ++idx) {
                const std::uint_least8_t district_id = district_ids_to_process[idx];
                Live& live = (*live_list)[district_id];
                paxs::District& district = district_cache[district_id];

                auto* const district_population_ptr = district_population_map.try_get(district_id);
                if (district_population_ptr == nullptr) continue;

                std::mt19937& thread_gen = getThreadLocalGen();
                std::discrete_distribution<> live_probability_dist(live.live_probabilities.begin(), live.live_probabilities.end());
                const std::uint_least8_t immigration_and_district_id = (is_farming) ?
                    SimulationConstants::getInstance().immigration_district_id : district_id;

                DistrictProcessingResult& result = district_results[idx];
                int remaining_population = static_cast<int>(*district_population_ptr);

                // 最適化4: エージェント生成バッチサイズを調整
                constexpr int BATCH_SIZE = 100; // 100人単位でバッチ処理
                std::vector<Agent> agent_batch;
                agent_batch.reserve(BATCH_SIZE);

                while (live.live_probabilities.size() > 0 && remaining_population > 0) {
                    const int live_probability_index = live_probability_dist(thread_gen);
                    const Vector2 live_position = Vector2::from(live.habitable_land_positions[live_probability_index]);

                    int settlement_population = std::uniform_int_distribution<>(
                        district.settlement_pop_min, district.settlement_pop_max)(thread_gen);
                    settlement_population = (std::min)(settlement_population, remaining_population);

                    Vector2 grid_position = live_position / SimulationConstants::getInstance().cell_group_length;
                    SettlementGridsType key = grid_position.to(SettlementGridsType{});

                    Settlement settlement = Settlement(
                        UniqueIdentification<std::uint_least32_t>::generate(),
                        thread_gen,
                        environment
                    );
                    if (is_bronze) settlement.setBronze(static_cast<std::uint_least32_t>(SimulationConstants::getInstance().bronze));
                    settlement.setPosition(live_position);

                    // 最適化5: バッチ処理でエージェント生成
                    std::vector<Agent> agents;
                    agents.reserve(settlement_population);

                    for (int i = 0; i < settlement_population; ++i) {
                        agents.emplace_back(createRandomAgentThreadSafe(is_farming, immigration_and_district_id, thread_gen));
                        if (is_farming) ++result.local_migration_count;
                    }

                    settlement.setAgents(std::move(agents));

                    result.settlements.push_back(std::move(settlement));
                    result.grid_positions.emplace_back(key, grid_position * SimulationConstants::getInstance().cell_group_length);
                    result.district_ids.push_back(district_id); // ✅ 地区IDを記録
                    result.population_processed += settlement_population;
                    remaining_population -= settlement_population;

                    // 分布の更新
                    if (live.live_probabilities.size() > 1) {
                        const std::size_t last_idx = live.live_probabilities.size() - 1;
                        if (live_probability_index != static_cast<int>(last_idx)) {
                            live.live_probabilities[live_probability_index] = live.live_probabilities[last_idx];
                            live.habitable_land_positions[live_probability_index] = live.habitable_land_positions[last_idx];
                        }
                        live.live_probabilities.pop_back();
                        live.habitable_land_positions.pop_back();

                        if (live.live_probabilities.size() > 0) {
                            live_probability_dist = std::discrete_distribution<>(
                                live.live_probabilities.begin(),
                                live.live_probabilities.end()
                            );
                        }
                    }
                    else {
                        break;
                    }
                }
            }

            // 最適化6: 結果マージ処理の高速化（unordered_setで重複チェック）
            int population_sum = 0;
            std::unordered_map<std::uint_least8_t, std::unordered_set<SettlementGridsType>> district_grid_cache;

            for (auto& result : district_results) {
                for (std::size_t i = 0; i < result.settlements.size(); ++i) {
                    const auto& settlement = result.settlements[i];
                    const auto& [key, grid_pos] = result.grid_positions[i];
                    const std::uint_least8_t district_id = result.district_ids[i];

                    auto [grid_it, inserted] = settlement_grids.try_emplace(key,
                        SettlementGrid(grid_pos, environment, gen));

                    grid_it->second.addSettlement(settlement);
                    grid_it->second.addDistrictId(district_id);

                    // unordered_setで高速重複チェック（O(1)）
                    auto& grid_set = district_grid_cache[district_id];
                    if (grid_set.insert(key).second) {
                        district_to_grids_[district_id].push_back(key);
                    }
                }

                population_sum += result.population_processed;
                migration_count += result.local_migration_count;
            }

            if (step_count == 0) {
                if (progress_reporter_ != nullptr) {
                    progress_reporter_->endProgress();
                }
                else {
                    StatusDisplayer::displayProgressBar(all_population, all_population);
                    std::cout << std::endl;
                }
            }

            // 最適化7: 残存人口処理も並列化可能な場合は並列化
            std::vector<std::pair<std::uint_least8_t, int>> remaining_populations;
            for (auto& district_population : district_population_map) {
                if (district_population.second > 0) {
                    remaining_populations.emplace_back(district_population.first, district_population.second);
                }
            }

            // 残存人口が少ない場合は直列処理、多い場合は並列化
            const bool parallelize_remaining = remaining_populations.size() > 10;

#ifdef _OPENMP
            if (parallelize_remaining) {
#pragma omp parallel for schedule(dynamic)
                for (int i = 0; i < static_cast<int>(remaining_populations.size()); ++i) {
                    const auto& [district_id, population] = remaining_populations[i];

                    std::vector<Settlement*> settlement_ptrs;
#pragma omp critical
                    {
                        getSettlements(settlement_ptrs, district_id);
                    }

                    if (settlement_ptrs.empty()) {
                        continue;
                    }

                    const int add_population = population / static_cast<int>(settlement_ptrs.size());
                    const std::uint_least8_t immigration_and_district_id = (is_farming) ?
                        SimulationConstants::getInstance().immigration_district_id : district_id;

                    std::mt19937& thread_gen = getThreadLocalGen();

                    for (auto* settlement_ptr : settlement_ptrs) {
                        std::vector<Agent> agents;
                        agents.reserve(add_population);

                        for (int j = 0; j < add_population; ++j) {
                            agents.emplace_back(createRandomAgentThreadSafe(is_farming, immigration_and_district_id, thread_gen));
                        }

#pragma omp critical
                        {
                            settlement_ptr->addAgents(agents);
                            if (is_farming) migration_count += add_population;
                            if (is_bronze) {
                                settlement_ptr->setBronze(static_cast<std::uint_least32_t>(SimulationConstants::getInstance().bronze));
                            }
                        }
                    }
                }
            }
            else
#endif
            {
                // 直列処理（元のコード）
                for (const auto& [district_id, population] : remaining_populations) {
                    std::vector<Settlement*> settlement_ptrs;
                    getSettlements(settlement_ptrs, district_id);

                    if (settlement_ptrs.empty()) {
                        PAXS_WARNING("Settlements not found. District ID: " + std::to_string(district_id));
                        continue;
                    }

                    const int add_population = population / static_cast<int>(settlement_ptrs.size());
                    const std::uint_least8_t immigration_and_district_id = (is_farming) ?
                        SimulationConstants::getInstance().immigration_district_id : district_id;

                    for (auto* settlement_ptr : settlement_ptrs) {
                        std::vector<Agent> agents;
                        agents.reserve(add_population);

                        for (int i = 0; i < add_population; ++i) {
                            agents.emplace_back(createRandomAgent(is_farming, immigration_and_district_id));
                            if (is_farming) ++migration_count;
                        }

                        settlement_ptr->addAgents(agents);

                        if (is_bronze) {
                            settlement_ptr->setBronze(static_cast<std::uint_least32_t>(SimulationConstants::getInstance().bronze));
                        }
                    }
                }
            }

            if (step_count == 0) {
                if (progress_reporter_ != nullptr) {
                    progress_reporter_->reportProgress(1.0f, "Done.");
                }
                else {
                    std::cout << "Done." << std::endl;
                }
            }
        }

        /// @brief 指定した地区のIDの集落グリッドを取得
        void getSettlementGrids(std::vector<SettlementGrid>& settlement_grids_, const std::uint_least8_t target_district_id) noexcept {
            for (auto& settlement_grid : settlement_grids) {
                std::vector<std::uint_least8_t> district_ids = settlement_grid.second.getDistrictIds();
                for (auto district_id : district_ids) {
                    if (district_id == target_district_id) {
                        settlement_grids_.emplace_back(settlement_grid.second);
                        break;
                    }
                }
            }
        }

        /// @brief 指定した地区のIDの集落を取得（最適化版）
        /// @brief Get settlements of specified district ID (optimized)
        void getSettlements(std::vector<Settlement*>& settlements, const std::uint_least8_t district_id) noexcept {
            auto it = district_to_grids_.find(district_id);
            if (it == district_to_grids_.end()) return;

            settlements.reserve(settlements.size() + it->second.size() * 10); // 概算予約

            for (const auto& grid_key : it->second) {
                if (auto* grid_ptr = settlement_grids.try_get(grid_key)) {
                    for (auto& settlement : grid_ptr->getSettlements()) {
                        settlements.push_back(&settlement);
                    }
                }
            }
        }
    };
}

#endif // !PAX_SAPIENTICA_SIMULATION_MANAGER_SETTLEMENT_SIMULATOR_HPP
