/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SIMULATION_OUTPUT_SIMULATION_RESULT_WRITER_HPP
#define PAX_SAPIENTICA_SIMULATION_OUTPUT_SIMULATION_RESULT_WRITER_HPP

#include <fstream>
#include <functional>
#include <map>
#include <string>

#include <PAX_SAPIENTICA/Core/Utility/TimeUtils.hpp>
#include <PAX_SAPIENTICA/IO/File/FileSystem.hpp>
#include <PAX_SAPIENTICA/Simulation/Config/SimulationConst.hpp>

namespace paxs {

    /// @brief 地区別統計データ
    /// @brief District statistics data
    struct DistrictStatistics {
        std::size_t population = 0;
        double snp_avg = 0.0;
        double language_avg = 0.0;
        std::map<std::uint_least8_t, int> mtdna_counts;
        std::map<std::uint_least8_t, int> language_counts;
    };

    /// @brief ステップ統計データ
    /// @brief Step statistics data
    struct StepStatistics {
        std::uint_least64_t step_count = 0;
        std::size_t settlement_count = 0;
        std::size_t population_count = 0;
        std::vector<DistrictStatistics> district_stats;
        std::vector<DistrictStatistics> region_stats;
        std::function<std::string(std::uint_least8_t)> get_mtdna_name;
        std::function<std::string(std::uint_least8_t)> get_language_name;
    };

    /// @brief シミュレーション結果をファイルに出力するクラス
    /// @brief Class for writing simulation results to files
    /// @details 人口、mtDNA、言語、SNPなどの統計データをTXTファイルに出力
    ///          Outputs statistical data such as population, mtDNA, language, SNP to TXT files
    class SimulationResultWriter {
    private:
        // 標準出力用ファイルストリーム / File streams for standard output
        std::ofstream pop_ofs;
        std::ofstream mtdna_ofs;
        std::ofstream language_dna_ofs;
        std::ofstream snp_ofs;
        std::ofstream language_ofs;
        std::ofstream live_ofs;

        std::ofstream pop_region_ofs;
        std::ofstream mtdna_region_ofs;
        std::ofstream language_dna_region_ofs;
        std::ofstream snp_region_ofs;
        std::ofstream language_region_ofs;
        std::ofstream live_region_ofs;

        // ラベル形式用のファイルストリーム / File streams for labeled format
        std::ofstream labeled_pop_ofs;
        std::ofstream labeled_mtdna_ofs;
        std::ofstream labeled_language_dna_ofs;
        std::ofstream labeled_snp_ofs;
        std::ofstream labeled_language_ofs;
        std::ofstream labeled_live_ofs;

        std::ofstream labeled_pop_region_ofs;
        std::ofstream labeled_mtdna_region_ofs;
        std::ofstream labeled_language_dna_region_ofs;
        std::ofstream labeled_snp_region_ofs;
        std::ofstream labeled_language_region_ofs;
        std::ofstream labeled_live_region_ofs;

        std::string result_directory_;
        std::string labeled_directory_;
        bool is_labeled_output_enabled_ = false;
        bool live_header_written_ = false;

        /// @brief 現在の日時を文字列として取得
        /// @brief Get current date and time as string
        /// @return YYYY-MM-DD-HH-MM-SS 形式の文字列 / String in YYYY-MM-DD-HH-MM-SS format
        std::string calcDateTime() const {
            return TimeUtils::getCurrentDateTime("%Y-%m-%d-%H-%M-%S");
        }

        /// @brief ファイルストリームを閉じる
        /// @brief Close a file stream
        /// @param ofs 閉じるファイルストリーム / File stream to close
        void closeStream(std::ofstream& ofs) {
            if (ofs.is_open()) {
                ofs.close();
            }
        }

        /// @brief 結果ファイルのヘッダー文字列を出力
        /// @brief Output header string for result file
        /// @param ofs 出力先ファイルストリーム / Output file stream
        void outputResultString(std::ofstream& ofs) {
            ofs << "steps,";
        }

        /// @brief 地区名を出力
        /// @brief Output district name
        /// @param ofs 出力先ファイルストリーム / Output file stream
        /// @param district_name 地区名 / District name
        void outputResultDistrictName(std::ofstream& ofs, const std::string& district_name) {
            ofs << district_name << ",";
        }

        /// @brief 最後の文字列（改行）を出力
        /// @brief Output last string (newline)
        /// @param ofs 出力先ファイルストリーム / Output file stream
        void outputResultLastString(std::ofstream& ofs) {
            ofs << std::endl;
        }

