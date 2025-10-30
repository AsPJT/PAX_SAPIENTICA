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
#include <PAX_MAHOROBA/MapViewport.hpp>
#include <PAX_MAHOROBA/LocationRange.hpp>
#include <PAX_MAHOROBA/PersonLocation.hpp>
#include <PAX_MAHOROBA/UIManager.hpp>
#include <PAX_MAHOROBA/MapPresenter.hpp>

#include <PAX_SAPIENTICA/Calendar/Koyomi.hpp>
#include <PAX_SAPIENTICA/MurMur3.hpp>

namespace paxs {

    /// @brief 地図ビューアー（Facadeパターン）
    /// @brief Map Viewer (Facade Pattern)
    /// @note MapPresenterに処理を委譲し、後方互換性を維持
    class MapViewer {
    public:
        std::unique_ptr<TextureLocation> texture_location; // 地図上に描画する画像の一覧

        PlaceNameLocation place_name_location{}; // 地名
        PersonNameLocation person_name_location{}; // 人名

    private:
#ifdef PAXS_USING_SIMULATOR
        std::unique_ptr<AgentLocation> agent_location; // エージェント
#endif
        MapPresenter presenter_; // プレゼンター（実際の処理を担当）

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

        /// @brief 更新処理（Presenterに委譲）
        /// @brief Update (delegate to Presenter)
        void update(
            MapViewport& map_viewport,
            const SelectLanguage& select_language,
            const paxs::Koyomi& koyomi,
            paxs::UIManager& ui_manager,
#ifdef PAXS_USING_SIMULATOR
            std::unique_ptr<paxs::SettlementSimulator>& simulator,
#endif
            paxs::GraphicVisualizationList& visible
            ) {
            if (visible[MurMur3::calcHash("Map")]) { // 地図が「可視」の場合は描画する
                // 地図上に画像を描画する
                texture_location->update(map_viewport.getCenterX(), map_viewport.getCenterY(), map_viewport.getWidth(), map_viewport.getHeight());

                // Presenterに描画を委譲
                presenter_.update(
                    map_viewport,
                    select_language,
                    koyomi,
                    ui_manager,
#ifdef PAXS_USING_SIMULATOR
                    simulator,
                    *agent_location,
#endif
                    place_name_location,
                    person_name_location,
                    visible
                );
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

#endif // !PAX_MAHOROBA_MAP_VIEWER_HPP
