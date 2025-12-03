/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com		https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_FLOW_CURVE_FEATURE_HPP
#define PAX_MAHOROBA_FLOW_CURVE_FEATURE_HPP

#include <string>
#include <vector>
#include <cmath>

#include <PAX_GRAPHICA/Color.hpp>
#include <PAX_GRAPHICA/Spline2D.hpp>
#include <PAX_GRAPHICA/Line.hpp>

#include <PAX_MAHOROBA/Map/Content/Feature/FeatureType.hpp>
#include <PAX_MAHOROBA/Map/Content/Feature/MapFeature.hpp>
#include <PAX_MAHOROBA/Map/Content/Interaction/ClickContext.hpp>
#include <PAX_MAHOROBA/Map/Content/Update/IUpdatable.hpp>
#include <PAX_MAHOROBA/Map/Content/Update/UpdateContext.hpp>
#include <PAX_MAHOROBA/Map/Core/MapCoordinateConverter.hpp>

#include <PAX_SAPIENTICA/Core/Type/Range.hpp>
#include <PAX_SAPIENTICA/Core/Type/Vector2.hpp>
#include <PAX_SAPIENTICA/Geography/Coordinate/Projection.hpp>
#include <PAX_SAPIENTICA/Map/FlowCurveLocationPoint.hpp>
#include <PAX_SAPIENTICA/Utility/Logger.hpp>
#include <PAX_SAPIENTICA/Utility/MurMur3.hpp>

namespace paxs {

/// @brief フロー曲線（移動や交易などを示すスプライン曲線）を表す地物クラス
/// @brief Feature class representing a flow curve (spline curves for movement, trade, etc.)
class FlowCurveFeature : public MapFeature, public ISpatiallyUpdatable {
public:
    /// @param data フロー曲線の位置データ / Flow curve location data
    /// @param group_data フロー曲線グループデータ / Flow curve group data
    FlowCurveFeature(const FlowCurveLocationData& data, const FlowCurveLocationGroup& group_data)
        : data_(data)
        , group_data_(group_data)
        , cached_screen_points_()
        , color_(0, 0, 0, 255)  // デフォルト色（黒） / Default color (black)
        , animation_offset_(0.0f)
        , animation_speed_(0.002f)  // アニメーション速度を遅く / Slower animation speed
    {
        // 色情報からRGBAを取得（将来的にはAppConfigから色マッピングを読み込む）
        // TODO: 色情報の管理システムを実装
    }

    FeatureType getType() const override {
        return FeatureType::FlowCurve;
    }

    std::uint_least32_t getId() const override {
        if (!data_.key.empty()) {
            return MurMur3::calcHash(data_.key.c_str());
        }
        return 0;
    }

    std::string getName() const override {
        return data_.key;
    }

    std::uint_least32_t getFeatureTypeHash() const override {
        return data_.feature_type_hash;
    }

    /// @brief アニメーションの更新
    /// @brief Update animation
    void updateAnimation() const {  // constにして、mutableなメンバーを更新
        // アニメーションオフセットを更新
        // Update animation offset
        animation_offset_ += animation_speed_;

        // オフセットが1.0を超えたらリセット（ループアニメーション）
        // Reset when offset exceeds 1.0 (loop animation)
        if (animation_offset_ >= 1.0f) {
            animation_offset_ -= 1.0f;
        }
    }

