/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]      2023-2024 As Project
    [Production]    2023-2024 As Project
    [Contact Us]    wanotaitei@gmail.com            https://github.com/AsPJT/PAX_SAPIENTICA
    [License]       Distributed under the CC0 1.0. https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_MAP_TILE_I_TILE_LOADER_HPP
#define PAX_MAHOROBA_MAP_TILE_I_TILE_LOADER_HPP

#include <memory>
#include <string>

#include <PAX_GRAPHICA/Texture.hpp>

namespace paxs {

    /// @brief タイルローダーのインターフェース
    /// @brief Interface for tile loaders (Strategy Pattern)
    class ITileLoader {
    public:
        virtual ~ITileLoader() = default;

        /// @brief タイルを読み込む
        /// @brief Load a tile
        /// @param z ズームレベル
        /// @param x X座標
        /// @param y Y座標
        /// @return 読み込みに成功した場合はテクスチャのunique_ptr、失敗した場合はnullptr
        virtual std::unique_ptr<paxg::Texture> load(
            unsigned int z,
            unsigned int x,
            unsigned int y
        ) = 0;

        /// @brief ローダーの名前を取得（デバッグ用）
        /// @brief Get loader name (for debugging)
        /// @return ローダーの名前
        virtual std::string getLoaderName() const = 0;
    };

} // namespace paxs

#endif // !PAX_MAHOROBA_MAP_TILE_I_TILE_LOADER_HPP