    public:
        SimulationResultWriter() = default;
        ~SimulationResultWriter() {
            close();
        }

        // コピー禁止 / Disable copy
        SimulationResultWriter(const SimulationResultWriter&) = delete;
        SimulationResultWriter& operator=(const SimulationResultWriter&) = delete;

        // ムーブ許可 / Enable move
        SimulationResultWriter(SimulationResultWriter&&) noexcept = default;
        SimulationResultWriter& operator=(SimulationResultWriter&&) noexcept = default;

        /// @brief 結果ファイルを初期化
        /// @brief Initialize result files
        /// @param label_name ラベル名（空の場合はラベル出力なし） / Label name (no labeled output if empty)
        /// @param district_list 地区リスト / District list
        template<typename DistrictList>
        void initialize(const std::string& label_name, const DistrictList& district_list) {
            close(); // 既存のファイルを閉じる / Close existing files

            // 標準出力用ディレクトリとファイルを作成
            result_directory_ = "SimulationResults/" + calcDateTime();
            FileSystem::createDirectories(result_directory_);

            pop_ofs.open(result_directory_ + "/Population.txt");
            mtdna_ofs.open(result_directory_ + "/mtDNA.txt");
            language_dna_ofs.open(result_directory_ + "/Language_DNA.txt");
            snp_ofs.open(result_directory_ + "/SNP.txt");
            language_ofs.open(result_directory_ + "/Language.txt");
            live_ofs.open(result_directory_ + "/HabitableLand.txt");

            pop_region_ofs.open(result_directory_ + "/Region_Population.txt");
            mtdna_region_ofs.open(result_directory_ + "/Region_mtDNA.txt");
            language_dna_region_ofs.open(result_directory_ + "/Region_Language_DNA.txt");
            snp_region_ofs.open(result_directory_ + "/Region_SNP.txt");
            language_region_ofs.open(result_directory_ + "/Region_Language.txt");
            live_region_ofs.open(result_directory_ + "/Region_HabitableLand.txt");

            // ラベル形式の出力が必要な場合
            if (!label_name.empty()) {
                is_labeled_output_enabled_ = true;
                const std::string timestamp = calcDateTime();
                labeled_directory_ = "LabeledSimulationResults/" + label_name;

                // ディレクトリ作成
                FileSystem::createDirectories(labeled_directory_ + "/Population");
                FileSystem::createDirectories(labeled_directory_ + "/mtDNA");
                FileSystem::createDirectories(labeled_directory_ + "/SNP");
                FileSystem::createDirectories(labeled_directory_ + "/Language");
                FileSystem::createDirectories(labeled_directory_ + "/Language_DNA");
                FileSystem::createDirectories(labeled_directory_ + "/Region_Population");
                FileSystem::createDirectories(labeled_directory_ + "/Region_mtDNA");
                FileSystem::createDirectories(labeled_directory_ + "/Region_SNP");
                FileSystem::createDirectories(labeled_directory_ + "/Region_Language");
                FileSystem::createDirectories(labeled_directory_ + "/Region_Language_DNA");

                // ファイルを開く
                labeled_pop_ofs.open(labeled_directory_ + "/Population/" + timestamp + ".txt");
                labeled_mtdna_ofs.open(labeled_directory_ + "/mtDNA/" + timestamp + ".txt");
                labeled_language_dna_ofs.open(labeled_directory_ + "/Language_DNA/" + timestamp + ".txt");
                labeled_snp_ofs.open(labeled_directory_ + "/SNP/" + timestamp + ".txt");
                labeled_language_ofs.open(labeled_directory_ + "/Language/" + timestamp + ".txt");
                labeled_pop_region_ofs.open(labeled_directory_ + "/Region_Population/" + timestamp + ".txt");
                labeled_mtdna_region_ofs.open(labeled_directory_ + "/Region_mtDNA/" + timestamp + ".txt");
                labeled_language_dna_region_ofs.open(labeled_directory_ + "/Region_Language_DNA/" + timestamp + ".txt");
                labeled_snp_region_ofs.open(labeled_directory_ + "/Region_SNP/" + timestamp + ".txt");
                labeled_language_region_ofs.open(labeled_directory_ + "/Region_Language/" + timestamp + ".txt");

                // ヘッダーを書き込む
                writeHeaders(true, district_list);
            }

            // 標準出力のヘッダーを書き込む
            writeHeaders(false, district_list);
        }

