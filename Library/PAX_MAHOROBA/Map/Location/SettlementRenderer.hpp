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

#include <PAX_MAHOROBA/Rendering/IRenderable.hpp>
#include <PAX_MAHOROBA/Rendering/SimulationColor.hpp>

#include <PAX_SAPIENTICA/MapUtility.hpp>
#include <PAX_SAPIENTICA/Map/LocationPoint.hpp>
#include <PAX_SAPIENTICA/Simulation/SettlementGrid.hpp>
#include <PAX_SAPIENTICA/UnorderedMap.hpp>

namespace paxs {
    /// @brief シミュレーションの集落を可視化する
    /// @brief Visualize simulation settlements
    class SettlementRenderer : public IRenderable {
    public:
        SettlementRenderer() = default;

        // IRenderable の実装
        void render() override {
            if (!visible_) return;

            // キャッシュされたデータが有効かチェック
            if (cached_agents_ == nullptr || cached_marriage_pos_list_ == nullptr) return;

            // キャッシュされたパラメータで draw() を呼び出し
            draw(cached_jdn_, *cached_agents_, *cached_marriage_pos_list_,
                 cached_map_view_width_, cached_map_view_height_,
                 cached_map_view_center_x_, cached_map_view_center_y_,
                 select_draw_, is_line_, is_arrow_);
        }

        RenderLayer getLayer() const override {
            return RenderLayer::MapContent;
        }

        bool isVisible() const override {
            return visible_;
        }

        void setVisible(bool visible) override {
            visible_ = visible;
        }

        /// @brief 描画パラメータを設定（MapContentManager から呼び出される）
        /// @brief Set drawing parameters (called from MapContentManager)
        void setDrawParams(
            double jdn,
            paxs::UnorderedMap<SettlementGridsType, paxs::SettlementGrid>& agents,
            const std::vector<GridType4>& marriage_pos_list,
            double map_view_width, double map_view_height,
            double map_view_center_x, double map_view_center_y,
            std::size_t select_draw, bool is_line, bool is_arrow
        ) {
            cached_jdn_ = jdn;
            cached_agents_ = &agents;
            cached_marriage_pos_list_ = &marriage_pos_list;
            cached_map_view_width_ = map_view_width;
            cached_map_view_height_ = map_view_height;
            cached_map_view_center_x_ = map_view_center_x;
            cached_map_view_center_y_ = map_view_center_y;
            select_draw_ = select_draw;
            is_line_ = is_line;
            is_arrow_ = is_arrow;
        }

    private:
        // 可視性管理
        bool visible_ = true;

        // 描画に必要なデータをキャッシュ（setDrawParams()で更新、render()で使用）
        double cached_jdn_ = 0.0;
        paxs::UnorderedMap<SettlementGridsType, paxs::SettlementGrid>* cached_agents_ = nullptr;
        const std::vector<GridType4>* cached_marriage_pos_list_ = nullptr;
        double cached_map_view_width_ = 0.0;
        double cached_map_view_height_ = 0.0;
        double cached_map_view_center_x_ = 0.0;
        double cached_map_view_center_y_ = 0.0;
        std::size_t select_draw_ = 1;
        bool is_line_ = false;
        bool is_arrow_ = true;

        /// @brief Get the mercator coordinate from the XYZTile coordinate.
        /// @brief 座標をメルカトル座標で取得
        paxs::Vector2<double> getLocation(
            const paxs::Vector2<int>& start_position,
            const paxs::Vector2<int>& position,
            const int z) const noexcept {
            return MapUtility::convertToMercatorCoordinate(start_position, position, z);
        }

