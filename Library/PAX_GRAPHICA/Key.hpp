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

    // X-Macro pattern: Define all keys once
    #define PAXG_KEY_LIST(X) \
        X(0) X(1) X(2) X(3) X(4) X(5) X(6) X(7) X(8) X(9) \
        X(A) X(B) X(C) X(D) X(E) X(F) X(G) X(H) X(I) X(J) \
        X(K) X(L) X(M) X(N) X(O) X(P) X(Q) X(R) X(S) X(T) \
        X(U) X(V) X(W) X(X) X(Y) X(Z) \
        X(LEFT) X(RIGHT) X(DOWN) X(UP)

#if defined(PAXS_USING_SIV3D)
    using InputKey = s3d::Input;

    // Siv3D: Direct mapping to s3d::Key*
    #define PAXG_DEFINE_KEY(name) constexpr InputKey PAXG_KEY_##name = s3d::Key##name;
    PAXG_KEY_LIST(PAXG_DEFINE_KEY)
    #undef PAXG_DEFINE_KEY

#elif defined(PAXS_USING_DXLIB)
    using InputKey = InputStruct;

    // DxLib: Wrap KEY_INPUT_* constants
    #define PAXG_DEFINE_KEY(name) inline const InputKey PAXG_KEY_##name = InputKey{KEY_INPUT_##name};
    PAXG_KEY_LIST(PAXG_DEFINE_KEY)
    #undef PAXG_DEFINE_KEY

#elif defined(PAXS_USING_SFML)
    using InputKey = InputStruct;

    // SFML: Map to sf::Keyboard::Key::* (with special handling for numbers)
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
    #define PAXG_DEFINE_KEY(name) inline const InputKey PAXG_KEY_##name = InputKey{sf::Keyboard::Key::name};
    PAXG_DEFINE_KEY(A) PAXG_DEFINE_KEY(B) PAXG_DEFINE_KEY(C) PAXG_DEFINE_KEY(D)
    PAXG_DEFINE_KEY(E) PAXG_DEFINE_KEY(F) PAXG_DEFINE_KEY(G) PAXG_DEFINE_KEY(H)
    PAXG_DEFINE_KEY(I) PAXG_DEFINE_KEY(J) PAXG_DEFINE_KEY(K) PAXG_DEFINE_KEY(L)
    PAXG_DEFINE_KEY(M) PAXG_DEFINE_KEY(N) PAXG_DEFINE_KEY(O) PAXG_DEFINE_KEY(P)
    PAXG_DEFINE_KEY(Q) PAXG_DEFINE_KEY(R) PAXG_DEFINE_KEY(S) PAXG_DEFINE_KEY(T)
    PAXG_DEFINE_KEY(U) PAXG_DEFINE_KEY(V) PAXG_DEFINE_KEY(W) PAXG_DEFINE_KEY(X)
    PAXG_DEFINE_KEY(Y) PAXG_DEFINE_KEY(Z)
    #undef PAXG_DEFINE_KEY
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
