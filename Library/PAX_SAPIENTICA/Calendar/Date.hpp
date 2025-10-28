/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_CALENDAR_DATE_HPP
#define PAX_SAPIENTICA_CALENDAR_DATE_HPP

// TODO:改修必須

#include <cstdint>

namespace paxs::cal {

    // 暦の出力形式
    enum class DateOutputType {
        name_and_ymd // 暦名 & 年月日（例：グレゴリオ暦１年１月１日）
        , name_and_value // 暦名 & 値
    };

    using DateGengo = std::int_least32_t;
    using DateYear = std::int_least32_t;
    using DateMonth = std::uint_least8_t;
    using DateDay = std::uint_least8_t;

    // シミュレーションのステップ数
    class SimulationSteps {
    private:
        std::uint_least32_t day{ 0 };
    public:
        SimulationSteps() = default;
        SimulationSteps(const std::uint_least32_t day_)
            :day(day_) {}
        constexpr void setGengo(const DateGengo) const {} // 何もしない（ Variant に用いているため定義）
        constexpr void setYear(const DateYear) const {} // 何もしない（ Variant に用いているため定義）
        constexpr void setMonth(const DateMonth) const {} // 何もしない（ Variant に用いているため定義）
        constexpr void setDay(const std::int_least32_t day_) { day = day_; }
        constexpr void setLeapMonth(const bool) const {} // 何もしない（ Variant に用いているため定義）
        constexpr DateGengo getGengo() const { return 0; }
        constexpr DateYear getYear() const { return 0; }
        constexpr DateMonth getMonth() const { return 0; }
        constexpr std::uint_least32_t& getDay() { return day; }
        constexpr DateGengo cgetGengo() const { return 0; }
        constexpr DateYear cgetYear() const { return 0; }
        constexpr DateMonth cgetMonth() const { return 0; }
        constexpr std::uint_least32_t cgetDay() const { return day; }
        constexpr static bool isLeapMonth() { return false; } // 閏月は必ず無い（ Variant に用いているため定義）
        constexpr static DateOutputType getDateOutputType() { return DateOutputType::name_and_value; } // 暦名＆年月日形式（ Variant に用いているため定義）
    };

    // 較正年代
    class CalBP {
    private:
        std::int_least32_t day{};
    public:
        CalBP() = default;
        CalBP(const std::int_least32_t day_)
            :day(day_) {}
        constexpr void setGengo(const DateGengo) const {} // 何もしない（ Variant に用いているため定義）
        constexpr void setYear(const DateYear) const {} // 何もしない（ Variant に用いているため定義）
        constexpr void setMonth(const DateMonth) const {} // 何もしない（ Variant に用いているため定義）
        constexpr void setDay(const std::int_least32_t day_) { day = day_; }
        constexpr void setLeapMonth(const bool) const {} // 何もしない（ Variant に用いているため定義）
        constexpr DateGengo getGengo() const { return 0; }
        constexpr DateYear getYear() const { return 0; }
        constexpr DateMonth getMonth() const { return 0; }
        constexpr std::int_least32_t& getDay() { return day; }
        constexpr DateGengo cgetGengo() const { return 0; }
        constexpr DateYear cgetYear() const { return 0; }
        constexpr DateMonth cgetMonth() const { return 0; }
        constexpr std::int_least32_t cgetDay() const { return day; }
        constexpr static bool isLeapMonth() { return false; } // 閏月は必ず無い（ Variant に用いているため定義）
        constexpr static DateOutputType getDateOutputType() { return DateOutputType::name_and_value; } // 暦名＆年月日形式（ Variant に用いているため定義）
    };

    // 年月日
    class IslamicDate {
    private:
        DateYear year{};
        DateMonth month{};
        DateDay day{};
    public:
        // ヒジュラ暦の日付から絶対年代（日付）を計算
        constexpr operator int() const {
            return (day + 29 * (month - 1) + month / 2
                + 354 * (year - 1) // 前年の閏日以外の日
                + (3 + (11 * year)) / 30 // 前年の閏日
                + 227014); // カレンダーの開始日の前日
        }

        IslamicDate() = default;
        IslamicDate(const DateYear year_, const DateMonth month_, const DateDay day_)
            :year(year_), month(month_), day(day_) {}

        constexpr void setGengo(const DateGengo) const {} // 何もしない（ Variant に用いているため定義）
        constexpr void setYear(const DateYear year_) { year = year_; }
        constexpr void setMonth(const DateMonth month_) { month = month_; }
        constexpr void setDay(const DateDay day_) { day = day_; }
        constexpr void setLeapMonth(const bool) const {} // 何もしない（ Variant に用いているため定義）
        constexpr DateGengo getGengo() const { return 0; }
        constexpr DateYear& getYear() { return year; }
        constexpr DateMonth& getMonth() { return month; }
        constexpr DateDay& getDay() { return day; }
        constexpr DateGengo cgetGengo() const { return 0; }
        constexpr DateYear cgetYear() const { return year; }
        constexpr DateMonth cgetMonth() const { return month; }
        constexpr DateDay cgetDay() const { return day; }
        constexpr static bool isLeapMonth() { return false; } // 閏月は必ず無い（ Variant に用いているため定義）
        constexpr static DateOutputType getDateOutputType() { return DateOutputType::name_and_ymd; } // 暦名＆年月日形式（ Variant に用いているため定義）
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

