/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_CORE_CALENDAR_CONSTANTS_HPP
#define PAX_SAPIENTICA_CORE_CALENDAR_CONSTANTS_HPP

namespace paxs {

/// @brief 暦に関する基本定数
/// @brief Calendar constants
template<typename T>
struct CalendarConstants {
    /// @brief グレゴリオ暦における1年の平均日数（閏年を含む）
    /// @brief Average days in a year in the Gregorian calendar (including leap years)
    constexpr static T daysInYearGregorian() noexcept { return static_cast<T>(365.2425); }

    /// @brief 一般的な1年の日数（平年の日数）
    /// @brief Common days in a year (non-leap year)
    constexpr static T daysInYearCommon() noexcept { return static_cast<T>(365); }

    /// @brief 平均太陽年（tropical year）の長さ（日）
    /// @brief Length of the tropical year (mean solar year), in days
    constexpr static T daysInTropicalYear() noexcept { return static_cast<T>(365.24219); }

    /// @brief 紀元1年1月1日 00:00 UT のユリウス日（プロレプティック・グレゴリオ暦）
    /// @brief Julian Date for 0001-01-01 00:00 UT (Proleptic Gregorian calendar)
    /// @note JD = 1721425.5 が標準的な値
    constexpr static T jdOfGregorianYear1Start() noexcept { return static_cast<T>(1721425.5); }

    /// @brief 紀元1年1月1日 00:00 UT のユリウス日（プロレプティック・ユリウス暦）
    /// @brief Julian Date for 0001-01-01 00:00 UT (Proleptic Julian calendar)
    /// @note JD = 1721059.5 が標準的な値
    constexpr static T jdOfJulianYear1Start() noexcept { return static_cast<T>(1721059.5); }
};

}

#endif // !PAX_SAPIENTICA_CORE_CALENDAR_CONSTANTS_HPP

