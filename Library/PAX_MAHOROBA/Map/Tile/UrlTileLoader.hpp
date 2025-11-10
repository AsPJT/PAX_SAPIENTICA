/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]      2023-2024 As Project
    [Production]    2023-2024 As Project
    [Contact Us]    wanotaitei@gmail.com            https://github.com/AsPJT/PAX_SAPIENTICA
    [License]       Distributed under the CC0 1.0. https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_MAP_TILE_URL_TILE_LOADER_HPP
#define PAX_MAHOROBA_MAP_TILE_URL_TILE_LOADER_HPP

#include <filesystem>
#include <memory>
#include <string>

#include <PAX_GRAPHICA/Network.hpp>
#include <PAX_GRAPHICA/Texture.hpp>
#include <PAX_SAPIENTICA/AppConfig.hpp>
#include <PAX_SAPIENTICA/Logger.hpp>
#include <PAX_SAPIENTICA/StringExtensions.hpp>

namespace paxs {

    /// @brief URLからタイルをダウンロードして読み込むローダー
    /// @brief Loader for downloading and loading tiles from URL
    class UrlTileLoader {
    public:
        /// @brief タイルを読み込む（URLからダウンロード）
        /// @brief Load a tile (download from URL)
        /// @param url_path_with_zy Z と Y が既に置換された URL パス（アセットルートからの相対パス）
        /// @param local_path_with_zy Z と Y が既に置換されたローカルパス（アセットルートからの相対パス）
        /// @param folder_path_with_zyx Z, Y, X が既に置換されたフォルダパス（アセットルートからの相対パス）
        /// @param x_value X 座標の文字列
        /// @return 読み込みに成功した場合はテクスチャのunique_ptr、失敗した場合はnullptr
        static std::unique_ptr<paxg::Texture> load(
            const std::string& url_path_with_zy,
            const std::string& local_path_with_zy,
            const std::string& folder_path_with_zyx,
            const std::string& x_value
        ) {
            // URL と ローカル相対パスの X を置換
            std::string url_path = url_path_with_zy;
            paxs::StringExtensions::replace(url_path, "{x}", x_value);

            std::string relative_path = local_path_with_zy;
            paxs::StringExtensions::replace(relative_path, "{x}", x_value);

            // アセットルートパスを前置して絶対パスを構築
            const std::string root_path = AppConfig::getInstance()->getRootPath();
            const std::string full_path = root_path + relative_path;
            const std::string full_folder_path = root_path + folder_path_with_zyx;

            // 保存先フォルダを作成（絶対パスを使用）
            std::filesystem::create_directories(full_folder_path);

            // ダウンロード（絶対パスを使用）
            if (!paxg::Network::downloadFile(url_path, full_path)) {
                // ダウンロード失敗
                return nullptr;
            }

            // ファイル存在チェック（絶対パスを使用）
            if (!std::filesystem::exists(full_path)) {
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
