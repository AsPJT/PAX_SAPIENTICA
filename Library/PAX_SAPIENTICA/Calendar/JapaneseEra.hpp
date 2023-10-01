/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023 As Project
	[Production]	2023 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_CALENDAR_JAPANESE_ERA_HPP
#define PAX_SAPIENTICA_CALENDAR_JAPANESE_ERA_HPP

/*##########################################################################################

##########################################################################################*/

#include <array>
#include <cmath>
#include <fstream>
#include <string>
#include <vector> // テンプレートにした後は不要

#include <PAX_SAPIENTICA/StringExtensions.hpp>

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
        JapaneseEra() = default;
        JapaneseEra(const std::array<int, 4>& gengo_,
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
        static int emptyCharSupportStoi(const std::string& str_) {
            // 例外発生時のデバッグ用
            //for (auto s : str_) {
            //	if (s < '0' || s > '9') {
            //		auto a = str_.size();
            //	}
            //}
            return (str_.size() == 0) ? 0 : std::stoi(str_);
        }

    public:
        /// @brief 日本の元号一覧を入力する
        /// @param japanese_era_list 元号リスト
        /// @param path 元号一覧のファイルパス
        static void inputList(std::vector<paxs::JapaneseEra>& japanese_era_list, const std::string& path){
#ifdef PAXS_USING_DXLIB // PAXS_USING_DXLIB
            const int file_handle = DxLib::FileRead_open(path.c_str());
            DxLib::FileRead_set_format(file_handle, DX_CHARCODEFORMAT_UTF8);
            if (file_handle == 0) return;
            std::string pline{};
            pline.resize(4096);
            std::string pline_tmp{};
            pline_tmp.resize(4096);
#else
            std::ifstream pifs(path); // 地名を読み込む
            if (pifs.fail()) return;
            std::string pline;
#endif
#ifdef PAXS_USING_DXLIB // PAXS_USING_DXLIB
            const int dline0 = DxLib::FileRead_gets(&(pline[0]), 4096, file_handle);
            if (dline0 == -1 || dline0 == 0) {} // 最初は破棄
            else while (true) {
                const int dline = DxLib::FileRead_gets(&(pline[0]), 4096, file_handle);
                if (dline == -1) break;
                if (dline == 0) break;
#else
            std::getline(pifs, pline); // 最初は破棄
            // 1 行ずつ読み込み（区切りはタブ）
            while (std::getline(pifs, pline)) {
#endif
                std::vector<std::string> strvec = paxs::StringExtensions::split(pline, '\t');

                japanese_era_list.emplace_back(
                    std::array<int, 4>({ 
                        emptyCharSupportStoi(strvec[14]),
                        emptyCharSupportStoi(strvec[16]),
                        emptyCharSupportStoi(strvec[18]),
                        emptyCharSupportStoi(strvec[20]) }),
                    std::array<int, 4>({
                        ((strvec[15].size() == 0) ? 0 : emptyCharSupportStoi(strvec[15])),
                        ((strvec[17].size() == 0) ? 0 : emptyCharSupportStoi(strvec[17])),
                        ((strvec[19].size() == 0) ? 0 : emptyCharSupportStoi(strvec[19])),
                        ((strvec[21].size() == 0) ? 0 : emptyCharSupportStoi(strvec[21])) }),
                        ((strvec[22].size() == 0) ? 0 : emptyCharSupportStoi(strvec[22])),
                        std::array<int, 2>({ ((strvec[23].size() == 0) ? 0 : emptyCharSupportStoi(strvec[23])),
                        ((strvec[24].size() == 0) ? 0 : emptyCharSupportStoi(strvec[24])) }),
                        std::array<int, 12>(
                            { emptyCharSupportStoi(strvec[0]), emptyCharSupportStoi(strvec[1]), emptyCharSupportStoi(strvec[2]),
                            emptyCharSupportStoi(strvec[3]), emptyCharSupportStoi(strvec[4]), emptyCharSupportStoi(strvec[5]),
                            emptyCharSupportStoi(strvec[6]), emptyCharSupportStoi(strvec[7]), emptyCharSupportStoi(strvec[8]),
                            emptyCharSupportStoi(strvec[9]), emptyCharSupportStoi(strvec[10]), emptyCharSupportStoi(strvec[11]) }),
                    ((strvec[12].size() == 0) ? 0 : emptyCharSupportStoi(strvec[12])),
                    ((strvec[13].size() == 0) ? 0 : emptyCharSupportStoi(strvec[13])));
            }
        }
    };
}

#endif // !PAX_SAPIENTICA_CALENDAR_JAPANESE_ERA_HPP
