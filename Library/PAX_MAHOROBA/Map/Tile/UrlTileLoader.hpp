/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]      2023-2024 As Project
    [Production]    2023-2024 As Project
    [Contact Us]    wanotaitei@gmail.com            https://github.com/AsPJT/PAX_SAPIENTICA
    [License]       Distributed under the CC0 1.0. https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_MAP_TILE_URL_TILE_LOADER_HPP
#define PAX_MAHOROBA_MAP_TILE_URL_TILE_LOADER_HPP

#include <memory>
#include <string>

#include <PAX_GRAPHICA/Network.hpp>
#include <PAX_GRAPHICA/Texture.hpp>

#include <PAX_SAPIENTICA/Core/Utility/StringUtils.hpp>
#include <PAX_SAPIENTICA/IO/File/FileSystem.hpp>
#include <PAX_SAPIENTICA/Utility/Logger.hpp>

namespace paxs {

    /// @brief URLからタイルをダウンロードして読み込むローダー
    /// @brief Loader for downloading and loading tiles from URL
    class UrlTileLoader {
    public:
        /// @brief タイルを読み込む（URLからダウンロード）
        /// @brief Load a tile (download from URL)
        /// @param url_path_with_zy Z と Y が既に置換された URL パス（アセットルートからの相対パス）
        /// @param local_path_with_zy Z と Y が既に置換されたローカルパス（アセットルートからの相対パス）
        /// @param x_value X 座標の文字列
        /// @return 読み込みに成功した場合はテクスチャのunique_ptr、失敗した場合はnullptr
        static std::unique_ptr<paxg::Texture> load(
            const std::string& url_path_with_zy,
            const std::string& local_path_with_zy,
            const std::string& x_value
        ) {
            // URL と ローカル相対パスの X を置換
            std::string url_path = url_path_with_zy;
            paxs::StringUtils::replace(url_path, "{x}", x_value);

            std::string relative_path = local_path_with_zy;
            paxs::StringUtils::replace(relative_path, "{x}", x_value);

            // 保存先フォルダを作成（相対パスから親ディレクトリを抽出）
            const std::string parent_dir = FileSystem::getParentPath(relative_path);
            if (!parent_dir.empty()) {
                FileSystem::createDirectories(parent_dir);
            }

            // ダウンロード
            if (!paxg::Network::downloadFile(url_path, relative_path)) {
                // ダウンロード失敗
                return nullptr;
            }

            // ファイル存在チェック（相対パスを使用）
            if (!FileSystem::exists(relative_path)) {
                return nullptr;
            }

            // テクスチャを読み込み（相対パスを使用）
            auto texture = std::make_unique<paxg::Texture>(relative_path);

            // 読み込み失敗チェック
            if (!(*texture)) {
                PAXS_WARNING("UrlTileLoader: Failed to load texture from downloaded file: " + relative_path);
                return nullptr;
            }

            return texture;
        }

    private:
        // インスタンス化を禁止
        UrlTileLoader() = delete;
        UrlTileLoader(const UrlTileLoader&) = delete;
        UrlTileLoader& operator=(const UrlTileLoader&) = delete;
    };

} // namespace paxs

#endif // !PAX_MAHOROBA_MAP_TILE_URL_TILE_LOADER_HPP
