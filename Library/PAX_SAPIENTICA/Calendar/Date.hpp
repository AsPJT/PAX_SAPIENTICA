/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_CALENDAR_DATE_HPP
#define PAX_SAPIENTICA_CALENDAR_DATE_HPP

#include <concepts>
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

    /// @brief 元号を表す型
    struct GengoTag {
        DateGengo value;
        constexpr explicit GengoTag(DateGengo val) : value(val) {}
        constexpr operator DateGengo() const { return value; }
    };

    /// @brief 年を表す型
    struct YearTag {
        DateYear value;
        constexpr explicit YearTag(DateYear val) : value(val) {}
        constexpr operator DateYear() const { return value; }
    };

    /// @brief 月を表す型
    struct MonthTag {
        DateMonth value;
        constexpr explicit MonthTag(DateMonth val) : value(val) {}
        constexpr operator DateMonth() const { return value; }
    };

    /// @brief 日を表す型
    struct DayTag {
        DateDay value;
        constexpr explicit DayTag(DateDay val) : value(val) {}
        constexpr operator DateDay() const { return value; }
    };

    /// @brief 閏月フラグを表す型
    struct LeapMonthTag {
        bool value;
        constexpr explicit LeapMonthTag(bool val) : value(val) {}
        constexpr operator bool() const { return value; }
    };

    /// @brief 日付型が持つべき基本的なインターフェース
    template<typename T>
    concept DateType = requires(const T date) {
        { date.getDay() } -> std::convertible_to<std::int_least64_t>;
        { date.isLeapMonth() } -> std::convertible_to<bool>;
        { date.getDateOutputType() } -> std::same_as<DateOutputType>;
    };

    /// @brief 年月日を持つ日付型のインターフェース
    template<typename T>
    concept YearMonthDayDateType = DateType<T> && requires(const T date) {
        { date.getYear() } -> std::convertible_to<DateYear>;
        { date.getMonth() } -> std::convertible_to<DateMonth>;
        { date.getDay() } -> std::convertible_to<DateDay>;
    };

    /// @brief 元号を持つ日付型のインターフェース
    template<typename T>
    concept GengoDateType = YearMonthDayDateType<T> && requires(const T date) {
        { date.getGengo() } -> std::convertible_to<DateGengo>;
    };

    /// @brief 閏月を持つ日付型のインターフェース
    template<typename T>
    concept LeapMonthDateType = YearMonthDayDateType<T> && requires(const T date) {
        { date.isLeapMonth() } -> std::same_as<bool>;
    };

    /// @brief シミュレーションのステップ数
    class SimulationSteps {
    private:
        std::uint_least32_t day{ 0 };

    public:
        constexpr SimulationSteps() = default;
        constexpr SimulationSteps(const std::uint_least32_t day_) : day(day_) {}

        constexpr void setDay(const std::uint_least32_t day_) { day = day_; }
        constexpr void increment() { ++day; }

        constexpr std::uint_least32_t getDay() const { return day; }
        constexpr static bool isLeapMonth() { return false; }
        constexpr static DateOutputType getDateOutputType() {
            return DateOutputType::name_and_value;
        }
    };
    static_assert(DateType<SimulationSteps>, "SimulationSteps must satisfy DateType concept");

    /// @brief 較正年代
    class CalBP {
    private:
        std::int_least32_t day{};

    public:
        constexpr CalBP() = default;
        constexpr CalBP(const std::int_least32_t day_) : day(day_) {}

        constexpr void setDay(const std::int_least32_t day_) { day = day_; }

        constexpr std::int_least32_t getDay() const { return day; }
        constexpr static bool isLeapMonth() { return false; }
        constexpr static DateOutputType getDateOutputType() {
            return DateOutputType::name_and_value;
        }
    };
    static_assert(DateType<CalBP>, "CalBP must satisfy DateType concept");

    /// @brief ヒジュラ暦の日付
    class IslamicDate {
    private:
        DateYear year{};
        DateMonth month{};
        DateDay day{};

    public:
        constexpr IslamicDate() = default;
        constexpr IslamicDate(const DateYear year_, const DateMonth month_, const DateDay day_)
            : year(year_), month(month_), day(day_) {}

        /// @brief ヒジュラ暦の日付から絶対年代（日付）を計算
        constexpr operator int() const {
            return (day + 29 * (month - 1) + month / 2
                + 354 * (year - 1) // 前年の閏日以外の日
                + (3 + (11 * year)) / 30 // 前年の閏日
                + 227014); // カレンダーの開始日の前日
        }

        constexpr void setYear(const DateYear year_) { year = year_; }
        constexpr void setMonth(const DateMonth month_) { month = month_; }
        constexpr void setDay(const DateDay day_) { day = day_; }
        constexpr void incrementYear() { ++year; }
        constexpr void incrementMonth() { ++month; }

        constexpr DateYear getYear() const { return year; }
        constexpr DateMonth getMonth() const { return month; }
        constexpr DateDay getDay() const { return day; }

        constexpr static bool isLeapMonth() { return false; }
        constexpr static DateOutputType getDateOutputType() {
            return DateOutputType::name_and_ymd;
        }
    };
    static_assert(YearMonthDayDateType<IslamicDate>, "IslamicDate must satisfy YearMonthDayDateType concept");

    /// @brief グレゴリオ暦の日付
    class GregorianDate {
    private:
        DateYear year{};
        DateMonth month{};
        DateDay day{};

    public:
        constexpr GregorianDate() = default;
        constexpr GregorianDate(const DateYear year_, const DateMonth month_, const DateDay day_)
            : year(year_), month(month_), day(day_) {}

        constexpr void setYear(const DateYear year_) { year = year_; }
        constexpr void setMonth(const DateMonth month_) { month = month_; }
        constexpr void setDay(const DateDay day_) { day = day_; }

        constexpr DateYear getYear() const { return year; }
        constexpr DateMonth getMonth() const { return month; }
        constexpr DateDay getDay() const { return day; }

        constexpr static bool isLeapMonth() { return false; }
        constexpr static DateOutputType getDateOutputType() {
            return DateOutputType::name_and_ymd;
        }
    };
    static_assert(YearMonthDayDateType<GregorianDate>, "GregorianDate must satisfy YearMonthDayDateType concept");

    /// @brief ユリウス暦の日付
    class JulianDate {
    private:
        DateYear year{};
        DateMonth month{};
        DateDay day{};

    public:
        constexpr JulianDate() = default;
        constexpr JulianDate(const DateYear year_, const DateMonth month_, const DateDay day_)
            : year(year_), month(month_), day(day_) {}

        constexpr void setYear(const DateYear year_) { year = year_; }
        constexpr void setMonth(const DateMonth month_) { month = month_; }
        constexpr void setDay(const DateDay day_) { day = day_; }

        constexpr DateYear getYear() const { return year; }
        constexpr DateMonth getMonth() const { return month; }
        constexpr DateDay getDay() const { return day; }

        constexpr static bool isLeapMonth() { return false; }
        constexpr static DateOutputType getDateOutputType() {
            return DateOutputType::name_and_ymd;
        }
    };
    static_assert(YearMonthDayDateType<JulianDate>, "JulianDate must satisfy YearMonthDayDateType concept");

    /// @brief 日本の年月日（和暦）
    class JapanDate {
    private:
        DateGengo gengo{};
        DateYear year{};
        DateMonth month{};
        DateDay day{};
        bool is_leap_month = false; // 閏月かどうか

    public:
        constexpr JapanDate() = default;

        constexpr JapanDate(GengoTag gengo_tag, YearTag year_tag, MonthTag month_tag,
                           DayTag day_tag, LeapMonthTag leap_month_tag = LeapMonthTag(false))
            : gengo(gengo_tag.value), year(year_tag.value), month(month_tag.value),
              day(day_tag.value), is_leap_month(leap_month_tag.value) {}

        constexpr void setGengo(const DateGengo gengo_) { gengo = gengo_; }
        constexpr void setYear(const DateYear year_) { year = year_; }
        constexpr void setMonth(const DateMonth month_) { month = month_; }
        constexpr void setDay(const DateDay day_) { day = day_; }
        constexpr void setLeapMonth(const bool leap_month_) { is_leap_month = leap_month_; }

        constexpr DateGengo getGengo() const { return gengo; }
        constexpr DateYear getYear() const { return year; }
        constexpr DateMonth getMonth() const { return month; }
        constexpr DateDay getDay() const { return day; }
        constexpr bool isLeapMonth() const { return is_leap_month; }

        constexpr static DateOutputType getDateOutputType() {
            return DateOutputType::name_and_ymd;
        }
    };
    static_assert(GengoDateType<JapanDate>, "JapanDate must satisfy GengoDateType concept");
    static_assert(LeapMonthDateType<JapanDate>, "JapanDate must satisfy LeapMonthDateType concept");

    /// @brief 中国大陸の年月日
    class ChinaDate {
    private:
        DateGengo gengo{};
        DateYear year{};
        DateMonth month{};
        DateDay day{};
        bool is_leap_month = false; // 閏月かどうか

    public:
        constexpr ChinaDate() = default;

        constexpr ChinaDate(GengoTag gengo_tag, YearTag year_tag, MonthTag month_tag,
                           DayTag day_tag, LeapMonthTag leap_month_tag = LeapMonthTag(false))
            : gengo(gengo_tag.value), year(year_tag.value), month(month_tag.value),
              day(day_tag.value), is_leap_month(leap_month_tag.value) {}

        constexpr void setGengo(const DateGengo gengo_) { gengo = gengo_; }
        constexpr void setYear(const DateYear year_) { year = year_; }
        constexpr void setMonth(const DateMonth month_) { month = month_; }
        constexpr void setDay(const DateDay day_) { day = day_; }
        constexpr void setLeapMonth(const bool leap_month_) { is_leap_month = leap_month_; }

        constexpr DateGengo getGengo() const { return gengo; }
        constexpr DateYear getYear() const { return year; }
        constexpr DateMonth getMonth() const { return month; }
        constexpr DateDay getDay() const { return day; }
        constexpr bool isLeapMonth() const { return is_leap_month; }

        constexpr static DateOutputType getDateOutputType() {
            return DateOutputType::name_and_ymd;
        }
    };
    static_assert(GengoDateType<ChinaDate>, "ChinaDate must satisfy GengoDateType concept");
    static_assert(LeapMonthDateType<ChinaDate>, "ChinaDate must satisfy LeapMonthDateType concept");

}

#endif // !PAX_SAPIENTICA_CALENDAR_DATE_HPP
