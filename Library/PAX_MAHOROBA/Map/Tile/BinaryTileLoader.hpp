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

#include <PAX_MAHOROBA/Map/Tile/ITileLoader.hpp>

#include <PAX_SAPIENTICA/AppConfig.hpp>
#include <PAX_SAPIENTICA/GeographicInformation/Slope.hpp>
#include <PAX_SAPIENTICA/Logger.hpp>
#include <PAX_SAPIENTICA/StringExtensions.hpp>

namespace paxs {

    /// @brief バイナリデータからPNGに変換してタイルを読み込むローダー
    /// @brief Loader for converting binary data to PNG and loading tiles
    class BinaryTileLoader {
    public:
        /// @brief タイルを読み込む（バイナリ→PNG変換）
        /// @brief Load a tile (binary to PNG conversion)
        /// @param binary_path_with_zy Z と Y が既に置換されたバイナリパス
        /// @param local_path_with_zy Z と Y が既に置換されたローカルパス
        /// @param folder_path_with_zyx Z, Y, X が既に置換されたフォルダパス
        /// @param x_value X 座標の文字列
        /// @param current_map_view_height 現在のマップビュー高さ
        /// @return 読み込みに成功した場合はテクスチャのunique_ptr、失敗した場合はnullptr
        static std::unique_ptr<paxg::Texture> load(
            const std::string& binary_path_with_zy,
            const std::string& local_path_with_zy,
            const std::string& folder_path_with_zyx,
            const std::string& x_value,
            double current_map_view_height
        ) {
            // バイナリパスと出力パスの X を置換
            std::string binary_path = binary_path_with_zy;
            paxs::StringExtensions::replace(binary_path, "{x}", x_value);

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
                RGBAa() : r(0), g(0), b(0), a(0) {}
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

            // 出力PNGパスの X を置換
            std::string local_file_path = local_path_with_zy;
            paxs::StringExtensions::replace(local_file_path, "{x}", x_value);

            // 保存先フォルダを作成
            std::filesystem::create_directories(folder_path_with_zyx);

            // PNGファイルとして保存
            stbi_write_png(local_file_path.c_str(), 256, 256, static_cast<int>(sizeof(RGBAa)), rgba, 0);

            // Note: current_map_view_heightはこの時点では使用されていない
            (void)current_map_view_height;

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

    private:
        // インスタンス化を禁止
        BinaryTileLoader() = delete;
        BinaryTileLoader(const BinaryTileLoader&) = delete;
        BinaryTileLoader& operator=(const BinaryTileLoader&) = delete;
    };

} // namespace paxs

#endif // !PAX_MAHOROBA_MAP_TILE_BINARY_TILE_LOADER_HPP
