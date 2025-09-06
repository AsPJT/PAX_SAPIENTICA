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
        sf::Keyboard::Key input{ -1 };
        InputStruct(sf::Keyboard::Key key) :input(key) {}

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

#define SIV3D_KEY_0 s3d::Key0
#define SIV3D_KEY_1 s3d::Key1
#define SIV3D_KEY_2 s3d::Key2
#define SIV3D_KEY_3 s3d::Key3
#define SIV3D_KEY_4 s3d::Key4
#define SIV3D_KEY_5 s3d::Key5
#define SIV3D_KEY_6 s3d::Key6
#define SIV3D_KEY_7 s3d::Key7
#define SIV3D_KEY_8 s3d::Key8
#define SIV3D_KEY_9 s3d::Key9
#define SIV3D_KEY_A s3d::KeyA
#define SIV3D_KEY_B s3d::KeyB
#define SIV3D_KEY_C s3d::KeyC
#define SIV3D_KEY_D s3d::KeyD
#define SIV3D_KEY_E s3d::KeyE
#define SIV3D_KEY_F s3d::KeyF
#define SIV3D_KEY_G s3d::KeyG
#define SIV3D_KEY_H s3d::KeyH
#define SIV3D_KEY_I s3d::KeyI
#define SIV3D_KEY_J s3d::KeyJ
#define SIV3D_KEY_K s3d::KeyK
#define SIV3D_KEY_L s3d::KeyL
#define SIV3D_KEY_M s3d::KeyM
#define SIV3D_KEY_N s3d::KeyN
#define SIV3D_KEY_O s3d::KeyO
#define SIV3D_KEY_P s3d::KeyP
#define SIV3D_KEY_Q s3d::KeyQ
#define SIV3D_KEY_R s3d::KeyR
#define SIV3D_KEY_S s3d::KeyS
#define SIV3D_KEY_T s3d::KeyT
#define SIV3D_KEY_U s3d::KeyU
#define SIV3D_KEY_V s3d::KeyV
#define SIV3D_KEY_W s3d::KeyW
#define SIV3D_KEY_X s3d::KeyX
#define SIV3D_KEY_Y s3d::KeyY
#define SIV3D_KEY_Z s3d::KeyZ
#define SIV3D_KEY_LEFT s3d::KeyLeft
#define SIV3D_KEY_RIGHT s3d::KeyRight
#define SIV3D_KEY_DOWN s3d::KeyDown
#define SIV3D_KEY_UP s3d::KeyUp

#elif defined(PAXS_USING_DXLIB)
    using InputKey = InputStruct;

#define SIV3D_KEY_0 InputKey{KEY_INPUT_0}
#define SIV3D_KEY_1 InputKey{KEY_INPUT_1}
#define SIV3D_KEY_2 InputKey{KEY_INPUT_2}
#define SIV3D_KEY_3 InputKey{KEY_INPUT_3}
#define SIV3D_KEY_4 InputKey{KEY_INPUT_4}
#define SIV3D_KEY_5 InputKey{KEY_INPUT_5}
#define SIV3D_KEY_6 InputKey{KEY_INPUT_6}
#define SIV3D_KEY_7 InputKey{KEY_INPUT_7}
#define SIV3D_KEY_8 InputKey{KEY_INPUT_8}
#define SIV3D_KEY_9 InputKey{KEY_INPUT_9}
#define SIV3D_KEY_A InputKey{KEY_INPUT_A}
#define SIV3D_KEY_B InputKey{KEY_INPUT_B}
#define SIV3D_KEY_C InputKey{KEY_INPUT_C}
#define SIV3D_KEY_D InputKey{KEY_INPUT_D}
#define SIV3D_KEY_E InputKey{KEY_INPUT_E}
#define SIV3D_KEY_F InputKey{KEY_INPUT_F}
#define SIV3D_KEY_G InputKey{KEY_INPUT_G}
#define SIV3D_KEY_H InputKey{KEY_INPUT_H}
#define SIV3D_KEY_I InputKey{KEY_INPUT_I}
#define SIV3D_KEY_J InputKey{KEY_INPUT_J}
#define SIV3D_KEY_K InputKey{KEY_INPUT_K}
#define SIV3D_KEY_L InputKey{KEY_INPUT_L}
#define SIV3D_KEY_M InputKey{KEY_INPUT_M}
#define SIV3D_KEY_N InputKey{KEY_INPUT_N}
#define SIV3D_KEY_O InputKey{KEY_INPUT_O}
#define SIV3D_KEY_P InputKey{KEY_INPUT_P}
#define SIV3D_KEY_Q InputKey{KEY_INPUT_Q}
#define SIV3D_KEY_R InputKey{KEY_INPUT_R}
#define SIV3D_KEY_S InputKey{KEY_INPUT_S}
#define SIV3D_KEY_T InputKey{KEY_INPUT_T}
#define SIV3D_KEY_U InputKey{KEY_INPUT_U}
#define SIV3D_KEY_V InputKey{KEY_INPUT_V}
#define SIV3D_KEY_W InputKey{KEY_INPUT_W}
#define SIV3D_KEY_X InputKey{KEY_INPUT_X}
#define SIV3D_KEY_Y InputKey{KEY_INPUT_Y}
#define SIV3D_KEY_Z InputKey{KEY_INPUT_Z}
#define SIV3D_KEY_LEFT InputKey{KEY_INPUT_LEFT}
#define SIV3D_KEY_RIGHT InputKey{KEY_INPUT_RIGHT}
#define SIV3D_KEY_DOWN InputKey{KEY_INPUT_DOWN}
#define SIV3D_KEY_UP InputKey{KEY_INPUT_UP}

