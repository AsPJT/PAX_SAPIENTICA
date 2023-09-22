/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023 As Project
    [Production]	2023 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_VEC2_HPP
#define PAX_GRAPHICA_VEC2_HPP

/*##########################################################################################

##########################################################################################*/

#if defined(PAXS_USING_SIV3D)
#include <Siv3D/Vector2D.hpp>
#elif defined(PAXS_USING_SFML)
#include <SFML/System/Vector2.hpp>
#endif

namespace paxg {

    struct Vec2i {
        constexpr Vec2i() = default;
#if defined(PAXS_USING_SIV3D)
        s3d::Point vec2{};
        constexpr Vec2i(const int x, const int y) : vec2(x, y) {}
        constexpr Vec2i(const s3d::Point& vec2) : vec2(vec2) {}
        constexpr operator s3d::Point() const { return vec2; }
        constexpr auto x() const {
            return vec2.x;
        }
        constexpr auto y() const {
            return vec2.y;
        }
        void setX(const int x_) {
            vec2.x = x_;
        }
        void setY(const int y_) {
            vec2.y = y_;
        }
        Vec2i operator+(const Vec2i& v_) {
            return Vec2i(
                vec2.x + v_.x(),
                vec2.y + v_.y());
        }
        bool operator==(const Vec2i& v_) {
            return vec2.x == v_.vec2.x && vec2.y == v_.vec2.y;
        }

#elif defined(PAXS_USING_SFML)
        sf::Vector2i vec2{};
        Vec2i(const int x, const int y) : vec2(x, y) {}
        Vec2i(const sf::Vector2i& vec2) : vec2(vec2) {}
        operator sf::Vector2i() const { return vec2; }
        constexpr auto x() const {
            return vec2.x;
        }
        constexpr auto y() const {
            return vec2.y;
        }
        void setX(const int x_) {
            vec2.x = x_;
        }
        void setY(const int y_) {
            vec2.y = y_;
        }
        Vec2i operator+(const Vec2i& v_) {
            return Vec2i(
                vec2.x + v_.x(),
                vec2.y + v_.y());
        }
        bool operator==(const Vec2i& v_) {
            return vec2.x == v_.vec2.x && vec2.y == v_.vec2.y;
        }
#else
        int x0{}, y0{};
        constexpr Vec2i(const int x, const int y) : x0(x), y0(y) {}
        constexpr auto x() const {
            return x0;
        }
        constexpr auto y() const {
            return y0;
        }
        void setX(const int x_) {
            x0 = x_;
        }
        void setY(const int y_) {
            y0 = y_;
        }
        Vec2i operator+(const Vec2i& v_) {
            return Vec2i(
                x0 + v_.x(),
                y0 + v_.y());
        }
        bool operator==(const Vec2i& v_) {
            return x0 == v_.x0 && y0 == v_.y0;
        }
#endif
    };

    struct Vec2f {
#if defined(PAXS_USING_SIV3D)
        s3d::Vec2 vec2;
        constexpr Vec2f(const float x, const float y) : vec2(x, y) {}
        constexpr Vec2f(const s3d::Vec2& vec2) : vec2(vec2) {}
        constexpr operator s3d::Vec2() const { return vec2; }
        constexpr float x() const {
            return static_cast<float>(vec2.x);
        }
        constexpr float y() const {
            return static_cast<float>(vec2.y);
        }
#elif defined(PAXS_USING_SFML)
        sf::Vector2f vec2;
        Vec2f(const float x, const float y) : vec2(x, y) {}
        Vec2f(const sf::Vector2f& vec2) : vec2(vec2) {}
        operator sf::Vector2f() const { return vec2; }
        constexpr float x() const {
            return vec2.x;
        }
        constexpr float y() const {
            return vec2.y;
        }
#else
        float x0, y0;
        constexpr Vec2f(const float x, const float y) : x0(x), y0(y) {}
        constexpr float x() const {
            return x0;
        }
        constexpr float y() const {
            return y0;
        }
#endif
    };
}

#endif // !PAX_GRAPHICA_VEC2_HPP