    /// @brief 空間的更新（ISpatiallyUpdatableの実装）
    /// @brief Spatial update (ISpatiallyUpdatable implementation)
    void updateSpatial(const SpatialContext& context) override {
        // 地物種別の可視性チェック（最優先）
        if ((context.visibility_manager != nullptr) && !context.visibility_manager->isVisible(data_.feature_type_hash)) {
            cached_screen_points_.clear();
            visible_ = false;
            return;
        }

        // データが空の場合は早期リターン
        if (data_.coordinates.empty()) {
            cached_screen_points_.clear();
            visible_ = false;
            return;
        }

        // フロー曲線のバウンディングボックスを計算（最小・最大経度/緯度）
        // Calculate bounding box of the flow curve (min/max longitude/latitude)
        double min_lon = data_.coordinates[0].x;
        double max_lon = data_.coordinates[0].x;
        double min_lat = data_.coordinates[0].y;
        double max_lat = data_.coordinates[0].y;

        for (const auto& coord : data_.coordinates) {
            min_lon = (std::min)(min_lon, coord.x);
            max_lon = (std::max)(max_lon, coord.x);
            min_lat = (std::min)(min_lat, coord.y);
            max_lat = (std::max)(max_lat, coord.y);
        }

        // 日付変更線をまたぐかチェック（経度差が180°以上）
        // Check if flow curve crosses date line (longitude difference >= 180°)
        const bool crosses_dateline = (max_lon - min_lon) > 180.0;

        // ±3°のマージンを追加
        // Add ±3° margin
        constexpr double bbox_margin = 3.0;
        min_lon -= bbox_margin;
        max_lon += bbox_margin;
        min_lat -= bbox_margin;
        max_lat += bbox_margin;

        // 画面の範囲を計算
        // Calculate screen bounds
        const double view_half_width = context.map_view_size.x / 2.0;
        const double view_half_height = context.map_view_size.y / 2.0;
        const double view_min_lon = context.map_view_center.x - view_half_width;
        const double view_max_lon = context.map_view_center.x + view_half_width;
        const double view_min_lat = context.map_view_center.y - view_half_height;
        const double view_max_lat = context.map_view_center.y + view_half_height;

        // 経度ラップ処理：3つのオフセット候補（-360°, 0°, +360°）で試す
        // Longitude wrapping: try 3 offset candidates (-360°, 0°, +360°)
        int best_offset_mult = 0;
        bool found_overlap = false;

        for (int offset_mult = -1; offset_mult <= 1; ++offset_mult) {
            const double offset_min_lon = min_lon + (offset_mult * 360.0);
            const double offset_max_lon = max_lon + (offset_mult * 360.0);

            // バウンディングボックスが画面範囲と重なるかチェック
            // Check if bounding box overlaps with screen bounds
            const bool lon_overlap = (offset_min_lon <= view_max_lon) && (offset_max_lon >= view_min_lon);
            const bool lat_overlap = (min_lat <= view_max_lat) && (max_lat >= view_min_lat);

            if (lon_overlap && lat_overlap) {
                best_offset_mult = offset_mult;
                found_overlap = true;
                break;  // 最初に見つかったオフセットを使用
            }
        }

        // 画面範囲と重ならない場合は非表示
        // Hide if no overlap with screen bounds
        if (!found_overlap) {
            cached_screen_points_.clear();
            visible_ = false;
            return;
        }

        // 最適なオフセットを適用した後、日付変更線をまたぐ場合の経度正規化
        // Apply best offset, then normalize longitude for date line wrapping
        std::vector<paxs::WebMercatorDeg> normalized_coords;
        normalized_coords.reserve(data_.coordinates.size());

        // 最初の点にオフセットを適用
        const double first_x = data_.coordinates[0].x + (best_offset_mult * 360.0);
        normalized_coords.push_back(paxs::WebMercatorDeg(paxs::Vector2<double>(first_x, data_.coordinates[0].y)));

        // 2点目以降は、日付変更線をまたぐ場合のみ正規化
        // For subsequent points, normalize only if crossing date line
        if (crosses_dateline) {
            // 日付変更線をまたぐ場合：前の点との経度差が180°を超えないように正規化
            // Date line crossing: normalize so longitude difference from previous point doesn't exceed 180°
            for (size_t i = 1; i < data_.coordinates.size(); ++i) {
                const double prev_lon = normalized_coords[i - 1].x;
                double current_lon = data_.coordinates[i].x + (best_offset_mult * 360.0);

                // 経度差を計算
                double lon_diff = current_lon - prev_lon;

                // 差が180°を超える場合、短い方の経路を選択
                // If difference exceeds 180°, choose shorter path
                if (lon_diff > 180.0) {
                    current_lon -= 360.0;
                } else if (lon_diff < -180.0) {
                    current_lon += 360.0;
                }

                normalized_coords.push_back(paxs::WebMercatorDeg(
                    paxs::Vector2<double>(current_lon, data_.coordinates[i].y)
                ));
            }
        } else {
            // 日付変更線をまたがない場合：そのままオフセットを適用
            // Non-crossing: simply apply offset
            for (size_t i = 1; i < data_.coordinates.size(); ++i) {
                const double current_lon = data_.coordinates[i].x + (best_offset_mult * 360.0);
                normalized_coords.push_back(paxs::WebMercatorDeg(
                    paxs::Vector2<double>(current_lon, data_.coordinates[i].y)
                ));
            }
        }

        // 正規化された座標をスクリーン座標に変換
        // Convert normalized coordinates to screen coordinates
        cached_screen_points_.clear();
        cached_screen_points_.reserve(normalized_coords.size());

        for (const auto& coord : normalized_coords) {
            // スクリーン座標に変換
            const paxs::Vector2<double> screen_pos = MapCoordinateConverter::toScreenPos(
                coord,
                context.map_view_size,
                context.map_view_center
            );

            cached_screen_points_.emplace_back(screen_pos);
        }

        visible_ = (cached_screen_points_.size() >= 2);
    }

