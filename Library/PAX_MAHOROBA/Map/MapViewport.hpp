/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_MAP_VIEWPORT_HPP
#define PAX_MAHOROBA_MAP_VIEWPORT_HPP

#include <algorithm>
#include <array>
#include <cmath>

#include <PAX_GRAPHICA/Circle.hpp>
#include <PAX_GRAPHICA/Color.hpp>
#include <PAX_GRAPHICA/Key.hpp>
#include <PAX_GRAPHICA/Mouse.hpp>
#include <PAX_GRAPHICA/TouchInput.hpp>
#include <PAX_GRAPHICA/Window.hpp>

#include <PAX_SAPIENTICA/Type/Vector2.hpp>
#include <PAX_SAPIENTICA/MapProjection.hpp>

namespace paxs {

    // Map viewport constants
    namespace MapViewportConstants {
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
        Coordinate center = Coordinate(
            MapViewportConstants::default_movement_size,
            paxs::EquirectangularDeg(paxs::Vector2<double>(145, 48)) // 韓国 128, 37 // 日本 135, 35 // 北海道 // 東アジア 127, 31, 75.0 // 全世界 100, 0
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
        std::array<Key, 1> enl_keys; // 拡大キー
        std::array<Key, 1> esc_keys; // 縮小キー

#ifdef __ANDROID__
        int touch_num = 0;
        int old_touch_num = 0;
        std::array<paxs::Vector2<int>, MapViewportConstants::max_touch_points> pos;
        std::array<paxs::Vector2<int>, MapViewportConstants::max_touch_points> old_pos;
#endif


    public:
        MapViewport() : enl_keys{Key(PAXG_KEY_Q)}, esc_keys{Key(PAXG_KEY_E)} {
        }
        void update() {
            // マウスホイールで地図の拡大・縮小
            {
                height *= (1.0 + (paxg::Mouse::getInstance()->getWheelRotVol() / MapViewportConstants::mouse_wheel_sensitivity));
                height = (std::clamp)(height, min_height, max_height);
                width = (height) / double(paxg::Window::height()) * double(paxg::Window::width());
            }
#ifdef __ANDROID__
            static int old_touch_num = 0;
            static int touch_num = 0;
            static std::array<paxs::Vector2<int>, MapViewportConstants::max_touch_points> pos = { paxs::Vector2<int>{0,0},paxs::Vector2<int>{0,0},paxs::Vector2<int>{0,0} };
            static std::array<paxs::Vector2<int>, MapViewportConstants::max_touch_points> old_pos = { paxs::Vector2<int>{0,0},paxs::Vector2<int>{0,0},paxs::Vector2<int>{0,0} };
#else
            if (paxg::Mouse::getInstance()->pressedLeft2()) {
                center.setX(
                    center.getX() +
                    height / static_cast<double>(paxg::Window::height()) *
                    static_cast<double>(paxg::Mouse::getInstance()->getPosXBefore1Frame() - paxg::Mouse::getInstance()->getPosX()));
                center.setY(
                    center.getY() +
                    height / static_cast<double>(paxg::Window::height()) *
                        static_cast<double>(paxg::Mouse::getInstance()->getPosY() - paxg::Mouse::getInstance()->getPosYBefore1Frame()));

                if (center.getX() < MapViewportConstants::longitude_min) {
                    center.setX(center.getX() + MapViewportConstants::longitude_range);
                }
                if (center.getX() >= MapViewportConstants::longitude_max) {
                    center.setX(center.getX() - MapViewportConstants::longitude_range);
                }
                if (center.getY() < MapViewportConstants::longitude_min) {
                    center.setY(center.getY() - MapViewportConstants::longitude_max);
                }
                if (center.getY() > MapViewportConstants::longitude_max) {
                    center.setY(center.getY() + MapViewportConstants::longitude_max);
                }

            }
#endif

#ifdef __ANDROID__
            old_touch_num = touch_num;
            old_pos = pos;

            touch_num = paxg::TouchInput::getTouchCount();


            for (int i = 0; i < touch_num; i++) {
                if (i >= MapViewportConstants::max_touch_points) break;

                if (paxg::TouchInput::getTouchPosition(i, pos[i])) {
                    paxg::Circle(pos[i].x, pos[i].y, 40).draw(paxg::Color(230, 230, 240));
                }
            }

            if (old_touch_num == 1 && touch_num == 1) {
                center.setX(center.getX() +
                    height / static_cast<double>(paxg::Window::height()) *
                    static_cast<double>(old_pos[0].x - pos[0].x));
                center.setY(center.getY() +
                    height / static_cast<double>(paxg::Window::height()) *
                    static_cast<double>(pos[0].y - old_pos[0].y));

                if (center.getX() < MapViewportConstants::longitude_min) {
                    center.setX(center.getX() + MapViewportConstants::longitude_range);
                }
                if (center.getX() >= MapViewportConstants::longitude_max) {
                    center.setX(center.getX() - MapViewportConstants::longitude_range);
                }
                if (center.getY() < MapViewportConstants::longitude_min) {
                    center.setY(center.getY() - MapViewportConstants::longitude_max);
                }
                if (center.getY() > MapViewportConstants::longitude_max) {
                    center.setY(center.getY() + MapViewportConstants::longitude_max);
                }

            }
            else if (old_touch_num == 2 && touch_num == 2) {

                const int len = (pos[0].x - pos[1].x) * (pos[0].x - pos[1].x) + (pos[0].y - pos[1].y) * (pos[0].y - pos[1].y);
                const int old_len = (old_pos[0].x - old_pos[1].x) * (old_pos[0].x - old_pos[1].x) + (old_pos[0].y - old_pos[1].y) * (old_pos[0].y - old_pos[1].y);
                const int sub = std::abs(len - old_len);

                if (len > old_len) {
                    if (height > min_height) {
                        height -= ((height * (1.0 + (sub / MapViewportConstants::touch_zoom_divisor))) / expansion_size);
                        width = (height) / double(paxg::Window::height()) * double(paxg::Window::width());
                    }
                    if (height < min_height) {
                        height = min_height;
                        width = (height) / double(paxg::Window::height()) * double(paxg::Window::width());
                    }
                }
                else if (len < old_len) {
                    // 画面広く
                    if (height < max_height) {
                        height += ((height * (1.0 + (sub / MapViewportConstants::touch_zoom_divisor))) / expansion_size);
                        width = (height) / double(paxg::Window::height()) * double(paxg::Window::width());
                    }
                    if (height > max_height) {
                        height = max_height;
                        width = (height) / double(paxg::Window::height()) * double(paxg::Window::width());
                    }
                }

            }
#endif

            center.update(height);
            if (pressed(enl_keys)) {
                if (height > min_height) {
                    height -= (height / expansion_size);
                    width = (height) / double(paxg::Window::height()) * double(paxg::Window::width());
                }
                if (height < min_height) {
                    height = min_height;
                    width = (height) / double(paxg::Window::height()) * double(paxg::Window::width());
                }
            }
            if (pressed(esc_keys)) {
                if (height < max_height) {
                    height += (height / expansion_size);
                    width = (height) / double(paxg::Window::height()) * double(paxg::Window::width());
                }
                if (height > max_height) {
                    height = max_height;
                    width = (height) / double(paxg::Window::height()) * double(paxg::Window::width());
                }
            }

            // 位置調整
            if (height < min_height) {
                height = min_height;
                width = (height) / double(paxg::Window::height()) * double(paxg::Window::width());
            }
            if (height > max_height) {
                height = max_height;
                width = (height) / double(paxg::Window::height()) * double(paxg::Window::width());
            }

#ifdef PAXS_MAHOROBA
            constexpr double west_max = (208.0 / MapViewportConstants::tile_size) * MapViewportConstants::longitude_range - MapViewportConstants::longitude_max;
            constexpr double east_max = (MapViewportConstants::tile_size / MapViewportConstants::tile_size) * MapViewportConstants::longitude_range - MapViewportConstants::longitude_max;
            // 位置調整
            if (center.getX() - width / 2 < west_max) {
                center.setX(west_max + width / 2);
            }
            if (center.getX() + width / 2 > east_max) {
                center.setX(east_max - width / 2);
            }

            constexpr double north_max = (1.0 - (80.0 / MapViewportConstants::tile_size)) * MapViewportConstants::longitude_range - MapViewportConstants::longitude_max;
            constexpr double south_max = (1.0 - (128.0 / MapViewportConstants::tile_size)) * MapViewportConstants::longitude_range - MapViewportConstants::longitude_max;
#else
            constexpr double north_max = (1.0 - (0.0 / MapViewportConstants::tile_size)) * MapViewportConstants::longitude_range - MapViewportConstants::longitude_max;
            constexpr double south_max = (1.0 - (MapViewportConstants::tile_size / MapViewportConstants::tile_size)) * MapViewportConstants::longitude_range - MapViewportConstants::longitude_max;
#endif
            // 位置調整
            if (center.getY() + height / 2 > north_max) {
                center.setY(north_max - height / 2);
            }
            if (center.getY() - height / 2 < south_max) {
                center.setY(south_max + height / 2);
            }



        } // Update

        void setWidth(const double width_) {
            width = width_;
        }
        void setHeight(const double height_) {
            height = height_;
        }
        void setCenterX(const double x_) {
            center.setX(x_);
        }
        void setCenterY(const double y_) {
            center.setY(y_);
        }

        Coordinate& getCoordinate() {
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
    };

}

#endif // !PAX_MAHOROBA_MAP_VIEWPORT_HPP
