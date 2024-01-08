/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SIV3D_KEY_HPP
#define PAX_SAPIENTICA_SIV3D_KEY_HPP

/*##########################################################################################

##########################################################################################*/

#include <algorithm>
#include <array>
#include <cmath>
#include <memory>
#include <new>
#include <vector>

#include <PAX_SAPIENTICA/Type/Vector2.hpp>
#include <PAX_SAPIENTICA/MapProjection.hpp> // 地図投影法

#include <PAX_GRAPHICA/Mouse.hpp>
#include <PAX_GRAPHICA/Window.hpp>

namespace paxs {

    struct InputStruct {
        InputStruct() = default;
#if defined(PAXS_USING_SIV3D)
        s3d::Input input{};
        InputStruct(s3d::Input& key) :input(key) {}

#elif defined(PAXS_USING_DXLIB)
        int input{ -1 };
        InputStruct(int key) :input(key) {}

#elif defined(PAXS_USING_SFML)
        int input{ -1 };
        InputStruct(int key) :input(key) {}

#endif
        bool pressed() const {
#if defined(PAXS_USING_SIV3D)
            return input.pressed();

#elif defined(PAXS_USING_DXLIB)
            return DxLib::CheckHitKey(input) == 1;

#elif defined(PAXS_USING_SFML)
            return sf::Keyboard::isKeyPressed((sf::Keyboard::Key)input);

#else
            return false;
#endif

        }


    };

    // inline static InputStruct KeyE{};

#if defined(PAXS_USING_SIV3D)
    using InputKey = s3d::Input;

#define SIV3D_KEY_E s3d::KeyE
#define SIV3D_KEY_Q s3d::KeyQ
#define SIV3D_KEY_A s3d::KeyA
#define SIV3D_KEY_D s3d::KeyD
#define SIV3D_KEY_S s3d::KeyS
#define SIV3D_KEY_W s3d::KeyW
#define SIV3D_KEY_LEFT s3d::KeyLeft
#define SIV3D_KEY_RIGHT s3d::KeyRight
#define SIV3D_KEY_DOWN s3d::KeyDown
#define SIV3D_KEY_UP s3d::KeyUp

#elif defined(PAXS_USING_DXLIB)
    using InputKey = InputStruct;

#define SIV3D_KEY_E InputKey{KEY_INPUT_E}
#define SIV3D_KEY_Q InputKey{KEY_INPUT_Q}
#define SIV3D_KEY_A InputKey{KEY_INPUT_A}
#define SIV3D_KEY_D InputKey{KEY_INPUT_D}
#define SIV3D_KEY_S InputKey{KEY_INPUT_S}
#define SIV3D_KEY_W InputKey{KEY_INPUT_W}
#define SIV3D_KEY_LEFT InputKey{KEY_INPUT_LEFT}
#define SIV3D_KEY_RIGHT InputKey{KEY_INPUT_RIGHT}
#define SIV3D_KEY_DOWN InputKey{KEY_INPUT_DOWN}
#define SIV3D_KEY_UP InputKey{KEY_INPUT_UP}

#elif defined(PAXS_USING_SFML)
    using InputKey = InputStruct;

#define SIV3D_KEY_E InputKey{sf::Keyboard::E}
#define SIV3D_KEY_Q InputKey{sf::Keyboard::Q}
#define SIV3D_KEY_A InputKey{sf::Keyboard::A}
#define SIV3D_KEY_D InputKey{sf::Keyboard::D}
#define SIV3D_KEY_S InputKey{sf::Keyboard::S}
#define SIV3D_KEY_W InputKey{sf::Keyboard::W}
#define SIV3D_KEY_LEFT InputKey{sf::Keyboard::Left}
#define SIV3D_KEY_RIGHT InputKey{sf::Keyboard::Right}
#define SIV3D_KEY_DOWN InputKey{sf::Keyboard::Down}
#define SIV3D_KEY_UP InputKey{sf::Keyboard::Up}

#else
    using InputKey = InputStruct;

#define SIV3D_KEY_E InputStruct{}
#define SIV3D_KEY_Q InputStruct{}
#define SIV3D_KEY_A InputStruct{}
#define SIV3D_KEY_D InputStruct{}
#define SIV3D_KEY_S InputStruct{}
#define SIV3D_KEY_W InputStruct{}
#define SIV3D_KEY_LEFT InputStruct{}
#define SIV3D_KEY_RIGHT InputStruct{}
#define SIV3D_KEY_DOWN InputStruct{}
#define SIV3D_KEY_UP InputStruct{}

#endif

