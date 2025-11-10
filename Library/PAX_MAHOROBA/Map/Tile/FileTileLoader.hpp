/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]      2023-2024 As Project
    [Production]    2023-2024 As Project
    [Contact Us]    wanotaitei@gmail.com            https://github.com/AsPJT/PAX_SAPIENTICA
    [License]       Distributed under the CC0 1.0. https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_MAP_TILE_FILE_TILE_LOADER_HPP
#define PAX_MAHOROBA_MAP_TILE_FILE_TILE_LOADER_HPP

#include <filesystem>
#include <memory>
#include <string>

#include <PAX_GRAPHICA/Texture.hpp>
#include <PAX_SAPIENTICA/AppConfig.hpp>
#include <PAX_SAPIENTICA/StringExtensions.hpp>

namespace paxs {

    /// @brief ローカルファイルからタイルを読み込むローダー
    /// @brief Loader for loading tiles from local files
    class FileTileLoader {
    public:
        /// @brief タイルを読み込む
        /// @brief Load a tile
        /// @param path_with_zy Z と Y が既に置換されたパス（アセットルートからの相対パス）
        /// @param x_value X 座標の文字列
        /// @return 読み込みに成功した場合はテクスチャのunique_ptr、失敗した場合はnullptr
        static std::unique_ptr<paxg::Texture> load(
            const std::string& path_with_zy,
            const std::string& x_value
        ) {
            // X を置換して相対パスを構築
            std::string relative_path = path_with_zy;
            paxs::StringExtensions::replace(relative_path, "{x}", x_value);

            // アセットルートパスを前置して絶対パスを構築
            const std::string full_path = AppConfig::getInstance()->getRootPath() + relative_path;

#if defined(PAXS_USING_DXLIB) && defined(__ANDROID__)
            // Android DxLib: ファイル存在チェックをスキップ
            // （アセットファイルは std::filesystem::exists で検出できないため）
#else
            if (!std::filesystem::exists(full_path)) {
                return nullptr;
            }
#endif

            // テクスチャを読み込み（相対パスを使用）
            auto texture = std::make_unique<paxg::Texture>(relative_path);

            // 読み込み失敗チェック
            if (!(*texture)) {
                return nullptr;
            }

            return texture;
        }

    private:
        // インスタンス化を禁止
        FileTileLoader() = delete;
        FileTileLoader(const FileTileLoader&) = delete;
        FileTileLoader& operator=(const FileTileLoader&) = delete;
    };

} // namespace paxs

#endif // !PAX_MAHOROBA_MAP_TILE_FILE_TILE_LOADER_HPP
