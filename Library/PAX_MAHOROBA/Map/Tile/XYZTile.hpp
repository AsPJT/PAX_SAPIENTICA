/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_XYZ_TILE_HPP
#define PAX_MAHOROBA_XYZ_TILE_HPP

#include <cmath>
#include <filesystem>
#include <string>
#include <utility>

#include <PAX_GRAPHICA/Network.hpp>
#include <PAX_GRAPHICA/Texture.hpp>
#include <PAX_GRAPHICA/Vec2.hpp>

#include <PAX_MAHOROBA/Core/Init.hpp>

#include <PAX_SAPIENTICA/AppConfig.hpp>
#include <PAX_SAPIENTICA/MurMur3.hpp>
#include <PAX_SAPIENTICA/StringExtensions.hpp>
#include <PAX_SAPIENTICA/UnorderedMap.hpp>
#include <PAX_SAPIENTICA/Type/Vector2.hpp>

// マクロ分岐の可能性があるため別の include とは別で記載
//#ifdef USING_BINARY_TEXTURE
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

#include <cstddef>
#include <memory>
#include <new>

#include <PAX_SAPIENTICA/GeographicInformation/Slope.hpp>

//#endif

namespace paxs {

    class XYZTile {
    private:
        // XYZ タイルの 1 つのセルのメルカトル座標を保持
        // 基本的に Z = 19 は無い
        using MapVec2 = Vector2<int>;
        using MapVec2D = Vector2<double>;

        // XYZ タイルの画像の情報を保持
        paxs::UnorderedMap<std::uint_least64_t, paxg::Texture> texture_list{};
        paxs::UnorderedMap<std::uint_least64_t, unsigned char> is_texture_list{}; // テクスチャが読み込まれているか

        std::string texture_url = ""; // URL
        std::string binary_file_name_format = ""; // バイナリデータ
        std::string map_name = ""; // 地図名
        std::string map_file_path_name = ""; // パス名 例） "Data/Map/XYZTile/Standard/Image/Land/2023/"
        std::string file_name_format = ("{z}/{x}/{y}");
        std::string texture_full_path_folder = ""; // フルパスのフォルダまでのパスを返す

        //const std::string* p_root_path = nullptr; // 初めのパス名 例） root_path

        // 1フレーム前のマップの高さ
        double current_map_view_height = -1.0;

        // XYZ タイルの画面上の始点セル
        MapVec2 start_cell{};
        // XYZ タイルの画面上の終点セル
        MapVec2 end_cell{};

        // 99999999 の場合は固定なし
        int min_date = 99999999;
        int max_date = 99999999;

        unsigned int default_z = 999; // 固定された Z （ 999 の場合は固定なし ）
        unsigned int min_z = 0; // 最小 Z
        unsigned int max_z = 25; // 最大 Z
        unsigned int z = 2; // 画面の幅に最適な XYZ タイルの Z を格納
        unsigned int magnification_z = z; // 画面上の Z の値
        unsigned int draw_min_z = 0; // 描画最小 Z
        unsigned int draw_max_z = 999; // 描画最大 Z
        unsigned int z_num = (1 << z); // 2 の z 乗 // std::pow(2, z) と等価

        std::uint_least32_t draw_type = paxs::MurMur3::calcHash("texture");

        std::uint_least32_t texture_root_path_type = paxs::MurMur3::calcHash("asset_file");
        std::uint_least32_t binary_root_path_type = paxs::MurMur3::calcHash("asset_file");

        std::uint_least32_t menu_bar_map = 0;
        bool menu_bar_map_bool = true;

    private:
        // テクスチャ UnorderedMap の添え字を XYZ から生成
        constexpr std::uint_least64_t textureIndex(std::uint_least64_t z_, std::uint_least64_t y_, std::uint_least64_t x_) const {
            return (z_ << 48) + (y_ << 24) + (x_);
        }
        constexpr std::uint_least64_t textureIndexZY(std::uint_least64_t z_, std::uint_least64_t y_) const {
            return (z_ << 48) + (y_ << 24);
        }
        constexpr std::uint_least64_t textureIndexZ(std::uint_least64_t z_) const {
            return (z_ << 48);
        }
        constexpr std::uint_least64_t textureIndexY(std::uint_least64_t y_) const {
            return (y_ << 24);
        }

        // Ｘ、Ｙ、Ｚの３つの文字列に変換された数値を入れ、画像を保存するフォルダを作成する
        void createTextureFolder(const std::string& x_value, const std::string& y_value, const std::string& z_value) const {
            std::string new_folder_path = texture_full_path_folder;
            paxs::StringExtensions::replace(new_folder_path, "{x}", x_value);
            paxs::StringExtensions::replace(new_folder_path, "{y}", y_value);
            paxs::StringExtensions::replace(new_folder_path, "{z}", z_value);
            if (map_name.size() != 0) paxs::StringExtensions::replace(new_folder_path, "{n}", map_name);
            std::filesystem::create_directories(new_folder_path);
        }

