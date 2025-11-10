/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SIMULATION_RANGE_HPP
#define PAX_SAPIENTICA_SIMULATION_RANGE_HPP

/*##########################################################################################
    * std::uint_least32_t Key
    * std::string StartAndEnd
    の２つの TSV 読み込み行うクラス
##########################################################################################*/

#include <cmath>
#include <string>

#include <PAX_SAPIENTICA/InputFile.hpp>
#include <PAX_SAPIENTICA/Logger.hpp>
#include <PAX_SAPIENTICA/Type/UnorderedMap.hpp>
#include <PAX_SAPIENTICA/Type/Vector2.hpp>

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
            return path_list[key_].start_position;
        }
        paxs::Vector2<int>& getEnd(const std::uint_least32_t key_) {
            return path_list[key_].end_position;
        }
        int getZ(const std::uint_least32_t key_) {
            return path_list[key_].z;
        }


    private:
        // 項目の ID を返す
        inline std::size_t inputPathGetMenuIndex(const paxs::UnorderedMap<std::uint_least32_t, std::size_t>& menu, const std::uint_least32_t& str_) {
            return  (menu.find(str_) != menu.end()) ? menu.at(str_) : SIZE_MAX;
        }
    public:

        // ルートパスを読み込む true 成功
        bool input(const std::string& str_) {

            paxs::InputFile pifs(str_);
            if (pifs.fail()) {
                PAXS_ERROR("Failed to read SimulationRange.");
                return false;
            }
            // 1 行目を読み込む
            if (!(pifs.getLine())) {
                PAXS_ERROR("The first line of the SimulationRange file is missing.");
                return false;
            }
            // BOM を削除
            pifs.deleteBOM();
            // 1 行目を分割する
            paxs::UnorderedMap<std::uint_least32_t, std::size_t> menu = pifs.splitHashMapMurMur3('\t');

            const std::size_t start_x = inputPathGetMenuIndex(menu, MurMur3::calcHash("start_x"));
            const std::size_t start_y = inputPathGetMenuIndex(menu, MurMur3::calcHash("start_y"));
            const std::size_t end_x = inputPathGetMenuIndex(menu, MurMur3::calcHash("end_x"));
            const std::size_t end_y = inputPathGetMenuIndex(menu, MurMur3::calcHash("end_y"));
            const std::size_t z_index = inputPathGetMenuIndex(menu, MurMur3::calcHash("z"));
            if (start_x == SIZE_MAX || start_y == SIZE_MAX || end_x == SIZE_MAX || end_y == SIZE_MAX) {
                PAXS_ERROR("SimulationRange is missing a StartAndEnd on the first line.");
                return false; // StartAndEnd がないのはデータにならない
            }
            const std::size_t file_type = inputPathGetMenuIndex(menu, MurMur3::calcHash("key"));
            if (file_type == SIZE_MAX) {
                PAXS_ERROR("SimulationRange is missing a Key on the first line.");
                return false; // Key がないのはデータにならない
            }

            // 1 行ずつ読み込み（区切りはタブ）
            while (pifs.getLine()) {
                std::vector<std::string> strvec = pifs.split('\t');

                // 列数が項目より小さい場合は読み込まない
                if (file_type >= strvec.size()) continue;
                if (start_x >= strvec.size()) continue;
                if (start_y >= strvec.size()) continue;
                if (end_x >= strvec.size()) continue;
                if (end_y >= strvec.size()) continue;
                if (z_index >= strvec.size()) continue;

                // テクスチャ名が空の場合は読み込まない
                if (strvec[file_type].size() == 0) continue;

                // テクスチャを追加
                path_list.emplace(MurMur3::calcHash(strvec[file_type].size(), strvec[file_type].c_str()),
                    StartAndEnd{
                        paxs::Vector2<int>(
                            std::stoi(strvec[start_x]),
                            std::stoi(strvec[start_y])
                        ),
                        paxs::Vector2<int>(
                            std::stoi(strvec[end_x]),
                            std::stoi(strvec[end_y])
                        ),
                    std::stoi(strvec[z_index])
                    });
            }
            return true;
        }

    };

}

#endif // !PAX_SAPIENTICA_SIMULATION_RANGE_HPP
