/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SIV3D_KEY_HPP
#define PAX_SAPIENTICA_SIV3D_KEY_HPP

#include <algorithm>
#include <array>
#include <cmath>
#include <memory>
#include <new>
#include <vector>

#if defined(PAXS_USING_SIV3D)
#include <Siv3D.hpp>
#elif defined(PAXS_USING_DXLIB)
#include <DxLib.h>
#elif defined(PAXS_USING_SFML)
#include <SFML/Graphics.hpp>
#endif

#include <PAX_GRAPHICA/Mouse.hpp>
#include <PAX_GRAPHICA/Window.hpp>

#include <PAX_SAPIENTICA/Type/Vector2.hpp>
#include <PAX_SAPIENTICA/MapProjection.hpp>

namespace paxs {

    // Coordinate constants
    namespace CoordinateConstants {
        // Longitude bounds
        static constexpr double longitude_min = -180.0;
        static constexpr double longitude_max = 180.0;
        static constexpr double longitude_range = 360.0;
    }

    struct InputStruct {
        InputStruct() = default;

#if defined(PAXS_USING_SIV3D)
        using InputType = s3d::Input;
        InputType input{};
        InputStruct(const InputType& key) : input(key) {}

        bool pressed() const {
            return paxg::Window::hasFocus() && input.pressed();
        }

#elif defined(PAXS_USING_DXLIB)
        using InputType = int;
        InputType input{ -1 };
        InputStruct(InputType key) : input(key) {}

        bool pressed() const {
            return paxg::Window::hasFocus() && DxLib::CheckHitKey(input) == 1;
        }

#elif defined(PAXS_USING_SFML)
        using InputType = sf::Keyboard::Key;
        InputType input{ static_cast<InputType>(-1) };
        InputStruct(InputType key) : input(key) {}

        bool pressed() const {
            return paxg::Window::hasFocus() && sf::Keyboard::isKeyPressed(input);
        }

#else
        using InputType = int;
        InputType input{ -1 };

        bool pressed() const {
            return false;
        }
#endif
    };

#define PAXG_KEY_LIST(X) \
        X(0) X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8) X(9) \
        X(A) X(B) X(C) X(D) X(E) X(F) X(G) X(H) X(I) X(J) \
        X(K) X(L) X(M) X(N) X(O) X(P) X(Q) X(R) X(S) X(T) \
        X(U) X(V) X(W) X(X) X(Y) X(Z) \
        X(LEFT) X(RIGHT) X(DOWN) X(UP)

#if defined(PAXS_USING_SIV3D)
    using InputKey = s3d::Input;

#define PAXG_DEFINE_KEY_COMMON(name) inline constexpr InputKey PAXG_KEY_##name = s3d::Key##name;
    PAXG_DEFINE_KEY_COMMON(0) PAXG_DEFINE_KEY_COMMON(1) PAXG_DEFINE_KEY_COMMON(2) PAXG_DEFINE_KEY_COMMON(3)
    PAXG_DEFINE_KEY_COMMON(4) PAXG_DEFINE_KEY_COMMON(5) PAXG_DEFINE_KEY_COMMON(6) PAXG_DEFINE_KEY_COMMON(7)
    PAXG_DEFINE_KEY_COMMON(8) PAXG_DEFINE_KEY_COMMON(9)
    PAXG_DEFINE_KEY_COMMON(A) PAXG_DEFINE_KEY_COMMON(B) PAXG_DEFINE_KEY_COMMON(C) PAXG_DEFINE_KEY_COMMON(D)
    PAXG_DEFINE_KEY_COMMON(E) PAXG_DEFINE_KEY_COMMON(F) PAXG_DEFINE_KEY_COMMON(G) PAXG_DEFINE_KEY_COMMON(H)
    PAXG_DEFINE_KEY_COMMON(I) PAXG_DEFINE_KEY_COMMON(J) PAXG_DEFINE_KEY_COMMON(K) PAXG_DEFINE_KEY_COMMON(L)
    PAXG_DEFINE_KEY_COMMON(M) PAXG_DEFINE_KEY_COMMON(N) PAXG_DEFINE_KEY_COMMON(O) PAXG_DEFINE_KEY_COMMON(P)
    PAXG_DEFINE_KEY_COMMON(Q) PAXG_DEFINE_KEY_COMMON(R) PAXG_DEFINE_KEY_COMMON(S) PAXG_DEFINE_KEY_COMMON(T)
    PAXG_DEFINE_KEY_COMMON(U) PAXG_DEFINE_KEY_COMMON(V) PAXG_DEFINE_KEY_COMMON(W) PAXG_DEFINE_KEY_COMMON(X)
    PAXG_DEFINE_KEY_COMMON(Y) PAXG_DEFINE_KEY_COMMON(Z)
    constexpr InputKey PAXG_KEY_LEFT = s3d::KeyLeft;
    constexpr InputKey PAXG_KEY_RIGHT = s3d::KeyRight;
    constexpr InputKey PAXG_KEY_DOWN = s3d::KeyDown;
    constexpr InputKey PAXG_KEY_UP = s3d::KeyUp;
