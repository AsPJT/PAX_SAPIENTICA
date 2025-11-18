/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_KEY_CALENDAR_KEYS_HPP
#define PAX_SAPIENTICA_KEY_CALENDAR_KEYS_HPP

#include <cstdint>

#include <PAX_SAPIENTICA/Utility/MurMur3.hpp>

namespace paxs {

    /// @brief 暦キーの文字列定数
    namespace CalendarKeys {
        // 各暦のキー
        constexpr const char* CALENDAR_JAPAN = "calendar_japan";
        constexpr const char* CALENDAR_GREGORIAN = "calendar_gregorian";
        constexpr const char* CALENDAR_JULIAN = "calendar_julian";
        constexpr const char* CALENDAR_HIJRI = "calendar_hijri";
        constexpr const char* CALENDAR_CHINESE = "calendar_chinese";
        constexpr const char* CALENDAR_JULIAN_DAY = "calendar_julian_day";
        constexpr const char* CALENDAR_CALBP = "calendar_calbp";

        // メニューバー関連
        constexpr const char* MENU_BAR_VIEW_SIMULATION = "menu_bar_view_simulation";

        // 須恵器編年
        constexpr const char* SUEKI_NAKAMURA = "sueki_nakamura";
        constexpr const char* SUEKI_TANABE = "sueki_tanabe";

        // ハッシュ化済みキー
        inline constexpr std::uint_least32_t CALENDAR_JAPAN_HASH = MurMur3::calcHash(CALENDAR_JAPAN);
        inline constexpr std::uint_least32_t CALENDAR_GREGORIAN_HASH = MurMur3::calcHash(CALENDAR_GREGORIAN);
        inline constexpr std::uint_least32_t CALENDAR_JULIAN_HASH = MurMur3::calcHash(CALENDAR_JULIAN);
        inline constexpr std::uint_least32_t CALENDAR_HIJRI_HASH = MurMur3::calcHash(CALENDAR_HIJRI);
        inline constexpr std::uint_least32_t CALENDAR_CHINESE_HASH = MurMur3::calcHash(CALENDAR_CHINESE);
        inline constexpr std::uint_least32_t CALENDAR_JULIAN_DAY_HASH = MurMur3::calcHash(CALENDAR_JULIAN_DAY);
        inline constexpr std::uint_least32_t CALENDAR_CALBP_HASH = MurMur3::calcHash(CALENDAR_CALBP);
        inline constexpr std::uint_least32_t MENU_BAR_VIEW_SIMULATION_HASH = MurMur3::calcHash(MENU_BAR_VIEW_SIMULATION);
        inline constexpr std::uint_least32_t SUEKI_NAKAMURA_HASH = MurMur3::calcHash(SUEKI_NAKAMURA);
        inline constexpr std::uint_least32_t SUEKI_TANABE_HASH = MurMur3::calcHash(SUEKI_TANABE);
    }

}

#endif // !PAX_SAPIENTICA_KEY_CALENDAR_KEYS_HPP
