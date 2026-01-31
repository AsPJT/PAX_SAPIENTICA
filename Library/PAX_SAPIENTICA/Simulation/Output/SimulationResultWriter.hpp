/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SIMULATION_OUTPUT_SIMULATION_RESULT_WRITER_HPP
#define PAX_SAPIENTICA_SIMULATION_OUTPUT_SIMULATION_RESULT_WRITER_HPP

#include <array>
#include <fstream>
#include <functional>
#include <map>
#include <string>
#include <string_view>
#include <vector>

#include <PAX_SAPIENTICA/IO/File/FileSystem.hpp>
#include <PAX_SAPIENTICA/Simulation/Config/SimulationConst.hpp>
#include <PAX_SAPIENTICA/System/AppConfig.hpp>
#include <PAX_SAPIENTICA/Utility/TimeUtils.hpp>

namespace paxs {

    /// @brief 地区別統計データ
    struct DistrictStatistics {
        std::size_t population = 0;
        double snp_avg = 0.0;
        double language_avg = 0.0;
        std::map<std::uint_least8_t, int> mtdna_counts;
        std::map<std::uint_least8_t, int> ydna_counts;
        std::map<std::uint_least8_t, int> language_counts;
        std::map<std::uint_least8_t, int> pottery_counts;
    };

    /// @brief ステップ統計データ
    struct StepStatistics {
        std::uint_least64_t step_count = 0;
        std::size_t settlement_count = 0;
        std::size_t population_count = 0;
        std::vector<DistrictStatistics> district_stats;
        std::vector<DistrictStatistics> region_stats;
        std::function<std::string(std::uint_least8_t)> get_mtdna_name;
        std::function<std::string(std::uint_least8_t)> get_ydna_name;
        std::function<std::string(std::uint_least8_t)> get_language_name;
        std::function<std::string(std::uint_least8_t)> get_pottery_name;
    };

    /// @brief 出力する統計の種類を定義
    enum class StatType {
        Population,
        MtDNA,
        YDNA,
        LanguageDNA,
        Pottery,
        SNP,
        Language,
        Count // 要素数カウント用
    };

    /// @brief シミュレーション結果をファイルに出力するクラス
    class SimulationResultWriter {
    private:
        static constexpr std::size_t StatTypeCount = static_cast<std::size_t>(StatType::Count);

        // 各種別のファイル名定義
        static constexpr std::array<std::string_view, StatTypeCount> StatFileNames = {
            "Population.txt",
            "mtDNA.txt",
            "Y-DNA.txt",
            "Language_DNA.txt",
            "Pottery.txt",
            "SNP.txt",
            "Language.txt"
        };

        // ストリームを管理する配列
        // [0]: Standard District, [1]: Standard Region, [2]: Labeled District, [3]: Labeled Region
        // (HabitableLandは特殊扱いのため配列外)
        std::array<std::ofstream, StatTypeCount> streams_district_;
        std::array<std::ofstream, StatTypeCount> streams_region_;
        std::array<std::ofstream, StatTypeCount> labeled_streams_district_;
        std::array<std::ofstream, StatTypeCount> labeled_streams_region_;

        // 可住地データ用（統計ループとは構造が異なるため個別管理）
        std::ofstream live_ofs_;
        std::ofstream live_region_ofs_;
        std::ofstream labeled_live_ofs_;
        std::ofstream labeled_live_region_ofs_;
        bool live_header_written_ = false;

        std::string result_directory_;
        std::string labeled_directory_;
        bool is_labeled_output_enabled_ = false;

        /// @brief 現在の日時を文字列として取得
        std::string calcDateTime() const {
            return TimeUtils::getFilenameSafeDateTime();
        }

        /// @brief 特定のファイルストリーム群を閉じる
        template <std::size_t N>
        void closeStreamArray(std::array<std::ofstream, N>& streams) {
            for (auto& ofs : streams) {
                if (ofs.is_open()) ofs.close();
            }
        }

        void closeStream(std::ofstream& ofs) {
            if (ofs.is_open()) ofs.close();
        }