#undef PAXG_DEFINE_KEY_COMMON

#elif defined(PAXS_USING_DXLIB)
    using InputKey = InputStruct;

#define PAXG_DXLIB_KEY(name) \
    inline const InputKey PAXG_KEY_##name = InputKey{ KEY_INPUT_##name };

    PAXG_DXLIB_KEY(0)  PAXG_DXLIB_KEY(1)  PAXG_DXLIB_KEY(2)  PAXG_DXLIB_KEY(3)
    PAXG_DXLIB_KEY(4)  PAXG_DXLIB_KEY(5)  PAXG_DXLIB_KEY(6)  PAXG_DXLIB_KEY(7)
    PAXG_DXLIB_KEY(8)  PAXG_DXLIB_KEY(9)
    PAXG_DXLIB_KEY(A)  PAXG_DXLIB_KEY(B)  PAXG_DXLIB_KEY(C)  PAXG_DXLIB_KEY(D)
    PAXG_DXLIB_KEY(E)  PAXG_DXLIB_KEY(F)  PAXG_DXLIB_KEY(G)  PAXG_DXLIB_KEY(H)
    PAXG_DXLIB_KEY(I)  PAXG_DXLIB_KEY(J)  PAXG_DXLIB_KEY(K)  PAXG_DXLIB_KEY(L)
    PAXG_DXLIB_KEY(M)  PAXG_DXLIB_KEY(N)  PAXG_DXLIB_KEY(O)  PAXG_DXLIB_KEY(P)
    PAXG_DXLIB_KEY(Q)  PAXG_DXLIB_KEY(R)  PAXG_DXLIB_KEY(S)  PAXG_DXLIB_KEY(T)
    PAXG_DXLIB_KEY(U)  PAXG_DXLIB_KEY(V)  PAXG_DXLIB_KEY(W)  PAXG_DXLIB_KEY(X)
    PAXG_DXLIB_KEY(Y)  PAXG_DXLIB_KEY(Z)
    PAXG_DXLIB_KEY(LEFT)
    PAXG_DXLIB_KEY(RIGHT)
    PAXG_DXLIB_KEY(DOWN)
    PAXG_DXLIB_KEY(UP)
#undef PAXG_DXLIB_KEY

#elif defined(PAXS_USING_SFML)
    using InputKey = InputStruct;

    inline const InputKey PAXG_KEY_0 = InputKey{sf::Keyboard::Key::Num0};
    inline const InputKey PAXG_KEY_1 = InputKey{sf::Keyboard::Key::Num1};
    inline const InputKey PAXG_KEY_2 = InputKey{sf::Keyboard::Key::Num2};
    inline const InputKey PAXG_KEY_3 = InputKey{sf::Keyboard::Key::Num3};
    inline const InputKey PAXG_KEY_4 = InputKey{sf::Keyboard::Key::Num4};
    inline const InputKey PAXG_KEY_5 = InputKey{sf::Keyboard::Key::Num5};
    inline const InputKey PAXG_KEY_6 = InputKey{sf::Keyboard::Key::Num6};
    inline const InputKey PAXG_KEY_7 = InputKey{sf::Keyboard::Key::Num7};
    inline const InputKey PAXG_KEY_8 = InputKey{sf::Keyboard::Key::Num8};
    inline const InputKey PAXG_KEY_9 = InputKey{sf::Keyboard::Key::Num9};