    bool isVisible() const override {
        return visible_;
    }

    bool isInTimeRange(double jdn) const override {
        return data_.year_range.contains(jdn);
    }

    const WrappedScreenPositions& getScreenPositions() const override {
        // FlowCurveは複数点を持つため、WrappedScreenPositionsは使用しない
        // Use empty positions since flow curves use multiple points
        static const WrappedScreenPositions empty_positions;
        return empty_positions;
    }

    int getDisplaySize() const override {
        return 0; // FlowCurveは点ではなく線なのでサイズは0
    }

    bool isHit(const paxs::Vector2<int>& mouse_pos) const override {
        // フロー曲線のクリック判定は実装しない（将来的には線の近傍判定を実装可能）
        // Click detection for flow curves is not implemented (can be added in the future)
        (void)mouse_pos;
        return false;
    }

    void onClick(const ClickContext& context) override {
        // std::cout << "FlowCurve clicked: " << getName() << std::endl;
        (void)context;
    }

    /// @brief スプライン曲線＋矢印を描画（アニメーション付き）
    /// @brief Draw spline curve with animated arrow
    void renderSpline() const {
        if (!visible_ || cached_screen_points_.size() < 2) {
            return;
        }

        // アニメーションを更新（毎フレーム呼び出す）
        // Update animation (called every frame)
        updateAnimation();

        // グループデータから線幅を取得
        const float line_width = group_data_.line_width;

        // 背景として全体のスプライン曲線を薄く描画
        // Draw full spline curve as background (lighter color)
        paxg::Color bg_color(80, 80, 80, 80);  // グレー、透明度を下げる / Gray with reduced opacity
        paxg::Spline2D(cached_screen_points_).draw(line_width * 0.5f, bg_color);

        // 複数のアニメーションする移動曲線を描画（7本の流路）
        // Draw multiple animated moving curve segments (7 flow paths)
        // 各線の開始位置と開始時間をずらしてバラつきを出す
        // Stagger start position and timing for each line to create variation

        // 流路定義：{lateral_offset, time_offset, start_position_offset}
        // Flow path definition: {lateral_offset, time_offset, start_position_offset}
        struct FlowPath {
            float lateral_offset;      // 左右のオフセット（ピクセル） / Lateral offset in pixels
            float time_offset;         // 時間オフセット（0.0～1.0） / Time offset (0.0-1.0)
            float start_pos_offset;    // 開始位置オフセット（0.0～1.0） / Start position offset (0.0-1.0)
        };

        constexpr FlowPath flow_paths[] = {
            {-6.0f, 0.0f,  0.0f},    // 左外側 / Left outer
            {-4.0f, 0.14f * 1.5, 0.4f},    // 左中外 / Left mid-outer
            {-2.0f, 0.28f * 1.5, 0.8f},    // 左中内 / Left mid-inner
            { 0.0f, 0.42f * 1.5, 0.2f},    // 中央 / Center
            { 2.0f, 0.56f * 1.5, 0.5f},    // 右中内 / Right mid-inner
            { 4.0f, 0.70f * 1.5, 0.9f},    // 右中外 / Right mid-outer
            { 6.0f, 0.84f * 1.5, 0.5f}     // 右外側 / Right outer
        };

        constexpr int num_segments_per_line = 4;  // 各流路あたりの動く曲線の本数 / Number of moving segments per flow path

        for (const auto& path : flow_paths) {
            for (int i = 0; i < num_segments_per_line; ++i) {
                // 各線内での位相オフセット
                // Phase offset within each line
                const float phase_offset = static_cast<float>(i) / num_segments_per_line;

                // 時間オフセットを加算してバラつきを出す
                // Add time offset for variation
                const float total_offset = phase_offset + path.time_offset;

                drawAnimatedSegment(line_width, total_offset, path.lateral_offset, path.start_pos_offset);
            }
        }
    }

    const FlowCurveLocationData& getData() const {
        return data_;
    }

    const FlowCurveLocationGroup& getGroupData() const {
        return group_data_;
    }

    const std::vector<paxs::Vector2<float>>& getScreenPoints() const {
        return cached_screen_points_;
    }

private:
    FlowCurveLocationData data_;           ///< フロー曲線位置データ / Flow curve location data
    FlowCurveLocationGroup group_data_;    ///< フロー曲線グループデータ / Flow curve group data

    std::vector<paxs::Vector2<float>> cached_screen_points_;  ///< キャッシュされたスクリーン座標列 / Cached screen points
    paxg::Color color_;                              ///< 描画色 / Drawing color
    bool visible_{true};                             ///< 可視性 / Visibility