#elif defined(PAXS_USING_SFML)
    using InputKey = InputStruct;

#define SIV3D_KEY_0 InputKey{sf::Keyboard::Key::Num0}
#define SIV3D_KEY_1 InputKey{sf::Keyboard::Key::Num1}
#define SIV3D_KEY_2 InputKey{sf::Keyboard::Key::Num2}
#define SIV3D_KEY_3 InputKey{sf::Keyboard::Key::Num3}
#define SIV3D_KEY_4 InputKey{sf::Keyboard::Key::Num4}
#define SIV3D_KEY_5 InputKey{sf::Keyboard::Key::Num5}
#define SIV3D_KEY_6 InputKey{sf::Keyboard::Key::Num6}
#define SIV3D_KEY_7 InputKey{sf::Keyboard::Key::Num7}
#define SIV3D_KEY_8 InputKey{sf::Keyboard::Key::Num8}
#define SIV3D_KEY_9 InputKey{sf::Keyboard::Key::Num9}
#define SIV3D_KEY_A InputKey{sf::Keyboard::Key::A}
#define SIV3D_KEY_B InputKey{sf::Keyboard::Key::B}
#define SIV3D_KEY_C InputKey{sf::Keyboard::Key::C}
#define SIV3D_KEY_D InputKey{sf::Keyboard::Key::D}
#define SIV3D_KEY_E InputKey{sf::Keyboard::Key::E}
#define SIV3D_KEY_F InputKey{sf::Keyboard::Key::F}
#define SIV3D_KEY_G InputKey{sf::Keyboard::Key::G}
#define SIV3D_KEY_H InputKey{sf::Keyboard::Key::H}
#define SIV3D_KEY_I InputKey{sf::Keyboard::Key::I}
#define SIV3D_KEY_J InputKey{sf::Keyboard::Key::J}
#define SIV3D_KEY_K InputKey{sf::Keyboard::Key::K}
#define SIV3D_KEY_L InputKey{sf::Keyboard::Key::L}
#define SIV3D_KEY_M InputKey{sf::Keyboard::Key::M}
#define SIV3D_KEY_N InputKey{sf::Keyboard::Key::N}
#define SIV3D_KEY_O InputKey{sf::Keyboard::Key::O}
#define SIV3D_KEY_P InputKey{sf::Keyboard::Key::P}
#define SIV3D_KEY_Q InputKey{sf::Keyboard::Key::Q}
#define SIV3D_KEY_R InputKey{sf::Keyboard::Key::R}
#define SIV3D_KEY_S InputKey{sf::Keyboard::Key::S}
#define SIV3D_KEY_T InputKey{sf::Keyboard::Key::T}
#define SIV3D_KEY_U InputKey{sf::Keyboard::Key::U}
#define SIV3D_KEY_V InputKey{sf::Keyboard::Key::V}
#define SIV3D_KEY_W InputKey{sf::Keyboard::Key::W}
#define SIV3D_KEY_X InputKey{sf::Keyboard::Key::X}
#define SIV3D_KEY_Y InputKey{sf::Keyboard::Key::Y}
#define SIV3D_KEY_Z InputKey{sf::Keyboard::Key::Z}
#define SIV3D_KEY_LEFT InputKey{sf::Keyboard::Key::Left}
#define SIV3D_KEY_RIGHT InputKey{sf::Keyboard::Key::Right}
#define SIV3D_KEY_DOWN InputKey{sf::Keyboard::Key::Down}
#define SIV3D_KEY_UP InputKey{sf::Keyboard::Key::Up}

#else
    using InputKey = InputStruct;

