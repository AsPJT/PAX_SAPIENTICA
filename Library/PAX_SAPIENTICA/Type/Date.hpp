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

#include <cstdint>

namespace paxs {

	using DateGengo = std::int_least32_t;
	using DateYear = std::int_least32_t;
	using DateMonth = std::uint_least8_t;
	using DateDay = std::uint_least8_t;

	// 年月日
	class Date {
	private:
		DateYear year{};
		DateMonth month{};
		DateDay day{};
	public:
		Date() = default;
		Date(const DateYear year_, const DateMonth month_, const DateDay day_)
			:year(year_), month(month_), day(day_) {}
		void setGengo(const DateGengo) const {} // 何もしない（ Variant に用いているため定義）
		void setYear(const DateYear year_) { year = year_; }
		void setMonth(const DateMonth month_) { month = month_; }
		void setDay(const DateDay day_) { day = day_; }
		void setLeapMonth(const bool) const {} // 何もしない（ Variant に用いているため定義）
		DateGengo getGengo() const { return 0; }
		DateYear& getYear() { return year; }
		DateMonth& getMonth() { return month; }
		DateDay& getDay() { return day; }
		DateGengo cgetGengo() const { return 0; }
		DateYear cgetYear() const { return year; }
		DateMonth cgetMonth() const { return month; }
		DateDay cgetDay() const { return day; }
		bool isLeapMonth() const { return false; } // 閏月は必ず無い（ Variant に用いているため定義）
	};


	// 日本の年月日
	class JapanDate {
	private:
		DateGengo gengo{};
		DateYear year{};
		DateMonth month{};
		DateDay day{};
		bool is_leap_month = false; // 閏月かどうか
	public:
		JapanDate() = default;
		JapanDate(const DateGengo gengo_, const DateYear year_, const DateMonth month_, const DateDay day_, const bool is_leap_month_ = false)
			:gengo(gengo_), year(year_), month(month_), day(day_), is_leap_month(is_leap_month_){}
		void setGengo(const DateGengo gengo_) { gengo = gengo_; }
		void setYear(const DateYear year_) { year = year_; }
		void setMonth(const DateMonth month_) { month = month_; }
		void setDay(const DateDay day_) { day = day_; }
		void setLeapMonth(const bool leap_month_) { is_leap_month = leap_month_; }
		DateGengo& getGengo() { return gengo; }
		DateYear& getYear() { return year; }
		DateMonth& getMonth() { return month; }
		DateDay& getDay() { return day; }
		DateGengo cgetGengo() const { return gengo; }
		DateYear cgetYear() const { return year; }
		DateMonth cgetMonth() const { return month; }
		DateDay cgetDay() const { return day; }
		bool isLeapMonth() const { return is_leap_month; }
	};

}

#endif // !PAX_SAPIENTICA_TYPE_DATE_HPP