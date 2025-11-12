/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_MAP_VIEWPORT_HPP
#define PAX_MAHOROBA_MAP_VIEWPORT_HPP

#include <PAX_GRAPHICA/Key.hpp>
#include <PAX_GRAPHICA/Window.hpp>

#include <PAX_SAPIENTICA/EventSystem/ApplicationEvents.hpp>
#include <PAX_SAPIENTICA/EventSystem/EventBus.hpp>
#include <PAX_SAPIENTICA/Type/Vector2.hpp>
#include <PAX_SAPIENTICA/MapProjection.hpp>

namespace paxs {

    // Map viewport constants
    namespace MapViewportConstants {
        // Floating-point comparison epsilon
        static constexpr double coordinate_epsilon = 1e-9;

        // Map view default values
        static constexpr double default_movement_size = 200.0;
        static constexpr double default_expansion_size = 50.0;
        static constexpr double default_height = 24.0;
        static constexpr double max_height_default = 240.0;
        static constexpr double max_height_mahoroba = 30.0;
        static constexpr double min_height = 0.005;

        // Geographic bounds
        static constexpr double longitude_min = -180.0;
        static constexpr double longitude_max = 180.0;
        static constexpr double longitude_range = 360.0;

        // Mouse wheel sensitivity
        static constexpr double mouse_wheel_sensitivity = 10.0;

        // Touch gesture constants (Android)
        static constexpr int max_touch_points = 3;
        static constexpr double touch_zoom_divisor = 6000.0;

        // Tile size for map projection
        static constexpr double tile_size = 256.0;
    }

    // GUI で表示する地図のビューポート（表示領域）を管理するクラス
    // Manages the viewport (visible area) of the map in the GUI
    class MapViewport {
    private:
        // 中央の座標を指定
        paxg::Coordinate center = paxg::Coordinate(
            paxs::MercatorDeg(paxs::EquirectangularDeg(paxs::Vector2<double>(145, 48))) // 韓国 128, 37 // 日本 135, 35 // 北海道 // 東アジア 127, 31, 75.0 // 全世界 100, 0
        ); // マップ座標の中央
        double height = MapViewportConstants::default_height; // 各国 16.0; // 全世界 240.0 // マップの高さ

        // マップの最大高さ
        double max_height =
#ifdef PAXS_MAHOROBA
            MapViewportConstants::max_height_mahoroba;
#else
            MapViewportConstants::max_height_default;
#endif
        double min_height = MapViewportConstants::min_height; // マップの最小高さ
        double width = (height) / double(paxg::Window::height()) * double(paxg::Window::width()); // マップの高さ
        double expansion_size = MapViewportConstants::default_expansion_size; // マップの拡大量

        void subscribeToEvents() {
            // ウィンドウリサイズイベントを購読
            paxs::EventBus::getInstance().subscribe<WindowResizedEvent>(
                [this](const WindowResizedEvent&) {
                    // ウィンドウサイズ変更時に幅を再計算し、ViewportChangedEventを発行
                    applyConstraints();
                    notifyViewportChanged();
                }
            );
        }

        /// @brief 浮動小数点数が異なるかどうかを判定（許容誤差考慮）
        static bool isDifferent(double a, double b) {
            return std::abs(a - b) >= MapViewportConstants::coordinate_epsilon;
        }

    public:
        MapViewport() {
            subscribeToEvents();
        }

        /// @brief ビューポート変更イベントを発行
        void notifyViewportChanged() {
            const int zoom_level = static_cast<int>(std::log2(MapViewportConstants::longitude_range / height));
            paxs::EventBus::getInstance().publish(ViewportChangedEvent(
                center.getX(), center.getY(), zoom_level));
        }

        /// @brief ビューポートの境界制約を適用（Domain層の責任）
        /// @brief Apply boundary constraints to viewport (Domain layer responsibility)
        /// @return 座標が変更された場合true
        bool applyConstraints() {
            bool changed = false;

            // 高さの制約
            height = (std::clamp)(height, min_height, max_height);
            width = height / double(paxg::Window::height()) * double(paxg::Window::width());

            // 境界制約適用前の座標を保存
            const double old_center_x = center.getX();
            const double old_center_y = center.getY();

#ifdef PAXS_MAHOROBA
            constexpr double west_max = (208.0 / MapViewportConstants::tile_size) * MapViewportConstants::longitude_range - MapViewportConstants::longitude_max;
            constexpr double east_max = (MapViewportConstants::tile_size / MapViewportConstants::tile_size) * MapViewportConstants::longitude_range - MapViewportConstants::longitude_max;
            constexpr double north_max = (1.0 - (80.0 / MapViewportConstants::tile_size)) * MapViewportConstants::longitude_range - MapViewportConstants::longitude_max;
            constexpr double south_max = (1.0 - (128.0 / MapViewportConstants::tile_size)) * MapViewportConstants::longitude_range - MapViewportConstants::longitude_max;

            // X座標の位置調整
            if (center.getX() - width / 2 < west_max) {
                center.setX(west_max + width / 2);
            }
            if (center.getX() + width / 2 > east_max) {
                center.setX(east_max - width / 2);
            }

            // Y座標の位置調整
            if (center.getY() + height / 2 > north_max) {
                center.setY(north_max - height / 2);
            }
            if (center.getY() - height / 2 < south_max) {
                center.setY(south_max + height / 2);
            }
#else
            constexpr double north_max = (1.0 - (0.0 / MapViewportConstants::tile_size)) * MapViewportConstants::longitude_range - MapViewportConstants::longitude_max;
            constexpr double south_max = (1.0 - (MapViewportConstants::tile_size / MapViewportConstants::tile_size)) * MapViewportConstants::longitude_range - MapViewportConstants::longitude_max;

            // Y座標の位置調整のみ（PAXS_MAHOROBA以外）
            if (center.getY() + height / 2 > north_max) {
                center.setY(north_max - height / 2);
            }
            if (center.getY() - height / 2 < south_max) {
                center.setY(south_max + height / 2);
            }
#endif

            // 座標が変更されたかチェック
            if (isDifferent(center.getX(), old_center_x) || isDifferent(center.getY(), old_center_y)) {
                changed = true;
            }

            return changed;
        }
        void setSize(const double new_height) {
            if (isDifferent(height, new_height)) {
                height = new_height;
                width = height / double(paxg::Window::height()) * double(paxg::Window::width());
            }
        }
        void setCenter(const double x_, const double y_) {
            if (isDifferent(center.getX(), x_) || isDifferent(center.getY(), y_)) {
                center.setX(x_);
                center.setY(y_);
            }
        }

        paxg::Coordinate& getCoordinate() {
            return center;
        }
        double getCenterX() const {
            return center.getX();
        }
        double getCenterY() const {
            return center.getY();
        }
        double getWidth() const {
            return width;
        }
        double getHeight() const {
            return height;
        }
        double getMinHeight() const {
            return min_height;
        }
        double getMaxHeight() const {
            return max_height;
        }
        double getExpansionSize() const {
            return expansion_size;
        }
    };

}

#endif // !PAX_MAHOROBA_MAP_VIEWPORT_HPP
