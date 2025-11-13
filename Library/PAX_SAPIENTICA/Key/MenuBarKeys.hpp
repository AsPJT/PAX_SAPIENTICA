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
#include <type_traits>

#include <PAX_SAPIENTICA/Utility/MurMur3.hpp>

namespace paxs {

    /// @brief メニュー項目識別子の基底クラス（CRTP）
    /// @brief Base class for menu item identifiers (CRTP)
    template<typename Derived>
    struct MenuItemEnum {
        using enum_type = Derived;
    };

    /// @brief メニューバー関連の文字列キー定数
    namespace MenuBarKeys {
        // メニューバー全体
        constexpr const char* view_menu = "view_menu";
        constexpr const char* place_names_menu = "place_names_menu";
        constexpr const char* map_menu = "map_menu";

        // View メニュー
        constexpr const char* view_header = "menu_bar_view";
        constexpr const char* view_calendar = "menu_bar_view_calendar";
        constexpr const char* view_map = "menu_bar_view_map";
        constexpr const char* view_ui = "menu_bar_view_ui";
        constexpr const char* view_simulation = "menu_bar_view_simulation";
        constexpr const char* view_license = "menu_bar_view_license";
        constexpr const char* view_debug = "menu_bar_view_debug";
        constexpr const char* view_3d = "menu_bar_view_3d";

        /// @brief View メニューの全項目ハッシュ配列（タイトル含む）
        inline constexpr std::array view_menu_hashes = {
            MurMur3::calcHash(view_header),
            MurMur3::calcHash(view_calendar),
            MurMur3::calcHash(view_map),
            MurMur3::calcHash(view_ui),
            MurMur3::calcHash(view_simulation),
            MurMur3::calcHash(view_license),
            MurMur3::calcHash(view_debug)
            // MurMur3::calcHash(view_3d)
        };

        // Feature メニュー
        constexpr const char* place_names_header = "place_names";
        constexpr const char* place_name = "place_names_place_name";
        constexpr const char* site = "place_names_site";
        constexpr const char* tumulus = "place_names_tumulus";
        constexpr const char* dolmen = "place_names_dolmen";
        constexpr const char* kamekanbo = "place_names_kamekanbo";
        constexpr const char* stone_coffin = "place_names_stone_coffin";
        constexpr const char* doken = "place_names_doken";
        constexpr const char* dotaku = "place_names_dotaku";
        constexpr const char* bronze_mirror = "place_names_bronze_mirror";

        // Genome メニュー
        constexpr const char* genomes_menu = "genomes_menu";
        constexpr const char* genomes_header = "genomes";
        constexpr const char* human_bone = "genomes_human_bone";
        constexpr const char* mtdna = "genomes_mtdna";
        constexpr const char* ydna = "genomes_ydna";

        /// @brief Feature メニューの全項目ハッシュ配列（タイトル含む）
        inline constexpr std::array feature_menu_hashes = {
            MurMur3::calcHash(place_names_header),
            MurMur3::calcHash(place_name),
            MurMur3::calcHash(site),
            MurMur3::calcHash(tumulus),
            MurMur3::calcHash(dolmen),
            MurMur3::calcHash(kamekanbo),
            MurMur3::calcHash(stone_coffin),
            MurMur3::calcHash(doken),
            MurMur3::calcHash(dotaku),
            MurMur3::calcHash(bronze_mirror)
        };

        /// @brief Genome メニューの全項目ハッシュ配列（タイトル含む）
        inline constexpr std::array genome_menu_hashes = {
            MurMur3::calcHash(genomes_header),
            MurMur3::calcHash(human_bone),
            MurMur3::calcHash(mtdna),
            MurMur3::calcHash(ydna)
        };

        // Map メニュー
        constexpr const char* map_header = "menu_bar_map";
        constexpr const char* map_land_and_water = "menu_bar_map_land_and_water";
        constexpr const char* map_soil = "menu_bar_map_soil";
        constexpr const char* map_ryosei_line = "menu_bar_map_ryosei_line";
        constexpr const char* map_slope = "menu_bar_map_slope";
        constexpr const char* map_line1 = "menu_bar_map_line1";

