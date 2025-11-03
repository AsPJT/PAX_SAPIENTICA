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
#include <PAX_MAHOROBA/Map/Tile/ITileLoader.hpp>
#include <PAX_SAPIENTICA/StringExtensions.hpp>

namespace paxs {

    /// @brief ローカルファイルからタイルを読み込むローダー
    /// @brief Loader for loading tiles from local files
    class FileTileLoader : public ITileLoader {
    public:
        /// @brief コンストラクタ
        /// @param file_name_format ファイル名フォーマット（例: "{z}/{x}/{y}"）
        /// @param map_name 地図名
        FileTileLoader(
            const std::string& file_name_format,
            const std::string& map_name
        ) : file_name_format_(file_name_format),
            map_name_(map_name) {}

        /// @brief タイルを読み込む
        std::unique_ptr<paxg::Texture> load(
            unsigned int z,
            unsigned int x,
            unsigned int y
        ) override {
            // パスを構築
            std::string path = buildPath(z, x, y);

#if defined(PAXS_USING_DXLIB) && defined(__ANDROID__)
            // Android DxLib: ファイル存在チェックをスキップ
            // （アセットファイルは std::filesystem::exists で検出できないため）
#else
            // ファイル存在チェック（重要: 存在しない場合は静かに失敗）
            if (!std::filesystem::exists(path)) {
                return nullptr;
            }
#endif

            // テクスチャを読み込み
            auto texture = std::make_unique<paxg::Texture>(
                paxs::StringExtensions::removeRelativePathPrefix(path)
            );

            // 読み込み失敗チェック
            if (!(*texture)) {
                return nullptr;
            }

            return texture;
        }

        /// @brief ローダー名を取得
        std::string getLoaderName() const override {
            return "FileTileLoader";
        }

    private:
        /// @brief パスを構築
        std::string buildPath(unsigned int z, unsigned int x, unsigned int y) const {
            std::string path = file_name_format_;
            paxs::StringExtensions::replace(path, "{z}", std::to_string(z));
            paxs::StringExtensions::replace(path, "{x}", std::to_string(x));
            paxs::StringExtensions::replace(path, "{y}", std::to_string(y));
            if (!map_name_.empty()) {
                paxs::StringExtensions::replace(path, "{n}", map_name_);
            }
            return path;
        }

        std::string file_name_format_;
        std::string map_name_;
    };

} // namespace paxs

#endif // !PAX_MAHOROBA_MAP_TILE_FILE_TILE_LOADER_HPP
