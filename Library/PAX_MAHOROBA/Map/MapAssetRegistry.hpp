/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_MAP_ASSET_REGISTRY_HPP
#define PAX_MAHOROBA_MAP_ASSET_REGISTRY_HPP

#include <PAX_GRAPHICA/Texture.hpp>

#include <PAX_SAPIENTICA/Core/Type/UnorderedMap.hpp>
#include <PAX_SAPIENTICA/IO/Data/KeyValueTSV.hpp>
#include <PAX_SAPIENTICA/Utility/Logger.hpp>

namespace paxs {

/// @brief 地図アイコンアセット（テクスチャ）を統合管理するレジストリ
/// @brief Unified registry for managing map icon assets (textures)
/// @details 人物アイコン（Portraits）と地図アイコン（MapIcons）の2種類のテクスチャを管理し、
///          ハッシュキーによる高速アクセスを提供
class MapAssetRegistry {
private:
    // カテゴリ別のテクスチャマップ（実際には2種類のみ）
    KeyValueTSV<paxg::Texture> person_textures_;   ///< Portraitsテクスチャ
    KeyValueTSV<paxg::Texture> map_icon_textures_; ///< MapIconsテクスチャ（地理・ゲノム共用）

    // 統合された全テクスチャマップ（高速アクセス用）
    UnorderedMap<std::uint_least32_t, paxg::Texture> merged_textures_;
    bool is_merged_ = false;

public:
    MapAssetRegistry() = default;

    // ============================================================================
    // ロード / Loading
    // ============================================================================

    /// @brief 人物アイコンをロード（Portraits）
    /// @brief Load person icons (Portraits)
    /// @param portraits_path ポートレートTSVファイルのパス / Path to portraits TSV
    /// @return 成功時true / True on success
    bool loadPersonIcons(const std::string& portraits_path) {
        return person_textures_.input(portraits_path);
    }

    /// @brief 地図アイコンをロード（MapIcons：地理・ゲノム共用）
    /// @brief Load map icons (MapIcons: shared by geographic and genome)
    /// @param map_icons_path 地図アイコンTSVファイルのパス / Path to map icons TSV
    /// @return 成功時true / True on success
    bool loadMapIcons(const std::string& map_icons_path) {
        return map_icon_textures_.input(map_icons_path);
    }

    /// @brief 全カテゴリをマージ
    /// @brief Merge all categories
    /// @details ロード後に一度だけ呼び出してマージマップを作成
    void mergeCategories() {
        if (is_merged_) {
            return;
        }

        merged_textures_.clear();

        // 人物テクスチャをマージ
        for (const auto& [key, texture] : person_textures_.get()) {
            merged_textures_.emplace(key, texture);
        }

        // 地図アイコンテクスチャをマージ
        for (const auto& [key, texture] : map_icon_textures_.get()) {
            merged_textures_.emplace(key, texture);
        }

        is_merged_ = true;
    }

    // ============================================================================
    // アクセス / Access
    // ============================================================================

    /// @brief ハッシュキーからテクスチャを取得
    /// @brief Get texture by hash key
    /// @param key ハッシュキー / Hash key
    /// @return テクスチャへのポインタ（存在しない場合nullptr） / Pointer to texture (nullptr if not found)
    const paxg::Texture* getTexture(std::uint_least32_t key) const {
        const auto iterator = merged_textures_.find(key);
        return (iterator != merged_textures_.end()) ? &iterator->second : nullptr;
    }

    /// @brief ハッシュキーからテクスチャを取得（参照）
    /// @brief Get texture by hash key (reference)
    /// @param key ハッシュキー / Hash key
    /// @return テクスチャへの参照 / Reference to texture
    /// @throws std::out_of_range キーが存在しない場合 / If key does not exist
    const paxg::Texture& at(std::uint_least32_t key) const {
        return merged_textures_.at(key);
    }

    /// @brief テクスチャが存在するかチェック
    /// @brief Check if texture exists
    /// @param key ハッシュキー / Hash key
    /// @return 存在する場合true / True if exists
    bool contains(std::uint_least32_t key) const {
        return merged_textures_.contains(key);
    }

    /// @brief 統合マップを取得
    /// @brief Get merged map
    /// @return 統合されたテクスチャマップへの参照 / Reference to merged texture map
    const UnorderedMap<std::uint_least32_t, paxg::Texture>& getMergedMap() const {
        return merged_textures_;
    }

    // ============================================================================
    // ステータス / Status
    // ============================================================================

    /// @brief 全テクスチャ数を取得
    /// @brief Get total texture count
    /// @return テクスチャ数 / Number of textures
    std::size_t size() const {
        return merged_textures_.size();
    }

    /// @brief マージ済みかチェック
    /// @brief Check if merged
    /// @return マージ済みの場合true / True if merged
    bool isMerged() const {
        return is_merged_;
    }
};

} // namespace paxs

#endif // !PAX_MAHOROBA_MAP_ASSET_REGISTRY_HPP
