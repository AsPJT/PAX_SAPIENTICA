/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_MAP_SETTLEMENT_RENDERER_HPP
#define PAX_MAHOROBA_MAP_SETTLEMENT_RENDERER_HPP

#include <PAX_GRAPHICA/Circle.hpp>
#include <PAX_GRAPHICA/Font.hpp>
#include <PAX_GRAPHICA/Line.hpp>
#include <PAX_GRAPHICA/Spline2D.hpp>
#include <PAX_GRAPHICA/String.hpp>
#include <PAX_GRAPHICA/RoundRect.hpp>

#include <PAX_MAHOROBA/Map/Location/LocationRendererHelper.hpp>
#include <PAX_MAHOROBA/Map/Location/MapCoordinateConverter.hpp>
#include <PAX_MAHOROBA/Rendering/SimulationColor.hpp>

#include <PAX_SAPIENTICA/Core/Type/UnorderedMap.hpp>
#include <PAX_SAPIENTICA/Core/Type/Vector2.hpp>
#include <PAX_SAPIENTICA/Map/LocationPoint.hpp>
#include <PAX_SAPIENTICA/Simulation/Entity/SettlementGrid.hpp>
#include <PAX_SAPIENTICA/Utility/MapUtils.hpp>

namespace paxs {
    /// @brief シミュレーションの集落を可視化する
    /// @brief Visualize simulation settlements
    class SettlementRenderer {
    public:
        /// @brief エージェント（集落）を描画（private化）
        /// @brief Draw agents (settlements) - made private
        /// @param jdn ユリウス日
        /// @param agents 集落グリッド
        /// @param marriage_pos_list 婚姻移動のリスト
        /// @param map_view_size マップビューのサイズ
        /// @param map_view_center マップビューの中心座標
        /// @param select_draw 表示モード (1-6)
        /// @param is_line グリッド線を表示するか
        /// @param is_arrow 移動矢印を表示するか
        static void draw(const double jdn,
            const paxs::UnorderedMap<SettlementGridsType, paxs::SettlementGrid>* agents,
            const std::vector<GridType4>* marriage_pos_list,
            const Vector2<double>& map_view_size,
            const Vector2<double>& map_view_center,
            const std::size_t select_draw, const bool is_line, const bool is_arrow
        ) {
            // 集落を描画
            drawSettlements(jdn, agents, map_view_size, map_view_center, select_draw);

            // グリッド線を描画
            if (is_line) {
                drawGridLines(map_view_size, map_view_center);
            }

            // 移動線を描画
            if (is_arrow) {
                drawMovementLines(jdn, agents, marriage_pos_list,
                    map_view_size, map_view_center);
            }
        }

    private:
        // 描画定数
        static constexpr double MAX_POPULATION_VISUALIZATION = 75.0;  // 人口可視化の最大値
        static constexpr double MTDNA_SCALE_FACTOR = 27.0;  // mtDNA可視化のスケール因子
        static constexpr int ZOOM_LEVEL = 10;  // ズームレベル
        static constexpr float CIRCLE_BASE_SIZE = 1.0f;  // 円の基本サイズ
        static constexpr float CIRCLE_POPULATION_SCALE = 10.0f;  // 円のサイズの人口スケール
        static constexpr float GRID_OUTER_LINE_WIDTH = 5.0f;  // グリッド外枠線の太さ
        static constexpr float GRID_INNER_LINE_WIDTH = 0.5f;  // グリッド内線の太さ
        static constexpr float MOVEMENT_LINE_WIDTH = 2.0f;  // 移動線の太さ
        static constexpr float MOVEMENT_ARROW_LINE_WIDTH = 0.1f;  // 移動矢印線の太さ

        // 言語の色定数
        static constexpr paxg::Color LANGUAGE_COLOR_UNKNOWN = paxg::Color(99, 99, 99);  // 灰色
        static constexpr paxg::Color LANGUAGE_COLOR_1 = paxg::Color(0, 131, 72);  // 緑色
        static constexpr paxg::Color LANGUAGE_COLOR_2 = paxg::Color(45, 87, 154);  // 青色
        static constexpr paxg::Color LANGUAGE_COLOR_3 = paxg::Color(182, 40, 46);  // 赤色
        static constexpr paxg::Color LANGUAGE_COLOR_4 = paxg::Color(230, 207, 0);  // 黄色
        static constexpr paxg::Color MARRIAGE_COLOR_MATRILOCAL = paxg::Color(221, 67, 98);  // 母方居住婚（ピンク）
        static constexpr paxg::Color MARRIAGE_COLOR_PATRILOCAL = paxg::Color(87, 66, 221);  // 父方居住婚（紫）

