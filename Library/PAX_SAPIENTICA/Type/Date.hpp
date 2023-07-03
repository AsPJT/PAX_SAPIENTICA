/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023 As Project
	[Production]	2023 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_TYPE_DATE_HPP
#define PAX_SAPIENTICA_TYPE_DATE_HPP

/*##########################################################################################
	Define a structure that handles the year, month, and day.
	年月日を扱う構造体を定義する。
##########################################################################################*/

// TODO:改修必須

namespace paxs {

	using DateYear = int;
	using DateMonth = unsigned char;
	using DateDay = unsigned char;

	struct Date {
		DateYear year;
		DateMonth month;
		DateDay day;

		Date() = default;
		Date(const DateYear year_, DateMonth month_, DateDay day_)
			:year(year_), month(month_), day(day_) {}
		void setYear(const DateYear year_) { year = year_; }
		void setMonth(const DateMonth month_) { month = month_; }
		void setDay(const DateDay day_) { day = day_; }
	};
}

#endif // !PAX_SAPIENTICA_TYPE_DATE_HPP