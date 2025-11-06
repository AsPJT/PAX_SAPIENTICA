/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_KEY_MENU_BAR_KEYS_HPP
#define PAX_SAPIENTICA_KEY_MENU_BAR_KEYS_HPP

#include <array>
#include <cstdint>

#include <PAX_SAPIENTICA/MurMur3.hpp>

namespace paxs {

    /// @brief メニューバー関連の文字列キー定数
    namespace MenuBarKeys {
        // View メニュー
        constexpr const char* VIEW = "menu_bar_view";
        constexpr const char* VIEW_CALENDAR = "menu_bar_view_calendar";
        constexpr const char* VIEW_MAP = "menu_bar_view_map";
        constexpr const char* VIEW_UI = "menu_bar_view_ui";
        constexpr const char* VIEW_SIMULATION = "menu_bar_view_simulation";
        constexpr const char* VIEW_LICENSE = "menu_bar_view_license";
        constexpr const char* VIEW_DEBUG = "menu_bar_view_debug";
        constexpr const char* VIEW_3D = "menu_bar_view_3d";

        /// @brief View メニューの全項目ハッシュ配列（タイトル含む）
        inline constexpr std::array VIEW_MENU_HASHES = {
            MurMur3::calcHash(VIEW),
            MurMur3::calcHash(VIEW_CALENDAR),
            MurMur3::calcHash(VIEW_MAP),
            MurMur3::calcHash(VIEW_UI),
            MurMur3::calcHash(VIEW_SIMULATION),
            MurMur3::calcHash(VIEW_LICENSE),
            MurMur3::calcHash(VIEW_DEBUG),
            MurMur3::calcHash(VIEW_3D)
        };

        // Feature メニュー
        constexpr const char* PLACE_NAMES = "place_names";
        constexpr const char* PLACE_NAME = "place_names_place_name";
        constexpr const char* SITE = "place_names_site";
        constexpr const char* TUMULUS = "place_names_tumulus";
        constexpr const char* DOLMEN = "place_names_dolmen";
        constexpr const char* KAMEKANBO = "place_names_kamekanbo";
        constexpr const char* STONE_COFFIN = "place_names_stone_coffin";
        constexpr const char* DOKEN = "place_names_doken";
        constexpr const char* DOTAKU = "place_names_dotaku";
        constexpr const char* BRONZE_MIRROR = "place_names_bronze_mirror";
        constexpr const char* HUMAN_BONE = "place_names_human_bone";
        constexpr const char* MTDNA = "place_names_mtdna";
        constexpr const char* YDNA = "place_names_ydna";

        /// @brief Feature メニューの全項目ハッシュ配列（タイトル含む）
        inline constexpr std::array FEATURE_MENU_HASHES = {
            MurMur3::calcHash(PLACE_NAMES),
            MurMur3::calcHash(PLACE_NAME),
            MurMur3::calcHash(SITE),
            MurMur3::calcHash(TUMULUS),
            MurMur3::calcHash(DOLMEN),
            MurMur3::calcHash(KAMEKANBO),
            MurMur3::calcHash(STONE_COFFIN),
            MurMur3::calcHash(DOKEN),
            MurMur3::calcHash(DOTAKU),
            MurMur3::calcHash(BRONZE_MIRROR),
            MurMur3::calcHash(HUMAN_BONE),
            MurMur3::calcHash(MTDNA),
            MurMur3::calcHash(YDNA)
        };

        // Map メニュー
        constexpr const char* MAP = "menu_bar_map";
        constexpr const char* MAP_BASE = "menu_bar_map_base";
        constexpr const char* MAP_LAND_AND_SEA = "menu_bar_map_land_and_sea";
        constexpr const char* MAP_LAND_AND_WATER = "menu_bar_map_land_and_water";
        constexpr const char* MAP_SOIL = "menu_bar_map_soil";
        constexpr const char* MAP_SOIL_TEMPERATURE = "menu_bar_map_soil_temperature";
        constexpr const char* MAP_RYOSEI_COUNTRY = "menu_bar_map_ryosei_country";
        constexpr const char* MAP_RYOSEI_LINE = "menu_bar_map_ryosei_line";
        constexpr const char* MAP_SLOPE = "menu_bar_map_slope";
        constexpr const char* MAP_LAKES_AND_RIVERS1 = "menu_bar_map_lakes_and_rivers1";
        constexpr const char* MAP_LAKES_AND_RIVERS2 = "menu_bar_map_lakes_and_rivers2";
        constexpr const char* MAP_LINE1 = "menu_bar_map_line1";
        constexpr const char* MAP_LINE2 = "menu_bar_map_line2";

        /// @brief Map メニューの全項目ハッシュ配列（タイトル含む）
        inline constexpr std::array MAP_MENU_HASHES = {
            MurMur3::calcHash(MAP),
            MurMur3::calcHash(MAP_BASE),
            MurMur3::calcHash(MAP_LAND_AND_SEA),
            MurMur3::calcHash(MAP_LAND_AND_WATER),
            MurMur3::calcHash(MAP_SOIL),
            MurMur3::calcHash(MAP_SOIL_TEMPERATURE),
            MurMur3::calcHash(MAP_RYOSEI_COUNTRY),
            MurMur3::calcHash(MAP_RYOSEI_LINE),
            MurMur3::calcHash(MAP_SLOPE),
            MurMur3::calcHash(MAP_LAKES_AND_RIVERS1),
            MurMur3::calcHash(MAP_LAKES_AND_RIVERS2),
            MurMur3::calcHash(MAP_LINE1),
            MurMur3::calcHash(MAP_LINE2)
        };
    }

}

#endif // !PAX_SAPIENTICA_KEY_MENU_KEYS_HPP
