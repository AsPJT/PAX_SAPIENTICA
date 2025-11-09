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
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <PAX_MAHOROBA/Map/Tile/BinaryTileLoader.hpp>
#include <PAX_MAHOROBA/Map/Tile/FileTileLoader.hpp>
#include <PAX_MAHOROBA/Map/Tile/UrlTileLoader.hpp>

#include <PAX_SAPIENTICA/AppConfig.hpp>
#include <PAX_SAPIENTICA/Map/TileCache.hpp>
#include <PAX_SAPIENTICA/Map/TileCoordinate.hpp>
#include <PAX_SAPIENTICA/Map/TileMetadata.hpp>
#include <PAX_SAPIENTICA/MurMur3.hpp>
#include <PAX_SAPIENTICA/StringExtensions.hpp>
#include <PAX_SAPIENTICA/Type/Vector2.hpp>

namespace paxs {

    class XYZTile {
    private:
        // 浮動小数点比較の許容誤差
        static constexpr double VIEWPORT_EPSILON = 1e-9;

        // XYZ タイルの 1 つのセルのメルカトル座標を保持
        // 基本的に Z = 19 は無い

        paxs::TileCache<paxg::Texture> tile_cache_;

        // XYZ タイルの設定情報
        paxs::TileMetadata metadata_;

        // XYZ タイルの画像の情報を保持
        std::string binary_file_name_format = ""; // バイナリデータ
        std::string map_name = ""; // 地図名
        std::string file_name_format = ("{z}/{x}/{y}");
        std::string texture_full_path_folder = ""; // フルパスのフォルダまでのパスを返す

        // 1フレーム前のズームレベル（整数キャッシュ）
        unsigned int cached_magnification_z = 0;
        unsigned int cached_z = 0;

        // 1フレーム前のビューポート座標（タイル範囲の再計算判定用）
        double cached_map_view_width = -1.0;
        double cached_map_view_center_x = 0.0;
        double cached_map_view_center_y = 0.0;

        // XYZ タイルの画面上の始点セル
        Vector2<int> start_cell{};
        // XYZ タイルの画面上の終点セル
        Vector2<int> end_cell{};

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

    private:
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

        /// @brief ズームレベルを更新
        void updateZoomLevel(const double map_view_height) {
            magnification_z = TileCoordinate::calculateZoomLevel(map_view_height);

            if (default_z == 999) {
                z = magnification_z;
                if (z < min_z) z = min_z;
                else if (z > max_z) z = max_z;
            }
            else {
                z = default_z;
            }
            z_num = (1 << z); // std::pow(2, z) と等価

            // 整数ズームレベルをキャッシュ
            cached_magnification_z = magnification_z;
            cached_z = z;
        }

        /// @brief 描画範囲内かどうかをチェック
        bool isInDrawRange() const {
            return magnification_z >= draw_min_z && magnification_z <= draw_max_z;
        }

        /// @brief 浮動小数点数が異なるかどうかを判定（許容誤差考慮）
        /// @param a 値1
        /// @param b 値2
        /// @return 異なる場合true
        static bool isDifferent(double a, double b) {
            return std::abs(a - b) >= VIEWPORT_EPSILON;
        }