    mutable float animation_offset_;                 ///< アニメーションオフセット (0.0 ~ 1.0) / Animation offset
    float animation_speed_;                          ///< アニメーション速度 / Animation speed

    /// @brief Catmull-Romスプライン補間
    /// @brief Catmull-Rom spline interpolation
    static paxs::Vector2<float> calculateCatmullRom(const paxs::Vector2<float>& p0, const paxs::Vector2<float>& p1,
                                           const paxs::Vector2<float>& p2, const paxs::Vector2<float>& p3, float t) {
        float t2 = t * t;
        float t3 = t2 * t;

        float v0 = (p2.x - p0.x) * 0.5f;
        float v1 = (p3.x - p1.x) * 0.5f;
        float x = (2 * p1.x - 2 * p2.x + v0 + v1) * t3 + (-3 * p1.x + 3 * p2.x - 2 * v0 - v1) * t2 + v0 * t + p1.x;

        v0 = (p2.y - p0.y) * 0.5f;
        v1 = (p3.y - p1.y) * 0.5f;
        float y = (2 * p1.y - 2 * p2.y + v0 + v1) * t3 + (-3 * p1.y + 3 * p2.y - 2 * v0 - v1) * t2 + v0 * t + p1.y;

        return {x, y};
    }

    /// @brief アニメーションする線分を描画
    /// @brief Draw animated curve segment
    /// @param line_width 線幅 / Line width
    /// @param phase_offset 位相オフセット (0.0 ~ 1.0) / Phase offset
    /// @param fixed_lateral_offset 固定の左右オフセット（ピクセル単位） / Fixed lateral offset in pixels
    /// @param start_position_offset 開始位置オフセット (0.0 ~ 1.0) / Start position offset
    void drawAnimatedSegment(float line_width, float phase_offset = 0.0f, float fixed_lateral_offset = 0.0f, float start_position_offset = 0.0f) const {
        if (cached_screen_points_.size() < 2) return;

        // 移動する線分の長さ（始点から約3点分の長さ）
        // Length of moving segment (approximately 3 points from start)
        constexpr float segment_length = 3.0f;

        const std::size_t point_count = cached_screen_points_.size();

        // スプライン曲線全体の長さに基づいて、アニメーションオフセットを適用
        // Apply animation offset based on total spline curve length
        const float total_segments = static_cast<float>(point_count - 1);

        // 時間オフセットと開始位置オフセットを組み合わせる
        // Combine time offset and start position offset
        float animated_position = (animation_offset_ + phase_offset);

        // 位相オフセットが1.0を超える場合、0.0～1.0に正規化
        // Normalize phase offset to 0.0-1.0 range
        while (animated_position >= 1.0f) {
            animated_position -= 1.0f;
        }

        // 開始位置オフセットを追加（各線の開始位置をずらす）
        // Add start position offset (stagger start position for each line)
        animated_position = animated_position + start_position_offset;
        if (animated_position >= 1.0f) {
            animated_position -= 1.0f;
        }

        animated_position *= total_segments;

        // 移動する線分の開始点と終了点を計算
        // Calculate start and end points of moving segment
        const float start_pos = animated_position;
        const float end_pos = animated_position + segment_length;

        // 線分を構成する点を収集
        // Collect points for the segment
        std::vector<paxs::Vector2<float>> segment_points;
        segment_points.reserve(static_cast<size_t>(segment_length * 20 + 2));

        // スプライン曲線上の点を補間して収集
        // Interpolate and collect points on spline curve
        const int divisions = 20;  // 各セグメント間の分割数 / Divisions per segment

        for (std::size_t i = 0; i < point_count - 1; ++i) {
            const float segment_start = static_cast<float>(i);
            const float segment_end = static_cast<float>(i + 1);

            // このセグメントが移動線分の範囲内にあるかチェック
            // Check if this segment is within the moving segment range
            if (segment_end < start_pos || segment_start > end_pos) {
                continue;  // 範囲外なのでスキップ / Skip if outside range
            }

            // Catmull-Rom補間用の制御点を取得
            // Get control points for Catmull-Rom interpolation
            paxs::Vector2<float> p0 = (i == 0) ? cached_screen_points_[0] : cached_screen_points_[i - 1];
            paxs::Vector2<float> p1 = cached_screen_points_[i];
            paxs::Vector2<float> p2 = cached_screen_points_[i + 1];
            paxs::Vector2<float> p3 = (i + 2 >= point_count) ? cached_screen_points_[i + 1] : cached_screen_points_[i + 2];

            // このセグメント内で補間する範囲を計算
            // Calculate interpolation range within this segment
            const float local_start = (std::max)(0.0f, start_pos - segment_start);
            const float local_end = (std::min)(1.0f, end_pos - segment_start);

            const int start_div = static_cast<int>(local_start * divisions);
            const int end_div = static_cast<int>(local_end * divisions);

            for (int j = start_div; j <= end_div; ++j) {
                float t = static_cast<float>(j) / divisions;
                paxs::Vector2<float> point = calculateCatmullRom(p0, p1, p2, p3, t);

                // 固定の左右オフセットを適用
                // Apply fixed lateral offset
                if (std::abs(fixed_lateral_offset) > 0.01f) {
                    // 曲線の向き（接線方向）を計算
                    // Calculate tangent direction
                    paxs::Vector2<float> tangent = calculateTangent(p0, p1, p2, p3, t);

                    // 接線に垂直な方向を計算（右向き）
                    // Calculate perpendicular direction (rightward)
                    paxs::Vector2<float> perpendicular(-tangent.y, tangent.x);
                    // 垂直方向にオフセットを適用
                    // Apply perpendicular offset
                    point = paxs::Vector2<float>(
                        point.x + perpendicular.x * fixed_lateral_offset,
                        point.y + perpendicular.y * fixed_lateral_offset
                    );
                }

                segment_points.push_back(point);
            }
        }

        // 収集した点で線分を描画
        // Draw segment with collected points
        if (segment_points.size() >= 2) {
            // 透明度を位相オフセットに基づいて調整
            // Adjust opacity based on phase offset
            const float alpha_factor = 1.0f - (phase_offset * 0.4f);  // 0.6 ~ 1.0
            const int alpha = static_cast<int>(180 * alpha_factor + 50);  // 50 ~ 230
            paxg::Color segment_color(0, 0, 0, alpha);  // 黒色、透明度可変 / Black with variable opacity

            for (size_t i = 0; i < segment_points.size() - 1; ++i) {
                paxg::Line(segment_points[i], segment_points[i + 1]).draw(line_width * 0.6f, segment_color);
            }

            // 線分の終端に小さい矢印を描画（位相オフセットが小さく、中央付近の流路のみ）
            // Draw small arrow at the end of segment (only for small phase offset and near-center paths)
            if (segment_points.size() >= 2 && phase_offset < 0.3f && std::abs(fixed_lateral_offset) < 4.0f) {
                const size_t last_idx = segment_points.size() - 1;
                const paxs::Vector2<float>& prev = segment_points[last_idx - 1];
                const paxs::Vector2<float>& end = segment_points[last_idx];

                const float arrow_length = line_width * 4.0f;
                const float arrow_width = line_width * 2.5f;

                paxg::Line(prev, end).drawArrow(line_width * 0.6f, paxs::Vector2<float>(arrow_width, arrow_length), segment_color);
            }
        }
    }

