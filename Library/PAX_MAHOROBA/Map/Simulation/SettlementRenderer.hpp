/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_MAP_SETTLEMENT_RENDERER_HPP
#define PAX_MAHOROBA_MAP_SETTLEMENT_RENDERER_HPP

#include <utility>
#include <vector>

#include <PAX_GRAPHICA/Circle.hpp>
#include <PAX_GRAPHICA/Line.hpp>
#include <PAX_GRAPHICA/Spline2D.hpp>

#include <PAX_MAHOROBA/Map/Content/Renderer/GeometryRenderer.hpp>
#include <PAX_MAHOROBA/Map/Core/MapCoordinateConverter.hpp>
#include <PAX_MAHOROBA/Rendering/SimulationColor.hpp>

#include <PAX_SAPIENTICA/Core/Type/UnorderedMap.hpp>
#include <PAX_SAPIENTICA/Core/Type/Vector2.hpp>
#include <PAX_SAPIENTICA/Geography/Coordinate/Projection.hpp>
#include <PAX_SAPIENTICA/Simulation/Entity/SettlementGrid.hpp>

namespace paxs {
    /// @brief シミュレーションの集落を可視化する
    /// @brief Visualize simulation settlements
    class SettlementRenderer : public GeometryRenderer {
    public:
        /// @brief エージェント（集落）を描画（private化）
        /// @brief Draw agents (settlements) - made private
        /// @param agents 集落グリッド
        /// @param marriage_pos_list 婚姻移動のリスト
        /// @param bronze_share_list 青銅交換のリスト
        /// @param map_view_size マップビューのサイズ
        /// @param map_view_center マップビューの中心座標
        /// @param select_draw 表示モード (1-6)
        /// @param is_line グリッド線を表示するか
        /// @param is_arrow 移動矢印を表示するか
        static void draw(
            const paxs::UnorderedMap<SettlementGridsType, paxs::SettlementGrid>* agents,
            const std::vector<GridType4>* marriage_pos_list,
            const std::vector<std::pair<paxs::Vector2<int>, paxs::Vector2<int>>>* bronze_share_list,
            const Vector2<double>& map_view_size,
            const Vector2<double>& map_view_center,
            const std::size_t select_draw, const bool is_line, const bool is_arrow
        ) {
            // 集落を描画
            drawSettlements(agents, map_view_size, map_view_center, select_draw);

            // グリッド線を描画
            if (is_line) {
                drawGridLines(map_view_size, map_view_center);
            }

            // 移動線を描画
            if (is_arrow) {
                // 青銅交換の矢印（移動・婚姻より下のレイヤーに描画）
                drawBronzeShareLines(bronze_share_list, map_view_size, map_view_center);

                drawMovementLines(agents, marriage_pos_list,
                    map_view_size, map_view_center);
            }
        }

    private:
        // 描画定数
        static constexpr double MAX_POPULATION_VISUALIZATION = 75.0;  // 人口可視化の最大値
        static constexpr double MTDNA_SCALE_FACTOR = 27.0;  // mtDNA可視化のスケール因子
        static constexpr double YDNA_SCALE_FACTOR = 27.0;  // Y-DNA可視化のスケール因子
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

        // 婚姻色の定義
        static constexpr paxg::Color MARRIAGE_COLOR_MATRILOCAL = paxg::Color(221, 67, 98);  // 母方居住婚（ピンク）
        static constexpr paxg::Color MARRIAGE_COLOR_PATRILOCAL = paxg::Color(87, 66, 221);  // 父方居住婚（紫）

        // 青銅交換色の定義
        static constexpr paxg::Color BRONZE_SHARE_COLOR = paxg::Color(221, 215, 66); // 黄色 #DDD742

        // 土器系統色の定義 (A-H)
        static constexpr paxg::Color POTTERY_COLOR_A = paxg::Color(231, 76, 60);
        static constexpr paxg::Color POTTERY_COLOR_B = paxg::Color(230, 126, 34);
        static constexpr paxg::Color POTTERY_COLOR_C = paxg::Color(241, 196, 15);
        static constexpr paxg::Color POTTERY_COLOR_D = paxg::Color(46, 204, 113);
        static constexpr paxg::Color POTTERY_COLOR_E = paxg::Color(26, 188, 156);
        static constexpr paxg::Color POTTERY_COLOR_F = paxg::Color(52, 152, 219);
        static constexpr paxg::Color POTTERY_COLOR_G = paxg::Color(155, 89, 182);
        static constexpr paxg::Color POTTERY_COLOR_H = paxg::Color(149, 165, 166);
        static constexpr paxg::Color POTTERY_COLOR_NONE = paxg::Color(99, 99, 99);

