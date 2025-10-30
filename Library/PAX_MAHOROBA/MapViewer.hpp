/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_MAP_VIEWER_HPP
#define PAX_MAHOROBA_MAP_VIEWER_HPP

#include <memory>
#include <new>

#ifdef PAXS_USING_SIMULATOR
#include <PAX_MAHOROBA/Simulation/AgentLocation.hpp>
#include <PAX_SAPIENTICA/Simulation/SettlementSimulator.hpp>
#endif

#include <PAX_MAHOROBA/LocationPoint.hpp>
#include <PAX_MAHOROBA/MapView.hpp>
#include <PAX_MAHOROBA/LocationRange.hpp>
#include <PAX_MAHOROBA/PersonLocation.hpp>
#include <PAX_MAHOROBA/UIManager.hpp>

#include <PAX_SAPIENTICA/MurMur3.hpp>

namespace paxs {

    class MapViewer {
    public:


        std::unique_ptr<TextureLocation> texture_location; // 地図上に描画する画像の一覧

        PlaceNameLocation place_name_location{}; // 地名
        PersonNameLocation person_name_location{}; // 人名

    private:
#ifdef PAXS_USING_SIMULATOR
        std::unique_ptr<AgentLocation> agent_location; // エージェント
#endif
    public:

        MapViewer()
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

        void update(
            MapView& map_view,
            const SelectLanguage& select_language,
            const paxs::KoyomiSiv3D& koyomi_siv,
            paxs::UIManager& ui_manager,
#ifdef PAXS_USING_SIMULATOR
            std::unique_ptr<paxs::SettlementSimulator>& simulator,
#endif
            paxs::GraphicVisualizationList& visible
            ) {
            if (visible[MurMur3::calcHash("Map")]) { // 地図が「可視」の場合は描画する
                // 地図上に画像を描画する
                texture_location->update(map_view.getCenterX(), map_view.getCenterY(), map_view.getWidth(), map_view.getHeight());
            }
#ifdef PAXS_USING_SIMULATOR
            if (visible[MurMur3::calcHash("Simulation")]) {
                if (agent_location.get() != nullptr && simulator.get() != nullptr) {
                    agent_location->draw(koyomi_siv.jdn.cgetDay(), simulator->getSettlementGrids(), simulator->getMarriagePosList(), map_view.getWidth(), map_view.getHeight(), map_view.getCenterX(), map_view.getCenterY()
                    );
                }
            }
#endif
            if (visible[MurMur3::calcHash("Map")]) { // 地図が「可視」の場合は描画する
                // 地図上に画像を描画する
                texture_location->update(map_view.getCenterX(), map_view.getCenterY(), map_view.getWidth(), map_view.getHeight());

                // フォントを指定
                paxg::Font* one_font = ui_manager.string_viewer.language_fonts.getAndAdd(select_language.cgetKey(), static_cast<std::uint_least8_t>(ui_manager.koyomi_font_size), static_cast<std::uint_least8_t>(ui_manager.koyomi_font_buffer_thickness_size));

                place_name_location.draw(
                    visible,
                    koyomi_siv.jdn.cgetDay(),
                    map_view.getWidth(),
                    map_view.getHeight(),
                    map_view.getCenterX(),
                    map_view.getCenterY(),
                    (one_font == nullptr) ? ui_manager.string_viewer.pin_font : (*one_font),
                    ui_manager.string_viewer.en_font,
                    ui_manager.string_viewer.pin_font);
                person_name_location.draw(
                    koyomi_siv.jdn.cgetDay(),
                    map_view.getWidth(),
                    map_view.getHeight(),
                    map_view.getCenterX(),
                    map_view.getCenterY(),
                    (one_font == nullptr) ? ui_manager.string_viewer.pin_font : (*one_font),
                    ui_manager.string_viewer.en_font,
                    ui_manager.string_viewer.pin_font);

            }

#ifdef PAXS_USING_SIMULATOR
            if (visible[MurMur3::calcHash("Simulation")]) {
                if (agent_location.get() != nullptr && simulator.get() != nullptr) {
                    agent_location->drawText();
                }
            }
#endif
        }
    };
}

#endif // !PAX_MAHOROBA_MAP_VIEWER_HPP