        SettlementRenderer() = default;

        /// @brief Get the mercator coordinate from the XYZTile coordinate.
        /// @brief 座標をメルカトル座標で取得
        static paxs::Vector2<double> getLocation(
            const paxs::Vector2<int>& start_position,
            const paxs::Vector2<int>& position,
            const int z) {
            return MapUtils::convertToMercatorCoordinate(start_position, position, z);
        }

        /// @brief 集落の位置からLocationPointを作成
        /// @brief Create LocationPoint from settlement position
        /// @param position 集落の位置 / Settlement position
        static LocationPoint createLocationPoint(const paxs::Vector2<int>& position) {
            return LocationPoint(
                "settlement_" + std::to_string(position.x) + "_" + std::to_string(position.y),  // key
                paxs::MercatorDeg(getLocation(
                    SimulationConstants::getInstance().getStartArea(),
                    position,
                    ZOOM_LEVEL
                )),
                10,
                Range<double>(0, 100),
                Range<double>(0, 99999999),
                MurMur3::calcHash("agent1"),
                0,  // テクスチャキーなし / No source
                1.0 // 拡大率 / Zoom factor
            );
        }

        /// @brief 言語番号から色を取得
        /// @brief Get color from language number
        /// @param language 言語番号 / Language number (0-4)
        static paxg::Color getLanguageColor(std::uint_least8_t language) {
            switch (language) {
            case 1: return LANGUAGE_COLOR_1;
            case 2: return LANGUAGE_COLOR_2;
            case 3: return LANGUAGE_COLOR_3;
            case 4: return LANGUAGE_COLOR_4;
            default: return LANGUAGE_COLOR_UNKNOWN;
            }
        }

        /// @brief 集落を描画
        /// @brief Draw settlements
        static void drawSettlements(
            const double jdn,
            const paxs::UnorderedMap<SettlementGridsType, paxs::SettlementGrid>* agents,
            const Vector2<double>& map_view_size,
            const Vector2<double>& map_view_center,
            const std::size_t select_draw
        ) {
            for (const auto& agent : *agents) {
                for (const auto& settlement : agent.second.cgetSettlements()) {
                    const auto location_point = createLocationPoint(settlement.getPosition());

                    // 経緯度の範囲外を除去
                    if (!LocationRendererHelper::isInViewBounds(
                        location_point.coordinate,
                        map_view_size, map_view_center)) continue;

                    // 時間範囲外を除去
                    if (location_point.year_range.excludes(jdn)) continue;

                    // 描画位置
                    const paxg::Vec2<double> draw_pos = MapCoordinateConverter::toScreenPos(
                        location_point.coordinate,
                        map_view_size,
                        map_view_center
                    );

                    // 円のサイズ
                    const float circle_size = CIRCLE_BASE_SIZE +
                        (settlement.getPopulation() / CIRCLE_POPULATION_SCALE);

                    // 表示モード別に描画
                    if (select_draw != 5) {
                        // 人口、農耕、mtDNA、SNP、青銅器
                        double pop_original = 0.0;
                        switch (select_draw) {
                        case 1:
                            pop_original = static_cast<double>(settlement.getPopulation());
                            break;
                        case 2:
                            pop_original = settlement.getFarmingPopulation() /
                                float(settlement.getPopulation()) * MAX_POPULATION_VISUALIZATION;
                            break;
                        case 3:
                            pop_original = settlement.getMostMtDNA() /
                                MTDNA_SCALE_FACTOR * MAX_POPULATION_VISUALIZATION;
                            break;
                        case 4:
                            pop_original = settlement.getSNP() * MAX_POPULATION_VISUALIZATION;
                            break;
                        case 6:
                            pop_original = static_cast<double>(settlement.getBronze());
                            break;
                        }

                        const std::uint_least8_t pop = (pop_original >= MAX_POPULATION_VISUALIZATION)
                            ? static_cast<std::uint_least8_t>(MAX_POPULATION_VISUALIZATION)
                            : static_cast<std::uint_least8_t>(pop_original);
                        paxg::Circle(draw_pos, circle_size).draw(
                            SimulationColor::getSettlementColor(pop));
                    }
                    else {
                        // 言語
                        paxg::Circle(draw_pos, circle_size).draw(
                            getLanguageColor(settlement.getLanguage()));
                    }
                }
            }
        }