        SettlementRenderer() = default;

        /// @brief グリッド座標をEPSG:3857(Webメルカトル) 座標に変換
        /// @brief Convert grid coordinate to Mercator coordinate
        /// @param position グリッド座標 / Grid coordinate
        /// @return EPSG:3857(Webメルカトル) 座標 / Web Mercator coordinate
        static paxs::WebMercatorDeg positionToWebMercator(const paxs::Vector2<int>& position) {
            return paxs::WebMercatorDeg::fromXYZTile(
                SimulationConstants::getInstance().getStartArea(),
                position,
                ZOOM_LEVEL
            );
        }

        static paxg::Color getPotteryColor(const std::uint_least8_t pottery_id) {
            switch (pottery_id) {
            case 0: return POTTERY_COLOR_A;
            case 1: return POTTERY_COLOR_B;
            case 2: return POTTERY_COLOR_C;
            case 3: return POTTERY_COLOR_D;
            case 4: return POTTERY_COLOR_E;
            case 5: return POTTERY_COLOR_F;
            case 6: return POTTERY_COLOR_G;
            case 7: return POTTERY_COLOR_H;
            default: return POTTERY_COLOR_NONE;
            }
        }

        static std::uint_least8_t getDominantPotteryId(const paxs::Settlement& settlement) {
            const auto stats = settlement.getStatistics();
            if (stats.pottery_counts.empty()) {
                return 255;
            }

            int max_count = 0;
            std::uint_least8_t max_id = 255;
            for (const auto& kv : stats.pottery_counts) {
                const std::uint_least8_t id = kv.first;
                const int count = kv.second;
                if (count > max_count) {
                    max_count = count;
                    max_id = id;
                }
            }
            return max_id;
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
            const paxs::UnorderedMap<SettlementGridsType, paxs::SettlementGrid>* agents,
            const Vector2<double>& map_view_size,
            const Vector2<double>& map_view_center,
            const std::size_t select_draw
        ) {
            for (const auto& agent : *agents) {
                for (const auto& settlement : agent.second.cgetSettlements()) {
                    const auto coordinate = positionToWebMercator(settlement.getPosition());

                    // 経緯度の範囲外を除去
                    if (!isInViewport(coordinate, map_view_size, map_view_center)) continue;

                    // 描画位置
                    const paxs::Vector2<double> draw_pos = MapCoordinateConverter::toScreenPos(
                        coordinate,
                        map_view_size,
                        map_view_center
                    );

                    // 円のサイズ
                    const float circle_size = CIRCLE_BASE_SIZE +
                        (settlement.getPopulation() / CIRCLE_POPULATION_SCALE);

                    // 表示モード別に描画
                    if (select_draw == 5) {
                        // 言語
                        paxg::Circle(draw_pos, circle_size).draw(
                            getLanguageColor(settlement.getLanguage()));
                    }
                    else if (select_draw == 7) {
                        // 土器（最多系統）
                        const std::uint_least8_t pottery_id = getDominantPotteryId(settlement);
                        paxg::Circle(draw_pos, circle_size).draw(
                            getPotteryColor(pottery_id));
                    }
                    else if (select_draw == 8) {
                        // Y-DNA
                        paxg::Circle(draw_pos, circle_size).draw(
                            getLanguageColor(settlement.getMostYDNA()));
                    }
                    else {
                        // 人口、農耕、mtDNA、SNP、青銅器
                        double pop_original = 0.0;
                        switch (select_draw) {
                        case 1:
                            pop_original = static_cast<double>(settlement.getPopulation());
                            break;
                        case 2:
                            pop_original = settlement.getFarmingPopulation() /
                                static_cast<double>(settlement.getPopulation()) * MAX_POPULATION_VISUALIZATION;
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
                }
            }
        }

        /// @brief 青銅交換の矢印を描画
        static void drawBronzeShareLines(
            const std::vector<std::pair<paxs::Vector2<int>, paxs::Vector2<int>>>* bronze_share_list,
            const Vector2<double>& map_view_size,
            const Vector2<double>& map_view_center
        ) {
            if (!bronze_share_list) return;

            for (const auto& share : *bronze_share_list) {
                const auto end_coord = positionToWebMercator(share.second);

                if (!isInViewport(end_coord, map_view_size, map_view_center)) {
                    continue;
                }

                const paxs::Vector2<double> end_pos = MapCoordinateConverter::toScreenPos(
                    end_coord,
                    map_view_size,
                    map_view_center);

                const auto start_coord = positionToWebMercator(share.first);
                const paxs::Vector2<double> start_pos = MapCoordinateConverter::toScreenPos(
                    start_coord,
                    map_view_size,
                    map_view_center);

                // 青銅交換を表す直線の矢印を描画
                paxg::Line{ start_pos, end_pos }
                .drawArrow(MOVEMENT_LINE_WIDTH, paxs::Vector2<float>{ 8.0f, 16.0f }, BRONZE_SHARE_COLOR);
            }
        }

        /// @brief 移動線を描画
               /// @brief Draw movement lines
        static void drawMovementLines(
            const paxs::UnorderedMap<SettlementGridsType, paxs::SettlementGrid>* agents,
            const std::vector<GridType4>* marriage_pos_list,
            const Vector2<double>& map_view_size,
            const Vector2<double>& map_view_center
        ) {
            // 集落の移動履歴を描画
            for (const auto& agent : *agents) {
                for (const auto& settlement : agent.second.cgetSettlements()) {
                    const auto coordinate = positionToWebMercator(settlement.getPosition());

                    if (!isInViewport(coordinate, map_view_size, map_view_center)) {
                        continue;
                    }

                    const paxs::Vector2<double> draw_pos = MapCoordinateConverter::toScreenPos(
                        coordinate,
                        map_view_size,
                        map_view_center);

                    if (settlement.getOldPosition().x == -1 || settlement.getOldPosition().x == 0) continue;

                    // ★修正箇所: A*の経路データがある場合 (通常2点以上入っているはず)
                    if (settlement.getPositions().size() >= 2) {
                        // スプライン曲線で移動履歴を描画
                        std::vector<paxs::Vector2<double>> spline_points;
                        spline_points.emplace_back(draw_pos);

                        // A*が生成した [Start -> ... -> End] の順序をそのまま使用する
                        // 余計な draw_pos(End) や old_pos(Start) の追加は行わない
                        for (const auto& p : settlement.getPositions()) {
                            const auto one_coord = positionToWebMercator(paxs::Vector2<int>(p.x, p.y));
                            const paxs::Vector2<double> one_pos = MapCoordinateConverter::toScreenPos(
                                one_coord,
                                map_view_size,
                                map_view_center);
                            spline_points.emplace_back(one_pos);
                        }

                        const auto old_coord = positionToWebMercator(settlement.getOldPosition());
                        const paxs::Vector2<double> old_pos = MapCoordinateConverter::toScreenPos(
                            old_coord,
                            map_view_size,
                            map_view_center);
                        spline_points.emplace_back(old_pos);

                        // 矢印を描画 (パスの最後の区間に矢印を乗せる)
                        if (spline_points.size() >= 2) {
                            const auto& last = spline_points.back();
                            const auto& prev = spline_points[spline_points.size() - 2];

                            // 矢印を描画
                            const auto first_coord = positionToWebMercator(settlement.getPositions()[0]);
                            const paxs::Vector2<double> first_pos = MapCoordinateConverter::toScreenPos(
                                first_coord,
                                map_view_size,
                                map_view_center);
                            paxg::Line{ first_pos, draw_pos }
                            .drawArrow(MOVEMENT_ARROW_LINE_WIDTH, paxs::Vector2<float>{ 8.0f, 16.0f }, paxg::Color(0, 0, 0));
                        }
                    }
                    else {
                        // A*データがない、または単純移動の場合 (直線)
                        const auto old_coord = positionToWebMercator(settlement.getOldPosition());
                        const paxs::Vector2<double> old_pos = MapCoordinateConverter::toScreenPos(
                            old_coord,
                            map_view_size,
                            map_view_center);
                        paxg::Line{ old_pos, draw_pos }
                        .drawArrow(MOVEMENT_LINE_WIDTH, paxs::Vector2<float>{ 8.0f, 16.0f }, paxg::Color(0, 0, 0));
                    }
                }
            }

            // 婚姻移動を描画 (変更なし)
            for (const auto& marriage_pos : *marriage_pos_list) {
                // ... (既存のコードのまま) ...
                const auto coordinate = positionToWebMercator(paxs::Vector2<int>(marriage_pos.ex, marriage_pos.ey));

                if (!isInViewport(coordinate, map_view_size, map_view_center)) {
                    continue;
                }

                if (marriage_pos.sx == -1 || marriage_pos.sx == 0) continue;

                const paxs::Vector2<double> draw_pos = MapCoordinateConverter::toScreenPos(
                    coordinate,
                    map_view_size,
                    map_view_center);

                const auto old_coord = positionToWebMercator(paxs::Vector2<int>(marriage_pos.sx, marriage_pos.sy));
                const paxs::Vector2<double> old_pos = MapCoordinateConverter::toScreenPos(
                    old_coord,
                    map_view_size,
                    map_view_center);

                const paxg::Color marriage_color = marriage_pos.is_matrilocality
                    ? MARRIAGE_COLOR_MATRILOCAL : MARRIAGE_COLOR_PATRILOCAL;
                paxg::Line{ old_pos, draw_pos }
                .drawArrow(MOVEMENT_LINE_WIDTH, paxs::Vector2<float>{ 8.0f, 16.0f }, marriage_color);
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

            const paxs::WebMercatorDeg start_coordinate = positionToWebMercator(paxs::Vector2<int>(0, 0));
            const paxs::Vector2<float> draw_start_pos {
                static_cast<float>((start_coordinate.x - (map_view_center.x - map_view_size.x / 2)) /
                    map_view_size.x * double(paxg::Window::width())),
                static_cast<float>(double(paxg::Window::height()) -
                    ((start_coordinate.y - (map_view_center.y - map_view_size.y / 2)) /
                        map_view_size.y * double(paxg::Window::height())))
            };

            const paxs::WebMercatorDeg end_coordinate = positionToWebMercator(
                paxs::Vector2<int>(area_width * 256, area_height * 256));
            const paxs::Vector2<float> draw_end_pos {
                static_cast<float>((end_coordinate.x - (map_view_center.x - map_view_size.x / 2)) /
                    map_view_size.x * double(paxg::Window::width())),
                static_cast<float>(double(paxg::Window::height()) -
                    ((end_coordinate.y - (map_view_center.y - map_view_size.y / 2)) /
                        map_view_size.y * double(paxg::Window::height())))
            };

            const paxs::WebMercatorDeg tile_coordinate = positionToWebMercator(
                paxs::Vector2<int>(SimulationConstants::getInstance().cell_group_length,
                    SimulationConstants::getInstance().cell_group_length));
            const paxs::Vector2<float> tile_pos {
                static_cast<float>((tile_coordinate.x - (map_view_center.x - map_view_size.x / 2)) /
                    map_view_size.x * double(paxg::Window::width())) - draw_start_pos.x,
                static_cast<float>(double(paxg::Window::height()) -
                    ((tile_coordinate.y - (map_view_center.y - map_view_size.y / 2)) /
                        map_view_size.y * double(paxg::Window::height()))) - draw_start_pos.y
            };

            // 外枠線を描画
            paxg::Line(draw_start_pos.x, draw_start_pos.y,
                draw_start_pos.x, draw_end_pos.y).draw(GRID_OUTER_LINE_WIDTH, paxg::Color(0, 0, 0));
            paxg::Line(draw_start_pos.x, draw_start_pos.y,
                draw_end_pos.x, draw_start_pos.y).draw(GRID_OUTER_LINE_WIDTH, paxg::Color(0, 0, 0));
            paxg::Line(draw_end_pos.x, draw_start_pos.y,
                draw_end_pos.x, draw_end_pos.y).draw(GRID_OUTER_LINE_WIDTH, paxg::Color(0, 0, 0));
            paxg::Line(draw_start_pos.x, draw_end_pos.y,
                draw_end_pos.x, draw_end_pos.y).draw(GRID_OUTER_LINE_WIDTH, paxg::Color(0, 0, 0));

            // 垂直グリッド線
            for (float i = draw_start_pos.x; i < draw_end_pos.x; i += tile_pos.x) {
                paxg::Line(i, draw_start_pos.y, i, draw_end_pos.y).draw(
                    GRID_INNER_LINE_WIDTH, paxg::Color(0, 0, 0));
            }
            // 水平グリッド線
            for (float i = draw_start_pos.y; i < draw_end_pos.y; i += tile_pos.y) {
                paxg::Line(draw_start_pos.x, i, draw_end_pos.x, i).draw(
                    GRID_INNER_LINE_WIDTH, paxg::Color(0, 0, 0));
            }
        }

    };
} // namespace paxs

#endif // !PAX_MAHOROBA_MAP_SETTLEMENT_RENDERER_HPP