        // フルパスのフォルダまでのパスを返す
        std::string setFullPathFolder() const {
            std::size_t slash_index = 99999999;
            for (std::size_t i = 0; i < file_name_format.size(); ++i) {
                if (file_name_format[i] == '/') {
                    slash_index = i;
                }
            }
            // スラッシュがない場合は空文字を返す
            if (slash_index == 99999999) {
                return "";
            }
            // スラッシュがある場合は最後のスラッシュまでのフォルダパスを返す
            std::string str = file_name_format;
            str[slash_index] = 0; // 終端文字を入れる
            return std::string(str.c_str());
        }

    public:
        XYZTile() = default;

        // タイルを更新
        void update(const double map_view_width, // 描画される地図の経度幅
            const double map_view_height, // 描画される地図の緯度幅
            const double map_view_center_x, // 描画される地図の中心経度
            const double map_view_center_y // 描画される地図の中心緯度
        ) {
            // 拡大率が変わった場合、拡大率にあわせて取得する地図の大きさを変える
            if (current_map_view_height != map_view_height) {
                magnification_z = int(-std::log2(map_view_height) + 12.5);
                if (default_z == 999) {
                    z = magnification_z;
                    if (z < min_z) z = min_z;
                    else if (z > max_z) z = max_z;
                }
                else {
                    z = default_z;
                }
                z_num = (1 << z); // std::pow(2, z) と等価
                current_map_view_height = map_view_height;
            }
            // 拡大率が描画範囲外の場合はここで処理を終了
            if (magnification_z < draw_min_z) return;
            if (magnification_z > draw_max_z) return;

            // 画像を更新する必要があるか
            bool need_update = false;

            const MapVec2 new_start_cell{
                int((((map_view_center_x - map_view_width / 2) + 180.0) / 360.0) * z_num + z_num) - static_cast<int>(z_num),
                int(((360.0 - ((map_view_center_y + map_view_height / 2) + 180.0)) / 360.0) * z_num + z_num) - static_cast<int>(z_num) };
            if (new_start_cell != start_cell) {
                start_cell = new_start_cell;
                need_update = true;
            }
            const MapVec2 new_end_cell = MapVec2{
                int((((map_view_center_x + map_view_width / 2) + 180.0) / 360.0) * z_num + z_num) - static_cast<int>(z_num),
                int(((360.0 - ((map_view_center_y - map_view_height / 2) + 180.0)) / 360.0) * z_num + z_num) - static_cast<int>(z_num) };
            if (new_end_cell != end_cell) {
                end_cell = new_end_cell;
                need_update = true;
            }

            if (!need_update) return; // 地図が前回と同じ場所の場合は更新処理をしない
            // もしタイルが更新されていたら更新処理

            if (map_name.size() == 0) return;

            const std::string z_value = std::to_string(z); // Z の値
            std::string local_file_path_zn = file_name_format;
            paxs::StringExtensions::replace(local_file_path_zn, "{z}", z_value);
            if (map_name.size() != 0) paxs::StringExtensions::replace(local_file_path_zn, "{n}", map_name);

            const std::uint_least64_t index_z = textureIndexZ(static_cast<std::uint_least64_t>(z));
            // ファイルを保存
            for (int i = start_cell.y, k = 0; i <= end_cell.y; ++i) {

                const std::string y_value = std::to_string((i + z_num) & (z_num - 1)); // Y の値
                std::string local_file_path_zny = local_file_path_zn;
                paxs::StringExtensions::replace(local_file_path_zny, "{y}", y_value);

                const std::uint_least64_t index_zy = textureIndexY(static_cast<std::uint_least64_t>((i + z_num) & (z_num - 1))) + index_z;
                for (int j = start_cell.x; j <= end_cell.x; ++j, ++k) {

                    // 画像を格納する index を生成
                    const std::uint_least64_t index_zyx = static_cast<std::uint_least64_t>((j + z_num) & (z_num - 1)) + index_zy;

                    // 画像が 1 度でも読み込みに試行した場合は終了
                    if (is_texture_list.find(index_zyx) != is_texture_list.end()) {
                        continue;
                    }

                    const std::string x_value = std::to_string((j + z_num) & (z_num - 1)); // X の値

                    std::string local_file_path = local_file_path_zny;
                    paxs::StringExtensions::replace(local_file_path, "{x}", x_value);

#if defined(PAXS_USING_DXLIB) && defined(__ANDROID__)
                    // 何もしない
#else
                    // ファイル読み込みができるかどうか
                    if (std::filesystem::exists(local_file_path)) {
#endif
                        // 新しいテクスチャ
                        paxg::Texture new_tex(paxs::StringExtensions::removeRelativePathPrefix(local_file_path));

                        // テクスチャが読み込めた場合
                        if (!!new_tex) {
                            texture_list.insert({ index_zyx, std::move(new_tex) });
                            is_texture_list.insert({ index_zyx, 0 }); // 読み込み成功
                            continue;
                        }
#if defined(PAXS_USING_DXLIB) && defined(__ANDROID__)
                        // 何もしない
#else
                    }
#endif
                    // 新しいテクスチャが読み込めなかった場合
                    // URL の記載がある場合
                    if (texture_url.size() != 0) {
                        std::string new_path = texture_url;
                        paxs::StringExtensions::replace(new_path, "{x}", x_value);
                        paxs::StringExtensions::replace(new_path, "{y}", y_value);
                        paxs::StringExtensions::replace(new_path, "{z}", z_value);

                        createTextureFolder(x_value, y_value, z_value); // 画像保存用のフォルダを作成
                        if (paxg::Network::downloadFile(new_path, local_file_path)) {

                            // ファイル読み込みができるかどうか
                            if (std::filesystem::exists(local_file_path)) {
                                paxg::Texture new_url_tex{ paxs::StringExtensions::removeRelativePathPrefix(local_file_path) };
                                if (!new_url_tex) {
                                    is_texture_list.insert({ index_zyx, 1 }); // 読み込み失敗
                                }
                                else {
                                    // URL から取得した新しい地図へ更新
                                    texture_list.insert({ index_zyx, std::move(new_url_tex) });
                                    is_texture_list.insert({ index_zyx, 0 }); // 読み込み成功
                                }
                            }
                        }
                    }
                    else if (binary_file_name_format.size() != 0 && // 画像の元になるバイナリデータがある場合
                             texture_full_path_folder.size() != 0) { // フルパスがある場合
                        std::string new_path = binary_file_name_format;
                        paxs::StringExtensions::replace(new_path, "{x}", x_value);
                        paxs::StringExtensions::replace(new_path, "{y}", y_value);
                            paxs::StringExtensions::replace(new_path, "{z}", z_value);
                            if (map_name.size() != 0) paxs::StringExtensions::replace(new_path, "{n}", map_name);

                            paxs::Input8BitBinary i8bbs(new_path,
                                AppConfig::getInstance()->getRootPath()
                            );

                            static unsigned char xyz_tiles[256 * 256]{};
                            // テクスチャが読み込めない場合
                            if (!i8bbs.calc(xyz_tiles)) {
                                is_texture_list.insert({ index_zyx, 1 }); // 読み込み失敗
                                continue;
                            }
                            struct RGBAa {
                                unsigned char r, g, b, a; //赤, 緑, 青, 透過
                                RGBAa() = default;
                                constexpr RGBAa(const unsigned char r_, const unsigned char g_, const unsigned char b_, const unsigned char a_) :r(r_), g(g_), b(b_), a(a_) {}
                            };
                            RGBAa rgba[256][256]{};
                            // 読み込んで画像として保存
                            for (std::size_t row{}; row < 256; ++row)
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
                            createTextureFolder(x_value, y_value, z_value); // 画像保存用のフォルダを作成
                            stbi_write_png(local_file_path.c_str(), 256, 256, static_cast<int>(sizeof(RGBAa)), rgba, 0);
                            current_map_view_height = 11111;

                            // ファイル読み込みができるかどうか
                            if (std::filesystem::exists(local_file_path)) {
                                // 画像として読み込み

                                // 新しいテクスチャ
                                paxg::Texture bin_tex(paxs::StringExtensions::removeRelativePathPrefix(local_file_path));
                                if (!bin_tex) {
                                    is_texture_list.insert({ index_zyx, 1 }); // 読み込み失敗
                                }
                                else {
                                    texture_list.insert({ index_zyx, std::move(bin_tex) });
                                    is_texture_list.insert({ index_zyx, 0 }); // 読み込み成功
                                }
                            }
                        }
                        else
                        {
                            is_texture_list.insert({ index_zyx, 1 }); // 読み込み失敗
                        }
                } // for (j)
            } // for (i)
        }

