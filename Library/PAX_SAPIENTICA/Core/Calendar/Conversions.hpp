/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_CORE_CALENDAR_CONVERSIONS_HPP
#define PAX_SAPIENTICA_CORE_CALENDAR_CONVERSIONS_HPP

#include <PAX_SAPIENTICA/Core/Calendar/Constants.hpp>

namespace paxs {

/// @brief 暦に関する変換関数（平均年を用いた近似変換）
/// @brief Calendar conversion functions using mean year lengths (approximate)
template<typename T>
struct CalendarConversions {

    /// @brief 年をユリウス日に変換（グレゴリオ暦の平均年を使用した近似）
    /// @brief Convert calendar year to Julian Date (approx., using mean Gregorian year length)
    /// @param year 西暦年（例: 1, 2025 など）
    /// @return ユリウス日（0001-01-01 00:00 UT を基準とした近似値）
    /// @note 月や日、閏年パターンは考慮しない単純な線形近似です
    constexpr static T yearToJulianDayGregorian(const T year) noexcept {
        return CalendarConstants<T>::jdOfGregorianYear1Start()
             + (year - static_cast<T>(1)) * CalendarConstants<T>::daysInYearGregorian();
    }

    /// @brief 年をユリウス日に変換（平年 365 日を用いた簡易近似）
    /// @brief Convert calendar year to Julian Date (simple approx., using 365-day common year)
    /// @param year 西暦年
    /// @return ユリウス日（0001-01-01 00:00 UT を基準とした近似値）
    constexpr static T yearToJulianDayCommon(const T year) noexcept {
        return CalendarConstants<T>::jdOfGregorianYear1Start()
             + (year - static_cast<T>(1)) * CalendarConstants<T>::daysInYearCommon();
    }

    /// @brief 年をユリウス日に変換（平均太陽年を使用した近似）
    /// @brief Convert calendar year to Julian Date (approx., using tropical year length)
    /// @param year 西暦年
    /// @return ユリウス日（0001-01-01 00:00 UT を基準とした近似値）
    /// @note 季節と対応させたい天文学的用途向けの近似です
    constexpr static T yearToJulianDayTropical(const T year) noexcept {
        return CalendarConstants<T>::jdOfGregorianYear1Start()
             + (year - static_cast<T>(1)) * CalendarConstants<T>::daysInTropicalYear();
    }

    /// @brief ユリウス日を年に変換（グレゴリオ暦の平均年を使用した近似）
    /// @brief Convert Julian Date to calendar year (approx., using mean Gregorian year length)
    /// @param julianDay ユリウス日
    /// @return 近似的な西暦年
    constexpr static T julianDayToYearGregorian(const T julianDay) noexcept {
        return static_cast<T>(1)
             + (julianDay - CalendarConstants<T>::jdOfGregorianYear1Start())
             / CalendarConstants<T>::daysInYearGregorian();
    }

    /// @brief ユリウス日を年に変換（平年 365 日を用いた簡易近似）
    /// @brief Convert Julian Date to calendar year (simple approx., using 365-day common year)
    /// @param julianDay ユリウス日
    /// @return 近似的な西暦年
    constexpr static T julianDayToYearCommon(const T julianDay) noexcept {
        return static_cast<T>(1)
             + (julianDay - CalendarConstants<T>::jdOfGregorianYear1Start())
             / CalendarConstants<T>::daysInYearCommon();
    }

    /// @brief ユリウス日を年に変換（平均太陽年を使用した近似）
    /// @brief Convert Julian Date to calendar year (approx., using tropical year length)
    /// @param julianDay ユリウス日
    /// @return 近似的な西暦年
    constexpr static T julianDayToYearTropical(const T julianDay) noexcept {
        return static_cast<T>(1)
             + (julianDay - CalendarConstants<T>::jdOfGregorianYear1Start())
             / CalendarConstants<T>::daysInTropicalYear();
    }
};

}

#endif // !PAX_SAPIENTICA_CORE_CALENDAR_CONVERSIONS_HPP

