/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_MAP_CONTROLLER_HPP
#define PAX_MAHOROBA_MAP_CONTROLLER_HPP

#include <memory>
#include <new>

#ifdef PAXS_USING_SIMULATOR
#include <PAX_MAHOROBA/Simulation/AgentLocation.hpp>
#include <PAX_SAPIENTICA/Simulation/SettlementSimulator.hpp>
#endif

#include <PAX_MAHOROBA/LocationPoint.hpp>
#include <PAX_MAHOROBA/MapViewport.hpp>
#include <PAX_MAHOROBA/LocationRange.hpp>
#include <PAX_MAHOROBA/PersonLocation.hpp>
#include <PAX_MAHOROBA/MapDrawer.hpp>
#include <PAX_SAPIENTICA/Map/MapDomainLogic.hpp>
#include <PAX_SAPIENTICA/Calendar/Koyomi.hpp>
#include <PAX_SAPIENTICA/GraphicVisualizationList.hpp>
#include <PAX_SAPIENTICA/Language.hpp>
#include <PAX_SAPIENTICA/MurMur3.hpp>

namespace paxs {

    /// @brief 地図コントローラー（統合制御クラス）
    /// @brief Map Controller (Integrated Control Class)
    class MapController {
    public:
        std::unique_ptr<TextureLocation> texture_location; // 地図上に描画する画像の一覧

        PlaceNameLocation place_name_location{}; // 地名
        PersonNameLocation person_name_location{}; // 人名

    private:
#ifdef PAXS_USING_SIMULATOR
        std::unique_ptr<AgentLocation> agent_location; // エージェント
#endif
        paxs::map::MapDomainLogic map_domain_logic_; // ドメインロジック
        MapDrawer drawer_; // 描画処理

    public:
        MapController()
            :texture_location(std::unique_ptr<TextureLocation>(new(std::nothrow) TextureLocation))
#ifdef PAXS_USING_SIMULATOR
            ,agent_location(std::unique_ptr<AgentLocation>(new(std::nothrow) AgentLocation))
#endif
        {}

        void init() {
            // 地名
            place_name_location.init();
            place_name_location.add();
            person_name_location.init();
            person_name_location.add();

        }

        /// @brief 更新処理（統合版）
        /// @brief Update and render (integrated version)
        void update(
            MapViewport& map_viewport,
            const paxs::Koyomi& koyomi,
            paxg::Font& main_font,
            paxg::Font& en_font,
            paxg::Font& pin_font,
#ifdef PAXS_USING_SIMULATOR
            std::unique_ptr<paxs::SettlementSimulator>& simulator,
#endif
            paxs::GraphicVisualizationList& visible
            ) {
            if (visible[MurMur3::calcHash("Map")]) { // 地図が「可視」の場合は描画する
                // 地図上に画像を描画する
                texture_location->update(map_viewport.getCenterX(), map_viewport.getCenterY(), map_viewport.getWidth(), map_viewport.getHeight());

                // 描画処理（旧Presenterの処理を統合）
                const double width = map_viewport.getWidth();
                const double height = map_viewport.getHeight();
                const double center_x = map_viewport.getCenterX();
                const double center_y = map_viewport.getCenterY();
                const double julian_day = koyomi.jdn.cgetDay();

                // 地名を描画
                drawer_.drawPlaceNames(
                    place_name_location,
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

                // 人名を描画
                drawer_.drawPersonNames(
                    person_name_location,
                    julian_day,
                    width,
                    height,
                    center_x,
                    center_y,
                    main_font,
                    en_font,
                    pin_font
                );

#ifdef PAXS_USING_SIMULATOR
                if (simulator) {
                    // エージェントを描画
                    drawer_.drawAgents(
                        *agent_location,
                        julian_day,
                        simulator->getSettlementGrids(),
                        simulator->getMarriagePosList(),
                        width,
                        height,
                        center_x,
                        center_y
                    );

                    // エージェントのテキストを描画
                    drawer_.drawAgentText(*agent_location);
                }
#endif
            }
#ifdef PAXS_USING_SIMULATOR
            else if (visible[MurMur3::calcHash("Simulation")]) {
                // シミュレーションのみ表示の場合
                if (agent_location.get() != nullptr && simulator.get() != nullptr) {
                    agent_location->draw(koyomi.jdn.cgetDay(), simulator->getSettlementGrids(), simulator->getMarriagePosList(), map_viewport.getWidth(), map_viewport.getHeight(), map_viewport.getCenterX(), map_viewport.getCenterY());
                    agent_location->drawText();
                }
            }
#endif
        }
    };
}

#endif // !PAX_MAHOROBA_MAP_CONTROLLER_HPP
