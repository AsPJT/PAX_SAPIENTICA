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
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <PAX_MAHOROBA/Map/Tile/Loader/BinaryTileLoader.hpp>
#include <PAX_MAHOROBA/Map/Tile/Loader/FileTileLoader.hpp>
#include <PAX_MAHOROBA/Map/Tile/Loader/UrlTileLoader.hpp>

#include <PAX_SAPIENTICA/Core/Platform.hpp>
#include <PAX_SAPIENTICA/Core/Type/Vector2.hpp>
#include <PAX_SAPIENTICA/Map/Tile/TileCache.hpp>
#include <PAX_SAPIENTICA/Map/Tile/TileCoordinate.hpp>
#include <PAX_SAPIENTICA/Map/Tile/TileMetadata.hpp>
#include <PAX_SAPIENTICA/Utility/MurMur3.hpp>
#include <PAX_SAPIENTICA/Utility/StringUtils.hpp>

namespace paxs {

    /// @brief ビューポート状態
    /// @brief Viewport state
    struct ViewportState {
        Vector2<double> size;
        Vector2<double> center;

        bool operator==(const ViewportState& other) const {
            return !isDifferent(size.x, other.size.x) &&
                   !isDifferent(center.x, other.center.x) &&
                   !isDifferent(center.y, other.center.y);
        }

        bool operator!=(const ViewportState& other) const {
            return !(*this == other);
        }

    private:
        static constexpr double EPSILON = 1e-9;

        static bool isDifferent(double a, double b) {
            return std::abs(a - b) >= EPSILON;
        }
    };

    /// @brief ズーム状態
    /// @brief Zoom state
    struct ZoomState {
        unsigned int magnification_z;  ///< ビューポートから計算されたズームレベル
        unsigned int actual_z;         ///< 実際に使用するズームレベル（データの制約考慮）
        unsigned int z_num;            ///< 2^actual_z

        bool operator==(const ZoomState& other) const {
            return magnification_z == other.magnification_z &&
                   actual_z == other.actual_z;
        }

        bool operator!=(const ZoomState& other) const {
            return !(*this == other);
        }
    };

    /// @brief タイル範囲
    /// @brief Tile range
    struct TileRange {
        Vector2<int> start;  ///< 開始タイル座標
        Vector2<int> end;    ///< 終了タイル座標

        bool operator==(const TileRange& other) const {
            return start == other.start && end == other.end;
        }

        bool operator!=(const TileRange& other) const {
            return !(*this == other);
        }
    };

    /// @brief タイル更新判定結果
    /// @brief Tile update decision result
    struct TileUpdateDecision {
        bool should_reload_tiles;  ///< タイルを再ロードすべきか
        bool zoom_changed;         ///< ズームレベルが変更されたか
        bool range_changed;        ///< タイル範囲が変更されたか

        TileUpdateDecision()
            : should_reload_tiles(false)
            , zoom_changed(false)
            , range_changed(false) {}
    };

    class XYZTile {
    private:
        // 浮動小数点比較の許容誤差
        static constexpr double VIEWPORT_EPSILON = 1e-9;

        // XYZ タイルの 1 つのセルのWebメルカトル座標を保持
        // 基本的に Z = 19 は無い

        paxs::TileCache<paxg::Texture> tile_cache_;

        // XYZ タイルの設定情報
        paxs::TileMetadata metadata_;

        // XYZ タイルの画像の情報を保持
        std::string binary_file_name_format = ""; // バイナリデータ
        std::string map_name = ""; // 地図名
        std::string file_name_format = ("{z}/{x}/{y}");

        // XYZ タイルの画面上の始点セル
        Vector2<int> start_cell{};
        // XYZ タイルの画面上の終点セル
        Vector2<int> end_cell{};

        // バイナリタイル読み込み用の再利用バッファ
        std::vector<unsigned char> binary_buffer_;
        std::vector<paxs::TileRGBA> rgba_buffer_;

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

        ViewportState current_viewport_{{0.0, 0.0}, {0.0, 0.0}};
        ZoomState current_zoom_{0, 0, 0};
        TileRange current_range_{{0, 0}, {0, 0}};

