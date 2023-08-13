/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023 As Project
	[Production]	2023 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_CALENDAR_CALENDARS_HPP
#define PAX_SAPIENTICA_CALENDAR_CALENDARS_HPP

/*##########################################################################################
	Define a structure that handles the year, month, and day.
	年月日を扱う構造体を定義する。
##########################################################################################*/

// TODO:改修必須

#include <cstddef>
#include <variant>

#include <PAX_SAPIENTICA/Calendar/Date.hpp>
#include <PAX_SAPIENTICA/Calendar/JulianDayNumber.hpp>

namespace paxs::cal {

	enum CalendarsType : std::size_t {
		jdn_s32_type		// ユリウス通日 (S32)
		, jdn_s64_type		// ユリウス通日 (S64)
		, jdn_f64_type		// ユリウス通日 (F64)
		, julian_date_type	// ユリウス暦
		, gregorian_date_type// グレゴリオ暦
		, islamic_date_type	// ヒジュラ暦
		, japan_date_type	// 和暦
		, calbp_type		// 較正年代
		, simulation_steps_type// シミュレーションのステップ数
	};

	// 暦
	using Calendars = std::variant<
		JDN_S32		// ユリウス通日 (S32)
		, JDN_S64		// ユリウス通日 (S64)
		, JDN_F64		// ユリウス通日 (F64)
		, JulianDate	// ユリウス暦
		, GregorianDate	// グレゴリオ暦
		, IslamicDate	// ヒジュラ暦
		, JapanDate		// 和暦
		, CalBP			// 較正年代
		, SimulationSteps// シミュレーションのステップ数
	>;


}

#endif // !PAX_SAPIENTICA_CALENDAR_CALENDARS_HPP