        /// @brief ヘッダー行を書き込む
        /// @brief Write header rows
        /// @param labeled ラベル出力用かどうか / Whether for labeled output
        /// @param district_names 地区名リスト / District names list
        template<typename DistrictList>
        void writeHeaders(bool labeled, const DistrictList& district_list) {
            if (labeled && !is_labeled_output_enabled_) return;

            auto& pop = labeled ? labeled_pop_ofs : pop_ofs;
            auto& mtdna = labeled ? labeled_mtdna_ofs : mtdna_ofs;
            auto& lang_dna = labeled ? labeled_language_dna_ofs : language_dna_ofs;
            auto& snp = labeled ? labeled_snp_ofs : snp_ofs;
            auto& lang = labeled ? labeled_language_ofs : language_ofs;
            auto& pop_reg = labeled ? labeled_pop_region_ofs : pop_region_ofs;
            auto& mtdna_reg = labeled ? labeled_mtdna_region_ofs : mtdna_region_ofs;
            auto& lang_dna_reg = labeled ? labeled_language_dna_region_ofs : language_dna_region_ofs;
            auto& snp_reg = labeled ? labeled_snp_region_ofs : snp_region_ofs;
            auto& lang_reg = labeled ? labeled_language_region_ofs : language_region_ofs;

            outputResultString(pop);
            outputResultString(mtdna);
            outputResultString(lang_dna);
            outputResultString(snp);
            outputResultString(lang);

            if (labeled) {
                outputResultString(pop_reg);
                outputResultString(mtdna_reg);
                outputResultString(lang_dna_reg);
                outputResultString(snp_reg);
                outputResultString(lang_reg);
            }

            for (std::size_t i = 0; i < max_number_of_districts - 1; ++i) {
                outputResultDistrictName(pop, district_list[i].name);
                outputResultDistrictName(mtdna, district_list[i].name);
                outputResultDistrictName(lang_dna, district_list[i].name);
                outputResultDistrictName(snp, district_list[i].name);
                outputResultDistrictName(lang, district_list[i].name);
            }

            outputResultLastString(pop);
            outputResultLastString(mtdna);
            outputResultLastString(lang_dna);
            outputResultLastString(snp);
            outputResultLastString(lang);

            if (labeled) {
                outputResultLastString(pop_reg);
                outputResultLastString(mtdna_reg);
                outputResultLastString(lang_dna_reg);
                outputResultLastString(snp_reg);
                outputResultLastString(lang_reg);
            }
        }

        /// @brief 全てのファイルストリームを閉じる
        /// @brief Close all file streams
        void close() {
            closeStream(pop_ofs);
            closeStream(mtdna_ofs);
            closeStream(language_dna_ofs);
            closeStream(snp_ofs);
            closeStream(language_ofs);
            closeStream(live_ofs);

            closeStream(pop_region_ofs);
            closeStream(mtdna_region_ofs);
            closeStream(language_dna_region_ofs);
            closeStream(snp_region_ofs);
            closeStream(language_region_ofs);
            closeStream(live_region_ofs);

            closeStream(labeled_pop_ofs);
            closeStream(labeled_mtdna_ofs);
            closeStream(labeled_language_dna_ofs);
            closeStream(labeled_snp_ofs);
            closeStream(labeled_language_ofs);
            closeStream(labeled_live_ofs);

            closeStream(labeled_pop_region_ofs);
            closeStream(labeled_mtdna_region_ofs);
            closeStream(labeled_language_dna_region_ofs);
            closeStream(labeled_snp_region_ofs);
            closeStream(labeled_language_region_ofs);
            closeStream(labeled_live_region_ofs);

            is_labeled_output_enabled_ = false;
        }

        /// @brief 可住地情報を書き込む
        /// @brief Write habitable land information
        /// @param district_name 地区名 / District name
        /// @param habitable_land_count 可住地数 / Habitable land count
        void writeHabitableLand(const std::string& district_name, std::size_t habitable_land_count) {
            if (live_ofs.is_open()) {
                if (!live_header_written_) {
                    live_ofs << "district\thabitable_land\n";
                    live_header_written_ = true;
                }
                live_ofs << district_name << '\t' << habitable_land_count << '\n';
            }
        }

