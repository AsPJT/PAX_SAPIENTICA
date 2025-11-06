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

#include <PAX_SAPIENTICA/Type/UnorderedMap.hpp>

namespace paxs {

    /// @brief 機能（Feature）の表示/非表示を管理するクラス
    /// @brief GUI上で可視化する項目の状態を管理
    class FeatureVisibilityManager {
    private:
        paxs::UnorderedMap<std::uint_least32_t, bool> visible_features{}; // 機能の可視性を管理する辞書

    public:
        FeatureVisibilityManager() = default;
        ~FeatureVisibilityManager() = default;

        /// @brief 新しい機能を登録
        /// @param feature_id 機能ID（ハッシュ値）
        /// @param is_visible 初期可視状態
        void emplace(const std::uint_least32_t& feature_id, const bool is_visible) {
            visible_features.emplace(feature_id, is_visible);
        }

        /// @brief 指定した機能の可視状態を反転
        /// @param feature_id 機能ID（ハッシュ値）
        void toggle(const std::uint_least32_t& feature_id) {
            visible_features[feature_id] = !visible_features[feature_id];
        }

        /// @brief 指定した機能の可視状態を設定
        /// @param feature_id 機能ID（ハッシュ値）
        /// @param is_visible 可視状態
        void setVisibility(const std::uint_least32_t& feature_id, const bool is_visible) {
            visible_features[feature_id] = is_visible;
        }

        /// @brief 指定した機能の可視状態を取得
        /// @param feature_id 機能ID（ハッシュ値）
        /// @return 可視状態（登録されていない場合はtrue）
        bool isVisible(const std::uint_least32_t& feature_id) const {
            return (visible_features.find(feature_id) != visible_features.end()) ? visible_features.at(feature_id) : true;
        }
    };

}

#endif // !PAX_SAPIENTICA_FEATURE_VISIBILITY_MANAGER_HPP