    class Key {
    public:
        Key() = default;
        explicit Key(InputKey key) : key(key) {}
        bool isPressed() const {
            return key.pressed();
        }
    private:
        InputKey key{};
    };

    enum class KeyCode {
        A,
        Left,
        D,
        Right,
        S,
        Down,
        W,
        Up,
        Q,
        E
    };

    bool pressed(const std::vector<Key>& keys) {
        for (auto& key : keys) {
            if (key.isPressed()) return true;
        }
        return false;
    }

    class Coordinate {
    public:
        double movement_size; // マップの移動量

        Coordinate(const paxs::MercatorDeg& coordinate_, double movement_size) : coordinate(coordinate_), movement_size(movement_size) {
            increase_x_keys.resize(2);
            increase_x_keys[0] = (Key(SIV3D_KEY_A));
            increase_x_keys[1] = (Key(SIV3D_KEY_LEFT));

            decrease_x_keys.resize(2);
            decrease_x_keys[0] = (Key(SIV3D_KEY_D));
            decrease_x_keys[1] = (Key(SIV3D_KEY_RIGHT));

            increase_y_keys.resize(2);
            increase_y_keys[0] = (Key(SIV3D_KEY_S));
            increase_y_keys[1] = (Key(SIV3D_KEY_DOWN));

            decrease_y_keys.resize(2);
            decrease_y_keys[0] = (Key(SIV3D_KEY_W));
            decrease_y_keys[1] = (Key(SIV3D_KEY_UP));
        }
        void update(const double width) {
            if (pressed(increase_x_keys)) increase_coordinate(coordinate.x, width);
            if (pressed(decrease_x_keys)) decrease_coordinate(coordinate.x, width);
            if (pressed(increase_y_keys)) increase_coordinate(coordinate.y, width);
            if (pressed(decrease_y_keys)) decrease_coordinate(coordinate.y, width);
        }
        double getX() const {
            return coordinate.x;
        }
        double getY() const {
            return coordinate.y;
        }
        void setX(const double x_) {
            coordinate.x = x_;
        }
        void setY(const double y_) {
            coordinate.y = y_;
        }

        double toEquirectangularRadY() const {
            return coordinate.toEquirectangularRadY();
        }
        double toEquirectangularDegY() const {
            return coordinate.toEquirectangularDegY();
        }

    private:
        paxs::MercatorDeg coordinate; // マップ座標の中央
        std::vector<Key> increase_x_keys;
        std::vector<Key> decrease_x_keys;
        std::vector<Key> increase_y_keys;
        std::vector<Key> decrease_y_keys;

        void increase_coordinate(double& value, const double width) {
            value -= (width / movement_size);
            if (value < -180.0) {
                value += 360.0;
            }
        }
        void decrease_coordinate(double& value, const double width) {
            value += (width / movement_size);
            if (value >= 180.0) {
                value -= 360.0;
            }
        }
    };
    // GUI で表示する地図の範囲を管理するクラス
    class MapView {
    private:
        // 中央の座標を指定
        Coordinate center = Coordinate(
            paxs::EquirectangularDeg(paxs::Vector2<double>(135, 35)), // 韓国 128, 37
            //paxs::Vector2(135.0, getLatitudeToMercatorY(35.0)),
            200.0); // マップ座標の中央
        double width = 12.0; // マップの幅

        // 平城京
        //Coordinate center = Coordinate(135.807, 37.009/*getLatitudeToMercatorY(35)*/, 200.0); // マップ座標の中央
        //double width = 0.1; // マップの幅
        double max_width = 640.0; // マップの最大幅
        double min_width = 0.00005; // マップの最小幅
        double height = (width) / double(paxg::Window::width()) * double(paxg::Window::height()); // マップの高さ
        double expansion_size = 50.0; // マップの拡大量
        std::vector<Key> enl_keys; // 拡大キー
        std::vector<Key> esc_keys; // 縮小キー

#ifdef __ANDROID__
        int touch_num = 0;
        int old_touch_num = 0;
        std::array<paxs::Vector2<int>, 3> pos;
        std::array<paxs::Vector2<int>, 3> old_pos;
#endif