        /// @brief ステップごとの統計データを書き込む
        /// @brief Write statistics for each step
        /// @param stats 統計データ / Statistics data
        void writeStepStatistics(const StepStatistics& stats) {
            writeStepStatisticsToStreams(stats,
                pop_ofs, mtdna_ofs, language_dna_ofs, snp_ofs, language_ofs,
                pop_region_ofs, mtdna_region_ofs, language_dna_region_ofs, snp_region_ofs, language_region_ofs
            );

            // ラベル付き出力
            if (is_labeled_output_enabled_) {
                writeStepStatisticsToStreams(stats,
                    labeled_pop_ofs, labeled_mtdna_ofs, labeled_language_dna_ofs, labeled_snp_ofs, labeled_language_ofs,
                    labeled_pop_region_ofs, labeled_mtdna_region_ofs, labeled_language_dna_region_ofs, labeled_snp_region_ofs, labeled_language_region_ofs
                );
            }
        }

    private:
        /// @brief ファイルストリームに統計データを書き込む
        /// @brief Write statistics to file streams
        struct OutputStreams {
            std::ofstream& pop;
            std::ofstream& mtdna;
            std::ofstream& lang_dna;
            std::ofstream& snp;
            std::ofstream& lang;
            std::ofstream& pop_reg;
            std::ofstream& mtdna_reg;
            std::ofstream& lang_dna_reg;
            std::ofstream& snp_reg;
            std::ofstream& lang_reg;
        };

        void writeStepStatisticsToStreams(
            const StepStatistics& stats,
            std::ofstream& pop, std::ofstream& mtdna, std::ofstream& lang_dna, std::ofstream& snp, std::ofstream& lang,
            std::ofstream& pop_reg, std::ofstream& mtdna_reg, std::ofstream& lang_dna_reg, std::ofstream& snp_reg, std::ofstream& lang_reg
        ) {
            const std::uint_least64_t sc = stats.step_count;
            const std::size_t set_count = stats.settlement_count;
            const std::size_t pop_count = stats.population_count;

            // ヘッダー行
            pop << sc << '\t' << set_count << '\t' << pop_count << '\t';
            mtdna << sc << '\t' << set_count << '\t' << pop_count << '\t';
            lang_dna << sc << '\t' << set_count << '\t' << pop_count << '\t';
            snp << sc << '\t' << set_count << '\t' << pop_count << '\t';
            lang << sc << '\t' << set_count << '\t' << pop_count << '\t';
            pop_reg << sc << '\t' << set_count << '\t' << pop_count << '\t';
            mtdna_reg << sc << '\t' << set_count << '\t' << pop_count << '\t';
            lang_dna_reg << sc << '\t' << set_count << '\t' << pop_count << '\t';
            snp_reg << sc << '\t' << set_count << '\t' << pop_count << '\t';
            lang_reg << sc << '\t' << set_count << '\t' << pop_count << '\t';

            // 地区別統計
            for (const auto& stat : stats.district_stats) {
                pop << stat.population << '\t';
                snp << stat.snp_avg << '\t';
                lang << stat.language_avg << '\t';

                for (const auto& [haplotype, count] : stat.mtdna_counts) {
                    if (count > 0) mtdna << stats.get_mtdna_name(haplotype) << ':' << count << '/';
                }
                mtdna << '\t';

                for (const auto& [language_id, count] : stat.language_counts) {
                    if (count > 0) lang_dna << stats.get_language_name(language_id) << ':' << count << '/';
                }
                lang_dna << '\t';
            }

            // 地域別統計
            for (const auto& stat : stats.region_stats) {
                pop_reg << stat.population << '\t';
                snp_reg << stat.snp_avg << '\t';
                lang_reg << stat.language_avg << '\t';

                for (const auto& [haplotype, count] : stat.mtdna_counts) {
                    if (count > 0) mtdna_reg << stats.get_mtdna_name(haplotype) << ':' << count << '/';
                }
                mtdna_reg << '\t';

                for (const auto& [language_id, count] : stat.language_counts) {
                    if (count > 0) lang_dna_reg << stats.get_language_name(language_id) << ':' << count << '/';
                }
                lang_dna_reg << '\t';
            }

            // 終端
            pop << sc << '\n';
            mtdna << sc << '\n';
            lang_dna << sc << '\n';
            snp << sc << '\n';
            lang << sc << '\n';
            pop_reg << sc << '\n';
            mtdna_reg << sc << '\n';
            lang_dna_reg << sc << '\n';
            snp_reg << sc << '\n';
            lang_reg << sc << '\n';
        }
    };

} // namespace paxs

#endif // !PAX_SAPIENTICA_SIMULATION_OUTPUT_SIMULATION_RESULT_WRITER_HPP
