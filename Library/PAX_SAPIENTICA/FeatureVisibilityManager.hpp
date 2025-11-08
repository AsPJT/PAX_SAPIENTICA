/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_FEATURE_VISIBILITY_MANAGER_HPP
#define PAX_SAPIENTICA_FEATURE_VISIBILITY_MANAGER_HPP

#include <cstdint>

#include <PAX_SAPIENTICA/MurMur3.hpp>
#include <PAX_SAPIENTICA/Type/UnorderedMap.hpp>

namespace paxs {

    /// @brief 機能（Feature）の表示/非表示を管理するクラス
    /// @brief GUI上で可視化する項目の状態を管理
    class FeatureVisibilityManager {
    private:
        paxs::UnorderedMap<std::uint_least32_t, bool> visible_features{}; // 機能の可視性を管理する辞書

    public:
        /// @brief Viewメニューの項目識別子
        /// @brief View menu item identifiers
        enum class View : std::uint_least32_t {
            Calendar   = MurMur3::calcHash("Calendar"),
            Map        = MurMur3::calcHash("Map"),
            UI         = MurMur3::calcHash("UI"),
            Simulation = MurMur3::calcHash("Simulation"),
            License    = MurMur3::calcHash("License"),
            Debug      = MurMur3::calcHash("Debug"),
            View3D     = MurMur3::calcHash("3D"),
        };

        /// @brief Place Namesメニューの項目識別子
        /// @brief Place Names menu item identifiers
        enum class PlaceNames : std::uint_least32_t {
            PlaceName     = MurMur3::calcHash("place_name"),
            Site          = MurMur3::calcHash("site"),
            Tumulus       = MurMur3::calcHash("tumulus"),
            Dolmen        = MurMur3::calcHash("dolmen"),
            Kamekanbo     = MurMur3::calcHash("kamekanbo"),
            StoneCoffin   = MurMur3::calcHash("stone_coffin"),
            Doken         = MurMur3::calcHash("doken"),
            Dotaku        = MurMur3::calcHash("dotaku"),
            BronzeMirror  = MurMur3::calcHash("bronze_mirror"),
            HumanBone     = MurMur3::calcHash("human_bone"),
            MtDNA         = MurMur3::calcHash("mtdna"),
            YDna          = MurMur3::calcHash("ydna"),
        };

        /// @brief Mapメニューの項目識別子（地図レイヤー）
        /// @brief Map menu item identifiers (map layers)
        enum class MapLayers : std::uint_least32_t {
            Base            = MurMur3::calcHash("menu_bar_map_base"),
            LandAndSea      = MurMur3::calcHash("menu_bar_map_land_and_sea"),
            LandAndWater    = MurMur3::calcHash("menu_bar_map_land_and_water"),
            Soil            = MurMur3::calcHash("menu_bar_map_soil"),
            SoilTemperature = MurMur3::calcHash("menu_bar_map_soil_temperature"),
            RyoseiCountry   = MurMur3::calcHash("menu_bar_map_ryosei_country"),
            RyoseiLine      = MurMur3::calcHash("menu_bar_map_ryosei_line"),
            Slope           = MurMur3::calcHash("menu_bar_map_slope"),
            LakesAndRivers1 = MurMur3::calcHash("menu_bar_map_lakes_and_rivers1"),
            LakesAndRivers2 = MurMur3::calcHash("menu_bar_map_lakes_and_rivers2"),
            Line1           = MurMur3::calcHash("menu_bar_map_line1"),
            Line2           = MurMur3::calcHash("menu_bar_map_line2"),
        };

        FeatureVisibilityManager() = default;
        ~FeatureVisibilityManager() = default;

        /// @brief 新しい機能を登録（ハッシュ値版）
        /// @param feature_id 機能ID（ハッシュ値）
        /// @param is_visible 初期可視状態
        void emplace(const std::uint_least32_t& feature_id, const bool is_visible) {
            visible_features.emplace(feature_id, is_visible);
        }

        /// @brief 指定した機能の可視状態を反転（ハッシュ値版）
        /// @param feature_id 機能ID（ハッシュ値）
        void toggle(const std::uint_least32_t& feature_id) {
            visible_features[feature_id] = !visible_features[feature_id];
        }

