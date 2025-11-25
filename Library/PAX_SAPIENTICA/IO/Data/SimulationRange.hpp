/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_IO_DATA_SIMULATION_RANGE_HPP
#define PAX_SAPIENTICA_IO_DATA_SIMULATION_RANGE_HPP

/*##########################################################################################
    * std::uint_least32_t Key
    * std::string StartAndEnd
    の２つの TSV 読み込み行うクラス
##########################################################################################*/

#include <string>

#include <PAX_SAPIENTICA/Core/Type/UnorderedMap.hpp>
#include <PAX_SAPIENTICA/Core/Type/Vector2.hpp>
#include <PAX_SAPIENTICA/IO/Data/TsvTable.hpp>
#include <PAX_SAPIENTICA/Utility/Logger.hpp>
#include <PAX_SAPIENTICA/Utility/MurMur3.hpp>
#include <PAX_SAPIENTICA/Utility/StringUtils.hpp>

namespace paxs {

    class SimulationRange {
    private:

        struct StartAndEnd {
            paxs::Vector2<int> start_position{}; // = paxs::Vector2<int>{ 861, 381 };
            paxs::Vector2<int> end_position{}; // = paxs::Vector2<int>{ 950, 450 };
            int z = 10;
        };

        paxs::UnorderedMap<std::uint_least32_t, StartAndEnd> path_list;
    public:

        StartAndEnd operator[](const std::uint_least32_t key_) {
            return (path_list.find(key_) == path_list.end()) ? StartAndEnd{} : path_list.at(key_);
        }

        paxs::Vector2<int>& getStart(const std::uint_least32_t key_) {
            return path_list.at(key_).start_position;
        }
        paxs::Vector2<int>& getEnd(const std::uint_least32_t key_) {
            return path_list.at(key_).end_position;
        }
        int getZ(const std::uint_least32_t key_) {
            return path_list.at(key_).z;
        }

        // ルートパスを読み込む true 成功
        bool input(const std::string& str_) {

            paxs::TsvTable table(str_);
            if (!table.isSuccessfullyLoaded()) {
                PAXS_ERROR("Failed to read SimulationRange.");
                return false;
            }

            // 必須カラムの存在確認
            if (!table.hasColumn("start_x") || !table.hasColumn("start_y") ||
                !table.hasColumn("end_x") || !table.hasColumn("end_y")) {
                PAXS_ERROR("SimulationRange is missing required columns (start_x, start_y, end_x, end_y).");
                return false;
            }
            if (!table.hasColumn("key")) {
                PAXS_ERROR("SimulationRange is missing a Key column.");
                return false;
            }

            // カラムハッシュキーを取得
            const std::uint_least32_t key_hash = MurMur3::calcHash("key");
            const std::uint_least32_t start_x_hash = MurMur3::calcHash("start_x");
            const std::uint_least32_t start_y_hash = MurMur3::calcHash("start_y");
            const std::uint_least32_t end_x_hash = MurMur3::calcHash("end_x");
            const std::uint_least32_t end_y_hash = MurMur3::calcHash("end_y");
            const std::uint_least32_t z_hash = MurMur3::calcHash("z");
            const bool has_z = table.hasColumn(z_hash);

            // 各行を処理
            table.forEachRow([&](std::size_t row_index, const std::vector<std::string>&) {
                const std::string& key_str = table.get(row_index, key_hash);
                if (key_str.empty()) return; // キーが空の場合はスキップ

                const std::string& start_x_str = table.get(row_index, start_x_hash);
                const std::string& start_y_str = table.get(row_index, start_y_hash);
                const std::string& end_x_str = table.get(row_index, end_x_hash);
                const std::string& end_y_str = table.get(row_index, end_y_hash);

                if (start_x_str.empty() || start_y_str.empty() ||
                    end_x_str.empty() || end_y_str.empty()) {
                    return; // 必須データが空の場合はスキップ
                }

                int z_value = 10; // デフォルト値
                if (has_z) {
                    const std::string& z_str = table.get(row_index, z_hash);
                    if (!z_str.empty()) {
                        z_value = StringUtils::safeStoi(z_str, 10, true);
                    }
                }

                path_list.emplace(
                    MurMur3::calcHash(key_str.size(), key_str.c_str()),
                    StartAndEnd{
                        paxs::Vector2<int>(StringUtils::safeStoi(start_x_str, 0, true), StringUtils::safeStoi(start_y_str, 0, true)),
                        paxs::Vector2<int>(StringUtils::safeStoi(end_x_str, 0, true), StringUtils::safeStoi(end_y_str, 0, true)),
                        z_value
                    }
                );
            });

            return true;
        }

    };

}

#endif // !PAX_SAPIENTICA_IO_DATA_SIMULATION_RANGE_HPP
