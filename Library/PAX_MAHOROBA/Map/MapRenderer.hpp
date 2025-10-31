/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023-2024 As Project
	[Production]	2023-2024 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_MAP_RENDERER_HPP
#define PAX_MAHOROBA_MAP_RENDERER_HPP

#include <PAX_GRAPHICA/Font.hpp>
#include <PAX_MAHOROBA/Map/Location/PersonNameManager.hpp>
#include <PAX_MAHOROBA/Map/Location/PlaceNameManager.hpp>

#include <PAX_SAPIENTICA/Calendar/JulianDayNumber.hpp>
#include <PAX_SAPIENTICA/FeatureVisibilityManager.hpp>

namespace paxs {

    /// @brief 地図の描画処理を担当するクラス
    /// @brief Class responsible for map rendering
    class MapRenderer {
    public:
        MapRenderer() = default;

        /// @brief 地名を描画
            /// @brief Draw place names
            /// @param place_name_manager 地名マネージャー
            /// @param visible 可視性マネージャー
            /// @param julian_day ユリウス日
            /// @param width 幅
            /// @param height 高さ
            /// @param center_x 中心X座標
            /// @param center_y 中心Y座標
            /// @param main_font メインフォント
            /// @param en_font 英語フォント
            /// @param pin_font ピンフォント
            void drawPlaceNames(
                PlaceNameManager& place_name_manager,
                FeatureVisibilityManager& visible,
                double julian_day,
                double width,
                double height,
                double center_x,
                double center_y,
                paxg::Font& main_font,
                paxg::Font& en_font,
                paxg::Font& pin_font
            ) {
                place_name_manager.draw(
                    visible,
                    julian_day,
                    width,
                    height,
                    center_x,
                    center_y,
                    main_font,
                    en_font,
                    pin_font
                );
            }

            /// @brief 人名を描画
            /// @brief Draw person names
            /// @param person_name_location 人名ロケーション
            /// @param julian_day ユリウス日
            /// @param width 幅
            /// @param height 高さ
            /// @param center_x 中心X座標
            /// @param center_y 中心Y座標
            /// @param main_font メインフォント
            /// @param en_font 英語フォント
            /// @param pin_font ピンフォント
            void drawPersonNames(
                PersonNameManager& person_name_manager,
                double julian_day,
                double width,
                double height,
                double center_x,
                double center_y,
                paxg::Font& main_font,
                paxg::Font& en_font,
                paxg::Font& pin_font
            ) {
                person_name_manager.draw(
                    julian_day,
                    width,
                    height,
                    center_x,
                    center_y,
                    main_font,
                    en_font,
                    pin_font
                );
            }

        #ifdef PAXS_USING_SIMULATOR
            /// @brief エージェントを描画
            /// @brief Draw agents
            /// @param agent_location エージェントロケーション
            /// @param julian_day ユリウス日
            /// @param settlement_grids 集落グリッド
            /// @param marriage_pos_list 結婚位置リスト
            /// @param width 幅
            /// @param height 高さ
            /// @param center_x 中心X座標
            /// @param center_y 中心Y座標
            template<typename SettlementRenderer, typename SettlementGrids, typename MarriagePosList>
            void drawSettlements(
                SettlementRenderer& settlement_renderer,
                double julian_day,
                SettlementGrids& settlement_grids,
                MarriagePosList& marriage_pos_list,
                double width,
                double height,
                double center_x,
                double center_y,
                std::size_t select_draw,
                bool is_line,
                bool is_arrow
            ) {
                settlement_renderer.draw(
                    julian_day,
                    settlement_grids,
                    marriage_pos_list,
                    width,
                    height,
                    center_x,
                    center_y,
                    select_draw,
                    is_line,
                    is_arrow
                );
            }

            /// @brief 集落のテキストを描画
            /// @brief Draw settlement text
            /// @param settlement_renderer 集落レンダラー
            /// @param select_draw 表示モード
            template<typename SettlementRenderer>
            void drawSettlementText(SettlementRenderer& settlement_renderer, std::size_t select_draw) {
                settlement_renderer.drawText(select_draw);
            }
        #endif
        };
    } // namespace paxs

#endif // !PAX_MAHOROBA_MAP_RENDERER_HPP
