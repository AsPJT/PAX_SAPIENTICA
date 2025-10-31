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
#include <PAX_MAHOROBA/Map/Location/AgentLocation.hpp>
#include <PAX_SAPIENTICA/Simulation/SettlementSimulator.hpp>
#endif

#include <PAX_MAHOROBA/Map/Location/PlaceNameManager.hpp>
#include <PAX_MAHOROBA/Map/MapViewport.hpp>
#include <PAX_MAHOROBA/Map/Location/LocationRange.hpp>
#include <PAX_MAHOROBA/Map/Location/PersonLocation.hpp>
#include <PAX_MAHOROBA/Map/MapRenderer.hpp>
#include <PAX_MAHOROBA/Rendering/FontManager.hpp>

#include <PAX_SAPIENTICA/Map/MapDomainLogic.hpp>
#include <PAX_SAPIENTICA/Calendar/Koyomi.hpp>
#include <PAX_SAPIENTICA/FeatureVisibilityManager.hpp>
#include <PAX_SAPIENTICA/Language.hpp>
#include <PAX_SAPIENTICA/MurMur3.hpp>

namespace paxs {

    /// @brief 地図コントローラー（統合制御クラス）
    /// @brief Map Controller (Integrated Control Class)
    class MapController {
    public:
        std::unique_ptr<TextureManager> texture_manager; // 地図上に描画する画像の一覧

        PlaceNameManager place_name_manager{}; // 地名
        PersonNameManager person_name_manager{}; // 人名

    private:
#ifdef PAXS_USING_SIMULATOR
        std::unique_ptr<AgentLocation> agent_location; // エージェント
#endif
        paxs::map::MapDomainLogic map_domain_logic_; // ドメインロジック
        MapRenderer renderer_; // 描画処理

        // 依存性注入された参照
        FontManager* font_manager_ = nullptr;
        const SelectLanguage* select_language_ = nullptr;
        std::uint_least8_t koyomi_font_size_ = 22;
        std::uint_least8_t koyomi_font_buffer_thickness_size_ = 3;

    public:
        MapController()
            :texture_manager(std::unique_ptr<TextureManager>(new(std::nothrow) TextureManager))
#ifdef PAXS_USING_SIMULATOR
            ,agent_location(std::unique_ptr<AgentLocation>(new(std::nothrow) AgentLocation))
#endif
        {}

        void init(FontManager& font_manager, const SelectLanguage& select_language,
                  std::uint_least8_t koyomi_font_size, std::uint_least8_t koyomi_font_buffer_thickness_size) {
            // 地名
            place_name_manager.init();
            place_name_manager.add();
            person_name_manager.init();
            person_name_manager.add();

            // フォント管理への参照を保存
            font_manager_ = &font_manager;
            select_language_ = &select_language;
            koyomi_font_size_ = koyomi_font_size;
            koyomi_font_buffer_thickness_size_ = koyomi_font_buffer_thickness_size;
        }

        /// @brief 更新処理（統合版）
        /// @brief Update and render (integrated version)
        void update(
            MapViewport& map_viewport,
            const paxs::Koyomi& koyomi,
#ifdef PAXS_USING_SIMULATOR
            std::unique_ptr<paxs::SettlementSimulator>& simulator,
#endif
            paxs::FeatureVisibilityManager& visible
            ) {
            if (visible[MurMur3::calcHash("Map")]) { // 地図が「可視」の場合は描画する
                // 地図上に画像を描画する
                texture_manager->update(map_viewport.getCenterX(), map_viewport.getCenterY(), map_viewport.getWidth(), map_viewport.getHeight());

                // フォントを取得
                paxg::Font* main_font = font_manager_->language_fonts.getAndAdd(
                    select_language_->cgetKey(),
                    koyomi_font_size_,
                    koyomi_font_buffer_thickness_size_
                );

                // 描画処理（旧Presenterの処理を統合）
                const double width = map_viewport.getWidth();
                const double height = map_viewport.getHeight();
                const double center_x = map_viewport.getCenterX();
                const double center_y = map_viewport.getCenterY();
                const double julian_day = koyomi.jdn.cgetDay();

                // 地名を描画
                renderer_.drawPlaceNames(
                    place_name_manager,
                    visible,
                    julian_day,
                    width,
                    height,
                    center_x,
                    center_y,
                    (main_font == nullptr) ? font_manager_->pin_font : (*main_font),
                    font_manager_->en_font,
                    font_manager_->pin_font
                );

                // 人名を描画
                renderer_.drawPersonNames(
                    person_name_manager,
                    julian_day,
                    width,
                    height,
                    center_x,
                    center_y,
                    (main_font == nullptr) ? font_manager_->pin_font : (*main_font),
                    font_manager_->en_font,
                    font_manager_->pin_font
                );

#ifdef PAXS_USING_SIMULATOR
                if (simulator) {
                    // エージェントを描画
                    renderer_.drawAgents(
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
                    renderer_.drawAgentText(*agent_location);
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