        /// @brief エージェント（集落）を描画（private化）
        /// @brief Draw agents (settlements) - made private
        /// @param jdn ユリウス日
        /// @param agents 集落グリッド
        /// @param marriage_pos_list 婚姻移動のリスト
        /// @param map_view_width マップビューの幅
        /// @param map_view_height マップビューの高さ
        /// @param map_view_center_x マップビューの中心X座標
        /// @param map_view_center_y マップビューの中心Y座標
        /// @param select_draw 表示モード (1-6)
        /// @param is_line グリッド線を表示するか
        /// @param is_arrow 移動矢印を表示するか
        void draw(const double jdn,
            paxs::UnorderedMap<SettlementGridsType, paxs::SettlementGrid>& agents,
            const std::vector<GridType4>& marriage_pos_list,
            const double map_view_width, const double map_view_height, const double map_view_center_x, const double map_view_center_y,
            std::size_t select_draw, bool is_line, bool is_arrow
        ) const {

            // 地名を描画
            for (const auto& agent : agents) {
                for (const auto& settlement : agent.second.cgetSettlements()) {
                    // エージェントの初期設定を定義
                    const auto lli = LocationPoint{
                        paxs::UnorderedMap < std::uint_least32_t, std::string>(),
                            paxs::MercatorDeg(getLocation(SimulationConstants::getInstance()->getStartArea(),
                            paxs::Vector2<int>(
                    settlement.getPosition().x,settlement.getPosition().y), 10)),
                            1, 1, 10, 100,0,0,99999999,
                        MurMur3::calcHash("agent1")
                        ,0 /* 出典なし */
                        ,1.0 // 拡大率
                    };

                    // 経緯度の範囲外を除去
                    if (lli.coordinate.x < (map_view_center_x - map_view_width / 1.6)
                        || lli.coordinate.x >(map_view_center_x + map_view_width / 1.6)
                        || lli.coordinate.y < (map_view_center_y - map_view_height / 1.6)
                        || lli.coordinate.y >(map_view_center_y + map_view_height / 1.6)) continue;

                    // 範囲内の場合
                    if (lli.min_view > map_view_height || lli.max_view < map_view_height || lli.min_year > jdn || lli.max_year < jdn) {
                        if (lli.min_year > jdn) continue;
                        if (lli.max_year < jdn) continue;

                        // 描画位置
                        const paxg::Vec2i draw_pos = paxg::Vec2i{
    static_cast<int>((lli.coordinate.x - (map_view_center_x - map_view_width / 2)) / map_view_width * double(paxg::Window::width())),
        static_cast<int>(double(paxg::Window::height()) - ((lli.coordinate.y - (map_view_center_y - map_view_height / 2)) / map_view_height * double(paxg::Window::height())))
                        };

                        // エージェント
                        // if (lli.lpe == MurMur3::calcHash("agent1"))
                        if (select_draw != 5) {
                            double pop_original = 0.0;
                            switch (select_draw)
                            {
                            case 1:
                                // const std::size_t
                                // pop_original = settlement.getFarmingPopulation();
                                pop_original = static_cast<double>(settlement.getPopulation());
                                break;
                            case 2:
                                //const float
                                pop_original = settlement.getFarmingPopulation() / float(settlement.getPopulation()) * 75.0;
                                break;
                            case 3:
                                //const float
                                pop_original = settlement.getMostMtDNA() / 27.0 * 75.0;
                                break;
                            case 4:
                                //const double
                                pop_original = settlement.getSNP() * 75.0;
                                break;
                            case 6:
                                pop_original = static_cast<double>(settlement.getBronze());
                                break;
                            }

                            const std::uint_least8_t pop = (pop_original >= 75) ? 75 : static_cast<std::uint_least8_t>(pop_original);
                            paxg::Circle(draw_pos,
                                1.0f + (settlement.getPopulation() / 10.0f)//2.0f
                            ).draw(SimulationColor::getSettlementColor(pop));
                        }
                        else {
                            paxg::Color language_color = paxg::Color(99, 99, 99); // 灰色
                            switch (settlement.getLanguage())
                            {
                            case 0:
                                language_color = paxg::Color(99, 99, 99); // 灰色
                                break;
                            case 1:
                                language_color = paxg::Color(0, 131, 72); // 緑色
                                break;
                            case 2:
                                language_color = paxg::Color(45, 87, 154); // 青色
                                break;
                            case 3:
                                language_color = paxg::Color(182, 40, 46); // 赤色
                                break;
                            case 4:
                                language_color = paxg::Color(230, 207, 0); // 黄色
                                break;
                            }
                            paxg::Circle(draw_pos,
                                1.0f + (settlement.getPopulation() / 10.0f)//2.0f
                            ).draw(language_color);
                        }

                    }
                }
            }

            // グリッド線を描画
            if (is_line) {
                const auto area_width = SimulationConstants::getInstance()->getEndArea().x - SimulationConstants::getInstance()->getStartArea().x;
                const auto area_height = SimulationConstants::getInstance()->getEndArea().y - SimulationConstants::getInstance()->getStartArea().y;

                const paxs::MercatorDeg start_coordinate = paxs::MercatorDeg(getLocation(SimulationConstants::getInstance()->getStartArea(),
                    paxs::Vector2<int>(0, 0), 10));
                const paxg::Vec2f draw_start_pos = paxg::Vec2f{
    static_cast<float>((start_coordinate.x - (map_view_center_x - map_view_width / 2)) / map_view_width * double(paxg::Window::width())),
        static_cast<float>(double(paxg::Window::height()) - ((start_coordinate.y - (map_view_center_y - map_view_height / 2)) / map_view_height * double(paxg::Window::height())))
                };
                const paxs::MercatorDeg end_coordinate = paxs::MercatorDeg(getLocation(SimulationConstants::getInstance()->getStartArea(),
                    paxs::Vector2<int>(area_width * 256, area_height * 256), 10));
                const paxg::Vec2f draw_end_pos = paxg::Vec2f{
    static_cast<float>((end_coordinate.x - (map_view_center_x - map_view_width / 2)) / map_view_width * double(paxg::Window::width())),
        static_cast<float>(double(paxg::Window::height()) - ((end_coordinate.y - (map_view_center_y - map_view_height / 2)) / map_view_height * double(paxg::Window::height())))
                };
                const paxs::MercatorDeg tile_coordinate = paxs::MercatorDeg(getLocation(SimulationConstants::getInstance()->getStartArea(),
                    paxs::Vector2<int>(SimulationConstants::getInstance()->cell_group_length, SimulationConstants::getInstance()->cell_group_length), 10));
                const paxg::Vec2f tile_pos = paxg::Vec2f{
    static_cast<float>((tile_coordinate.x - (map_view_center_x - map_view_width / 2)) / map_view_width * double(paxg::Window::width())) - draw_start_pos.x(),
        static_cast<float>(double(paxg::Window::height()) - ((tile_coordinate.y - (map_view_center_y - map_view_height / 2)) / map_view_height * double(paxg::Window::height()))) - draw_start_pos.y()
                };

                paxg::Line(
                    static_cast<float>(draw_start_pos.x()), static_cast<float>(draw_start_pos.y()),
                    static_cast<float>(draw_start_pos.x()), static_cast<float>(draw_end_pos.y())
                ).draw(5, paxg::Color(0, 0, 0));
                paxg::Line(
                    static_cast<float>(draw_start_pos.x()), static_cast<float>(draw_start_pos.y()),
                    static_cast<float>(draw_end_pos.x()), static_cast<float>(draw_start_pos.y())
                ).draw(5, paxg::Color(0, 0, 0));
                paxg::Line(
                    static_cast<float>(draw_end_pos.x()), static_cast<float>(draw_start_pos.y()),
                    static_cast<float>(draw_end_pos.x()), static_cast<float>(draw_end_pos.y())
                ).draw(5, paxg::Color(0, 0, 0));
                paxg::Line(
                    static_cast<float>(draw_start_pos.x()), static_cast<float>(draw_end_pos.y()),
                    static_cast<float>(draw_end_pos.x()), static_cast<float>(draw_end_pos.y())
                ).draw(5, paxg::Color(0, 0, 0));

                for (float i = draw_start_pos.x(); i < draw_end_pos.x(); i += tile_pos.x()) {
                    paxg::Line(
                        static_cast<float>(i), static_cast<float>(draw_start_pos.y()),
                        static_cast<float>(i), static_cast<float>(draw_end_pos.y())
                    ).draw(0.5, paxg::Color(0, 0, 0));
                }
                for (float i = draw_start_pos.y(); i < draw_end_pos.y(); i += tile_pos.y()) {
                    paxg::Line(
                        static_cast<float>(draw_start_pos.x()), static_cast<float>(i),
                        static_cast<float>(draw_end_pos.x()), static_cast<float>(i)
                    ).draw(0.5, paxg::Color(0, 0, 0));
                }
            }

            // 移動線を描画
            if (is_arrow) {
                for (const auto& agent : agents) {
                    for (const auto& settlement : agent.second.cgetSettlements()) {
                        // エージェントの初期設定を定義
                        const auto lli = LocationPoint{
                            paxs::UnorderedMap < std::uint_least32_t, std::string>(),
                                paxs::MercatorDeg(getLocation(SimulationConstants::getInstance()->getStartArea(),
                                paxs::Vector2<int>(
                        settlement.getPosition().x,settlement.getPosition().y), 10)),
                                1, 1, 10, 100,0,0,99999999,
                            MurMur3::calcHash("agent1")
                            ,0 /* 出典なし */
                            ,1.0 // 拡大率
                        };

                        // 経緯度の範囲外を除去
                        if (lli.coordinate.x < (map_view_center_x - map_view_width / 1.6)
                            || lli.coordinate.x >(map_view_center_x + map_view_width / 1.6)
                            || lli.coordinate.y < (map_view_center_y - map_view_height / 1.6)
                            || lli.coordinate.y >(map_view_center_y + map_view_height / 1.6)) continue;

                        // 範囲内の場合
                        if (lli.min_view > map_view_height || lli.max_view < map_view_height || lli.min_year > jdn || lli.max_year < jdn) {
                            if (lli.min_year > jdn) continue;
                            if (lli.max_year < jdn) continue;

                            // 描画位置
                            const paxg::Vec2f draw_pos = paxg::Vec2f{
        static_cast<float>((lli.coordinate.x - (map_view_center_x - map_view_width / 2)) / map_view_width * double(paxg::Window::width())),
            static_cast<float>(double(paxg::Window::height()) - ((lli.coordinate.y - (map_view_center_y - map_view_height / 2)) / map_view_height * double(paxg::Window::height())))
                            };

                            if (settlement.getOldPosition().x != -1 && settlement.getOldPosition().x != 0) {
                                if (settlement.getPositions().size() >= 1) {

                                    // 過去の位置
                                    auto old_lli = lli;
                                    old_lli.coordinate = paxs::MercatorDeg(getLocation(SimulationConstants::getInstance()->getStartArea(),
                                        paxs::Vector2<int>(
                                            settlement.getOldPosition().x,
                                            settlement.getOldPosition().y), 10));
                                    const paxg::Vec2i draw_old_pos = paxg::Vec2i{
            static_cast<int>((old_lli.coordinate.x - (map_view_center_x - map_view_width / 2)) / map_view_width * double(paxg::Window::width())),
                static_cast<int>(double(paxg::Window::height()) - ((old_lli.coordinate.y - (map_view_center_y - map_view_height / 2)) / map_view_height * double(paxg::Window::height())))
                                    };

                                    std::vector<paxg::Vec2f> spline_points;
                                    spline_points.push_back(paxg::Vec2f{ draw_pos.x(), draw_pos.y() });
                                    for (auto&& p : settlement.getPositions()) {
                                        auto one_lli = lli;
                                        one_lli.coordinate = paxs::MercatorDeg(getLocation(SimulationConstants::getInstance()->getStartArea(),
                                            paxs::Vector2<int>(p.x, p.y), 10));
                                        const paxg::Vec2i draw_one_pos = paxg::Vec2i{
        static_cast<int>((one_lli.coordinate.x - (map_view_center_x - map_view_width / 2)) / map_view_width * double(paxg::Window::width())),
            static_cast<int>(double(paxg::Window::height()) - ((one_lli.coordinate.y - (map_view_center_y - map_view_height / 2)) / map_view_height * double(paxg::Window::height())))
                                        };
                                        spline_points.push_back(paxg::Vec2f{ static_cast<float>(draw_one_pos.x()), static_cast<float>(draw_one_pos.y()) });
                                    }
                                    spline_points.push_back(paxg::Vec2f{ static_cast<float>(draw_old_pos.x()), static_cast<float>(draw_old_pos.y()) });

                                    const paxg::Spline2D spline(spline_points);
                                    spline.draw(2.0, paxg::Color(0, 0, 0));

                                    // 過去の位置
                                    auto one_lli = lli;
                                    one_lli.coordinate = paxs::MercatorDeg(getLocation(SimulationConstants::getInstance()->getStartArea(),
                                        paxs::Vector2<int>(
                                            settlement.getPositions()[0].x,
                                            settlement.getPositions()[0].y), 10));
                                    const paxg::Vec2f draw_one_pos = paxg::Vec2f{
            static_cast<float>((one_lli.coordinate.x - (map_view_center_x - map_view_width / 2)) / map_view_width * double(paxg::Window::width())),
                static_cast<float>(double(paxg::Window::height()) - ((one_lli.coordinate.y - (map_view_center_y - map_view_height / 2)) / map_view_height * double(paxg::Window::height())))
                                    };
                                    paxg::Line{ draw_one_pos.x(), draw_one_pos.y(), draw_pos.x(), draw_pos.y() }.drawArrow(0.1, paxg::Vec2f{ 8.0f, 16.0f }, paxg::Color(0, 0, 0));
                                }
                                else {
                                    // 過去の位置
                                    auto old_lli = lli;
                                    old_lli.coordinate = paxs::MercatorDeg(getLocation(SimulationConstants::getInstance()->getStartArea(),
                                        paxs::Vector2<int>(
                                            settlement.getOldPosition().x,
                                            settlement.getOldPosition().y), 10));
                                    const paxg::Vec2f draw_old_pos = paxg::Vec2f{
            static_cast<float>((old_lli.coordinate.x - (map_view_center_x - map_view_width / 2)) / map_view_width * double(paxg::Window::width())),
                static_cast<float>(double(paxg::Window::height()) - ((old_lli.coordinate.y - (map_view_center_y - map_view_height / 2)) / map_view_height * double(paxg::Window::height())))
                                    };
                                    paxg::Line{ draw_old_pos.x(), draw_old_pos.y(), draw_pos.x(), draw_pos.y() }.drawArrow(2, paxg::Vec2f{ 8.0f, 16.0f }, paxg::Color(0, 0, 0));
                                }
                            }

                        }

                    }
                }
                // 移動線を描画
                for (const auto& marriage_pos : marriage_pos_list) {
                    // エージェントの初期設定を定義
                    const auto lli = LocationPoint{
                        paxs::UnorderedMap < std::uint_least32_t, std::string>(),
                            paxs::MercatorDeg(getLocation(SimulationConstants::getInstance()->getStartArea(),
                            paxs::Vector2<int>(
                    marriage_pos.ex,marriage_pos.ey), 10)),
                            1, 1, 10, 100,0,0,99999999,
                        MurMur3::calcHash("agent1")
                        ,0 /* 出典なし */
                        ,1.0 // 拡大率
                    };

                    // 経緯度の範囲外を除去
                    if (lli.coordinate.x < (map_view_center_x - map_view_width / 1.6)
                        || lli.coordinate.x >(map_view_center_x + map_view_width / 1.6)
                        || lli.coordinate.y < (map_view_center_y - map_view_height / 1.6)
                        || lli.coordinate.y >(map_view_center_y + map_view_height / 1.6)) continue;

                    // 範囲内の場合
                    if (lli.min_view > map_view_height || lli.max_view < map_view_height || lli.min_year > jdn || lli.max_year < jdn) {
                        if (lli.min_year > jdn) continue;
                        if (lli.max_year < jdn) continue;

                        // 描画位置
                        const paxg::Vec2f draw_pos = paxg::Vec2f{
    static_cast<float>((lli.coordinate.x - (map_view_center_x - map_view_width / 2)) / map_view_width * double(paxg::Window::width())),
        static_cast<float>(double(paxg::Window::height()) - ((lli.coordinate.y - (map_view_center_y - map_view_height / 2)) / map_view_height * double(paxg::Window::height())))
                        };

                        if (marriage_pos.sx != -1 && marriage_pos.sx != 0) {
                            // 過去の位置
                            auto old_lli = lli;
                            old_lli.coordinate = paxs::MercatorDeg(getLocation(SimulationConstants::getInstance()->getStartArea(),
                                paxs::Vector2<int>(
                                    marriage_pos.sx,
                                    marriage_pos.sy), 10));
                            const paxg::Vec2f draw_old_pos = paxg::Vec2f{
    static_cast<float>((old_lli.coordinate.x - (map_view_center_x - map_view_width / 2)) / map_view_width * double(paxg::Window::width())),
        static_cast<float>(double(paxg::Window::height()) - ((old_lli.coordinate.y - (map_view_center_y - map_view_height / 2)) / map_view_height * double(paxg::Window::height())))
                            };
                            paxg::Line{ draw_old_pos.x(), draw_old_pos.y(), draw_pos.x(), draw_pos.y() }.drawArrow(2, paxg::Vec2f{ 8.0f, 16.0f }, (marriage_pos.is_matrilocality) ? paxg::Color(221, 67, 98) : paxg::Color(87, 66, 221));
                        }
                    }
                }
            }
        }

    };
} // namespace paxs

#endif // !PAX_MAHOROBA_MAP_SETTLEMENT_RENDERER_HPP
