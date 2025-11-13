/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_CALENDAR_JAPANESE_ERA_HPP
#define PAX_SAPIENTICA_CALENDAR_JAPANESE_ERA_HPP

#include <array>
#include <cmath>
#include <string>
#include <vector>

#include <PAX_SAPIENTICA/Core/Utility/StringUtils.hpp>
#include <PAX_SAPIENTICA/IO/Data/TsvTable.hpp>
#include <PAX_SAPIENTICA/Utility/Logger.hpp>
#include <PAX_SAPIENTICA/Utility/MurMur3.hpp>

namespace paxs {

    // 日本の元号
    struct JapaneseEra {
        std::array<int, 4> gengo{};			// 元号
        std::array<int, 4> gengo_num{};				// 年
        int start_jdn{};							// その元号が開始するユリウス日
        std::array<int, 2> kaigen_jdn{};			// 改元するユリウス日
        std::array<int, 12> number_of_days{};		// 毎月の日数
        int number_of_days_of_leap_month{};			// 閏月の日数
        int leap_month{};							// 閏月

        // コンストラクタ
        constexpr JapaneseEra() = default;
        constexpr JapaneseEra(const std::array<int, 4>& gengo_,
            const std::array<int, 4>& gengo_num_,
            const int start_jdn_,
            const std::array<int, 2>& kaigen_jdn_,
            const std::array<int, 12>& number_of_days_,
            const int number_of_days_of_leap_month_,
            const int leap_month_)
            :gengo(gengo_), gengo_num(gengo_num_), start_jdn(start_jdn_),
            kaigen_jdn(kaigen_jdn_), number_of_days(number_of_days_),
            number_of_days_of_leap_month(number_of_days_of_leap_month_),
            leap_month(leap_month_) {}
    private:
        constexpr static int emptyCharSupportStoi(const std::string& str_) {
            return (str_.size() == 0) ? 0 : std::stoi(str_);
        }

    public:
        /// @brief 日本の元号一覧を入力する
        /// @param japanese_era_list 元号リスト
        /// @param path 元号一覧のファイルパス
        static void inputList(std::vector<paxs::JapaneseEra>& japanese_era_list, const std::string& path) {

            paxs::TsvTable table(path);
            if (!table.isSuccessfullyLoaded()) {
                PAXS_ERROR(path + " could not be loaded.");
                return;
            }

            // 必須カラム数の確認（最低25列必要：0-24）
            if (table.columnCount() < 25) {
                PAXS_ERROR(path + " has insufficient columns (expected at least 25 columns).");
                return;
            }

            // 1 行ずつ読み込み
            table.forEachRow([&](std::size_t row_index, const std::vector<std::string>& row) {
                // 列数が不足している行はスキップ
                if (row.size() < 25) {
                    return;
                }

                japanese_era_list.emplace_back(
                    std::array<int, 4>({
                        emptyCharSupportStoi(table.get(row_index, std::size_t(14))),
                        emptyCharSupportStoi(table.get(row_index, std::size_t(16))),
                        emptyCharSupportStoi(table.get(row_index, std::size_t(18))),
                        emptyCharSupportStoi(table.get(row_index, std::size_t(20))) }),
                    std::array<int, 4>({
                        emptyCharSupportStoi(table.get(row_index, std::size_t(15))),
                        emptyCharSupportStoi(table.get(row_index, std::size_t(17))),
                        emptyCharSupportStoi(table.get(row_index, std::size_t(19))),
                        emptyCharSupportStoi(table.get(row_index, std::size_t(21))) }),
                    emptyCharSupportStoi(table.get(row_index, std::size_t(22))),
                    std::array<int, 2>({
                        emptyCharSupportStoi(table.get(row_index, std::size_t(23))),
                        emptyCharSupportStoi(table.get(row_index, std::size_t(24))) }),
                    std::array<int, 12>({
                        emptyCharSupportStoi(table.get(row_index, std::size_t(0))),
                        emptyCharSupportStoi(table.get(row_index, std::size_t(1))),
                        emptyCharSupportStoi(table.get(row_index, std::size_t(2))),
                        emptyCharSupportStoi(table.get(row_index, std::size_t(3))),
                        emptyCharSupportStoi(table.get(row_index, std::size_t(4))),
                        emptyCharSupportStoi(table.get(row_index, std::size_t(5))),
                        emptyCharSupportStoi(table.get(row_index, std::size_t(6))),
                        emptyCharSupportStoi(table.get(row_index, std::size_t(7))),
                        emptyCharSupportStoi(table.get(row_index, std::size_t(8))),
                        emptyCharSupportStoi(table.get(row_index, std::size_t(9))),
                        emptyCharSupportStoi(table.get(row_index, std::size_t(10))),
                        emptyCharSupportStoi(table.get(row_index, std::size_t(11))) }),
                    emptyCharSupportStoi(table.get(row_index, std::size_t(12))),
                    emptyCharSupportStoi(table.get(row_index, std::size_t(13))));
            });
        }
    };
}

#endif // !PAX_SAPIENTICA_CALENDAR_JAPANESE_ERA_HPP
