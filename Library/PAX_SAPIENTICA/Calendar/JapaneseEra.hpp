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

	};

	// 暫定的
	void inputJapaneseEra(std::vector<paxs::JapaneseEra>& japanese_era_list,const std::string& str_) {
		std::ifstream ifs(str_);
		if (ifs.fail()) return;
		std::string line;
		std::getline(ifs, line); // 最初は破棄
		while (std::getline(ifs, line)) {
			std::vector<std::string> strvec = paxs::StringExtensions::split(line, '\t');

			japanese_era_list.emplace_back(
				std::array<std::string, 4>({ strvec[14], strvec[16], strvec[18], strvec[20] }),
				std::array<int, 4>({
					((strvec[15].size() == 0) ? 0 : paxs::getUintFromString(strvec[15])),
					((strvec[17].size() == 0) ? 0 : paxs::getUintFromString(strvec[17])),
					((strvec[19].size() == 0) ? 0 : paxs::getUintFromString(strvec[19])),
					((strvec[21].size() == 0) ? 0 : paxs::getUintFromString(strvec[21])) }),
					((strvec[22].size() == 0) ? 0 : paxs::getUintFromString(strvec[22])),
					std::array<int, 2>({ ((strvec[23].size() == 0) ? 0 : paxs::getUintFromString(strvec[23])),
					((strvec[24].size() == 0) ? 0 : paxs::getUintFromString(strvec[24])) }),
					std::array<int, 12>(
						{ paxs::getUintFromString(strvec[0]), paxs::getUintFromString(strvec[1]), paxs::getUintFromString(strvec[2]),
						paxs::getUintFromString(strvec[3]), paxs::getUintFromString(strvec[4]), paxs::getUintFromString(strvec[5]),
						paxs::getUintFromString(strvec[6]), paxs::getUintFromString(strvec[7]), paxs::getUintFromString(strvec[8]),
						paxs::getUintFromString(strvec[9]), paxs::getUintFromString(strvec[10]), paxs::getUintFromString(strvec[11]) }),
				((strvec[12].size() == 0) ? 0 : paxs::getUintFromString(strvec[12])),
				((strvec[13].size() == 0) ? 0 : paxs::getUintFromString(strvec[13])));

		}
	}


}

#endif // !PAX_SAPIENTICA_CALENDAR_JAPANESE_ERA_HPP