        /// @brief ファイル名フォーマットを構築（プラットフォーム固有処理を含む）
        /// @brief Build file name format (including platform-specific processing)
        /// @param texture_root_path_type テクスチャルートパスタイプ
        /// @param map_file_path_name マップファイルパス名
        /// @param file_name_format_ ファイル名フォーマット
        /// @return 構築されたファイル名フォーマット
        static std::string buildFileNameFormat(
            std::uint_least32_t texture_root_path_type,
            const std::string& map_file_path_name,
            const std::string& file_name_format_
        ) {
#if defined(PAXS_USING_DXLIB) && defined(PAXS_PLATFORM_ANDROID)
            static char internal_data_path[1024]{};

            switch (texture_root_path_type) {
            case paxs::MurMur3::calcHash("asset_file"):
                return map_file_path_name + file_name_format_ + ".png";
            case paxs::MurMur3::calcHash("internal_file"):
                DxLib::GetInternalDataPath(internal_data_path, 1024);
                return std::string(internal_data_path) + "/" +
                    map_file_path_name + file_name_format_ + ".png";
            case paxs::MurMur3::calcHash("external_file"):
                DxLib::GetExternalDataPath(internal_data_path, 1024);
                return std::string(internal_data_path) + "/" +
                    map_file_path_name + file_name_format_ + ".png";
            default:
                printfDx("\n[error:%u,%s,%s,%s]",
                    texture_root_path_type,
                    "", // map_binary_name
                    map_file_path_name.c_str(),
                    file_name_format_.c_str());
                return map_file_path_name + file_name_format_ + ".png";
            }
#else
            // アセットルートからの相対パスとして保持
            (void)texture_root_path_type; // 未使用警告を抑制
            return map_file_path_name + file_name_format_ + ".png";
#endif
        }

        /// @brief ビューポートから新しいズーム状態を計算
        /// @param map_view_height ビューポートの高さ
        /// @return 計算されたズーム状態
        ZoomState calculateZoom(double map_view_height) const {
            ZoomState zoom;
            zoom.magnification_z = TileCoordinate::calculateZoomLevel(map_view_height);

            // データが存在するZレベルに調整
            if (default_z == 999) {
                zoom.actual_z = zoom.magnification_z;
                if (zoom.actual_z < min_z) zoom.actual_z = min_z;
                else if (zoom.actual_z > max_z) zoom.actual_z = max_z;
            }
            else {
                zoom.actual_z = default_z;
            }

            zoom.z_num = (1 << zoom.actual_z);
            return zoom;
        }

        /// @brief ビューポートとズーム状態から新しいタイル範囲を計算
        /// @param viewport ビューポート状態
        /// @param zoom ズーム状態
        /// @return 計算されたタイル範囲
        TileRange calculateTileRange(const ViewportState& viewport, const ZoomState& zoom) const {
            TileCoordinate coord(zoom.actual_z, zoom.z_num);

            TileRange range;
            range.start = coord.calculateStartCell(viewport.center, viewport.size);
            range.end = coord.calculateEndCell(viewport.center, viewport.size);

            return range;
        }

        /// @brief 変更を検出して更新判定を返す
        /// @param new_zoom 新しいズーム状態
        /// @param new_range 新しいタイル範囲
        /// @return 更新判定結果
        TileUpdateDecision detectChanges(
            const ZoomState& new_zoom,
            const TileRange& new_range
        ) {
            TileUpdateDecision decision;

            // ズーム変更チェック
            decision.zoom_changed = (new_zoom != current_zoom_);

            // タイル範囲変更チェック
            decision.range_changed = (new_range != current_range_);

            // 描画範囲チェック（magnification_zを使用）
            if (new_zoom.magnification_z < draw_min_z || new_zoom.magnification_z > draw_max_z) {
                decision.should_reload_tiles = false;
                return decision;
            }

            // ズームまたは範囲が変わった場合は再ロード
            decision.should_reload_tiles = decision.zoom_changed || decision.range_changed;

            return decision;
        }


