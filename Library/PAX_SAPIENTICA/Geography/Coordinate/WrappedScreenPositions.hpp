/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com		https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_GEOGRAPHY_COORDINATE_WRAPPED_SCREEN_POSITIONS_HPP
#define PAX_SAPIENTICA_GEOGRAPHY_COORDINATE_WRAPPED_SCREEN_POSITIONS_HPP

#include <array>
#include <cstddef>

#include <PAX_GRAPHICA/Vec2.hpp>

namespace paxs {

/// @brief 経度ラップ処理後のスクリーン座標（-360°, 0°, +360°の3つ）
/// @brief Screen positions after longitude wrapping (3 positions: -360°, 0°, +360°)
/// @details 地球は球体であるため、経度は -180° 〜 +180° で循環します。
///          地図上では同じ地物が画面の複数箇所に表示される可能性があるため、
///          3つのスクリーン座標（元の経度±360°）を保持します。
/// @details Since the Earth is spherical, longitude wraps around from -180° to +180°.
///          On a map, the same feature may appear at multiple screen locations,
///          so we store 3 screen positions (original longitude ± 360°).
struct WrappedScreenPositions {
    /// @brief 経度ラップ後の座標数（常に3つ）
    /// @brief Number of wrapped positions (always 3)
    static constexpr std::size_t POSITION_COUNT = 3;

    /// @brief 3つのスクリーン座標（-360°, 0°, +360°）
    /// @brief Three screen positions (-360°, 0°, +360°)
    std::array<paxg::Vec2<double>, POSITION_COUNT> positions;

    /// @brief 有効な座標を持つかどうか（clear()で無効化、toScreenPositions()で有効化）
    /// @brief Whether this holds valid positions (invalidated by clear(), validated by toScreenPositions())
    bool is_valid = false;

    // ========== イテレータサポート（既存コードとの互換性） ==========
    // Iterator support (for compatibility with existing code)

    [[nodiscard]] auto begin() const noexcept { return positions.begin(); }
    [[nodiscard]] auto end() const noexcept { return positions.end(); }
    auto begin() noexcept { return positions.begin(); }
    auto end() noexcept { return positions.end(); }

    // ========== 配列的アクセス ==========
    // Array-like access

    const paxg::Vec2<double>& operator[](std::size_t i) const noexcept {
        return positions[i];
    }

    paxg::Vec2<double>& operator[](std::size_t i) noexcept {
        return positions[i];
    }

    // ========== サイズ・状態 ==========
    // Size and state

    [[nodiscard]] constexpr std::size_t size() const noexcept {
        return POSITION_COUNT;
    }

    [[nodiscard]] bool empty() const noexcept {
        return !is_valid;  // Empty if not valid
    }

    /// @brief 無効化
    /// @brief Invalidate
    void clear() noexcept {
        is_valid = false;  // Just set flag, don't write to positions array
    }
};

} // namespace paxs

#endif // !PAX_SAPIENTICA_GEOGRAPHY_COORDINATE_WRAPPED_SCREEN_POSITIONS_HPP
