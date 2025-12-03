/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com		https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_CORE_TYPE_RECT_HPP
#define PAX_SAPIENTICA_CORE_TYPE_RECT_HPP

#include <PAX_SAPIENTICA/Core/Type/Vector2.hpp>

namespace paxs {

    /// @brief Rectangle class representing position and size
    /// @brief 矩形を表すクラス(位置とサイズ)
    template<typename T>
    struct Rect {
        Vector2<T> pos_{};  // 左上の座標 (x, y) / Top-left position (x, y)
        Vector2<T> sz_{};      // サイズ (width, height) / Size (width, height)

        constexpr Rect() noexcept = default;

        constexpr Rect(T x, T y, T width, T height) noexcept
            : pos_(x, y), sz_(width, height) {}

        constexpr Rect(const Vector2<T>& pos, const Vector2<T>& sz) noexcept
            : pos_(pos), sz_(sz) {}

        // Getters
        [[nodiscard]] constexpr T x() const noexcept { return pos_.x; }
        [[nodiscard]] constexpr T y() const noexcept { return pos_.y; }
        [[nodiscard]] constexpr T width() const noexcept { return sz_.x; }
        [[nodiscard]] constexpr T height() const noexcept { return sz_.y; }
        [[nodiscard]] constexpr Vector2<T> position() const noexcept { return pos_; }
        [[nodiscard]] constexpr Vector2<T> size() const noexcept { return sz_; }

        // Computed properties
        [[nodiscard]] constexpr T left() const noexcept { return pos_.x; }
        [[nodiscard]] constexpr T top() const noexcept { return pos_.y; }
        [[nodiscard]] constexpr T right() const noexcept { return pos_.x + sz_.x; }
        [[nodiscard]] constexpr T bottom() const noexcept { return pos_.y + sz_.y; }

        [[nodiscard]] constexpr Vector2<T> topLeft() const noexcept { return pos_; }
        [[nodiscard]] constexpr Vector2<T> topRight() const noexcept { return Vector2<T>(right(), top()); }
        [[nodiscard]] constexpr Vector2<T> bottomLeft() const noexcept { return Vector2<T>(left(), bottom()); }
        [[nodiscard]] constexpr Vector2<T> bottomRight() const noexcept { return Vector2<T>(right(), bottom()); }

        [[nodiscard]] constexpr Vector2<T> center() const noexcept {
            return Vector2<T>(pos_.x + (sz_.x / 2), pos_.y + (sz_.y / 2));
        }

        // Point containment check
        /// @brief Check if a point is inside the rectangle
        /// @brief 点が矩形内にあるかチェック
        [[nodiscard]] constexpr bool contains(const Vector2<T>& point) const noexcept {
            return point.x >= left() && point.x <= right() &&
                   point.y >= top() && point.y <= bottom();
        }

        [[nodiscard]] constexpr bool contains(T point_x, T point_y) const noexcept {
            return contains(Vector2<T>(point_x, point_y));
        }

        // Rectangle intersection check
        /// @brief Check if two rectangles intersect
        /// @brief 矩形同士の交差判定
        [[nodiscard]] constexpr bool intersects(const Rect<T>& other) const noexcept {
            return !(right() <= other.left() || left() >= other.right() ||
                     bottom() <= other.top() || top() >= other.bottom());
        }

        // Create expanded rectangle with margin
        /// @brief Create a rectangle expanded by a margin
        /// @brief マージン付き矩形を作成
        [[nodiscard]] constexpr Rect<T> expanded(T margin) const noexcept {
            return Rect<T>(
                pos_.x - margin,
                pos_.y - margin,
                sz_.x + (margin * 2),
                sz_.y + (margin * 2)
            );
        }

        // Create rectangle from center and size
        /// @brief Create a rectangle from center position and size
        /// @brief 中心からサイズで矩形を作成
        [[nodiscard]] static constexpr Rect<T> fromCenter(const Vector2<T>& center, const Vector2<T>& size) noexcept {
            return Rect<T>(
                center.x - (size.x / 2),
                center.y - (size.y / 2),
                size.x,
                size.y
            );
        }

        // Create rectangle from two points
        /// @brief Create a rectangle from two points
        /// @brief 2点から矩形を作成
        [[nodiscard]] static constexpr Rect<T> fromPoints(const Vector2<T>& p1, const Vector2<T>& p2) noexcept {
            T min_x = (p1.x < p2.x) ? p1.x : p2.x;
            T min_y = (p1.y < p2.y) ? p1.y : p2.y;
            T max_x = (p1.x > p2.x) ? p1.x : p2.x;
            T max_y = (p1.y > p2.y) ? p1.y : p2.y;
            return Rect<T>(min_x, min_y, max_x - min_x, max_y - min_y);
        }

        auto operator<=>(const Rect<T>& rect) const noexcept = default;

        /// @brief Convert rectangle to another type
        /// @brief 矩形を別の型に変換
        template<typename U>
        [[nodiscard]] Rect<U> toType() const noexcept {
            return Rect<U>(
                static_cast<U>(pos_.x),
                static_cast<U>(pos_.y),
                static_cast<U>(sz_.x),
                static_cast<U>(sz_.y)
            );
        }
    };

} // namespace paxs

#endif // !PAX_SAPIENTICA_CORE_TYPE_RECT_HPP