        /// @brief 指定した機能の可視状態を設定（ハッシュ値版）
        /// @param feature_id 機能ID（ハッシュ値）
        /// @param is_visible 可視状態
        /// @return 値が変更されたらtrue
        bool setVisibility(const std::uint_least32_t& feature_id, const bool is_visible) {
            auto it = visible_features.find(feature_id);
            if (it != visible_features.end() && it->second == is_visible) {
                return false; // 変更なし
            }
            visible_features[feature_id] = is_visible;
            return true; // 変更あり
        }

        /// @brief 指定した機能の可視状態を取得（ハッシュ値版）
        /// @param feature_id 機能ID（ハッシュ値）
        /// @return 可視状態（登録されていない場合はtrue）
        bool isVisible(const std::uint_least32_t& feature_id) const {
            return (visible_features.find(feature_id) != visible_features.end()) ? visible_features.at(feature_id) : true;
        }

        /// @brief 指定した機能の可視状態を取得（View enum版）
        /// @param view View項目識別子
        /// @return 可視状態（登録されていない場合はtrue）
        bool isVisible(View view) const {
            return isVisible(static_cast<std::uint_least32_t>(view));
        }

        /// @brief 指定した機能の可視状態を設定（View enum版）
        /// @param view View項目識別子
        /// @param is_visible 可視状態
        /// @return 値が変更されたらtrue
        bool setVisibility(View view, bool is_visible) {
            return setVisibility(static_cast<std::uint_least32_t>(view), is_visible);
        }

        /// @brief 指定した機能の可視状態を反転（View enum版）
        /// @param view View項目識別子
        void toggle(View view) {
            toggle(static_cast<std::uint_least32_t>(view));
        }

        /// @brief 機能を登録（View enum版）
        /// @param view View項目識別子
        /// @param is_visible 初期可視状態
        void emplace(View view, bool is_visible) {
            emplace(static_cast<std::uint_least32_t>(view), is_visible);
        }

        /// @brief 指定した機能の可視状態を取得（PlaceNames enum版）
        /// @param place_name Place Names項目識別子
        /// @return 可視状態（登録されていない場合はtrue）
        bool isVisible(PlaceNames place_name) const {
            return isVisible(static_cast<std::uint_least32_t>(place_name));
        }

        /// @brief 指定した機能の可視状態を設定（PlaceNames enum版）
        /// @param place_name Place Names項目識別子
        /// @param is_visible 可視状態
        /// @return 値が変更されたらtrue
        bool setVisibility(PlaceNames place_name, bool is_visible) {
            return setVisibility(static_cast<std::uint_least32_t>(place_name), is_visible);
        }

        /// @brief 指定した機能の可視状態を反転（PlaceNames enum版）
        /// @param place_name Place Names項目識別子
        void toggle(PlaceNames place_name) {
            toggle(static_cast<std::uint_least32_t>(place_name));
        }

        /// @brief 機能を登録（PlaceNames enum版）
        /// @param place_name Place Names項目識別子
        /// @param is_visible 初期可視状態
        void emplace(PlaceNames place_name, bool is_visible) {
            emplace(static_cast<std::uint_least32_t>(place_name), is_visible);
        }

        /// @brief 指定した機能の可視状態を取得（MapLayers enum版）
        /// @param map_layer Map Layers項目識別子
        /// @return 可視状態（登録されていない場合はtrue）
        bool isVisible(MapLayers map_layer) const {
            return isVisible(static_cast<std::uint_least32_t>(map_layer));
        }

        /// @brief 指定した機能の可視状態を設定（MapLayers enum版）
        /// @param map_layer Map Layers項目識別子
        /// @param is_visible 可視状態
        /// @return 値が変更されたらtrue
        bool setVisibility(MapLayers map_layer, bool is_visible) {
            return setVisibility(static_cast<std::uint_least32_t>(map_layer), is_visible);
        }

        /// @brief 指定した機能の可視状態を反転（MapLayers enum版）
        /// @param map_layer Map Layers項目識別子
        void toggle(MapLayers map_layer) {
            toggle(static_cast<std::uint_least32_t>(map_layer));
        }

        /// @brief 機能を登録（MapLayers enum版）
        /// @param map_layer Map Layers項目識別子
        /// @param is_visible 初期可視状態
        void emplace(MapLayers map_layer, bool is_visible) {
            emplace(static_cast<std::uint_least32_t>(map_layer), is_visible);
        }
    };

}

#endif // !PAX_SAPIENTICA_FEATURE_VISIBILITY_MANAGER_HPP