        /// @brief 移動線を描画
        /// @brief Draw movement lines
        static void drawMovementLines(
            const double jdn,
            const paxs::UnorderedMap<SettlementGridsType, paxs::SettlementGrid>* agents,
            const std::vector<GridType4>* marriage_pos_list,
            const Vector2<double>& map_view_size,
            const Vector2<double>& map_view_center
        ) {
            // 集落の移動履歴を描画
            for (const auto& agent : *agents) {
                for (const auto& settlement : agent.second.cgetSettlements()) {
                    const auto location_point = createLocationPoint(settlement.getPosition());

                    if (!LocationRendererHelper::isInViewBounds(
                        location_point.coordinate,
                        map_view_size, map_view_center)) continue;

                    if (location_point.year_range.excludes(jdn)) continue;

                    const paxg::Vec2<double> draw_pos = MapCoordinateConverter::toScreenPos(
                        location_point.coordinate,
                        map_view_size,
                        map_view_center);

                    if (settlement.getOldPosition().x == -1 || settlement.getOldPosition().x == 0) continue;

                    if (settlement.getPositions().size() >= 1) {
                        // スプライン曲線で移動履歴を描画
                        std::vector<paxg::Vec2f> spline_points;
                        spline_points.emplace_back(draw_pos);

                        for (auto&& p : settlement.getPositions()) {
                            auto one_lli = createLocationPoint(paxs::Vector2<int>(p.x, p.y));
                            const paxg::Vec2<double> one_pos = MapCoordinateConverter::toScreenPos(
                                one_lli.coordinate,
                                map_view_size,
                                map_view_center);
                            spline_points.emplace_back(paxg::Vec2f{
                                static_cast<float>(one_pos.x()), static_cast<float>(one_pos.y()) });
                        }

                        auto old_lli = createLocationPoint(settlement.getOldPosition());
                        const paxg::Vec2<double> old_pos = MapCoordinateConverter::toScreenPos(
                            old_lli.coordinate,
                            map_view_size,
                            map_view_center);
                        spline_points.emplace_back(paxg::Vec2f{
                            static_cast<float>(old_pos.x()), static_cast<float>(old_pos.y()) });

                        paxg::Spline2D(spline_points).draw(MOVEMENT_LINE_WIDTH, paxg::Color(0, 0, 0));

                        // 矢印を描画
                        auto first_lli = createLocationPoint(settlement.getPositions()[0]);
                        const paxg::Vec2<double> first_pos = MapCoordinateConverter::toScreenPos(
                            first_lli.coordinate,
                            map_view_size,
                            map_view_center);
                        paxg::Line{first_pos, draw_pos}
                            .drawArrow(MOVEMENT_ARROW_LINE_WIDTH, paxg::Vec2f{ 8.0f, 16.0f }, paxg::Color(0, 0, 0));
                    }
                    else {
                        // 単純な移動線
                        auto old_lli = createLocationPoint(settlement.getOldPosition());
                        const paxg::Vec2<double> old_pos = MapCoordinateConverter::toScreenPos(
                            old_lli.coordinate,
                            map_view_size,
                            map_view_center);
                        paxg::Line{old_pos, draw_pos}
                            .drawArrow(MOVEMENT_LINE_WIDTH, paxg::Vec2f{ 8.0f, 16.0f }, paxg::Color(0, 0, 0));
                    }
                }
            }

            // 婚姻移動を描画
            for (const auto& marriage_pos : *marriage_pos_list) {
                const auto location_point = createLocationPoint(paxs::Vector2<int>(marriage_pos.ex, marriage_pos.ey));

                if (!LocationRendererHelper::isInViewBounds(
                    location_point.coordinate,
                    map_view_size, map_view_center)) continue;

                if (location_point.year_range.excludes(jdn)) continue;
                if (marriage_pos.sx == -1 || marriage_pos.sx == 0) continue;

                const paxg::Vec2<double> draw_pos = MapCoordinateConverter::toScreenPos(
                    location_point.coordinate,
                    map_view_size,
                    map_view_center);

                auto old_lli = createLocationPoint(paxs::Vector2<int>(marriage_pos.sx, marriage_pos.sy));
                const paxg::Vec2<double> old_pos = MapCoordinateConverter::toScreenPos(
                    old_lli.coordinate,
                    map_view_size,
                    map_view_center);

                const paxg::Color marriage_color = marriage_pos.is_matrilocality
                    ? MARRIAGE_COLOR_MATRILOCAL : MARRIAGE_COLOR_PATRILOCAL;
                paxg::Line{old_pos, draw_pos}
                    .drawArrow(MOVEMENT_LINE_WIDTH, paxg::Vec2f{ 8.0f, 16.0f }, marriage_color);
            }
        }