    public:
        MapView() {
            enl_keys.resize(1);
            enl_keys[0] = (Key(SIV3D_KEY_Q));

            esc_keys.resize(1);
            esc_keys[0] = (Key(SIV3D_KEY_E));
        }
        void update() {
            // マウスホイールで地図の拡大・縮小
            {
                width *= (1.0 + (paxg::Mouse::getInstance()->getWheelRotVol() / 10.0));
                width = (std::clamp)(width, min_width, max_width);
                height = (width) / double(paxg::Window::width()) * double(paxg::Window::height());
            }
#ifdef __ANDROID__
            static int old_touch_num = 0;
            static int touch_num = 0;
            static std::array<paxs::Vector2<int>, 3> pos = { paxs::Vector2<int>{0,0},paxs::Vector2<int>{0,0},paxs::Vector2<int>{0,0} };
            static std::array<paxs::Vector2<int>, 3> old_pos = { paxs::Vector2<int>{0,0},paxs::Vector2<int>{0,0},paxs::Vector2<int>{0,0} };
#else
            if (paxg::Mouse::getInstance()->pressedLeft2()) {
                center.setX(
                    center.getX() +
                    width / static_cast<double>(paxg::Window::width()) *
                    static_cast<double>(paxg::Mouse::getInstance()->getPosXBefore1Frame() - paxg::Mouse::getInstance()->getPosX()));
                center.setY(
                    center.getY() +
                    width / static_cast<double>(paxg::Window::width()) *
                        static_cast<double>(paxg::Mouse::getInstance()->getPosY() - paxg::Mouse::getInstance()->getPosYBefore1Frame()));

                if (center.getX() < -180.0) {
                    center.setX(center.getX() + 360.0);
                }
                if (center.getX() >= 180.0) {
                    center.setX(center.getX() - 360.0);
                }
                if (center.getY() < -180.0) {
                    center.setY(center.getY() - 180.0);
                }
                if (center.getY() > 180.0) {
                    center.setY(center.getY() + 180.0);
                }

            }
#endif

#ifdef __ANDROID__
            old_touch_num = touch_num;
            old_pos = pos;

            touch_num = DxLib::GetTouchInputNum();


            for (int i = 0; i < touch_num; i++) {
                if (i >= 3) break;
                int pos_x = 0, pos_y = 0;

                DxLib::GetTouchInput(i, &pos_x, &pos_y, NULL, NULL);
                pos[i] = Vector2<int>(pos_x, pos_y);

                DxLib::DrawCircle(pos_x, pos_y, 40, GetColor(230, 230, 240), TRUE);
            }

            if (old_touch_num == 1 && touch_num == 1) {
                center.setX(center.getX() +
                    width / static_cast<double>(paxg::Window::width()) *
                    static_cast<double>(old_pos[0].x - pos[0].x));
                center.setY(center.getY() +
                    width / static_cast<double>(paxg::Window::width()) *
                    static_cast<double>(pos[0].y - old_pos[0].y));

                if (center.getX() < -180.0) {
                    center.setX(center.getX() + 360.0);
                }
                if (center.getX() >= 180.0) {
                    center.setX(center.getX() - 360.0);
                }
                if (center.getY() < -180.0) {
                    center.setY(center.getY() - 180.0);
                }
                if (center.getY() > 180.0) {
                    center.setY(center.getY() + 180.0);
                }

            }
            else if (old_touch_num == 2 && touch_num == 2) {

                const int len = (pos[0].x - pos[1].x) * (pos[0].x - pos[1].x) + (pos[0].y - pos[1].y) * (pos[0].y - pos[1].y);
                const int old_len = (old_pos[0].x - old_pos[1].x) * (old_pos[0].x - old_pos[1].x) + (old_pos[0].y - old_pos[1].y) * (old_pos[0].y - old_pos[1].y);
                const int sub = std::abs(len - old_len);

                if (len > old_len) {
                    if (width > min_width) {
                        width -= ((width * (1.0 + (sub / 6000.0))) / expansion_size);
                        height = (width) / double(paxg::Window::width()) * double(paxg::Window::height());
                    }
                    if (width < min_width) {
                        width = min_width;
                        height = (width) / double(paxg::Window::width()) * double(paxg::Window::height());
                    }
                }
                else if (len < old_len) {
                    // 画面広く
                    if (width < max_width) {
                        width += ((width * (1.0 + (sub / 6000.0))) / expansion_size);
                        height = (width) / double(paxg::Window::width()) * double(paxg::Window::height());
                    }
                    if (width > max_width) {
                        width = max_width;
                        height = (width) / double(paxg::Window::width()) * double(paxg::Window::height());
                    }
                }

            }
#endif

            center.update(width);
            if (pressed(enl_keys)) {
                if (width > min_width) {
                    width -= (width / expansion_size);
                    height = (width) / double(paxg::Window::width()) * double(paxg::Window::height());
                }
                if (width < min_width) {
                    width = min_width;
                    height = (width) / double(paxg::Window::width()) * double(paxg::Window::height());
                }
            }
            if (pressed(esc_keys)) {
                if (width < max_width) {
                    width += (width / expansion_size);
                    height = (width) / double(paxg::Window::width()) * double(paxg::Window::height());
                }
                if (width > max_width) {
                    width = max_width;
                    height = (width) / double(paxg::Window::width()) * double(paxg::Window::height());
                }
            }
        }

