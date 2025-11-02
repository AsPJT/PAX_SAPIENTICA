/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]      2023-2024 As Project
    [Production]    2023-2024 As Project
    [Contact Us]    wanotaitei@gmail.com            https://github.com/AsPJT/PAX_SAPIENTICA
    [License]       Distributed under the CC0 1.0. https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_MAP_TILE_BINARY_TILE_LOADER_HPP
#define PAX_MAHOROBA_MAP_TILE_BINARY_TILE_LOADER_HPP

#include <cstddef>
#include <filesystem>
#include <memory>
#include <string>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

#include <PAX_GRAPHICA/Texture.hpp>
#include <PAX_MAHOROBA/Core/Init.hpp>
#include <PAX_MAHOROBA/Map/Tile/ITileLoader.hpp>
#include <PAX_SAPIENTICA/AppConfig.hpp>
#include <PAX_SAPIENTICA/GeographicInformation/Slope.hpp>
#include <PAX_SAPIENTICA/Logger.hpp>
#include <PAX_SAPIENTICA/StringExtensions.hpp>

//#ifdef USING_BINARY_TEXTURE

//#endif

namespace paxs {

    /// @brief バイナリデータからPNGに変換してタイルを読み込むローダー
    /// @brief Loader for converting binary data to PNG and loading tiles
    class BinaryTileLoader : public ITileLoader {
    public:
        /// @brief コンストラクタ
        /// @param binary_file_name_format バイナリファイル名フォーマット
        /// @param file_name_format 出力PNG保存先フォーマット
        /// @param map_name 地図名
        /// @param texture_full_path_folder フルパスフォルダ
        /// @param current_map_view_height_ref 現在のマップビュー高さへの参照（更新用）
        BinaryTileLoader(
            const std::string& binary_file_name_format,
            const std::string& file_name_format,
            const std::string& map_name,
            const std::string& texture_full_path_folder,
            double& current_map_view_height_ref
        ) : binary_file_name_format_(binary_file_name_format),
            file_name_format_(file_name_format),
            map_name_(map_name),
            texture_full_path_folder_(texture_full_path_folder),
            current_map_view_height_ref_(current_map_view_height_ref) {}

        /// @brief タイルを読み込む（バイナリ→PNG変換）
        std::unique_ptr<paxg::Texture> load(
            unsigned int z,
            unsigned int x,
            unsigned int y
        ) override {
            // バイナリファイルパスを構築
            std::string binary_path = buildBinaryPath(z, x, y);

            // バイナリファイルを読み込み
            paxs::Input8BitBinary i8bbs(binary_path,
                AppConfig::getInstance()->getRootPath()
            );

            static unsigned char xyz_tiles[256 * 256]{};

            // バイナリデータが読み込めない場合（静かに失敗）
            if (!i8bbs.calc(xyz_tiles)) {
                return nullptr;
            }

            // RGBA構造体
            struct RGBAa {
                unsigned char r, g, b, a; //赤, 緑, 青, 透過
                RGBAa() = default;
                constexpr RGBAa(const unsigned char r_, const unsigned char g_, const unsigned char b_, const unsigned char a_)
                    : r(r_), g(g_), b(b_), a(a_) {}
            };
            RGBAa rgba[256][256]{};

            // バイナリデータをRGBAに変換
            for (std::size_t row{}; row < 256; ++row) {
                for (std::size_t col{}; col < 256; ++col) {
                    const unsigned char color = xyz_tiles[row * 256 + col];
                    if (color >= 251 || color == 0) {
                        rgba[row][col].r = 0;
                        rgba[row][col].g = 0;
                        rgba[row][col].b = 0;
                        rgba[row][col].a = 0; //透過
                    }
                    else {
                        if (color >= 181) { // 25.64100582
                            rgba[row][col].r = static_cast<unsigned char>(180 - 15.0 * (256.0 - color) / (256.0 - 181.0));
                            rgba[row][col].g = static_cast<unsigned char>(220 - 10.0 * (256.0 - color) / (256.0 - 181.0));
                            rgba[row][col].b = static_cast<unsigned char>(185 - 15.0 * (256.0 - color) / (256.0 - 181.0));
                        }
                        else if (color >= 127) { // 9.090276921
                            rgba[row][col].r = static_cast<unsigned char>(200 - 30.0 * (181.0 - color) / (181.0 - 127.0));
                            rgba[row][col].g = static_cast<unsigned char>(235 - 20.0 * (181.0 - color) / (181.0 - 127.0));
                            rgba[row][col].b = static_cast<unsigned char>(210 - 30.0 * (181.0 - color) / (181.0 - 127.0));
                        }
                        else {
                            rgba[row][col].r = static_cast<unsigned char>(235 - 40.0 * color / 127.0);
                            rgba[row][col].g = static_cast<unsigned char>(235);
                            rgba[row][col].b = static_cast<unsigned char>(240 - 40.0 * color / 127.0);
                        }

                        rgba[row][col].a = 255; //不透過
                    }
                }
            }

            // 出力PNGパスを構築
            std::string local_file_path = buildLocalPath(z, x, y);

            // 保存先フォルダを作成
            createTextureFolder(std::to_string(x), std::to_string(y), std::to_string(z));

            // PNGファイルとして保存
            stbi_write_png(local_file_path.c_str(), 256, 256, static_cast<int>(sizeof(RGBAa)), rgba, 0);

            // current_map_view_heightを更新（元のロジックを保持）
            current_map_view_height_ref_ = 11111;

            // ファイル存在チェック
            if (!std::filesystem::exists(local_file_path)) {
                return nullptr;
            }

            // テクスチャを読み込み
            auto texture = std::make_unique<paxg::Texture>(
                paxs::StringExtensions::removeRelativePathPrefix(local_file_path)
            );

            // 読み込み失敗チェック
            if (!(*texture)) {
                PAXS_WARNING("BinaryTileLoader: Failed to load texture from binary generated file: " + local_file_path);
                return nullptr;
            }

            return texture;
        }

        /// @brief ローダー名を取得
        std::string getLoaderName() const override {
            return "BinaryTileLoader";
        }

    private:
        /// @brief バイナリファイルパスを構築
        std::string buildBinaryPath(unsigned int z, unsigned int x, unsigned int y) const {
            std::string path = binary_file_name_format_;
            paxs::StringExtensions::replace(path, "{z}", std::to_string(z));
            paxs::StringExtensions::replace(path, "{x}", std::to_string(x));
            paxs::StringExtensions::replace(path, "{y}", std::to_string(y));
            if (!map_name_.empty()) {
                paxs::StringExtensions::replace(path, "{n}", map_name_);
            }
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

        std::string binary_file_name_format_;
        std::string file_name_format_;
        std::string map_name_;
        std::string texture_full_path_folder_;
        double& current_map_view_height_ref_;
    };

} // namespace paxs

#endif // !PAX_MAHOROBA_MAP_TILE_BINARY_TILE_LOADER_HPP