        /// @brief Map メニューの全項目ハッシュ配列（タイトル含む）
        inline constexpr std::array map_menu_hashes = {
            MurMur3::calcHash(map_header),
            MurMur3::calcHash(map_land_and_water),
            MurMur3::calcHash(map_soil),
            MurMur3::calcHash(map_ryosei_line),
            MurMur3::calcHash(map_slope),
            MurMur3::calcHash(map_line1)
        };
    }

    /// @brief メニューバーの項目識別子
    /// @brief Menu bar item identifiers
    enum class MenuBarType : std::uint_least32_t {
        view        = MurMur3::calcHash(MenuBarKeys::view_menu),
        place_names = MurMur3::calcHash(MenuBarKeys::place_names_menu),
        genomes     = MurMur3::calcHash(MenuBarKeys::genomes_menu),
        map         = MurMur3::calcHash(MenuBarKeys::map_menu),
    };

    /// @brief Viewメニューの項目識別子
    /// @brief View menu item identifiers
    enum class ViewMenu : std::uint_least32_t {
        calendar   = MurMur3::calcHash(MenuBarKeys::view_calendar),
        map        = MurMur3::calcHash(MenuBarKeys::view_map),
        ui         = MurMur3::calcHash(MenuBarKeys::view_ui),
        simulation = MurMur3::calcHash(MenuBarKeys::view_simulation),
        license    = MurMur3::calcHash(MenuBarKeys::view_license),
        debug      = MurMur3::calcHash(MenuBarKeys::view_debug),
        view_3d    = MurMur3::calcHash(MenuBarKeys::view_3d),
    };
    template<> struct MenuItemEnum<ViewMenu> { using enum_type = ViewMenu; };

    /// @brief Place Namesメニューの項目識別子
    /// @brief Place Names menu item identifiers
    enum class PlaceNamesMenu : std::uint_least32_t {
        place_name     = MurMur3::calcHash("place_name"),
        site           = MurMur3::calcHash("site"),
        tumulus        = MurMur3::calcHash("tumulus"),
        dolmen         = MurMur3::calcHash("dolmen"),
        kamekanbo      = MurMur3::calcHash("kamekanbo"),
        stone_coffin   = MurMur3::calcHash("stone_coffin"),
        doken          = MurMur3::calcHash("doken"),
        dotaku         = MurMur3::calcHash("dotaku"),
        bronze_mirror  = MurMur3::calcHash("bronze_mirror"),
    };
    template<> struct MenuItemEnum<PlaceNamesMenu> { using enum_type = PlaceNamesMenu; };

    /// @brief Genomeメニューの項目識別子
    /// @brief Genome menu item identifiers
    enum class GenomeMenu : std::uint_least32_t {
        human_bone     = MurMur3::calcHash("bone"),
        mtdna          = MurMur3::calcHash("mtdna"),
        ydna           = MurMur3::calcHash("ydna"),
    };
    template<> struct MenuItemEnum<GenomeMenu> { using enum_type = GenomeMenu; };

    /// @brief Mapメニューの項目識別子（地図レイヤー）
    /// @brief Map menu item identifiers (map layers)
    enum class MapLayersMenu : std::uint_least32_t {
        land_and_water = MurMur3::calcHash("menu_bar_map_land_and_water"),
        soil           = MurMur3::calcHash("menu_bar_map_soil"),
        ryosei_line    = MurMur3::calcHash("menu_bar_map_ryosei_line"),
        slope          = MurMur3::calcHash("menu_bar_map_slope"),
        line1          = MurMur3::calcHash(MenuBarKeys::map_line1),
    };
    template<> struct MenuItemEnum<MapLayersMenu> { using enum_type = MapLayersMenu; };

    /// @brief 型がMenuItemEnumに登録されているかチェック
    /// @brief Check if type is registered in MenuItemEnum
    template<typename T, typename = void>
    struct is_menu_item_enum : std::false_type {};

    template<typename T>
    struct is_menu_item_enum<T, std::void_t<typename MenuItemEnum<T>::enum_type>> : std::true_type {};

    template<typename T>
    inline constexpr bool is_menu_item_enum_v = is_menu_item_enum<T>::value;

}

#endif // !PAX_SAPIENTICA_KEY_MENU_KEYS_HPP
