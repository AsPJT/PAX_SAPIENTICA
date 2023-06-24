/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023 As Project
	[Production]	2023 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_CALENDAR_DATE_HPP
#define PAX_SAPIENTICA_CALENDAR_DATE_HPP

/*##########################################################################################
	日付の構造体
##########################################################################################*/

namespace paxs {

	// 名年月日の暦
	// 例：和暦、中国暦
	template<typename N, typename Y, typename M, typename D>
	struct Date3N {
		N name{};
		Y year{};
		M month{};
		D day{};
	};

	// 年月日の暦
	// 例：グレゴリオ暦、ユリウス暦
	template<typename Y, typename M, typename D>
	struct Date3 {
		Y year{};
		M month{};
		D day{};
	};

	// 日の暦
	// 例：グレゴリオ暦、ユリウス暦
	template<typename D>
	struct Date1 {
		D day{};
	};



}

#endif // !PAX_SAPIENTICA_CALENDAR_DATE_HPP