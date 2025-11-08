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
#include <PAX_MAHOROBA/Core/ApplicationEvents.hpp>
#include <PAX_MAHOROBA/Core/EventBus.hpp>
#include <PAX_MAHOROBA/Map/MapViewport.hpp>
#include <PAX_MAHOROBA/Map/Tile/TileRenderer.hpp>
#include <PAX_MAHOROBA/Map/Tile/TileRepository.hpp>
#include <PAX_MAHOROBA/Map/Tile/XYZTile.hpp>
#include <PAX_MAHOROBA/Rendering/IRenderable.hpp>

#include <PAX_SAPIENTICA/AppConfig.hpp>
#include <PAX_SAPIENTICA/Calendar/JulianDayNumber.hpp>
#include <PAX_SAPIENTICA/FeatureVisibilityManager.hpp>

namespace paxs {

    /// @brief 地図タイルレイヤー
    /// @brief Map tile layer
    class MapTileLayer : public IRenderable {
    private:
        std::vector<XYZTile> xyz_tile_list;
        TileRepository tile_repository_;

        bool visible_ = true;
        bool initial_tiles_preloaded_ = false;

        EventBus* event_bus_ = nullptr;
        AppStateManager* app_state_manager_ = nullptr;

    public:
        MapTileLayer() {
            // XYZタイルを初期化
            AppConfig::getInstance()->calcDataSettings(MurMur3::calcHash("XYZTiles"),
                [&](const std::string& path) {
                    std::vector<XYZTile> loaded_tiles = tile_repository_.loadFromFile(path);
                    xyz_tile_list.insert(xyz_tile_list.end(), loaded_tiles.begin(), loaded_tiles.end());
                });
            // グリッド線を追加
            xyz_tile_list.emplace_back(tile_repository_.createGridLineTile());
        }

        /// @brief AppStateManagerを設定してイベント駆動を有効化
        void setAppStateManager(AppStateManager* app_state_manager) {
            app_state_manager_ = app_state_manager;
            if (app_state_manager_ != nullptr) {
                event_bus_ = &EventBus::getInstance();
                subscribeToEvents();
                // 初回更新を即座に実行
                updateTileData();
                // 初期タイルをプリロード
                preloadInitialTiles();
            }
        }

        void render() const override {
            if (!visible_ || !app_state_manager_) return;

            // AppStateManagerから最新データを直接取得して描画のみ実行
            const auto& visible = app_state_manager_->getVisibilityManager();
            const auto& map_viewport = app_state_manager_->getMapViewport();
            const auto& koyomi = app_state_manager_->getKoyomi();

            TileRenderer::drawBackground();
            TileRenderer::drawTiles(xyz_tile_list, visible, map_viewport, koyomi.jdn.cgetDay());
        }
        RenderLayer getLayer() const override {
            return RenderLayer::MapTile;
        }
        bool isVisible() const override {
            return visible_;
        }
        void setVisible(bool visible) override {
            visible_ = visible;
        }

    private:
        /// @brief 初期タイルをプリロード
        /// @brief Preload initial tiles before first render
        void preloadInitialTiles() {
            if (initial_tiles_preloaded_ || !app_state_manager_) return;

            const auto& visible = app_state_manager_->getVisibilityManager();
            const auto& map_viewport = app_state_manager_->getMapViewport();

            const double map_viewport_width = map_viewport.getWidth();
            const double map_viewport_height = map_viewport.getHeight();
            const double map_viewport_center_x = map_viewport.getCenterX();
            const double map_viewport_center_y = map_viewport.getCenterY();

            // 初期ビューポート範囲のタイルをプリロード
            for (auto&& xyz_tile : xyz_tile_list) {
                if (xyz_tile.getMenuBarMap() != 0 && visible.isVisible(xyz_tile.getMenuBarMap()) != xyz_tile.getMenuBarMapBool()) continue;
                xyz_tile.update(map_viewport_width, map_viewport_height, map_viewport_center_x, map_viewport_center_y);
            }

            initial_tiles_preloaded_ = true;
        }

        /// @brief タイルデータを更新
        /// @brief Update tile data
        void updateTileData() {
            if (!app_state_manager_) return;

            const auto& visible = app_state_manager_->getVisibilityManager();
            const auto& map_viewport = app_state_manager_->getMapViewport();

            const double map_viewport_width = map_viewport.getWidth();
            const double map_viewport_height = map_viewport.getHeight();
            const double map_viewport_center_x = map_viewport.getCenterX();
            const double map_viewport_center_y = map_viewport.getCenterY();

            // 更新処理
            for (auto&& xyz_tile : xyz_tile_list) {
                if (xyz_tile.getMenuBarMap() != 0 && visible.isVisible(xyz_tile.getMenuBarMap()) != xyz_tile.getMenuBarMapBool()) continue;
                xyz_tile.update(map_viewport_width, map_viewport_height, map_viewport_center_x, map_viewport_center_y);
            }
        }

        /// @brief イベントを購読
        /// @brief Subscribe to events
        void subscribeToEvents() {
            if (event_bus_ == nullptr) return;

            // ビューポート変更イベントの購読
            event_bus_->subscribe<ViewportChangedEvent>(
                [this](const ViewportChangedEvent& event) {
                    (void)event;
                    if (app_state_manager_) {
                        // イベント受信時に即座にタイルデータを更新
                        updateTileData();
                    }
                }
            );

            // 日付変更イベントの購読
            event_bus_->subscribe<DateChangedEvent>(
                [this](const DateChangedEvent& event) {
                    (void)event;
                    if (app_state_manager_) {
                        // イベント受信時に即座にタイルデータを更新
                        updateTileData();
                    }
                }
            );

            // レイヤー可視性変更イベントの購読
            event_bus_->subscribe<MapLayerVisibilityChangedEvent>(
                [this](const MapLayerVisibilityChangedEvent& event) {
                    (void)event;
                    if (app_state_manager_) {
                        // イベント受信時に即座にタイルデータを更新
                        updateTileData();
                    }
                }
            );
        }
    };
}

#endif // !PAX_MAHOROBA_TILE_MANAGER_HPP
