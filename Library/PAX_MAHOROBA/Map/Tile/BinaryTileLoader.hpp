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
#include <vector>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

#include <PAX_GRAPHICA/Texture.hpp>

#include <PAX_SAPIENTICA/AppConfig.hpp>
#include <PAX_SAPIENTICA/GeographicInformation/Slope.hpp>
#include <PAX_SAPIENTICA/Logger.hpp>
#include <PAX_SAPIENTICA/StringExtensions.hpp>

namespace paxs {

    /// @brief RGBA構造体 (BinaryTileLoader で使用)
    struct TileRGBA {
        unsigned char r, g, b, a; //赤, 緑, 青, 透過
        TileRGBA() : r(0), g(0), b(0), a(0) {}
        constexpr TileRGBA(const unsigned char r_, const unsigned char g_, const unsigned char b_, const unsigned char a_)
            : r(r_), g(g_), b(b_), a(a_) {}
    };

    /// @brief バイナリデータからPNGに変換してタイルを読み込むローダー
    /// @brief Loader for converting binary data to PNG and loading tiles
    class BinaryTileLoader {
    public:
        /// @brief タイルを読み込む（バイナリ→PNG変換）
        /// @brief Load a tile (binary to PNG conversion)
        /// @param binary_path_with_zy Z と Y が既に置換されたバイナリパス（アセットルートからの相対パス）
        /// @param local_path_with_zy Z と Y が既に置換されたローカルパス（アセットルートからの相対パス）
        /// @param folder_path_with_zyx Z, Y, X が既に置換されたフォルダパス（アセットルートからの相対パス）
        /// @param x_value X 座標の文字列
        /// @param current_map_view_height 現在のマップビュー高さ
        /// @param binary_buffer [in/out] バイナリデータ読み込み用の再利用バッファ (サイズ tile_pixels)
        /// @param rgba_buffer [in/out] RGBA変換用の再利用バッファ (サイズ tile_pixels)
        /// @return 読み込みに成功した場合はテクスチャのunique_ptr、失敗した場合はnullptr
        static std::unique_ptr<paxg::Texture> load(
            const std::string& binary_path_with_zy,
            const std::string& local_path_with_zy,
            const std::string& folder_path_with_zyx,
            const std::string& x_value,
            std::vector<unsigned char>& binary_buffer, // 引数で受け取る
            std::vector<TileRGBA>& rgba_buffer         // 引数で受け取る
        ) {
            // バイナリ相対パスの X を置換
            std::string binary_relative_path = binary_path_with_zy;
            paxs::StringExtensions::replace(binary_relative_path, "{x}", x_value);

            // バイナリファイルを読み込み（Input8BitBinaryは内部でrootpathを前置）
            paxs::Input8BitBinary i8bbs(binary_relative_path,
                AppConfig::getInstance()->getRootPath()
            );

            // スタックオーバーフロー回避のためヒープに確保
            constexpr std::size_t tile_size = 256;
            constexpr std::size_t tile_pixels = tile_size * tile_size;
            // バッファサイズチェック (呼び出し元で tile_pixels に初期化されている想定)
            if (binary_buffer.size() < tile_pixels) {
                // サイズが異なる場合、リサイズと初期化を行う
                binary_buffer.assign(tile_pixels, 0);
            }
            if (rgba_buffer.size() < tile_pixels) {
                // サイズが異なる場合、リサイズを行う
                rgba_buffer.resize(tile_pixels);
            }


            // バイナリデータが読み込めない場合（静かに失敗）
            if (!i8bbs.calc(binary_buffer.data())) { // 引数のバッファを使用
                return nullptr;
            }

            // バイナリデータをRGBAに変換
            for (std::size_t row{}; row < tile_size; ++row) {
                for (std::size_t col{}; col < tile_size; ++col) {
                    const std::size_t index = row * tile_size + col;
                    const unsigned char color = binary_buffer[index]; // 引数のバッファを使用
                    TileRGBA& pixel = rgba_buffer[index];           // 引数のバッファを使用

                    if (color >= 251 || color == 0) {
                        pixel.r = 0;
                        pixel.g = 0;
                        pixel.b = 0;
                        pixel.a = 0; //透過
                    }
                    else {
                        if (color >= 181) { // 25.64100582
                            pixel.r = static_cast<unsigned char>(180 - 15.0 * (256.0 - color) / (256.0 - 181.0));
                            pixel.g = static_cast<unsigned char>(220 - 10.0 * (256.0 - color) / (256.0 - 181.0));
                            pixel.b = static_cast<unsigned char>(185 - 15.0 * (256.0 - color) / (256.0 - 181.0));
                        }
                        else if (color >= 127) { // 9.090276921
                            pixel.r = static_cast<unsigned char>(200 - 30.0 * (181.0 - color) / (181.0 - 127.0));
                            pixel.g = static_cast<unsigned char>(235 - 20.0 * (181.0 - color) / (181.0 - 127.0));
                            pixel.b = static_cast<unsigned char>(210 - 30.0 * (181.0 - color) / (181.0 - 127.0));
                        }
                        else {
                            pixel.r = static_cast<unsigned char>(235 - 40.0 * color / 127.0);
                            pixel.g = static_cast<unsigned char>(235);
                            pixel.b = static_cast<unsigned char>(240 - 40.0 * color / 127.0);
                        }

                        pixel.a = 255; //不透過
                    }
                }
            }

            // 出力PNG相対パスの X を置換
            std::string relative_path = local_path_with_zy;
            paxs::StringExtensions::replace(relative_path, "{x}", x_value);

            // アセットルートパスを前置して絶対パスを構築
            const std::string root_path = AppConfig::getInstance()->getRootPath();
            const std::string full_path = root_path + relative_path;
            const std::string full_folder_path = root_path + folder_path_with_zyx;

            // 保存先フォルダを作成（絶対パスを使用）
            std::filesystem::create_directories(full_folder_path);

            // PNGファイルとして保存（絶対パスを使用）
            stbi_write_png(full_path.c_str(), tile_size, tile_size, static_cast<int>(sizeof(TileRGBA)), rgba_buffer.data(), 0); // 引数のバッファを使用

            // ファイル存在チェック（絶対パスを使用）
            if (!std::filesystem::exists(full_path)) {
                return nullptr;
            }

            // テクスチャを読み込み（相対パスを使用）
            auto texture = std::make_unique<paxg::Texture>(relative_path);

            // 読み込み失敗チェック
            if (!(*texture)) {
                PAXS_WARNING("BinaryTileLoader: Failed to load texture from binary generated file: " + relative_path);
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
