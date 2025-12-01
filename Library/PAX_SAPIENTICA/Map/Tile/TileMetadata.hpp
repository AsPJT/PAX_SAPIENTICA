/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]      2023-2024 As Project
    [Production]    2023-2024 As Project
    [Contact Us]    wanotaitei@gmail.com            https://github.com/AsPJT/PAX_SAPIENTICA
    [License]       Distributed under the CC0 1.0. https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_MAP_TILE_TILE_METADATA_HPP
#define PAX_SAPIENTICA_MAP_TILE_TILE_METADATA_HPP

#include <cstdint>
#include <string>

namespace paxs {

    /// @brief XYZTile の設定情報を保持する構造体
    /// @brief Structure holding configuration information for XYZTile
    struct TileMetadata {
        /// @brief メニューバーマップID
        std::uint_least32_t menu_bar_map = 0;

        /// @brief テクスチャのルートパス種別
        std::uint_least32_t texture_root_path_type = 0;

        /// @brief バイナリのルートパス種別
        std::uint_least32_t binary_root_path_type = 0;

        /// @brief バイナリファイル名（拡張子なし）
        std::string map_binary_name = "";

        /// @brief 地図ファイルパス名
        std::string map_file_path_name = "";

        /// @brief ファイル名フォーマット（例: "{z}/{x}/{y}"）
        std::string file_name_format = "";

        /// @brief 描画タイプ
        std::uint_least32_t draw_type = 0;

        /// @brief テクスチャのURL（空の場合はURL読み込みなし）
        std::string texture_url = "";
    };

} // namespace paxs

#endif // !PAX_SAPIENTICA_MAP_TILE_TILE_METADATA_HPP
