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

// TODO: メニューバーキー定数群を整理する
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
        constexpr const char* item_menu = "item_menu";
        constexpr const char* structure_menu = "structure_menu";
        constexpr const char* map_menu = "map_menu";

        // View メニュー
        constexpr const char* view_header = "view_header";
        constexpr const char* view_calendar = "view_calendar";
        constexpr const char* view_map = "view_map";
        constexpr const char* view_ui = "view_ui";
        constexpr const char* view_simulation = "view_simulation";
        constexpr const char* view_person = "view_person";
        constexpr const char* view_territory = "view_territory";
        constexpr const char* view_flow_curve = "view_flow_curve";
        constexpr const char* view_license = "view_license";
        constexpr const char* view_debug = "view_debug";
        constexpr const char* view_3d = "view_3d";

        /// @brief View メニューの全項目ハッシュ配列（タイトル含む）
        inline constexpr std::array view_menu_hashes = {
            MurMur3::calcHash(view_header),
            MurMur3::calcHash(view_calendar),
            MurMur3::calcHash(view_map),
            MurMur3::calcHash(view_ui),
            MurMur3::calcHash(view_simulation),
            MurMur3::calcHash(view_person),
            MurMur3::calcHash(view_territory),
            MurMur3::calcHash(view_flow_curve),
            MurMur3::calcHash(view_license),
            MurMur3::calcHash(view_debug)
            // MurMur3::calcHash(view_3d)
        };

        // Place Names メニュー
        constexpr const char* place_names_header = "place_names_header";
        constexpr const char* ancient_text = "ancient_text";
        constexpr const char* administrative = "administrative";
        constexpr const char* indigenous = "indigenous";
        constexpr const char* historical_state = "historical_state";
        constexpr const char* general = "general";

        // Item メニュー
        constexpr const char* item = "item";
        constexpr const char* kamekanbo = "kamekanbo";
        constexpr const char* stone_coffin = "stone_coffin";
        constexpr const char* doken = "doken";
        constexpr const char* dotaku = "dotaku";
        constexpr const char* coin = "coin";

        // Structure メニュー
        constexpr const char* structure = "structure";
        constexpr const char* site = "site";
        constexpr const char* tomb = "tomb";
        constexpr const char* dolmen = "dolmen";

        // Genome メニュー
        constexpr const char* genomes_menu = "genomes_menu";
        constexpr const char* genomes_header = "genomes_header";
        constexpr const char* human_bone = "human_bone";
        constexpr const char* mtdna = "mtdna";
        constexpr const char* ydna = "ydna";

        /// @brief Place Names メニューの全項目ハッシュ配列（タイトル含む）
        inline constexpr std::array place_names_menu_hashes = {
            MurMur3::calcHash(place_names_header),
            MurMur3::calcHash(ancient_text),
            MurMur3::calcHash(administrative),
            MurMur3::calcHash(indigenous),
            MurMur3::calcHash(historical_state),
            MurMur3::calcHash(general)
        };

        /// @brief Item メニューの全項目ハッシュ配列（タイトル含む）
        inline constexpr std::array item_menu_hashes = {
            MurMur3::calcHash(item),
            MurMur3::calcHash(kamekanbo),
            MurMur3::calcHash(stone_coffin),
            MurMur3::calcHash(doken),
            MurMur3::calcHash(dotaku),
            MurMur3::calcHash(coin)
        };

        /// @brief Structure メニューの全項目ハッシュ配列（タイトル含む）
        inline constexpr std::array structure_menu_hashes = {
            MurMur3::calcHash(structure),
            MurMur3::calcHash(site),
            MurMur3::calcHash(tomb),
            MurMur3::calcHash(dolmen)
        };

        /// @brief Genome メニューの全項目ハッシュ配列（タイトル含む）
        inline constexpr std::array genome_menu_hashes = {
            MurMur3::calcHash(genomes_header),
            MurMur3::calcHash(human_bone),
            MurMur3::calcHash(mtdna),
            MurMur3::calcHash(ydna)
        };

        // Map メニュー
        constexpr const char* map_header = "map_header";
        constexpr const char* map_land_and_water = "map_land_and_water";
        constexpr const char* map_soil = "map_soil";
        constexpr const char* map_ryosei_line = "map_ryosei_line";
        constexpr const char* map_slope = "map_slope";
        constexpr const char* map_line1 = "map_line1";

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
        item        = MurMur3::calcHash(MenuBarKeys::item_menu),
        structure   = MurMur3::calcHash(MenuBarKeys::structure_menu),
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
        person     = MurMur3::calcHash(MenuBarKeys::view_person),
        territory = MurMur3::calcHash(MenuBarKeys::view_territory),
        flow_curve = MurMur3::calcHash(MenuBarKeys::view_flow_curve),
        license    = MurMur3::calcHash(MenuBarKeys::view_license),
        debug      = MurMur3::calcHash(MenuBarKeys::view_debug),
        view_3d    = MurMur3::calcHash(MenuBarKeys::view_3d),
    };
    template<> struct MenuItemEnum<ViewMenu> { using enum_type = ViewMenu; };

    /// @brief Place Namesメニューの項目識別子
    /// @brief Place Names menu item identifiers
    enum class PlaceNamesMenu : std::uint_least32_t {
        ancient_text     = MurMur3::calcHash(MenuBarKeys::ancient_text),
        administrative   = MurMur3::calcHash(MenuBarKeys::administrative),
        indigenous       = MurMur3::calcHash(MenuBarKeys::indigenous),
        historical_state = MurMur3::calcHash(MenuBarKeys::historical_state),
        general          = MurMur3::calcHash(MenuBarKeys::general),
    };
    template<> struct MenuItemEnum<PlaceNamesMenu> { using enum_type = PlaceNamesMenu; };

    /// @brief Itemメニューの項目識別子
    /// @brief Item menu item identifiers
    enum class ItemMenu : std::uint_least32_t {
        kamekanbo    = MurMur3::calcHash(MenuBarKeys::kamekanbo),
        stone_coffin = MurMur3::calcHash(MenuBarKeys::stone_coffin),
        doken        = MurMur3::calcHash(MenuBarKeys::doken),
        dotaku       = MurMur3::calcHash(MenuBarKeys::dotaku),
        coin         = MurMur3::calcHash(MenuBarKeys::coin),
    };
    template<> struct MenuItemEnum<ItemMenu> { using enum_type = ItemMenu; };

    /// @brief Structureメニューの項目識別子
    /// @brief Structure menu item identifiers
    enum class StructureMenu : std::uint_least32_t {
        site   = MurMur3::calcHash(MenuBarKeys::site),
        tomb   = MurMur3::calcHash(MenuBarKeys::tomb),
        dolmen = MurMur3::calcHash(MenuBarKeys::dolmen),
    };
    template<> struct MenuItemEnum<StructureMenu> { using enum_type = StructureMenu; };

    /// @brief Genomeメニューの項目識別子
    /// @brief Genome menu item identifiers
    enum class GenomeMenu : std::uint_least32_t {
        human_bone     = MurMur3::calcHash(MenuBarKeys::human_bone),
        mtdna          = MurMur3::calcHash(MenuBarKeys::mtdna),
        ydna           = MurMur3::calcHash(MenuBarKeys::ydna),
    };
    template<> struct MenuItemEnum<GenomeMenu> { using enum_type = GenomeMenu; };

    /// @brief Mapメニューの項目識別子（地図レイヤー）
    /// @brief Map menu item identifiers (map layers)
    enum class MapLayersMenu : std::uint_least32_t {
        land_and_water = MurMur3::calcHash(MenuBarKeys::map_land_and_water),
        soil           = MurMur3::calcHash(MenuBarKeys::map_soil),
        ryosei_line    = MurMur3::calcHash(MenuBarKeys::map_ryosei_line),
        slope          = MurMur3::calcHash(MenuBarKeys::map_slope),
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

    /// @brief メニュー項目のEnum配列（ハッシュ配列と対応）
    /// @brief Menu item enum arrays (corresponds to hash arrays)
    namespace MenuBarKeys {
        /// @brief Viewメニューの項目リスト（ヘッダーを除く実際の項目のみ）
        inline constexpr std::array view_menu_items = {
            paxs::ViewMenu::calendar,
            paxs::ViewMenu::map,
            paxs::ViewMenu::ui,
            paxs::ViewMenu::simulation,
            paxs::ViewMenu::person,
            paxs::ViewMenu::territory,
            paxs::ViewMenu::flow_curve,
            paxs::ViewMenu::license,
            paxs::ViewMenu::debug
        };

        /// @brief Place Namesメニューの項目リスト
        inline constexpr std::array place_names_menu_items = {
            paxs::PlaceNamesMenu::ancient_text,
            paxs::PlaceNamesMenu::administrative,
            paxs::PlaceNamesMenu::indigenous,
            paxs::PlaceNamesMenu::historical_state,
            paxs::PlaceNamesMenu::general
        };

        /// @brief Itemメニューの項目リスト
        inline constexpr std::array item_menu_items = {
            paxs::ItemMenu::kamekanbo,
            paxs::ItemMenu::stone_coffin,
            paxs::ItemMenu::doken,
            paxs::ItemMenu::dotaku,
            paxs::ItemMenu::coin
        };

        /// @brief Structureメニューの項目リスト
        inline constexpr std::array structure_menu_items = {
            paxs::StructureMenu::site,
            paxs::StructureMenu::tomb,
            paxs::StructureMenu::dolmen
        };

        /// @brief Genomeメニューの項目リスト
        inline constexpr std::array genome_menu_items = {
            paxs::GenomeMenu::human_bone,
            paxs::GenomeMenu::mtdna,
            paxs::GenomeMenu::ydna
        };

        /// @brief Mapメニューの項目リスト
        inline constexpr std::array map_menu_items = {
            paxs::MapLayersMenu::land_and_water,
            paxs::MapLayersMenu::soil,
            paxs::MapLayersMenu::ryosei_line,
            paxs::MapLayersMenu::slope,
            paxs::MapLayersMenu::line1
        };
    }

}

#endif // !PAX_SAPIENTICA_KEY_MENU_KEYS_HPP