        XYZTile(
            const std::uint_least32_t menu_bar_map_,
            const bool menu_bar_map_bool_,
            const std::uint_least32_t texture_root_path_type_,
            const std::uint_least32_t binary_root_path_type_,
            //const std::string* p_root_path_, // 例 "./../../"
            const std::string& map_binary_name_,
            const std::string& map_file_path_name_,
            const std::string& file_name_format_,
            const std::uint_least32_t draw_type_,
            const std::string& texture_url_
        )
            : texture_root_path_type(texture_root_path_type_), binary_root_path_type(binary_root_path_type_)
            , menu_bar_map(menu_bar_map_)
            , menu_bar_map_bool(menu_bar_map_bool_)
            , draw_type(draw_type_)
            //, p_root_path(p_root_path_) // ルートパスを指定するための相対パス 例） "./../../"
            , map_file_path_name(map_file_path_name_)
            , texture_url(texture_url_)
        {
            // バイナリデータを定義
            if (map_binary_name_.size() != 0 && texture_root_path_type_ != 0 &&
                file_name_format_.size() != 0) {
                binary_file_name_format =
                    map_binary_name_ + file_name_format_ + std::string(".bin");
            }

            // フォーマットが指定されている場合
            if (file_name_format_.size() != 0) {
                // 地図のテクスチャのファイルパスを保存
#if defined(PAXS_USING_DXLIB) && defined(__ANDROID__)
                static char internal_data_path[1024]{};

                switch (texture_root_path_type_) {
                case paxs::MurMur3::calcHash("asset_file"):
                    file_name_format = map_file_path_name_ + file_name_format_ + std::string(".png");
                    break;
                case paxs::MurMur3::calcHash("internal_file"):
                    DxLib::GetInternalDataPath(internal_data_path, 1024);
                    file_name_format = std::string(internal_data_path) + "/" +
                        map_file_path_name_ + file_name_format_ + std::string(".png");
                    break;
                case paxs::MurMur3::calcHash("external_file"):
                    DxLib::GetExternalDataPath(internal_data_path, 1024);
                    file_name_format = std::string(internal_data_path) + "/" +
                        map_file_path_name_ + file_name_format_ + std::string(".png");
                    break;
                default:
                    file_name_format = map_file_path_name_ + file_name_format_ + std::string(".png");
                    printfDx("\n[error:%u,%s,%s,%s]", texture_root_path_type_, map_binary_name_.c_str(), map_file_path_name_.c_str(), file_name_format_.c_str());
                    break;
                }
#else
                file_name_format = AppConfig::getInstance()->getRootPath() + map_file_path_name_ + file_name_format_ + std::string(".png");
#endif
                //if (file_name_format.size() != 0) printfDx("2. %s\n", file_name_format.c_str());
            }
            if (file_name_format.size() != 0) {
                texture_full_path_folder = setFullPathFolder();
                //if (texture_full_path_folder.size() != 0) printfDx("3. %s\n", texture_full_path_folder.c_str());
            }
        }

