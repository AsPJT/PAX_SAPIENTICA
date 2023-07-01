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
#include <string>
#include <vector> // テンプレートにした後は不要

#include <PAX_SAPIENTICA/StringExtensions.hpp>

namespace paxs {

	// 日本の元号
	struct JapaneseEra {
		std::array<std::string, 4> gengo{};			// 元号
		std::array<int, 4> gengo_num{};				// 年
		int start_jdn{};							// その元号が開始するユリウス日
		std::array<int, 2> kaigen_jdn{};			// 改元するユリウス日
		std::array<int, 12> number_of_days{};		// 毎月の日数
		int number_of_days_of_leap_month{};			// 閏月の日数
		int leap_month{};							// 閏月

		// コンストラクタ
		JapaneseEra() = default;
		JapaneseEra(const std::array<std::string, 4>& gengo_,
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

		/// @brief 日本の元号一覧を入力する
		/// @param japanese_era_list 元号リスト
		/// @param path 元号一覧のファイルパス
		static void inputList(std::vector<paxs::JapaneseEra>& japanese_era_list, const std::string& path){
			std::ifstream ifs(path);
			if (ifs.fail()) return;
			std::string line;
			std::getline(ifs, line); // 最初は破棄
			while (std::getline(ifs, line)) {
				std::vector<std::string> strvec = paxs::StringExtensions::split(line, '\t');

				japanese_era_list.emplace_back(
					std::array<std::string, 4>({ strvec[14], strvec[16], strvec[18], strvec[20] }),
					std::array<int, 4>({
						((strvec[15].size() == 0) ? 0 : std::stoi(strvec[15])),
						((strvec[17].size() == 0) ? 0 : std::stoi(strvec[17])),
						((strvec[19].size() == 0) ? 0 : std::stoi(strvec[19])),
						((strvec[21].size() == 0) ? 0 : std::stoi(strvec[21])) }),
						((strvec[22].size() == 0) ? 0 : std::stoi(strvec[22])),
						std::array<int, 2>({ ((strvec[23].size() == 0) ? 0 : std::stoi(strvec[23])),
						((strvec[24].size() == 0) ? 0 : std::stoi(strvec[24])) }),
						std::array<int, 12>(
							{ std::stoi(strvec[0]), std::stoi(strvec[1]), std::stoi(strvec[2]),
							std::stoi(strvec[3]), std::stoi(strvec[4]), std::stoi(strvec[5]),
							std::stoi(strvec[6]), std::stoi(strvec[7]), std::stoi(strvec[8]),
							std::stoi(strvec[9]), std::stoi(strvec[10]), std::stoi(strvec[11]) }),
					((strvec[12].size() == 0) ? 0 : std::stoi(strvec[12])),
					((strvec[13].size() == 0) ? 0 : std::stoi(strvec[13])));
			}
		}
	};
}

#endif // !PAX_SAPIENTICA_CALENDAR_JAPANESE_ERA_HPP