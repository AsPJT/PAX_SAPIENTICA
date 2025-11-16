/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SYSTEM_FEATURE_VISIBILITY_MANAGER_HPP
#define PAX_SAPIENTICA_SYSTEM_FEATURE_VISIBILITY_MANAGER_HPP

#include <cstdint>

#include <PAX_SAPIENTICA/Key/MenuBarKeys.hpp>
#include <PAX_SAPIENTICA/Core/Type/UnorderedMap.hpp>

namespace paxs {

    /// @brief 機能（Feature）の表示/非表示を管理するクラス
    /// @brief GUI上で可視化する項目の状態を管理
    class FeatureVisibilityManager {
    private:
        paxs::UnorderedMap<std::uint_least32_t, bool> visible_features; // 機能の可視性を管理する辞書

    public:
        FeatureVisibilityManager() = default;

        /// @brief 新しい機能を登録（ハッシュ値版）
        /// @param feature_id 機能ID（ハッシュ値）
        /// @param is_visible 初期可視状態
        void emplace(const std::uint_least32_t& feature_id, const bool is_visible) {
            visible_features.emplace(feature_id, is_visible);
        }

        /// @brief 新しい機能を登録（enum版）
        /// @tparam T メニュー項目enum型（MenuItemEnumに登録されている型のみ）
        /// @param item メニュー項目識別子
        /// @param is_visible 初期可視状態
        template<typename T>
        void emplace(T item, bool is_visible) requires (is_menu_item_enum_v<T>) {
            emplace(static_cast<std::uint_least32_t>(item), is_visible);
        }

        /// @brief 指定した機能の可視状態を反転（ハッシュ値版）
        /// @param feature_id 機能ID（ハッシュ値）
        void toggle(const std::uint_least32_t& feature_id) {
            visible_features[feature_id] = !visible_features[feature_id];
        }

        /// @brief 指定した機能の可視状態を反転（enum版）
        /// @tparam T メニュー項目enum型（MenuItemEnumに登録されている型のみ）
        /// @param item メニュー項目識別子
        template<typename T>
        void toggle(T item) requires (is_menu_item_enum_v<T>) {
            toggle(static_cast<std::uint_least32_t>(item));
        }

        /// @brief 指定した機能の可視状態を設定（ハッシュ値版）
        /// @param feature_id 機能ID（ハッシュ値）
        /// @param is_visible 可視状態
        /// @return 値が変更されたらtrue
        bool setVisibility(const std::uint_least32_t& feature_id, const bool is_visible) {
            const auto iterator = visible_features.find(feature_id);
            if (iterator != visible_features.end() && iterator->second == is_visible) {
                return false; // 変更なし
            }
            visible_features[feature_id] = is_visible;
            return true; // 変更あり
        }

        /// @brief 指定した機能の可視状態を設定（enum版）
        /// @tparam T メニュー項目enum型（MenuItemEnumに登録されている型のみ）
        /// @param item メニュー項目識別子
        /// @param is_visible 可視状態
        /// @return 値が変更されたらtrue
        template<typename T>
        bool setVisibility(T item, bool is_visible) requires (is_menu_item_enum_v<T>) {
            return setVisibility(static_cast<std::uint_least32_t>(item), is_visible);
        }

        /// @brief 指定した機能の可視状態を取得（ハッシュ値版）
        /// @param feature_id 機能ID（ハッシュ値）
        /// @return 可視状態（登録されていない場合はtrue）
        [[nodiscard]] bool isVisible(const std::uint_least32_t& feature_id) const {
            return (visible_features.find(feature_id) != visible_features.end()) ? visible_features.at(feature_id) : true;
        }

        /// @brief 指定した機能の可視状態を取得（enum版）
        /// @tparam T メニュー項目enum型（MenuItemEnumに登録されている型のみ）
        /// @param item メニュー項目識別子
        /// @return 可視状態（登録されていない場合はtrue）
        template<typename T>
        [[nodiscard]] bool isVisible(T item) const requires (is_menu_item_enum_v<T>) {
            return isVisible(static_cast<std::uint_least32_t>(item));
        }
    };

}

#endif // !PAX_SAPIENTICA_SYSTEM_FEATURE_VISIBILITY_MANAGER_HPP