        MapVec2 getStartCell()const {
            return start_cell;
        }
        MapVec2 getEndCell()const {
            return end_cell;
        }
        unsigned int getDefaultZ()const {
            return default_z;
        }
        unsigned int getZ()const {
            return z;
        }
        unsigned int getZNum()const {
            return z_num;
        }
        std::uint_least32_t getDrawType() const {
            return draw_type;
        }
        bool getMenuBarMapBool() const {
            return menu_bar_map_bool;
        }
        std::uint_least32_t getMenuBarMap() const {
            return menu_bar_map;
        }
        unsigned int getMagnificationZ() const {
            return magnification_z;
        }
        unsigned int getDrawMinZ() const {
            return draw_min_z;
        }
        unsigned int getDrawMaxZ() const {
            return draw_max_z;
        }
        int getMinDate() const {
            return min_date;
        }
        int getMaxDate() const {
            return max_date;
        }
        /// @brief 指定座標のテクスチャを取得
        const paxg::Texture* getTextureAt(unsigned int z_, unsigned int y_, unsigned int x_) const {
            const std::uint_least64_t index = textureIndex(z_, y_, x_);
            if (texture_list.find(index) != texture_list.end()) {
                return &texture_list.at(index);
            }
            return nullptr;
        }
        void setDefaultZ(const unsigned int default_z_) {
            default_z = default_z_;
        }
        void setMinZ(const unsigned int min_z_) {
            min_z = min_z_;
        }
        void setMaxZ(const unsigned int max_z_) {
            max_z = max_z_;
        }
        void setDrawMinZ(const unsigned int min_z_) {
            draw_min_z = min_z_;
        }
        void setDrawMaxZ(const unsigned int max_z_) {
            draw_max_z = max_z_;
        }
        void setMinDate(const int min_date_) {
            min_date = min_date_;
        }
        void setMaxDate(const int max_date_) {
            max_date = max_date_;
        }
        void setMapURL(const std::string& texture_url_) {
            texture_url = texture_url_;
        }
        void setMapName(const std::string& map_name_) {
            map_name = map_name_;
        }
    };
}

#endif // !PAX_MAHOROBA_XYZ_TILES_HPP
