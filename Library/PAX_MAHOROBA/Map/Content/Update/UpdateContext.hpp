/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_UPDATE_CONTEXT_HPP
#define PAX_MAHOROBA_UPDATE_CONTEXT_HPP

#include <cstdint>

#include <PAX_GRAPHICA/Font.hpp>
#include <PAX_GRAPHICA/Texture.hpp>

#include <PAX_SAPIENTICA/Core/Type/Rect.hpp>
#include <PAX_SAPIENTICA/Core/Type/UnorderedMap.hpp>
#include <PAX_SAPIENTICA/Core/Type/Vector2.hpp>
#include <PAX_SAPIENTICA/System/FeatureVisibilityManager.hpp>

namespace paxs {

/// @brief 基底コンテキスト - 全地物共通の不変情報
/// @brief Base context - Immutable information common to all features
struct BaseContext {
    const FeatureVisibilityManager* visibility_manager = nullptr; ///< 地物種別ごとの可視性管理
    const UnorderedMap<std::uint_least32_t, paxg::Texture>* texture_map = nullptr; ///< テクスチャマップ
};

/// @brief 空間的コンテキスト - ビューポートに関する情報
/// @brief Spatial context - Viewport-related information
struct SpatialContext : BaseContext {
    Vector2<double> map_view_size{0.0, 0.0};    ///< 地図ビューのサイズ（幅、高さ） / Map view size (width, height)
    Vector2<double> map_view_center{0.0, 0.0};  ///< 地図ビューの中心座標 / Map view center

    /// @brief 座標がビューの範囲内にあるかチェック（日付変更線対応）
    /// @brief Check if coordinates are within view bounds (with date line wrapping)
    /// @param mercator_pos メルカトル座標 / Mercator position
    /// @param margin マージン倍率（デフォルト1.6） / Margin multiplier (default 1.6)
    /// @return 範囲内ならtrue / True if within bounds
    /// @details 経度が±180°で循環するため、3つのラップ座標（-360°, 0°, +360°）のいずれかが範囲内にあるかチェックします
    /// @details Checks if any of the three wrapped coordinates (-360°, 0°, +360°) are within bounds since longitude wraps at ±180°
    [[nodiscard]] bool isInViewBounds(Vector2<double> mercator_pos, double margin = 1.6) const {
        const Rect<double> view_rect = Rect<double>::fromCenter(
            map_view_center,
            map_view_size * margin
        );

        // 経度の3つのラップ座標（-360°, 0°, +360°）のいずれかが範囲内にあるかチェック
        // Check if any of the three longitude-wrapped coordinates are within bounds
        for (int offset_mult = -1; offset_mult <= 1; ++offset_mult) {
            const Vector2<double> wrapped_pos(
                mercator_pos.x + (offset_mult * 360.0),
                mercator_pos.y
            );
            if (view_rect.contains(wrapped_pos)) {
                return true;
            }
        }
        return false;
    }
};

/// @brief 時間的コンテキスト - 日付に関する情報
/// @brief Temporal context - Date-related information
struct TemporalContext : BaseContext {
    double jdn = 0.0;  ///< ユリウス日 / Julian Day Number
};

/// @brief ローカライゼーションコンテキスト - 言語とフォントに関する情報
/// @brief Localization context - Language and font-related information
struct LocalizationContext : BaseContext {
    paxg::Font* font = nullptr;                 ///< 地名・人名描画用フォント / Font for place/person names
    std::uint_least32_t language_key = 0;       ///< 現在の言語キー（MurMur3ハッシュ） / Current language key (MurMur3 hash)
};

/// @brief 統合コンテキスト - 全パラメータを含む（初期化・全更新時のみ使用）
/// @brief Unified context - Contains all parameters (for initialization/full update only)
struct UnifiedContext : BaseContext {
    // 空間的パラメータ / Spatial parameters
    Vector2<double> map_view_size{0.0, 0.0};
    Vector2<double> map_view_center{0.0, 0.0};

    // 時間的パラメータ / Temporal parameters
    double jdn = 0.0;

    // ローカライゼーションパラメータ / Localization parameters
    paxg::Font* font = nullptr;
    std::uint_least32_t language_key = 0;

    /// @brief 空間的コンテキストに変換
    /// @brief Convert to spatial context
    [[nodiscard]] SpatialContext toSpatial() const {
        SpatialContext ctx;
        ctx.visibility_manager = visibility_manager;
        ctx.texture_map = texture_map;
        ctx.map_view_size = map_view_size;
        ctx.map_view_center = map_view_center;
        return ctx;
    }

    /// @brief 時間的コンテキストに変換
    /// @brief Convert to temporal context
    [[nodiscard]] TemporalContext toTemporal() const {
        TemporalContext ctx;
        ctx.visibility_manager = visibility_manager;
        ctx.texture_map = texture_map;
        ctx.jdn = jdn;
        return ctx;
    }

    /// @brief ローカライゼーションコンテキストに変換
    /// @brief Convert to localization context
    [[nodiscard]] LocalizationContext toLocalization() const {
        LocalizationContext ctx;
        ctx.visibility_manager = visibility_manager;
        ctx.texture_map = texture_map;
        ctx.font = font;
        ctx.language_key = language_key;
        return ctx;
    }

    /// @brief 座標がビューの範囲内にあるかチェック（SpatialContextと同じ実装）
    /// @brief Check if coordinates are within view bounds (same as SpatialContext)
    [[nodiscard]] bool isInViewBounds(Vector2<double> mercator_pos, double margin = 1.6) const {
        const Rect<double> view_rect = Rect<double>::fromCenter(
            map_view_center,
            map_view_size * margin
        );
        for (int offset_mult = -1; offset_mult <= 1; ++offset_mult) {
            const Vector2<double> wrapped_pos(
                mercator_pos.x + (offset_mult * 360.0),
                mercator_pos.y
            );
            if (view_rect.contains(wrapped_pos)) {
                return true;
            }
        }
        return false;
    }
};

} // namespace paxs

#endif // !PAX_MAHOROBA_UPDATE_CONTEXT_HPP