        /// @brief グリッド線を描画
        /// @brief Draw grid lines
        static void drawGridLines(
            const Vector2<double>& map_view_size,
            const Vector2<double>& map_view_center
        ) {
            const auto area_width = SimulationConstants::getInstance().getEndArea().x -
                SimulationConstants::getInstance().getStartArea().x;
            const auto area_height = SimulationConstants::getInstance().getEndArea().y -
                SimulationConstants::getInstance().getStartArea().y;

            const paxs::MercatorDeg start_coordinate = paxs::MercatorDeg(getLocation(
                SimulationConstants::getInstance().getStartArea(),
                paxs::Vector2<int>(0, 0), ZOOM_LEVEL));
            const paxg::Vec2f draw_start_pos = paxg::Vec2f{
                static_cast<float>((start_coordinate.x - (map_view_center.x - map_view_size.x / 2)) /
                    map_view_size.x * double(paxg::Window::width())),
                static_cast<float>(double(paxg::Window::height()) -
                    ((start_coordinate.y - (map_view_center.y - map_view_size.y / 2)) /
                        map_view_size.y * double(paxg::Window::height())))
            };

            const paxs::MercatorDeg end_coordinate = paxs::MercatorDeg(getLocation(
                SimulationConstants::getInstance().getStartArea(),
                paxs::Vector2<int>(area_width * 256, area_height * 256), ZOOM_LEVEL));
            const paxg::Vec2f draw_end_pos = paxg::Vec2f{
                static_cast<float>((end_coordinate.x - (map_view_center.x - map_view_size.x / 2)) /
                    map_view_size.x * double(paxg::Window::width())),
                static_cast<float>(double(paxg::Window::height()) -
                    ((end_coordinate.y - (map_view_center.y - map_view_size.y / 2)) /
                        map_view_size.y * double(paxg::Window::height())))
            };

            const paxs::MercatorDeg tile_coordinate = paxs::MercatorDeg(getLocation(
                SimulationConstants::getInstance().getStartArea(),
                paxs::Vector2<int>(SimulationConstants::getInstance().cell_group_length,
                    SimulationConstants::getInstance().cell_group_length), ZOOM_LEVEL));
            const paxg::Vec2f tile_pos = paxg::Vec2f{
                static_cast<float>((tile_coordinate.x - (map_view_center.x - map_view_size.x / 2)) /
                    map_view_size.x * double(paxg::Window::width())) - draw_start_pos.x(),
                static_cast<float>(double(paxg::Window::height()) -
                    ((tile_coordinate.y - (map_view_center.y - map_view_size.y / 2)) /
                        map_view_size.y * double(paxg::Window::height()))) - draw_start_pos.y()
            };

            // 外枠線を描画
            paxg::Line(draw_start_pos.x(), draw_start_pos.y(),
                draw_start_pos.x(), draw_end_pos.y()).draw(GRID_OUTER_LINE_WIDTH, paxg::Color(0, 0, 0));
            paxg::Line(draw_start_pos.x(), draw_start_pos.y(),
                draw_end_pos.x(), draw_start_pos.y()).draw(GRID_OUTER_LINE_WIDTH, paxg::Color(0, 0, 0));
            paxg::Line(draw_end_pos.x(), draw_start_pos.y(),
                draw_end_pos.x(), draw_end_pos.y()).draw(GRID_OUTER_LINE_WIDTH, paxg::Color(0, 0, 0));
            paxg::Line(draw_start_pos.x(), draw_end_pos.y(),
                draw_end_pos.x(), draw_end_pos.y()).draw(GRID_OUTER_LINE_WIDTH, paxg::Color(0, 0, 0));

            // 垂直グリッド線
            for (float i = draw_start_pos.x(); i < draw_end_pos.x(); i += tile_pos.x()) {
                paxg::Line(i, draw_start_pos.y(), i, draw_end_pos.y()).draw(
                    GRID_INNER_LINE_WIDTH, paxg::Color(0, 0, 0));
            }
            // 水平グリッド線
            for (float i = draw_start_pos.y(); i < draw_end_pos.y(); i += tile_pos.y()) {
                paxg::Line(draw_start_pos.x(), i, draw_end_pos.x(), i).draw(
                    GRID_INNER_LINE_WIDTH, paxg::Color(0, 0, 0));
            }
        }

    };
} // namespace paxs

#endif // !PAX_MAHOROBA_MAP_SETTLEMENT_RENDERER_HPP
