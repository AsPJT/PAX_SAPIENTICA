/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023 As Project
	[Production]	2023 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_CALENDAR_JULIAN_DAY_NUMBER_HPP
#define PAX_SAPIENTICA_CALENDAR_JULIAN_DAY_NUMBER_HPP

/*##########################################################################################
	Deals with calculations related to the Julian calendar, the Gregorian calendar, and the Julian day.
	ユリウス暦、グレゴリオ暦、ユリウス日に関する計算を扱う。
##########################################################################################*/

#include <cmath>
#include <cstdint>
#include <vector>

#include <PAX_SAPIENTICA/Calendar/Date.hpp>
#include <PAX_SAPIENTICA/Calendar/JapaneseEra.hpp>

namespace paxs::cal {

    template<typename Day>
    class JulianDayNumber {
    private:
        Day day{ 1808286 };
    public:
        JulianDayNumber() = default;
        template<typename T>
        JulianDayNumber(const T& jdn_) { day = static_cast<Day>(jdn_); }

    public:

        constexpr void setGengo(const DateGengo) const {} // 何もしない（ Variant に用いているため定義）
        constexpr void setYear(const DateYear) const {} // 何もしない（ Variant に用いているため定義）
        constexpr void setMonth(const DateMonth) const {} // 何もしない（ Variant に用いているため定義）
        constexpr void setDay(const Day day_) { day = day_; }
        constexpr void setLeapMonth(const bool) const {} // 何もしない（ Variant に用いているため定義）
        constexpr DateGengo getGengo() const { return 0; }
        constexpr DateYear getYear() const { return 0; }
        constexpr DateMonth getMonth() const { return 0; }
        constexpr Day& getDay() { return day; }
        constexpr DateGengo cgetGengo() const { return 0; }
        constexpr DateYear cgetYear() const { return 0; }
        constexpr DateMonth cgetMonth() const { return 0; }
        constexpr Day cgetDay() const { return day; }
        constexpr static bool isLeapMonth() { return false; } // 閏月は必ず無い（ Variant に用いているため定義）
        constexpr static DateOutputType getDateOutputType() { return DateOutputType::name_and_value; } // 暦名＆年月日形式（ Variant に用いているため定義）

