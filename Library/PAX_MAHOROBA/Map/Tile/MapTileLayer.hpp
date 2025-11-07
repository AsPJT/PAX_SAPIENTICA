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

        // 描画に必要なデータを保持（updateData()で更新、render()で使用）
        // TODO: イベント駆動完全移行時にAppStateManagerから直接取得するよう変更
        const paxs::FeatureVisibilityManager* cached_visible_ = nullptr;
        MapViewport cached_map_viewport_;
        cal::JDN_F64 cached_jdn_ = 0.0;

        // イベント駆動用（オプション）
        EventBus* event_bus_ = nullptr;
        bool needs_update_ = false;

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

        /// @brief イベントバスを設定してイベント駆動を有効化
        /// @brief Set EventBus to enable event-driven updates
        /// @param event_bus EventBusへのポインタ
        void setEventBus(EventBus* event_bus) {
            event_bus_ = event_bus;
            if (event_bus_ != nullptr) {
                subscribeToEvents();
            }
        }

        /// @brief データ更新（描画は行わない）
        void updateData(const paxs::FeatureVisibilityManager& visible, const MapViewport& map_viewport, cal::JDN_F64 jdn) {
            const double map_viewport_width = map_viewport.getWidth();
            const double map_viewport_height = map_viewport.getHeight();
            const double map_viewport_center_x = map_viewport.getCenterX();
            const double map_viewport_center_y = map_viewport.getCenterY();

            // 更新処理
            for (auto&& xyz_tile : xyz_tile_list) {
                if (xyz_tile.getMenuBarMap() != 0 && visible.isVisible(xyz_tile.getMenuBarMap()) != xyz_tile.getMenuBarMapBool()) continue;
                xyz_tile.update(map_viewport_width, map_viewport_height, map_viewport_center_x, map_viewport_center_y);
            }

            // 描画用にデータをキャッシュ
            cached_visible_ = &visible;
            cached_map_viewport_ = map_viewport;
            cached_jdn_ = jdn;
        }

        void render() const override {
            if (!visible_ || cached_visible_ == nullptr) return;

            TileRenderer::drawBackground();
            TileRenderer::drawTiles(xyz_tile_list, *cached_visible_, cached_map_viewport_, cached_jdn_);
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
        /// @brief イベントを購読
        /// @brief Subscribe to events
        void subscribeToEvents() {
            if (event_bus_ == nullptr) return;

            // ビューポート変更イベントの購読
            event_bus_->subscribe<ViewportChangedEvent>(
                [this](const ViewportChangedEvent& event) {
                    needs_update_ = true;
                    // 必要に応じてタイルの読み込みをトリガー
                }
            );

            // 日付変更イベントの購読
            event_bus_->subscribe<DateChangedEvent>(
                [this](const DateChangedEvent& event) {
                    needs_update_ = true;
                }
            );

            // レイヤー可視性変更イベントの購読
            event_bus_->subscribe<MapLayerVisibilityChangedEvent>(
                [this](const MapLayerVisibilityChangedEvent& event) {
                    // 地図タイルレイヤー関連の可視性変更を処理
                    needs_update_ = true;
                }
            );
        }
    };
}

#endif // !PAX_MAHOROBA_TILE_MANAGER_HPP
