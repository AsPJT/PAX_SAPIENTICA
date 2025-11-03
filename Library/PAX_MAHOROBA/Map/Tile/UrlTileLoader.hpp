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
#include <PAX_MAHOROBA/Map/Tile/ITileLoader.hpp>
#include <PAX_SAPIENTICA/Logger.hpp>
#include <PAX_SAPIENTICA/StringExtensions.hpp>

namespace paxs {

    /// @brief URLからタイルをダウンロードして読み込むローダー
    /// @brief Loader for downloading and loading tiles from URL
    class UrlTileLoader : public ITileLoader {
    public:
        /// @brief コンストラクタ
        /// @param texture_url テクスチャURL（例: "https://example.com/{z}/{x}/{y}.png"）
        /// @param file_name_format ローカル保存先ファイル名フォーマット
        /// @param map_name 地図名
        /// @param texture_full_path_folder フルパスフォルダ
        UrlTileLoader(
            const std::string& texture_url,
            const std::string& file_name_format,
            const std::string& map_name,
            const std::string& texture_full_path_folder
        ) : texture_url_(texture_url),
            file_name_format_(file_name_format),
            map_name_(map_name),
            texture_full_path_folder_(texture_full_path_folder) {}

        /// @brief タイルを読み込む（URLからダウンロード）
        std::unique_ptr<paxg::Texture> load(
            unsigned int z,
            unsigned int x,
            unsigned int y
        ) override {
            // URLパスを構築
            std::string url_path = buildUrlPath(z, x, y);

            // ローカル保存先パスを構築
            std::string local_path = buildLocalPath(z, x, y);

            // 保存先フォルダを作成
            createTextureFolder(std::to_string(x), std::to_string(y), std::to_string(z));

            // ダウンロード
            if (!paxg::Network::downloadFile(url_path, local_path)) {
                // ダウンロード失敗（静かに失敗）
                return nullptr;
            }

            // ファイル存在チェック
            if (!std::filesystem::exists(local_path)) {
                return nullptr;
            }

            // テクスチャを読み込み
            auto texture = std::make_unique<paxg::Texture>(
                paxs::StringExtensions::removeRelativePathPrefix(local_path)
            );

            // 読み込み失敗チェック
            if (!(*texture)) {
                PAXS_WARNING("UrlTileLoader: Failed to load texture from downloaded file: " + local_path);
                return nullptr;
            }

            return texture;
        }

        /// @brief ローダー名を取得
        std::string getLoaderName() const override {
            return "UrlTileLoader";
        }

    private:
        /// @brief URLパスを構築
        std::string buildUrlPath(unsigned int z, unsigned int x, unsigned int y) const {
            std::string path = texture_url_;
            paxs::StringExtensions::replace(path, "{z}", std::to_string(z));
            paxs::StringExtensions::replace(path, "{x}", std::to_string(x));
            paxs::StringExtensions::replace(path, "{y}", std::to_string(y));
            return path;
        }

        /// @brief ローカル保存先パスを構築
        std::string buildLocalPath(unsigned int z, unsigned int x, unsigned int y) const {
            std::string path = file_name_format_;
            paxs::StringExtensions::replace(path, "{z}", std::to_string(z));
            paxs::StringExtensions::replace(path, "{x}", std::to_string(x));
            paxs::StringExtensions::replace(path, "{y}", std::to_string(y));
            if (!map_name_.empty()) {
                paxs::StringExtensions::replace(path, "{n}", map_name_);
            }
            return path;
        }

        /// @brief テクスチャ保存フォルダを作成
        void createTextureFolder(const std::string& x_value, const std::string& y_value, const std::string& z_value) const {
            std::string new_folder_path = texture_full_path_folder_;
            paxs::StringExtensions::replace(new_folder_path, "{x}", x_value);
            paxs::StringExtensions::replace(new_folder_path, "{y}", y_value);
            paxs::StringExtensions::replace(new_folder_path, "{z}", z_value);
            if (!map_name_.empty()) {
                paxs::StringExtensions::replace(new_folder_path, "{n}", map_name_);
            }
            std::filesystem::create_directories(new_folder_path);
        }

        std::string texture_url_;
        std::string file_name_format_;
        std::string map_name_;
        std::string texture_full_path_folder_;
    };

} // namespace paxs

#endif // !PAX_MAHOROBA_MAP_TILE_URL_TILE_LOADER_HPP
