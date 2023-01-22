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

}

#endif // !PAX_SAPIENTICA_CALENDAR_JAPANESE_ERA_HPP