    /// @brief 接線ベクトルを計算（Catmull-Romスプラインの1次微分）
    /// @brief Calculate tangent vector (first derivative of Catmull-Rom spline)
    static paxs::Vector2<float> calculateTangent(const paxs::Vector2<float>& p0, const paxs::Vector2<float>& p1,
                                        const paxs::Vector2<float>& p2, const paxs::Vector2<float>& p3, float t) {
        const float t2 = t * t;

        // Catmull-Romスプラインの1次微分
        // First derivative of Catmull-Rom spline
        const float v0_x = (p2.x - p0.x) * 0.5f;
        const float v1_x = (p3.x - p1.x) * 0.5f;
        const float dx = (2 * p1.x - 2 * p2.x + v0_x + v1_x) * 3.0f * t2 +
                         (-3 * p1.x + 3 * p2.x - 2 * v0_x - v1_x) * 2.0f * t + v0_x;

        const float v0_y = (p2.y - p0.y) * 0.5f;
        const float v1_y = (p3.y - p1.y) * 0.5f;
        const float dy = (2 * p1.y - 2 * p2.y + v0_y + v1_y) * 3.0f * t2 +
                         (-3 * p1.y + 3 * p2.y - 2 * v0_y - v1_y) * 2.0f * t + v0_y;

        // 正規化（単位ベクトル化）
        // Normalize to unit vector
        const float length = std::sqrt(dx * dx + dy * dy);
        if (length > 0.0001f) {
            return {dx / length, dy / length};
        }
        return {1.0f, 0.0f};  // デフォルト方向 / Default direction
    }
};

} // namespace paxs

#endif // !PAX_MAHOROBA_FLOW_CURVE_FEATURE_HPP