        void setWidth(const double width_) {
            width = width_;
        }
        void setHeight(const double height_) {
            height = height_;
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

    // キーボード入力を更新（どの入力が必要か未確定なのでまだクラス化前）
    void updateKey(
        double& map_view_center_x, // マップ座標の中央 X
        double& map_view_center_y, // マップ座標の中央 Y
        double& map_view_width, // マップの幅
        double& map_view_max_width, // マップの最大幅
        double& map_view_min_width, // マップの最小幅
        double& map_view_height, // マップの高さ
        double& map_view_movement_size, // マップの移動量
        double& map_view_expansion_size // マップの拡大量
    ) {
        // #ifdef PAXS_USING_SIV3D
                // 西へ移動
        if (SIV3D_KEY_A.pressed() || SIV3D_KEY_LEFT.pressed()) {
            map_view_center_x -= (map_view_width / map_view_movement_size);
            if (map_view_center_x < -180.0) {
                map_view_center_x += 360.0;
            }
        }
        // 東へ移動
        if (SIV3D_KEY_D.pressed() || SIV3D_KEY_RIGHT.pressed()) {
            map_view_center_x += (map_view_width / map_view_movement_size);
            if (map_view_center_x >= 180.0) {
                map_view_center_x -= 360.0;
            }
        }
        // 南へ移動
        if (SIV3D_KEY_S.pressed() || SIV3D_KEY_DOWN.pressed()) {
            map_view_center_y -= (map_view_width / map_view_movement_size);
            if (map_view_center_y < -180.0) {
                map_view_center_y = -180.0;
            }
        }
        // 北へ移動
        if (SIV3D_KEY_W.pressed() || SIV3D_KEY_UP.pressed()) {
            map_view_center_y += (map_view_width / map_view_movement_size);
            if (map_view_center_y > 180.0) {
                map_view_center_y = 180.0;
            }
        }
        // 地図を拡大
        if (SIV3D_KEY_Q.pressed()) {
            if (map_view_width > map_view_min_width) {
                map_view_width -= (map_view_width / map_view_expansion_size);
                map_view_height = (map_view_width) / double(paxg::Window::width()) * double(paxg::Window::height());
            }
            if (map_view_width < map_view_min_width) {
                map_view_width = map_view_min_width;
                map_view_height = (map_view_width) / double(paxg::Window::width()) * double(paxg::Window::height());
            }
        }
        // 地図を縮小
        if (SIV3D_KEY_E.pressed()) {
            if (map_view_width < map_view_max_width) {
                map_view_width += (map_view_width / map_view_expansion_size);
                map_view_height = (map_view_width) / double(paxg::Window::width()) * double(paxg::Window::height());
            }
            if (map_view_width > map_view_max_width) {
                map_view_width = map_view_max_width;
                map_view_height = (map_view_width) / double(paxg::Window::width()) * double(paxg::Window::height());
            }
        }
        // #endif
    }

}

#endif // !PAX_SAPIENTICA_SIV3D_KEY_HPP