    private:
        // グレゴリオ暦の計算用
        constexpr static double getK(const double month) { return std::floor((14 - month) / 12); }
    public:
        // グレゴリオ暦からユリウス日を取得
        constexpr void fromGregorianCalendar(const double year, const double month = 1.0, const double day = 1.0) {
            const double K = getK(month);
            this->day = static_cast<Day>(std::floor((-K + year + 4800) * 1461 / 4)
                + std::floor((K * 12 + month - 2) * 367 / 12)
                - std::floor(std::floor((-K + year + 4900) / 100) * 3 / 4)
                + day - 32075);
        }
        // ユリウス暦からユリウス日を取得
        constexpr void fromJulianCalendar(const double year, const double month = 1.0, const double day = 1.0) {
            const double K = getK(month);
            this->day = static_cast<Day>(std::floor((-K + year + 4800) * 1461 / 4)
                + std::floor((K * 12 + month - 2) * 367 / 12)
                + day - 32113);
        }
        // グレゴリオ暦を取得
        constexpr GregorianDate toGregorianCalendar() const {
            GregorianDate ymd;
            //JDN ⇒ グレゴリオ暦
            double L = static_cast<double>(this->day) + 68569;
            const double N = std::floor(4 * L / 146097);
            L = L - std::floor((146097 * N + 3) / 4);
            const double I = std::floor(4000 * (L + 1) / 1461001);
            L = L - std::floor(1461 * I / 4) + 31;
            const double J = std::floor(80 * L / 2447);
            ymd.setDay(static_cast<DateDay>(L - std::floor(2447 * J / 80)));
            L = std::floor(J / 11);
            ymd.setMonth(static_cast<DateMonth>(J + 2 - 12 * L));
            ymd.setYear(static_cast<DateYear>(100 * (N - 49) + I + L));
            return ymd;
        }
        // ユリウス暦を取得
        constexpr JulianDate toJulianCalendar() const {
            JulianDate ymd;
            double L = static_cast<double>(this->day) + 1402;
            const double N = std::floor((L - 1) / 1461);
            L = L - 1461 * N;
            const double I = std::floor((L - 1) / 365) - std::floor(L / 1461);
            L = L - 365 * I + 30;
            const double J = std::floor(80 * L / 2447);
            ymd.setDay(static_cast<DateDay>(L - std::floor(2447 * J / 80)));
            L = std::floor(J / 11);
            ymd.setMonth(static_cast<DateMonth>(J + 2 - 12 * L));
            ymd.setYear(static_cast<DateYear>(4 * N + I + L - 4716));
            return ymd;
        }
        // 和暦を取得
        constexpr JapanDate toJapaneseCalendar(const std::vector<paxs::JapaneseEra>& japanese_era_list) const {
            JapanDate jp_date{ 0,1,1,1,false };

            // ユリウス日が 1480407 以上（神武 1 年 1 月 1 日以降、グレゴリオ暦 紀元前 660 年 2 月 11 日以降）
            if (day >= 1480407) {
                bool is_break = false;
                // 元号一覧からその日に合った元号を取得
                for (std::size_t i = 0; i < japanese_era_list.size() - 1; ++i) {
                    auto& jeli = japanese_era_list[i];
                    if (day >= jeli.start_jdn
                        && day < japanese_era_list[i + 1].start_jdn) {
                        is_break = true; // 元号一覧からその日に合った元号が見つかったのでループを抜ける
                        // 改元されている場合
                        if (jeli.kaigen_jdn[0] != 0 &&
                            day >= jeli.kaigen_jdn[0]) {
                            jp_date.setGengo(jeli.gengo[1]);
                            jp_date.setYear(jeli.gengo_num[1]);
                        }
                        else {
                            jp_date.setGengo(jeli.gengo[0]);
                            jp_date.setYear(jeli.gengo_num[0]);
                        }
                    }
                    // 元号一覧からその日に合った元号が見つかった場合
                    if (is_break) {
                        int lm = ((jeli.leap_month == 0) ? 999 : jeli.leap_month - 1);

                        int calc_day = int(day) - jeli.start_jdn; // １月１日
                        // 月と日の計算
                        for (int j = 0; j < 12; ++j) {
                            if (calc_day < jeli.number_of_days[j]) {
                                jp_date.setMonth(DateMonth(j + 1));
                                jp_date.setDay(DateDay(calc_day + 1));
                                jp_date.setLeapMonth(false);
                                break;
                            }
                            calc_day -= jeli.number_of_days[j];
                            if (j == lm) {
                                if (calc_day < jeli.number_of_days_of_leap_month) {
                                    jp_date.setMonth(DateMonth(j + 1));
                                    jp_date.setDay(DateDay(calc_day + 1));
                                    jp_date.setLeapMonth(true);
                                    break;
                                }
                                calc_day -= jeli.number_of_days_of_leap_month;
                            }
                        }
                    }
                    if (is_break) break;
                }
            }
            return jp_date;
        }
        // 較正年代を取得
        constexpr CalBP toCalBP() const {
            GregorianDate ymd = toGregorianCalendar();
            int value = int(ymd.cgetYear());
            if (value >= 1950) value = 0;
            else value = 1950 - value;
            return CalBP{ value };
        }
        private:
            // ヒジュラ暦の閏年かどうか
            constexpr bool isIslamicLeapYear(const int year) const { return ((((11 * year) + 14) % 30) < 11); }
            // ヒジュラ暦の月の日数計算
            constexpr int getLastMonthDay(const int year, const int month) const {
                return (((month % 2) == 1) || ((month == 12) && isIslamicLeapYear(year))) ? 30 : 29;
            }
        public:
            // ヒジュラ暦を取得
            constexpr IslamicDate toIslamicCalendar() const {
                // islamic_day(227014) = jdn(1948439)
                const int islamic_day = static_cast<int>(day) - 1721425;
                // ヒジュラ暦以前の日付
                if (islamic_day <= 227014) return IslamicDate(0, 0, 0);
                IslamicDate ymd{};
                // おおよその年から1年ずつ前倒しで検索
                ymd.setYear((islamic_day - 227014) / 355);
                while (islamic_day >= IslamicDate(ymd.getYear() + 1, 1, 1)) ymd.getYear()++;
                // ムハッラム（ Muharram ・１月）から月単位で検索
                ymd.setMonth(1);
                while (islamic_day > IslamicDate(ymd.getYear(), ymd.getMonth(), static_cast<DateDay>(getLastMonthDay(ymd.getYear(), ymd.getMonth())))) ymd.getMonth()++;
                ymd.setDay(static_cast<DateDay>(islamic_day - static_cast<int>(IslamicDate(ymd.getYear(), ymd.getMonth(), static_cast<DateDay>(1))) + 1));
                return ymd;
            }
    };

    using JDN_F64 = JulianDayNumber<double>;
    using JDN_S32 = JulianDayNumber<std::int_least32_t>;
    using JDN_S64 = JulianDayNumber<std::int_least64_t>;

}

#endif // !PAX_SAPIENTICA_CALENDAR_JULIAN_DAY_NUMBER_HPP