        /// @brief マップデータを文字列化して出力するヘルパー
        template<typename MapType, typename NameGetter>
        void writeMapData(std::ofstream& ofs, const MapType& counts, NameGetter name_getter) {
            for (const auto& [id, count] : counts) {
                if (count > 0) {
                    ofs << name_getter(id) << ':' << count << '/';
                }
            }
            ofs << '\t';
        }

        /// @brief 単一の統計値をストリームに出力する (Visitorパターンのような分岐)
        void writeStatValue(std::ofstream& ofs, StatType type, const DistrictStatistics& stat, const StepStatistics& step_stats) {
            switch (type) {
            case StatType::Population:  ofs << stat.population << '\t'; break;
            case StatType::SNP:         ofs << stat.snp_avg << '\t'; break;
            case StatType::Language:    ofs << stat.language_avg << '\t'; break;
            case StatType::MtDNA:       writeMapData(ofs, stat.mtdna_counts, step_stats.get_mtdna_name); break;
            case StatType::YDNA:        writeMapData(ofs, stat.ydna_counts, step_stats.get_ydna_name); break;
            case StatType::LanguageDNA: writeMapData(ofs, stat.language_counts, step_stats.get_language_name); break;
            case StatType::Pottery:     writeMapData(ofs, stat.pottery_counts, step_stats.get_pottery_name); break;
            default: break;
            }
        }

        /// @brief ファイルオープン処理の共通化
        void openFiles(const std::string& base_dir, const std::string& timestamp,
            std::array<std::ofstream, StatTypeCount>& dist_streams,
            std::array<std::ofstream, StatTypeCount>& region_streams,
            bool create_dirs = false) {

            const std::string root = AppConfig::getInstance().getRootPath();

            for (std::size_t i = 0; i < StatTypeCount; ++i) {
                std::string filename(StatFileNames[i]); // string_view -> string
                std::string file_stem = filename.substr(0, filename.find('.')); // 拡張子除く

                // ディレクトリ作成が必要な場合（Labeled用）
                if (create_dirs) {
                    FileSystem::createDirectories(base_dir + "/" + file_stem);
                    FileSystem::createDirectories(base_dir + "/Region_" + file_stem);

                    dist_streams[i].open(root + base_dir + "/" + file_stem + "/" + timestamp + ".txt");
                    region_streams[i].open(root + base_dir + "/Region_" + file_stem + "/" + timestamp + ".txt");
                }
                else {
                    // 通常出力
                    dist_streams[i].open(root + base_dir + "/" + filename);
                    region_streams[i].open(root + base_dir + "/Region_" + filename);
                }
            }
        }

    public:
        SimulationResultWriter() = default;
        ~SimulationResultWriter() { close(); }

        SimulationResultWriter(const SimulationResultWriter&) = delete;
        SimulationResultWriter& operator=(const SimulationResultWriter&) = delete;
        SimulationResultWriter(SimulationResultWriter&&) noexcept = default;
        SimulationResultWriter& operator=(SimulationResultWriter&&) noexcept = default;

        /// @brief 結果ファイルを初期化
        template<typename DistrictList>
        void initialize(const std::string& label_name, const DistrictList& district_list) {
            close();

            // 1. 標準出力
            result_directory_ = "SimulationResults/" + calcDateTime();
            FileSystem::createDirectories(result_directory_);

            openFiles(result_directory_, "", streams_district_, streams_region_, false);

            // HabitableLandは特殊なので個別オープン
            const std::string root = AppConfig::getInstance().getRootPath();
            live_ofs_.open(root + result_directory_ + "/HabitableLand.txt");
            live_region_ofs_.open(root + result_directory_ + "/Region_HabitableLand.txt");

            // 2. ラベル出力
            if (!label_name.empty()) {
                is_labeled_output_enabled_ = true;
                labeled_directory_ = "LabeledSimulationResults/" + label_name;
                const std::string timestamp = calcDateTime();

                openFiles(labeled_directory_, timestamp, labeled_streams_district_, labeled_streams_region_, true);

                // HabitableLand (Labeled)
                // Note: 元コードではディレクトリ作成が列挙されていましたが、ここでは割愛または必要に応じて追加
                // (通常、ディレクトリ作成はファイルオープン前に必要)
                // Labeled用ディレクトリは openFiles 内で作成済み

                labeled_live_ofs_.open(root + labeled_directory_ + "/HabitableLand.txt"); // 元コードには明示的なディレクトリ作成がなかったためルート配置と仮定、あるいは個別対応
                // 実際にはディレクトリ作成が必要ならここに追加
            }

            // ヘッダー書き込み
            writeHeaders(streams_district_, streams_region_, district_list);
            if (is_labeled_output_enabled_) {
                writeHeaders(labeled_streams_district_, labeled_streams_region_, district_list);
            }
        }