        /// @brief タイル範囲を更新
        /// @param zoom_changed ズームレベルが変更されたかどうか
        /// @return タイル範囲が変更された場合true
        bool updateTileRange(
            const double map_view_width,
            const double map_view_height,
            const double map_view_center_x,
            const double map_view_center_y,
            const bool zoom_changed
        ) {
            // ズームレベルが変わった場合は必ず更新
            if (zoom_changed) {
                // キャッシュを更新
                cached_map_view_width = map_view_width;
                cached_map_view_center_x = map_view_center_x;
                cached_map_view_center_y = map_view_center_y;

                // タイル範囲を計算
                TileCoordinate coord(z, z_num);
                start_cell = coord.calculateStartCell(
                    map_view_center_x, map_view_center_y,
                    map_view_width, map_view_height
                );
                end_cell = coord.calculateEndCell(
                    map_view_center_x, map_view_center_y,
                    map_view_width, map_view_height
                );
                return true;
            }

            // ビューポートが変わっていない場合はスキップ（浮動小数点の精度問題を回避）
            if (!isDifferent(cached_map_view_width, map_view_width) &&
                !isDifferent(cached_map_view_center_x, map_view_center_x) &&
                !isDifferent(cached_map_view_center_y, map_view_center_y)) {
                return false;
            }

            // ビューポートが変わった場合はタイル範囲を再計算
            cached_map_view_width = map_view_width;
            cached_map_view_center_x = map_view_center_x;
            cached_map_view_center_y = map_view_center_y;

            TileCoordinate coord(z, z_num);
            const Vector2<int> new_start_cell = coord.calculateStartCell(
                map_view_center_x, map_view_center_y,
                map_view_width, map_view_height
            );

            const Vector2<int> new_end_cell = coord.calculateEndCell(
                map_view_center_x, map_view_center_y,
                map_view_width, map_view_height
            );

            // 実際に範囲が変わったかチェック
            if (new_start_cell != start_cell || new_end_cell != end_cell) {
                start_cell = new_start_cell;
                end_cell = new_end_cell;
                return true;
            }

            return false;
        }

        /// @brief 表示範囲内のタイルを読み込む
        void loadVisibleTiles() {
            if (map_name.size() == 0) return;

            const std::string z_value = std::to_string(z); // Z の値
            std::string local_file_path_zn = file_name_format;
            paxs::StringExtensions::replace(local_file_path_zn, "{z}", z_value);
            if (map_name.size() != 0) paxs::StringExtensions::replace(local_file_path_zn, "{n}", map_name);

            const std::uint_least64_t index_z = tile_cache_.encodeKeyZ(static_cast<std::uint_least64_t>(z));

            // ファイルを保存
            for (int i = start_cell.y, k = 0; i <= end_cell.y; ++i) {

                const std::string y_value = std::to_string((i + z_num) & (z_num - 1)); // Y の値
                std::string local_file_path_zny = local_file_path_zn;
                paxs::StringExtensions::replace(local_file_path_zny, "{y}", y_value);

                const std::uint_least64_t index_zy = tile_cache_.encodeKeyY(static_cast<std::uint_least64_t>((i + z_num) & (z_num - 1))) + index_z;

                // URL と Binary のパスを Y 置換まで事前計算（X ループ外）
                std::string url_path_zny;
                std::string binary_path_zny;
                std::string texture_folder_path_zny;

                if (!metadata_.texture_url.empty()) {
                    url_path_zny = metadata_.texture_url;
                    paxs::StringExtensions::replace(url_path_zny, "{z}", z_value);
                    paxs::StringExtensions::replace(url_path_zny, "{y}", y_value);
                }

                if (!binary_file_name_format.empty()) {
                    binary_path_zny = binary_file_name_format;
                    paxs::StringExtensions::replace(binary_path_zny, "{z}", z_value);
                    paxs::StringExtensions::replace(binary_path_zny, "{y}", y_value);
                    if (map_name.size() != 0) paxs::StringExtensions::replace(binary_path_zny, "{n}", map_name);
                }

                if (!texture_full_path_folder.empty()) {
                    texture_folder_path_zny = texture_full_path_folder;
                    paxs::StringExtensions::replace(texture_folder_path_zny, "{z}", z_value);
                    paxs::StringExtensions::replace(texture_folder_path_zny, "{y}", y_value);
                    if (map_name.size() != 0) paxs::StringExtensions::replace(texture_folder_path_zny, "{n}", map_name);
                }

                for (int j = start_cell.x; j <= end_cell.x; ++j, ++k) {

                    // 画像を格納する index を生成
                    const std::uint_least64_t index_zyx = static_cast<std::uint_least64_t>((j + z_num) & (z_num - 1)) + index_zy;

                    if (tile_cache_.hasAttempted(index_zyx)) {
                        continue;
                    }

                    const std::string x_value = std::to_string((j + z_num) & (z_num - 1)); // X の値

                    // 1. FileTileLoader（ローカルファイル読み込み）
                    auto texture = FileTileLoader::load(
                        local_file_path_zny,
                        x_value
                    );

                    if (texture) {
                        tile_cache_.insert(index_zyx, std::move(texture));
                        continue;
                    }

                    // 2. UrlTileLoader（URL からダウンロード）
                    if (!metadata_.texture_url.empty()) {
                        std::string texture_folder_path_znyx = texture_folder_path_zny;
                        paxs::StringExtensions::replace(texture_folder_path_znyx, "{x}", x_value);

                        texture = UrlTileLoader::load(
                            url_path_zny,
                            local_file_path_zny,
                            texture_folder_path_znyx,
                            x_value
                        );

                        if (texture) {
                            tile_cache_.insert(index_zyx, std::move(texture));
                            continue;
                        }
                    }

                    // 3. BinaryTileLoader（バイナリデータから PNG 生成）
                    if (!binary_file_name_format.empty() && !texture_full_path_folder.empty()) {
                        std::string texture_folder_path_znyx = texture_folder_path_zny;
                        paxs::StringExtensions::replace(texture_folder_path_znyx, "{x}", x_value);

                        texture = BinaryTileLoader::load(
                            binary_path_zny,
                            local_file_path_zny,
                            texture_folder_path_znyx,
                            x_value
                        );

                        if (texture) {
                            tile_cache_.insert(index_zyx, std::move(texture));
                            continue;
                        }
                    }

                    // すべてのローダーが失敗した場合
                    tile_cache_.insertFailure(index_zyx);
                } // for (j)
            } // for (i)
        }

