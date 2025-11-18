/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com		https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_CORE_TYPE_RANGE_HPP
#define PAX_SAPIENTICA_CORE_TYPE_RANGE_HPP

namespace paxs {

    /// @brief 範囲を表すクラス（minimum/maximum）
    /// @brief Class representing a range (minimum/maximum)
    template<typename T>
    struct Range {
        T minimum{};
        T maximum{};

        constexpr Range() noexcept = default;
        constexpr Range(T minimum_val, T maximum_val) noexcept : minimum(minimum_val), maximum(maximum_val) {}

        /// @brief 値が範囲内にあるかチェック（minimum <= value <= maximum）
        /// @brief Check if a value is within the range (minimum <= value <= maximum)
        [[nodiscard]] constexpr bool contains(T value) const noexcept {
            return value >= minimum && value <= maximum;
        }

        /// @brief 値が範囲外にあるかチェック（value < minimum || value > maximum）
        /// @brief Check if a value is outside the range (value < minimum || value > maximum)
        [[nodiscard]] constexpr bool excludes(T value) const noexcept {
            return value < minimum || value > maximum;
        }

        /// @brief 範囲の長さを取得
        /// @brief Get the length of the range
        [[nodiscard]] constexpr T length() const noexcept {
            return maximum - minimum;
        }

        /// @brief 範囲の中心値を取得
        /// @brief Get the center value of the range
        [[nodiscard]] constexpr T center() const noexcept {
            return (minimum + maximum) / static_cast<T>(2);
        }

        /// @brief 値を範囲内にクランプ
        /// @brief Clamp a value within the range
        [[nodiscard]] constexpr T clamp(T value) const noexcept {
            if (value < minimum) return minimum;
            if (value > maximum) return maximum;
            return value;
        }

        /// @brief 2つの範囲が重なっているかチェック
        /// @brief Check if two ranges overlap
        [[nodiscard]] constexpr bool overlaps(const Range<T>& other) const noexcept {
            return !(maximum < other.minimum || minimum > other.maximum);
        }

        /// @brief デフォルトの比較演算子
        /// @brief Default comparison operator
        auto operator<=>(const Range<T>& other) const noexcept = default;
    };

} // namespace paxs

#endif // !PAX_SAPIENTICA_CORE_TYPE_RANGE_HPP