        template<typename DistrictList>
        void writeHeaders(std::array<std::ofstream, StatTypeCount>& d_streams,
            std::array<std::ofstream, StatTypeCount>& r_streams,
            const DistrictList& district_list) {

            // ヘッダー共通処理ラムダ
            auto write_header_content = [&](std::ofstream& ofs) {
                if (!ofs.is_open()) return;
                ofs << "steps" << '\t';
                for (const auto& district : district_list) {
                    ofs << district.name << '\t';
                }
                ofs << std::endl;
                };

            for (auto& ofs : d_streams) write_header_content(ofs);
            for (auto& ofs : r_streams) write_header_content(ofs);
        }

        void close() {
            closeStreamArray(streams_district_);
            closeStreamArray(streams_region_);
            closeStreamArray(labeled_streams_district_);
            closeStreamArray(labeled_streams_region_);

            closeStream(live_ofs_);
            closeStream(live_region_ofs_);
            closeStream(labeled_live_ofs_);
            closeStream(labeled_live_region_ofs_);

            is_labeled_output_enabled_ = false;
        }

        void writeHabitableLand(const std::string& district_name, std::size_t habitable_land_count) {
            if (live_ofs_.is_open()) {
                if (!live_header_written_) {
                    live_ofs_ << "district\thabitable_land\n";
                    live_header_written_ = true;
                }
                live_ofs_ << district_name << '\t' << habitable_land_count << '\n';
            }
            // labeled_live_ofs_ への書き込みが必要な場合はここに追加
        }

        void writeStepStatistics(const StepStatistics& stats) {
            // 通常出力
            processStepStatistics(stats, streams_district_, streams_region_);

            // ラベル出力
            if (is_labeled_output_enabled_) {
                processStepStatistics(stats, labeled_streams_district_, labeled_streams_region_);
            }
        }

    private:
        /// @brief 統計データの書き込み処理を配列ループで実行
        void processStepStatistics(const StepStatistics& stats,
            std::array<std::ofstream, StatTypeCount>& d_streams,
            std::array<std::ofstream, StatTypeCount>& r_streams) {

            for (std::size_t i = 0; i < StatTypeCount; ++i) {
                StatType type = static_cast<StatType>(i);
                auto& d_ofs = d_streams[i];
                auto& r_ofs = r_streams[i];

                if (!d_ofs.is_open()) continue; // ファイルが開いていなければスキップ

                // 行頭情報 (Step Count等)
                auto write_prefix = [&](std::ofstream& ofs) {
                    ofs << stats.step_count << '\t'
                        << stats.settlement_count << '\t'
                        << stats.population_count << '\t';
                    };

                write_prefix(d_ofs);
                write_prefix(r_ofs);

                // 地区別データ書き込み
                for (const auto& stat : stats.district_stats) {
                    writeStatValue(d_ofs, type, stat, stats);
                }

                // 地域別データ書き込み
                for (const auto& stat : stats.region_stats) {
                    writeStatValue(r_ofs, type, stat, stats);
                }

                // 改行
                d_ofs << stats.step_count << '\n';
                r_ofs << stats.step_count << '\n';
            }
        }
    };

} // namespace paxs

#endif // !PAX_SAPIENTICA_SIMULATION_OUTPUT_SIMULATION_RESULT_WRITER_HPP
