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

#ifdef PAXS_USING_SIMULATOR
#include <PAX_MAHOROBA/Map/Location/SettlementRenderer.hpp>
#include <PAX_MAHOROBA/Map/Location/Input/SettlementInputHandler.hpp>
#include <PAX_SAPIENTICA/Simulation/SettlementSimulator.hpp>
#endif

#include <PAX_MAHOROBA/Map/Location/PlaceNameManager.hpp>
#include <PAX_MAHOROBA/Map/MapViewport.hpp>
#include <PAX_MAHOROBA/Rendering/TextureManager.hpp>
#include <PAX_MAHOROBA/Map/Location/PersonNameManager.hpp>
#include <PAX_MAHOROBA/Map/MapRenderer.hpp>
#include <PAX_MAHOROBA/Rendering/FontManager.hpp>

#include <PAX_SAPIENTICA/Map/MapDomainLogic.hpp>
#include <PAX_SAPIENTICA/Calendar/Koyomi.hpp>
#include <PAX_SAPIENTICA/FeatureVisibilityManager.hpp>
#include <PAX_SAPIENTICA/FontConfig.hpp>
#include <PAX_SAPIENTICA/Language.hpp>
#include <PAX_SAPIENTICA/Logger.hpp>
#include <PAX_SAPIENTICA/MurMur3.hpp>

namespace paxs {

    /// @brief 地図コントローラー（統合制御クラス）
    /// @brief Map Controller (Integrated Control Class)
    class MapController {
    private:
        std::unique_ptr<TextureManager> texture_manager_; // 地図上に描画する画像の一覧

        PlaceNameManager place_name_manager_{}; // 地名
        PersonNameManager person_name_manager_{}; // 人名
#ifdef PAXS_USING_SIMULATOR
        std::unique_ptr<SettlementRenderer> settlement_renderer; // 集落描画
        SettlementInputHandler settlement_input_handler_; // 集落入力処理
#endif
        paxs::map::MapDomainLogic map_domain_logic_; // ドメインロジック
        MapRenderer renderer_; // 描画処理

        // 依存性注入された参照
        FontManager* font_manager_ = nullptr;
        const SelectLanguage* select_language_ = nullptr;

    public:
        MapController()
            :texture_manager_(std::make_unique<TextureManager>())
#ifdef PAXS_USING_SIMULATOR
            ,settlement_renderer(std::make_unique<SettlementRenderer>())
#endif
        {
            // メモリ割り当てチェック
            if (!texture_manager_) {
                PAXS_ERROR("Failed to allocate TextureManager");
            }
#ifdef PAXS_USING_SIMULATOR
            if (!settlement_renderer) {
                PAXS_ERROR("Failed to allocate SettlementRenderer");
            }
#endif
        }

        void init(FontManager& font_manager, const SelectLanguage& select_language) {
            // 地名
            place_name_manager_.init();
            place_name_manager_.add();
            person_name_manager_.init();
            person_name_manager_.add();

            // フォント管理への参照を保存
            font_manager_ = &font_manager;
            select_language_ = &select_language;
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
                texture_manager_->update(map_viewport.getCenterX(), map_viewport.getCenterY(), map_viewport.getWidth(), map_viewport.getHeight());

                // フォントを取得
                paxg::Font* main_font = font_manager_->getMainFont(*select_language_);

                // 描画処理（旧Presenterの処理を統合）
                const double width = map_viewport.getWidth();
                const double height = map_viewport.getHeight();
                const double center_x = map_viewport.getCenterX();
                const double center_y = map_viewport.getCenterY();
                const double julian_day = koyomi.jdn.cgetDay();

                // 地名を描画
                renderer_.drawPlaceNames(
                    place_name_manager_,
                    visible,
                    julian_day,
                    width,
                    height,
                    center_x,
                    center_y,
                    (main_font == nullptr) ? font_manager_->getPinFont() : (*main_font),
                    font_manager_->getEnFont(),
                    font_manager_->getPinFont()
                );

                // 人名を描画
                renderer_.drawPersonNames(
                    person_name_manager_,
                    julian_day,
                    width,
                    height,
                    center_x,
                    center_y,
                    (main_font == nullptr) ? font_manager_->getPinFont() : (*main_font),
                    font_manager_->getEnFont(),
                    font_manager_->getPinFont()
                );

#ifdef PAXS_USING_SIMULATOR
                if (simulator) {
                    // 入力処理を更新
                    settlement_input_handler_.update();

                    // エージェントを描画
                    renderer_.drawSettlements(
                        *settlement_renderer,
                        julian_day,
                        simulator->getSettlementGrids(),
                        simulator->getMarriagePosList(),
                        width,
                        height,
                        center_x,
                        center_y,
                        settlement_input_handler_.getSelectDraw(),
                        settlement_input_handler_.getIsLine(),
                        settlement_input_handler_.getIsArrow()
                    );

                    // エージェントのテキストを描画
                    renderer_.drawSettlementText(*settlement_renderer, settlement_input_handler_.getSelectDraw());
                }
#endif
            }
#ifdef PAXS_USING_SIMULATOR
            else if (visible[MurMur3::calcHash("Simulation")]) {
                // シミュレーションのみ表示の場合
                if (settlement_renderer.get() != nullptr && simulator.get() != nullptr) {
                    // 入力処理を更新
                    settlement_input_handler_.update();

                    settlement_renderer->draw(
                        koyomi.jdn.cgetDay(),
                        simulator->getSettlementGrids(),
                        simulator->getMarriagePosList(),
                        map_viewport.getWidth(),
                        map_viewport.getHeight(),
                        map_viewport.getCenterX(),
                        map_viewport.getCenterY(),
                        settlement_input_handler_.getSelectDraw(),
                        settlement_input_handler_.getIsLine(),
                        settlement_input_handler_.getIsArrow()
                    );
                    settlement_renderer->drawText(settlement_input_handler_.getSelectDraw());
                }
            }
#endif
        }
    };
}

#endif // !PAX_MAHOROBA_MAP_CONTROLLER_HPP
