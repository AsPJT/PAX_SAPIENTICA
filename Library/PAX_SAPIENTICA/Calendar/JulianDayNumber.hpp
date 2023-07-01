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

#include <PAX_SAPIENTICA/Type/Date.hpp>

namespace paxs {
	// TODO:改修必須
	namespace tmp {
		double getK(const double month) {
			return std::floor((14 - month) / 12);
		}
	}
	// グレゴリオ暦からユリウス日を取得
	double getJulianDayNumberFromGregorianCalendar(const double year, const double month, const double day) {
		const double K = tmp::getK(month);
		return std::floor((-K + year + 4800) * 1461 / 4)
			+ std::floor((K * 12 + month - 2) * 367 / 12)
			- std::floor(std::floor((-K + year + 4900) / 100) * 3 / 4)
			+ day - 32075;
	}
	// ユリウス暦からユリウス日を取得
	double getJulianDayNumberFromJulianCalendar(const double year, const double month, const double day) {
		const double K = tmp::getK(month);
		return std::floor((-K + year + 4800) * 1461 / 4)
			+ std::floor((K * 12 + month - 2) * 367 / 12)
			+ day - 32113;
	}
	// ユリウス日からグレゴリオ暦を取得
	Date getGregorianCalendarFromJulianDayNumber(const double JDN) {
		Date ymd;
		//JDN ⇒ グレゴリオ暦
		double L = JDN + 68569;
		const double N = std::floor(4 * L / 146097);
		L = L - std::floor((146097 * N + 3) / 4);
		const double I = std::floor(4000 * (L + 1) / 1461001);
		L = L - std::floor(1461 * I / 4) + 31;
		const double J = std::floor(80 * L / 2447);
		ymd.day = DateDay(L - std::floor(2447 * J / 80));
		L = std::floor(J / 11);
		ymd.month = DateMonth(J + 2 - 12 * L);
		ymd.year = DateYear(100 * (N - 49) + I + L);
		return ymd;
	}
	// ユリウス日からユリウス暦を取得
	Date getJulianCalendarFromJulianDayNumber(const double JDN) {
		Date ymd;
		double L = JDN + 1402;
		const double N = std::floor((L - 1) / 1461);
		L = L - 1461 * N;
		const double I = std::floor((L - 1) / 365) - std::floor(L / 1461);
		L = L - 365 * I + 30;
		const double J = std::floor(80 * L / 2447);
		ymd.day = DateDay(L - std::floor(2447 * J / 80));
		L = std::floor(J / 11);
		ymd.month = DateMonth(J + 2 - 12 * L);
		ymd.year = DateYear(4 * N + I + L - 4716);
		return ymd;
	}
}

#endif // !PAX_SAPIENTICA_CALENDAR_JULIAN_DAY_NUMBER_HPP