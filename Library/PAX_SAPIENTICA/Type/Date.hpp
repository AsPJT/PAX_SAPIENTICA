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

	// 暦の出力形式
	enum class DateOutputType {
		name_and_ymd // 暦名 & 年月日（例：グレゴリオ暦１年１月１日）
		,name_and_value // 暦名 & 値
	};

	using DateGengo = std::int_least32_t;
	using DateYear = std::int_least32_t;
	using DateMonth = std::uint_least8_t;
	using DateDay = std::uint_least8_t;
	// 較正年代
	class CalBP {
	private:
		std::int_least32_t day{};
	public:
		CalBP() = default;
		CalBP(const std::int_least32_t day_)
			:day(day_) {}
		void setGengo(const DateGengo) const {} // 何もしない（ Variant に用いているため定義）
		void setYear(const DateYear) {} // 何もしない（ Variant に用いているため定義）
		void setMonth(const DateMonth) {} // 何もしない（ Variant に用いているため定義）
		void setDay(const std::int_least32_t day_) { day = day_; }
		void setLeapMonth(const bool) const {} // 何もしない（ Variant に用いているため定義）
		DateGengo getGengo() { return 0; }
		DateYear getYear() { return 0; }
		DateMonth getMonth() { return 0; }
		std::int_least32_t& getDay() { return day; }
		DateGengo cgetGengo() const { return 0; }
		DateYear cgetYear() const { return 0; }
		DateMonth cgetMonth() const { return 0; }
		std::int_least32_t cgetDay() const { return day; }
		static bool isLeapMonth() { return false; } // 閏月は必ず無い（ Variant に用いているため定義）
		static DateOutputType getDateOutputType() { return DateOutputType::name_and_value; } // 暦名＆年月日形式（ Variant に用いているため定義）
	};

	// 年月日
	class IslamicDate {
	private:
		DateYear year{};
		DateMonth month{};
		DateDay day{};
	public:
		// ヒジュラ暦の日付から絶対年代（日付）を計算
		operator int() {
			return (day + 29 * (month - 1) + month / 2
				+ 354 * (year - 1) // 前年の閏日以外の日
				+ (3 + (11 * year)) / 30 // 前年の閏日
				+ 227014); // カレンダーの開始日の前日
		}

		IslamicDate() = default;
		IslamicDate(const DateYear year_, const DateMonth month_, const DateDay day_)
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
		static bool isLeapMonth() { return false; } // 閏月は必ず無い（ Variant に用いているため定義）
		static DateOutputType getDateOutputType() { return DateOutputType::name_and_ymd; } // 暦名＆年月日形式（ Variant に用いているため定義）
	};

	// 年月日
	class GregorianDate {
	private:
		DateYear year{};
		DateMonth month{};
		DateDay day{};
	public:
		GregorianDate() = default;
		GregorianDate(const DateYear year_, const DateMonth month_, const DateDay day_)
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
		static bool isLeapMonth() { return false; } // 閏月は必ず無い（ Variant に用いているため定義）
		static DateOutputType getDateOutputType() { return DateOutputType::name_and_ymd; } // 暦名＆年月日形式（ Variant に用いているため定義）
	};

	// 年月日
	class JulianDate {
	private:
		DateYear year{};
		DateMonth month{};
		DateDay day{};
	public:
		JulianDate() = default;
		JulianDate(const DateYear year_, const DateMonth month_, const DateDay day_)
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
		static bool isLeapMonth() { return false; } // 閏月は必ず無い（ Variant に用いているため定義）
		static DateOutputType getDateOutputType() { return DateOutputType::name_and_ymd; } // 暦名＆年月日形式（ Variant に用いているため定義）
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
		static DateOutputType getDateOutputType() { return DateOutputType::name_and_ymd; } // 暦名＆年月日形式（ Variant に用いているため定義）
	};

}

#endif // !PAX_SAPIENTICA_TYPE_DATE_HPP