    public:
        XYZTile() = default;

        // タイルを更新
        void update(const double map_view_width, // 描画される地図の経度幅
            const double map_view_height, // 描画される地図の緯度幅
            const double map_view_center_x, // 描画される地図の中心経度
            const double map_view_center_y // 描画される地図の中心緯度
        ) {
            // ズームレベルを更新（毎フレーム実行）
            updateZoomLevel(map_view_height);

            // ★ 整数ズームレベルが変わった場合のみzoom_changed=trueにする
            const bool zoom_changed = (cached_magnification_z != magnification_z || cached_z != z);

            // タイル範囲を常に更新（座標空間の不整合を防ぐため）
            const bool range_changed = updateTileRange(map_view_width, map_view_height,
                                                        map_view_center_x, map_view_center_y,
                                                        zoom_changed);

            // 拡大率が描画範囲外の場合はタイルロードをスキップ
            if (!isInDrawRange()) return;

            // ズームレベルが変わったか、範囲が変わった場合のみタイルをロード
            if (zoom_changed || range_changed) {
                loadVisibleTiles();
            }
        }

        XYZTile(
            const std::uint_least32_t menu_bar_map_,
            const bool menu_bar_map_bool_,
            const std::uint_least32_t texture_root_path_type_,
            const std::uint_least32_t binary_root_path_type_,
            const std::string& map_binary_name_,
            const std::string& map_file_path_name_,
            const std::string& file_name_format_,
            const std::uint_least32_t draw_type_,
            const std::string& texture_url_
        )
        {
            // TileMetadata を初期化
            metadata_.menu_bar_map = menu_bar_map_;
            metadata_.menu_bar_map_bool = menu_bar_map_bool_;
            metadata_.texture_root_path_type = texture_root_path_type_;
            metadata_.binary_root_path_type = binary_root_path_type_;
            metadata_.map_binary_name = map_binary_name_;
            metadata_.map_file_path_name = map_file_path_name_;
            metadata_.file_name_format = file_name_format_;
            metadata_.draw_type = draw_type_;
            metadata_.texture_url = texture_url_;

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
            }
            if (file_name_format.size() != 0) {
                texture_full_path_folder = setFullPathFolder();
            }
        }

        Vector2<int> getStartCell()const {
            return start_cell;
        }
        Vector2<int> getEndCell()const {
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
            return metadata_.draw_type;
        }
        bool getMenuBarMapBool() const {
            return metadata_.menu_bar_map_bool;
        }
        std::uint_least32_t getMenuBarMap() const {
            return metadata_.menu_bar_map;
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
            return tile_cache_.getTextureAt(z_, y_, x_);
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
            metadata_.texture_url = texture_url_;
        }
        void setMapName(const std::string& map_name_) {
            map_name = map_name_;
        }
    };
}

#endif // !PAX_MAHOROBA_XYZ_TILES_HPP