        /// @brief 表示範囲内のタイルを読み込む
        void loadVisibleTiles() {
            if (map_name.size() == 0) return;

            const std::string z_value = std::to_string(z); // Z の値
            std::string local_file_path_zn = file_name_format;
            paxs::StringUtils::replace(local_file_path_zn, "{z}", z_value);
            if (map_name.size() != 0) paxs::StringUtils::replace(local_file_path_zn, "{n}", map_name);

            const std::uint_least64_t index_z = tile_cache_.encodeKeyZ(static_cast<std::uint_least64_t>(z));

            // ファイルを保存
            for (int i = start_cell.y, k = 0; i <= end_cell.y; ++i) {

                const std::string y_value = std::to_string((i + z_num) & (z_num - 1)); // Y の値
                std::string local_file_path_zny = local_file_path_zn;
                paxs::StringUtils::replace(local_file_path_zny, "{y}", y_value);

                const std::uint_least64_t index_zy = tile_cache_.encodeKeyY(static_cast<std::uint_least64_t>((i + z_num) & (z_num - 1))) + index_z;

                // URL と Binary のパスを Y 置換まで事前計算（X ループ外）
                std::string url_path_zny;
                std::string binary_path_zny;

                if (!metadata_.texture_url.empty()) {
                    url_path_zny = metadata_.texture_url;
                    paxs::StringUtils::replace(url_path_zny, "{z}", z_value);
                    paxs::StringUtils::replace(url_path_zny, "{y}", y_value);
                }

                if (!binary_file_name_format.empty()) {
                    binary_path_zny = binary_file_name_format;
                    paxs::StringUtils::replace(binary_path_zny, "{z}", z_value);
                    paxs::StringUtils::replace(binary_path_zny, "{y}", y_value);
                    if (map_name.size() != 0) paxs::StringUtils::replace(binary_path_zny, "{n}", map_name);
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
                        texture = UrlTileLoader::load(
                            url_path_zny,
                            local_file_path_zny,
                            x_value
                        );

                        if (texture) {
                            tile_cache_.insert(index_zyx, std::move(texture));
                            continue;
                        }
                    }

                    // 3. BinaryTileLoader（バイナリデータから PNG 生成）
                    if (!binary_file_name_format.empty()) {
                        texture = BinaryTileLoader::load(
                            binary_path_zny,
                            local_file_path_zny,
                            x_value,
                            binary_buffer_,  // バッファを渡す
                            rgba_buffer_     // バッファを渡す
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

        /// @brief タイルを更新
        /// @brief Update tiles
        /// @param map_view_size 描画される地図のサイズ（経度幅, 緯度幅） / Map view size (longitude width, latitude height)
        /// @param map_view_center 描画される地図の中心座標（中心経度, 中心緯度） / Map view center (center longitude, center latitude)
        void update(
            const Vector2<double> map_view_size,
            const Vector2<double> map_view_center
        ) {
            // 1. 新しい状態を計算
            ViewportState new_viewport(map_view_size, map_view_center);
            ZoomState new_zoom = calculateZoom(map_view_size.y);
            TileRange new_range = calculateTileRange(new_viewport, new_zoom);

            // 2. 変更を検出
            TileUpdateDecision decision = detectChanges(new_zoom, new_range);

            // 3. 必要に応じてタイルをロード
            if (decision.should_reload_tiles) {
                // ★ 旧変数を更新してからloadVisibleTiles()を呼ぶ（互換性のため）
                z = new_zoom.actual_z;
                magnification_z = new_zoom.magnification_z;
                z_num = new_zoom.z_num;
                start_cell = new_range.start;
                end_cell = new_range.end;

                loadVisibleTiles();
            }

            // 4. 状態を更新（常に最後に実行）
            current_viewport_ = new_viewport;
            current_zoom_ = new_zoom;
            current_range_ = new_range;
        }

        XYZTile(
            const std::uint_least32_t menu_bar_map_,
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
                file_name_format = buildFileNameFormat(
                    texture_root_path_type_,
                    map_file_path_name_,
                    file_name_format_
                );
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