#define PAXG_SFML_ALPHA(name) inline const InputKey PAXG_KEY_##name = InputKey{ sf::Keyboard::Key::name };
    PAXG_SFML_ALPHA(A) PAXG_SFML_ALPHA(B) PAXG_SFML_ALPHA(C) PAXG_SFML_ALPHA(D)
    PAXG_SFML_ALPHA(E) PAXG_SFML_ALPHA(F) PAXG_SFML_ALPHA(G) PAXG_SFML_ALPHA(H)
    PAXG_SFML_ALPHA(I) PAXG_SFML_ALPHA(J) PAXG_SFML_ALPHA(K) PAXG_SFML_ALPHA(L)
    PAXG_SFML_ALPHA(M) PAXG_SFML_ALPHA(N) PAXG_SFML_ALPHA(O) PAXG_SFML_ALPHA(P)
    PAXG_SFML_ALPHA(Q) PAXG_SFML_ALPHA(R) PAXG_SFML_ALPHA(S) PAXG_SFML_ALPHA(T)
    PAXG_SFML_ALPHA(U) PAXG_SFML_ALPHA(V) PAXG_SFML_ALPHA(W) PAXG_SFML_ALPHA(X)
    PAXG_SFML_ALPHA(Y) PAXG_SFML_ALPHA(Z)
#undef PAXG_SFML_ALPHA
    inline const InputKey PAXG_KEY_LEFT = InputKey{sf::Keyboard::Key::Left};
    inline const InputKey PAXG_KEY_RIGHT = InputKey{sf::Keyboard::Key::Right};
    inline const InputKey PAXG_KEY_DOWN = InputKey{sf::Keyboard::Key::Down};
    inline const InputKey PAXG_KEY_UP = InputKey{sf::Keyboard::Key::Up};

#else
    using InputKey = InputStruct;

    // Null implementation
    #define PAXG_DEFINE_KEY(name) inline const InputKey PAXG_KEY_##name = InputStruct{};
    PAXG_KEY_LIST(PAXG_DEFINE_KEY)
    #undef PAXG_DEFINE_KEY

#endif

#undef PAXG_KEY_LIST

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

    template<std::size_t N>
    bool pressed(const std::array<Key, N>& keys) {
        for (const auto& key : keys) {
            if (key.isPressed()) return true;
        }
        return false;
    }

    class Coordinate {
    public:
        double movement_size; // マップの移動量

        Coordinate(double movement_size, const paxs::MercatorDeg& coordinate_)
            : movement_size(movement_size),
              coordinate(coordinate_),
              increase_x_keys{Key(PAXG_KEY_A), Key(PAXG_KEY_LEFT)},
              decrease_x_keys{Key(PAXG_KEY_D), Key(PAXG_KEY_RIGHT)},
              increase_y_keys{Key(PAXG_KEY_S), Key(PAXG_KEY_DOWN)},
              decrease_y_keys{Key(PAXG_KEY_W), Key(PAXG_KEY_UP)}
        {
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
        std::array<Key, 2> increase_x_keys;
        std::array<Key, 2> decrease_x_keys;
        std::array<Key, 2> increase_y_keys;
        std::array<Key, 2> decrease_y_keys;

        void increase_coordinate(double& value, const double width) {
            value -= (width / movement_size);
            if (value < CoordinateConstants::longitude_min) {
                value += CoordinateConstants::longitude_range;
            }
        }
        void decrease_coordinate(double& value, const double width) {
            value += (width / movement_size);
            if (value >= CoordinateConstants::longitude_max) {
                value -= CoordinateConstants::longitude_range;
            }
        }
    };

}

#endif // !PAX_SAPIENTICA_SIV3D_KEY_HPP
