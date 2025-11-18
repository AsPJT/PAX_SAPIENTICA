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

#include <PAX_MAHOROBA/Core/AppStateManager.hpp>
#include <PAX_MAHOROBA/Map/MapViewport.hpp>
#include <PAX_MAHOROBA/Map/Tile/TileRenderer.hpp>
#include <PAX_MAHOROBA/Map/Tile/TileRepository.hpp>
#include <PAX_MAHOROBA/Map/Tile/XYZTile.hpp>
#include <PAX_MAHOROBA/Rendering/IRenderable.hpp>

#include <PAX_SAPIENTICA/System/AppConfig.hpp>
#include <PAX_SAPIENTICA/System/ApplicationEvents.hpp>
#include <PAX_SAPIENTICA/System/EventBus.hpp>
#include <PAX_SAPIENTICA/System/FeatureVisibilityManager.hpp>

namespace paxs {

    /// @brief 地図タイルレイヤー
    /// @brief Map tile layer
    class MapTileLayer : public IRenderable {
    private:
        std::vector<XYZTile> xyz_tile_list;
        TileRepository tile_repository_;

        const AppStateManager& app_state_manager_;

    public:
        MapTileLayer(const AppStateManager& app_state_manager)
            : app_state_manager_(app_state_manager) {
            // XYZタイルを初期化
            AppConfig::getInstance().ifSettingExists(MurMur3::calcHash("XYZTiles"),
                [&](const std::string& path) {
                    std::vector<XYZTile> loaded_tiles = tile_repository_.loadFromFile(path);
                    xyz_tile_list.insert(xyz_tile_list.end(), loaded_tiles.begin(), loaded_tiles.end());
                });
            // グリッド線を追加
            xyz_tile_list.emplace_back(tile_repository_.createGridLineTile());

            subscribeToEvents();
            // 初回更新を即座に実行
            updateTileData();
        }

        void render() const override {
            const auto& visible = app_state_manager_.getVisibilityManager();
            const auto& map_viewport = app_state_manager_.getMapViewport();
            const auto& koyomi = app_state_manager_.getKoyomi();

            TileRenderer::drawBackground();
            TileRenderer::drawTiles(xyz_tile_list, visible, map_viewport, koyomi.jdn.getDay());
        }

        RenderLayer getLayer() const override { return RenderLayer::MapTile; }
        bool isVisible() const override { return true; }

    private:
        /// @brief タイルデータを更新（ViewportChangedEventから呼び出し）
        void updateTileData() {
            const auto& visible = app_state_manager_.getVisibilityManager();
            const auto& map_viewport = app_state_manager_.getMapViewport();

            const double map_viewport_width = map_viewport.getWidth();
            const double map_viewport_height = map_viewport.getHeight();
            const double map_viewport_center_x = map_viewport.getCenterX();
            const double map_viewport_center_y = map_viewport.getCenterY();

            // 更新処理
            for (auto&& xyz_tile : xyz_tile_list) {
                if (xyz_tile.getMenuBarMap() != 0 && visible.isVisible(xyz_tile.getMenuBarMap()) != xyz_tile.getMenuBarMapBool()) {
                    continue;
                }
                xyz_tile.update(map_viewport_width, map_viewport_height, map_viewport_center_x, map_viewport_center_y);
            }
        }
        /// @brief イベントを購読
        void subscribeToEvents() {
            EventBus::getInstance().subscribe<ViewportChangedEvent>(
                [this](const ViewportChangedEvent& /*event*/) {
                    updateTileData();
                }
            );
        }
    };
}

#endif // !PAX_MAHOROBA_TILE_MANAGER_HPP
