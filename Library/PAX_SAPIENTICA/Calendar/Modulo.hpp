/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023 As Project
	[Production]	2023 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_CALENDAR_MODULO_HPP
#define PAX_SAPIENTICA_CALENDAR_MODULO_HPP

/*##########################################################################################
	Calculate the calendar with cycles, such as days of the week.
	曜日などの周期のある暦を計算する。
##########################################################################################*/

namespace paxs {

	constexpr const char32_t* yobi_char = { U'日',U'月',U'火',U'水',U'木',U'金',U'土' };
	constexpr const char32_t* jikkan_char = { U'甲',U'乙',U'丙',U'丁',U'戊',U'己',U'庚',U'辛',U'壬',U'癸' };
	constexpr const char32_t* junishi_char = { U'子',U'丑',U'寅',U'卯',U'辰',U'巳',U'午',U'未',U'申',U'酉',U'戌',U'亥' };

}

#endif // !PAX_SAPIENTICA_CALENDAR_MODULO_HPP