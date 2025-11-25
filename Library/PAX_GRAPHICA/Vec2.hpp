/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_VEC2_HPP
#define PAX_GRAPHICA_VEC2_HPP

#include <type_traits>

#if defined(PAXS_USING_SIV3D)
#include <Siv3D/Vector2D.hpp>
#elif defined(PAXS_USING_DXLIB)
#include <DxLib.h>
#elif defined(PAXS_USING_SFML)
#include <SFML/System/Vector2.hpp>
#endif

#include <PAX_SAPIENTICA/Core/Type/Vector2.hpp>

namespace paxg {

    template<typename T>
    struct Vec2 {
        constexpr Vec2() = default;

#if defined(PAXS_USING_SIV3D)
        using NativeVec2 = typename std::conditional<std::is_integral<T>::value, s3d::Point, s3d::Vec2>::type;
        NativeVec2 vec2{};

        constexpr Vec2(const T x, const T y) : vec2(static_cast<typename NativeVec2::value_type>(x), static_cast<typename NativeVec2::value_type>(y)) {}
        constexpr Vec2(const NativeVec2& vec2) : vec2(vec2) {}

        // 型変換コンストラクタ (conversion constructor for different numeric types)
        template<typename U>
        constexpr Vec2(const Vec2<U>& other) : vec2(static_cast<typename NativeVec2::value_type>(other.x()), static_cast<typename NativeVec2::value_type>(other.y())) {}

        // paxs::Vector2からの変換コンストラクタ
        constexpr Vec2(const paxs::Vector2<T>& v) : vec2(static_cast<typename NativeVec2::value_type>(v.x), static_cast<typename NativeVec2::value_type>(v.y)) {}
        constexpr operator NativeVec2() const { return vec2; }

        constexpr auto x() const { return static_cast<T>(vec2.x); }
        constexpr auto y() const { return static_cast<T>(vec2.y); }

        void setX(const T x_) { vec2.x = static_cast<typename NativeVec2::value_type>(x_); }
        void setY(const T y_) { vec2.y = static_cast<typename NativeVec2::value_type>(y_); }

#elif defined(PAXS_USING_SFML)
        using NativeVec2 = sf::Vector2<T>;
        NativeVec2 vec2{};

        constexpr Vec2(const T x, const T y) : vec2(x, y) {}
        constexpr Vec2(const NativeVec2& vec2) : vec2(vec2) {}

        // 型変換コンストラクタ (conversion constructor for different numeric types)
        template<typename U>
        constexpr Vec2(const Vec2<U>& other) : vec2(static_cast<T>(other.x()), static_cast<T>(other.y())) {}

        // paxs::Vector2からの変換コンストラクタ
        constexpr Vec2(const paxs::Vector2<T>& v) : vec2(static_cast<T>(v.x), static_cast<T>(v.y)) {}

        operator NativeVec2() const { return vec2; }

        constexpr auto x() const { return vec2.x; }
        constexpr auto y() const { return vec2.y; }

        void setX(const T x_) { vec2.x = x_; }
        void setY(const T y_) { vec2.y = y_; }

#else
        T x0{}, y0{};

        constexpr Vec2(const T x, const T y) : x0(x), y0(y) {}

        // 型変換コンストラクタ (conversion constructor for different numeric types)
        template<typename U>
        constexpr Vec2(const Vec2<U>& other) : x0(static_cast<T>(other.x())), y0(static_cast<T>(other.y())) {}

        // paxs::Vector2からの変換コンストラクタ
        constexpr Vec2(const paxs::Vector2<T>& v) : x0(static_cast<T>(v.x)), y0(static_cast<T>(v.y)) {}

        constexpr auto x() const { return x0; }
        constexpr auto y() const { return y0; }

        void setX(const T x_) { x0 = x_; }
        void setY(const T y_) { y0 = y_; }
#endif

        // 共通演算子
        Vec2 operator+(const Vec2& v_) const {
            return Vec2(x() + v_.x(), y() + v_.y());
        }

        Vec2 operator-(const Vec2& v_) const {
            return Vec2(x() - v_.x(), y() - v_.y());
        }

        Vec2& operator+=(const Vec2& v_) {
            setX(x() + v_.x());
            setY(y() + v_.y());
            return *this;
        }

        Vec2& operator-=(const Vec2& v_) {
            setX(x() - v_.x());
            setY(y() - v_.y());
            return *this;
        }

        bool operator==(const Vec2& v_) const {
            return x() == v_.x() && y() == v_.y();
        }

        bool operator!=(const Vec2& v_) const {
            return !(*this == v_);
        }
    };

    using Vec2i = Vec2<int>;
    using Vec2f = Vec2<float>;
}

#endif // !PAX_GRAPHICA_VEC2_HPP