        constexpr void setGengo(const DateGengo) const {} // 何もしない（ Variant に用いているため定義）
        constexpr void setYear(const DateYear year_) { year = year_; }
        constexpr void setMonth(const DateMonth month_) { month = month_; }
        constexpr void setDay(const DateDay day_) { day = day_; }
        constexpr void setLeapMonth(const bool) const {} // 何もしない（ Variant に用いているため定義）
        constexpr DateGengo getGengo() const { return 0; }
        constexpr DateYear& getYear() { return year; }
        constexpr DateMonth& getMonth() { return month; }
        constexpr DateDay& getDay() { return day; }
        constexpr DateGengo cgetGengo() const { return 0; }
        constexpr DateYear cgetYear() const { return year; }
        constexpr DateMonth cgetMonth() const { return month; }
        constexpr DateDay cgetDay() const { return day; }
        constexpr static bool isLeapMonth() { return false; } // 閏月は必ず無い（ Variant に用いているため定義）
        constexpr static DateOutputType getDateOutputType() { return DateOutputType::name_and_ymd; } // 暦名＆年月日形式（ Variant に用いているため定義）
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

        constexpr void setGengo(const DateGengo) const {} // 何もしない（ Variant に用いているため定義）
        constexpr void setYear(const DateYear year_) { year = year_; }
        constexpr void setMonth(const DateMonth month_) { month = month_; }
        constexpr void setDay(const DateDay day_) { day = day_; }
        constexpr void setLeapMonth(const bool) const {} // 何もしない（ Variant に用いているため定義）
        constexpr DateGengo getGengo() const { return 0; }
        constexpr DateYear& getYear() { return year; }
        constexpr DateMonth& getMonth() { return month; }
        constexpr DateDay& getDay() { return day; }
        constexpr DateGengo cgetGengo() const { return 0; }
        constexpr DateYear cgetYear() const { return year; }
        constexpr DateMonth cgetMonth() const { return month; }
        constexpr DateDay cgetDay() const { return day; }
        constexpr static bool isLeapMonth() { return false; } // 閏月は必ず無い（ Variant に用いているため定義）
        constexpr static DateOutputType getDateOutputType() { return DateOutputType::name_and_ymd; } // 暦名＆年月日形式（ Variant に用いているため定義）
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
            :gengo(gengo_), year(year_), month(month_), day(day_), is_leap_month(is_leap_month_) {}

        constexpr void setGengo(const DateGengo gengo_) { gengo = gengo_; }
        constexpr void setYear(const DateYear year_) { year = year_; }
        constexpr void setMonth(const DateMonth month_) { month = month_; }
        constexpr void setDay(const DateDay day_) { day = day_; }
        constexpr void setLeapMonth(const bool leap_month_) { is_leap_month = leap_month_; }
        constexpr DateGengo& getGengo() { return gengo; }
        constexpr DateYear& getYear() { return year; }
        constexpr DateMonth& getMonth() { return month; }
        constexpr DateDay& getDay() { return day; }
        constexpr DateGengo cgetGengo() const { return gengo; }
        constexpr DateYear cgetYear() const { return year; }
        constexpr DateMonth cgetMonth() const { return month; }
        constexpr DateDay cgetDay() const { return day; }
        constexpr bool isLeapMonth() const { return is_leap_month; }
        constexpr static DateOutputType getDateOutputType() { return DateOutputType::name_and_ymd; } // 暦名＆年月日形式（ Variant に用いているため定義）
    };

    // 中国大陸の年月日
    class ChinaDate {
    private:
        DateGengo gengo{};
        DateYear year{};
        DateMonth month{};
        DateDay day{};
        bool is_leap_month = false; // 閏月かどうか
    public:
        ChinaDate() = default;
        ChinaDate(const DateGengo gengo_, const DateYear year_, const DateMonth month_, const DateDay day_, const bool is_leap_month_ = false)
            :gengo(gengo_), year(year_), month(month_), day(day_), is_leap_month(is_leap_month_) {}

        constexpr void setGengo(const DateGengo gengo_) { gengo = gengo_; }
        constexpr void setYear(const DateYear year_) { year = year_; }
        constexpr void setMonth(const DateMonth month_) { month = month_; }
        constexpr void setDay(const DateDay day_) { day = day_; }
        constexpr void setLeapMonth(const bool leap_month_) { is_leap_month = leap_month_; }
        constexpr DateGengo& getGengo() { return gengo; }
        constexpr DateYear& getYear() { return year; }
        constexpr DateMonth& getMonth() { return month; }
        constexpr DateDay& getDay() { return day; }
        constexpr DateGengo cgetGengo() const { return gengo; }
        constexpr DateYear cgetYear() const { return year; }
        constexpr DateMonth cgetMonth() const { return month; }
        constexpr DateDay cgetDay() const { return day; }
        constexpr bool isLeapMonth() const { return is_leap_month; }
        constexpr static DateOutputType getDateOutputType() { return DateOutputType::name_and_ymd; } // 暦名＆年月日形式（ Variant に用いているため定義）
    };

}

#endif // !PAX_SAPIENTICA_CALENDAR_DATE_HPP
