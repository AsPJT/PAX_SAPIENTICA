/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_TILE_MANAGER_HPP
#define PAX_MAHOROBA_TILE_MANAGER_HPP

#include <string>
#include <vector>

#include <PAX_MAHOROBA/Map/MapViewport.hpp>
#include <PAX_MAHOROBA/Map/Tile/TileRenderer.hpp>
#include <PAX_MAHOROBA/Map/Tile/TileRepository.hpp>
#include <PAX_MAHOROBA/Map/Tile/XYZTile.hpp>
#include <PAX_MAHOROBA/Rendering/IRenderable.hpp>

#include <PAX_SAPIENTICA/Calendar/JulianDayNumber.hpp>
#include <PAX_SAPIENTICA/FeatureVisibilityManager.hpp>

namespace paxs {

    /// @brief タイル管理クラス
    /// @brief Tile manager class
    class TileManager : public IRenderable {
    private:
        // 描画する XYZ タイルを管理
        std::vector<XYZTile> xyz_tile_list;

        // タイル描画を担当
        TileRenderer tile_renderer_;

        // タイルデータ読み込みを担当
        TileRepository tile_repository_;

        // 可視性管理
        bool visible_ = true;

        // 描画に必要なデータを保持（updateData()で更新、render()で使用）
        const paxs::FeatureVisibilityManager* cached_visible_ = nullptr;
        MapViewport cached_map_viewport_;
        cal::JDN_F64 cached_jdn_ = 0.0;

    public:
        // XYZ Tiles を追加（TileRepositoryに委譲）
        void add(const std::string& file_path) {
            std::vector<XYZTile> loaded_tiles = tile_repository_.loadFromFile(file_path);
            xyz_tile_list.insert(xyz_tile_list.end(), loaded_tiles.begin(), loaded_tiles.end());
        }

        // グリッド線を追加（TileRepositoryに委譲）
        void addGridLine() {
            xyz_tile_list.emplace_back(tile_repository_.createGridLineTile());
        }

        /// @brief データ更新（描画は行わない）
        /// @brief Update data (no drawing)
        void updateData(const paxs::FeatureVisibilityManager& visible, const MapViewport& map_viewport, cal::JDN_F64 jdn) {
            const double map_viewport_width = map_viewport.getWidth();
            const double map_viewport_height = map_viewport.getHeight();
            const double map_viewport_center_x = map_viewport.getCenterX();
            const double map_viewport_center_y = map_viewport.getCenterY();

            // 更新処理
            for (auto&& xyzi : xyz_tile_list) {
                if (xyzi.getMenuBarMap() != 0 && visible.isVisible(xyzi.getMenuBarMap()) != xyzi.getMenuBarMapBool()) continue;
                xyzi.update(map_viewport_width, map_viewport_height, map_viewport_center_x, map_viewport_center_y);
            }

            // 描画用にデータをキャッシュ
            cached_visible_ = &visible;
            cached_map_viewport_ = map_viewport;
            cached_jdn_ = jdn;
        }

        // IRenderable の実装
        // IRenderable implementation

        /// @brief レンダリング処理
        /// @brief Render
        void render() const override {
            if (!visible_ || cached_visible_ == nullptr) return;

            // 描画処理
            tile_renderer_.drawBackground();
            tile_renderer_.drawTiles(xyz_tile_list, *cached_visible_, cached_map_viewport_, cached_jdn_);
        }

        /// @brief レンダリングレイヤーを取得
        /// @brief Get rendering layer
        RenderLayer getLayer() const override {
            return RenderLayer::MapBase;
        }

        /// @brief 可視性を取得
        /// @brief Get visibility
        bool isVisible() const override {
            return visible_;
        }

        /// @brief 可視性を設定
        /// @brief Set visibility
        void setVisible(bool visible) override {
            visible_ = visible;
        }
    };
}

#endif // !PAX_MAHOROBA_TILE_MANAGER_HPP