#define SIV3D_KEY_0 InputStruct{}
#define SIV3D_KEY_1 InputStruct{}
#define SIV3D_KEY_2 InputStruct{}
#define SIV3D_KEY_3 InputStruct{}
#define SIV3D_KEY_4 InputStruct{}
#define SIV3D_KEY_5 InputStruct{}
#define SIV3D_KEY_6 InputStruct{}
#define SIV3D_KEY_7 InputStruct{}
#define SIV3D_KEY_8 InputStruct{}
#define SIV3D_KEY_9 InputStruct{}
#define SIV3D_KEY_A InputStruct{}
#define SIV3D_KEY_B InputStruct{}
#define SIV3D_KEY_C InputStruct{}
#define SIV3D_KEY_D InputStruct{}
#define SIV3D_KEY_E InputStruct{}
#define SIV3D_KEY_F InputStruct{}
#define SIV3D_KEY_G InputStruct{}
#define SIV3D_KEY_H InputStruct{}
#define SIV3D_KEY_I InputStruct{}
#define SIV3D_KEY_J InputStruct{}
#define SIV3D_KEY_K InputStruct{}
#define SIV3D_KEY_L InputStruct{}
#define SIV3D_KEY_M InputStruct{}
#define SIV3D_KEY_N InputStruct{}
#define SIV3D_KEY_O InputStruct{}
#define SIV3D_KEY_P InputStruct{}
#define SIV3D_KEY_Q InputStruct{}
#define SIV3D_KEY_R InputStruct{}
#define SIV3D_KEY_S InputStruct{}
#define SIV3D_KEY_T InputStruct{}
#define SIV3D_KEY_U InputStruct{}
#define SIV3D_KEY_V InputStruct{}
#define SIV3D_KEY_W InputStruct{}
#define SIV3D_KEY_X InputStruct{}
#define SIV3D_KEY_Y InputStruct{}
#define SIV3D_KEY_Z InputStruct{}
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

    //enum class KeyCode {
    //    A,
    //    Left,
    //    D,
    //    Right,
    //    S,
    //    Down,
    //    W,
    //    Up,
    //    Q,
    //    E
    //};

    bool pressed(const std::vector<Key>& keys) {
        for (auto& key : keys) {
            if (key.isPressed()) return true;
        }
        return false;
    }

    class Coordinate {
    public:
        double movement_size; // マップの移動量

        Coordinate(double movement_size, const paxs::MercatorDeg& coordinate_) : movement_size(movement_size), coordinate(coordinate_) {
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
            200.0,
            paxs::EquirectangularDeg(paxs::Vector2<double>(145, 48)) // 韓国 128, 37 // 日本 135, 35 // 北海道 // 東アジア 127, 31, 75.0 // 全世界 100, 0
            //paxs::Vector2(135.0, getLatitudeToMercatorY(35.0))
        ); // マップ座標の中央
        double height = 24.0; // 各国 16.0; // 全世界 240.0 // マップの高さ

        // 平城京
        //Coordinate center = Coordinate(135.807, 37.009/*getLatitudeToMercatorY(35)*/, 200.0); // マップ座標の中央
        //double width = 0.1; // マップの幅

        // マップの最大高さ
        double max_height =
#ifdef PAXS_MAHOROBA
            30.0;
#else
            240.0;
#endif
        double min_height = 0.005; // マップの最小高さ
        double width = (height) / double(paxg::Window::height()) * double(paxg::Window::width()); // マップの高さ
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
                height *= (1.0 + (paxg::Mouse::getInstance()->getWheelRotVol() / 10.0));
                height = (std::clamp)(height, min_height, max_height);
                width = (height) / double(paxg::Window::height()) * double(paxg::Window::width());
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
                    height / static_cast<double>(paxg::Window::height()) *
                    static_cast<double>(paxg::Mouse::getInstance()->getPosXBefore1Frame() - paxg::Mouse::getInstance()->getPosX()));
                center.setY(
                    center.getY() +
                    height / static_cast<double>(paxg::Window::height()) *
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
                    height / static_cast<double>(paxg::Window::height()) *
                    static_cast<double>(old_pos[0].x - pos[0].x));
                center.setY(center.getY() +
                    height / static_cast<double>(paxg::Window::height()) *
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
                    if (height > min_height) {
                        height -= ((height * (1.0 + (sub / 6000.0))) / expansion_size);
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
                        height += ((height * (1.0 + (sub / 6000.0))) / expansion_size);
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
            constexpr double west_max = (208.0 / 256.0) * 360.0 - 180.0;
            constexpr double east_max = (256.0 / 256.0) * 360.0 - 180.0;
            // 位置調整
            if (center.getX() - width / 2 < west_max) {
                center.setX(west_max + width / 2);
            }
            if (center.getX() + width / 2 > east_max) {
                center.setX(east_max - width / 2);
            }

            constexpr double north_max = (1.0 - (80.0 / 256.0)) * 360.0 - 180.0;
            constexpr double south_max = (1.0 - (128.0 / 256.0)) * 360.0 - 180.0;
#else
            constexpr double north_max = (1.0 - (0.0 / 256.0)) * 360.0 - 180.0;
            constexpr double south_max = (1.0 - (256.0 / 256.0)) * 360.0 - 180.0;
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

#endif // !PAX_SAPIENTICA_SIV3D_KEY_HPP
