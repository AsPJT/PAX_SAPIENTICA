/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_UI_LAYER_HPP
#define PAX_MAHOROBA_UI_LAYER_HPP

#include <algorithm>
#include <limits>
#include <string>
#include <vector>

#ifdef PAXS_USING_SIMULATOR
#include <PAX_MAHOROBA/UI/SimulationPanel.hpp>
#include <PAX_MAHOROBA/UI/SettlementStatusPanel.hpp>
#endif

#include <PAX_GRAPHICA/ScopedRenderState.hpp>
#include <PAX_GRAPHICA/Texture.hpp>

#include <PAX_MAHOROBA/UI/Calendar/CalendarPanel.hpp>
#include <PAX_MAHOROBA/UI/UILayout.hpp>
#include <PAX_MAHOROBA/UI/DebugInfoPanel.hpp>
#include <PAX_MAHOROBA/UI/UIPanelBackground.hpp>
#include <PAX_MAHOROBA/Map/MapViewport.hpp>
#include <PAX_MAHOROBA/Rendering/IWidget.hpp>

#include <PAX_SAPIENTICA/Calendar/Date.hpp>
#include <PAX_SAPIENTICA/Calendar/Koyomi.hpp>
#include <PAX_SAPIENTICA/FeatureVisibilityManager.hpp>
#include <PAX_SAPIENTICA/Logger.hpp>
#include <PAX_SAPIENTICA/MurMur3.hpp>


namespace paxs {

    /// @brief UIレイヤーの統合管理を担当するクラス
    /// @brief Integrated management class for UI layer
    class UILayer : public IWidget{
    private:
        const FeatureVisibilityManager* feature_visibility_manager_ptr = nullptr;

        // 描画に必要なデータをキャッシュ（updateData()で更新、render()で使用）
        const MapViewport* cached_map_viewport_ptr = nullptr;
#ifdef PAXS_USING_SIMULATOR
        std::unique_ptr<paxs::SettlementSimulator>* cached_simulator_ = nullptr;
#endif
        paxs::Koyomi cached_koyomi_;
        paxs::FeatureVisibilityManager* visible_manager_ptr = nullptr;

        std::size_t map_viewport_width_str_index = 0;
        std::size_t map_viewport_center_x_str_index = 0;
        std::size_t map_viewport_center_y_str_index = 0;
        std::size_t map_viewport_center_lat_str_index = 0;

        paxs::UILayout ui_layout;

        paxs::CalendarPanel calendar_panel;
        paxs::DebugInfoPanel debug_info_panel;

#ifdef PAXS_USING_SIMULATOR
        paxs::SimulationPanel simulation_panel;
        paxs::SettlementStatusPanel settlement_status_panel;
#endif

        // 背景コンポーネント
        paxs::UIPanelBackground calendar_bg_;
        paxs::UIPanelBackground debug_info_bg_;
#ifdef PAXS_USING_SIMULATOR
        paxs::UIPanelBackground simulation_bg_;
        paxs::UIPanelBackground settlement_status_bg_;
#endif

        std::vector<IWidget*> panels;

        void sortPanelsByLayer() {
            std::sort(panels.begin(), panels.end(),
                [](IWidget* a, IWidget* b) {
                    // 降順
                    return a->getLayer() > b->getLayer();
                });
        }

    public:
        UILayer(
            paxs::FeatureVisibilityManager* visible_manager,
            const MapViewport* map_viewport)
            : feature_visibility_manager_ptr(visible_manager),
              visible_manager_ptr(visible_manager),
              calendar_panel(ui_layout),
              debug_info_panel(ui_layout, visible_manager),
#ifdef PAXS_USING_SIMULATOR
              settlement_status_panel(visible_manager),
              simulation_panel(visible_manager),
#endif
              calendar_bg_("CalendarBackground", &ui_layout.calendar_panel),
              debug_info_bg_("DebugInfoBackground", &ui_layout.debug_info_panel)
#ifdef PAXS_USING_SIMULATOR
              , simulation_bg_("SimulationBackground", &ui_layout.simulation_panel)
              , settlement_status_bg_("SettlementStatusBackground", &ui_layout.settlement_status_panel)
#endif
        {
            // CalendarPanel の初期化
            calendar_panel.init(visible_manager_ptr);

            // DebugInfoPanel の初期化
            debug_info_panel.init(map_viewport);
            map_viewport_width_str_index = (MurMur3::calcHash(25, "debug_magnification_power"));
            map_viewport_center_x_str_index = (MurMur3::calcHash(24, "debug_mercator_longitude"));
            map_viewport_center_y_str_index = (MurMur3::calcHash(23, "debug_mercator_latitude"));
            map_viewport_center_lat_str_index = (MurMur3::calcHash(14, "debug_latitude"));

#ifdef PAXS_USING_SIMULATOR
            settlement_status_panel.init();
#endif

            // 影描画用のRenderTextureを最大画面サイズで初期化（一回のみ）
            PanelBackgroundRenderer::initShadowTextures(paxs::Vector2<int>{3840, 2160});

            panels.clear();
            // 背景コンポーネント（RenderLayer::UIBackground = 300）
            panels.push_back(&calendar_bg_);
            panels.push_back(&debug_info_bg_);
#ifdef PAXS_USING_SIMULATOR
            panels.push_back(&simulation_bg_);
            panels.push_back(&settlement_status_bg_);
#endif
            // コンテンツコンポーネント（RenderLayer::UIContent = 400）
            panels.push_back(&calendar_panel);
            panels.push_back(&debug_info_panel);
#ifdef PAXS_USING_SIMULATOR
            panels.push_back(&simulation_panel);
            panels.push_back(&settlement_status_panel);
#endif
            sortPanelsByLayer();
        }

        /// @brief データ更新（描画は行わない）
        /// @brief Update data (no drawing)
        void updateData(
#ifdef PAXS_USING_SIMULATOR
            std::unique_ptr<paxs::SettlementSimulator>& simulator,
#endif
            paxs::Koyomi& koyomi
            ) {
            // UIレイアウトを計算
            ui_layout.calculate(koyomi.date_list.size(), calendar_panel.getTimeControlHeight());

#ifdef PAXS_USING_SIMULATOR
            bool simulation_visible = visible_manager_ptr->isVisible(MurMur3::calcHash("Simulation")) &&
                                    visible_manager_ptr->isVisible(MurMur3::calcHash(2, "UI"));
            simulation_panel.setVisible(simulation_visible);

            simulation_panel.setReferences(simulator, koyomi,
                ui_layout.koyomi_font_y + ui_layout.next_rect_start_y + 20);

            simulation_panel.updateSimulationAuto();

            simulation_bg_.setVisible(simulation_panel.isVisible());

            settlement_status_bg_.setVisible(settlement_status_panel.isVisible());
#endif

            // CalendarPanelの可視性と設定
            if (calendar_panel.isVisible()) {
                calendar_panel.setCalendarParams(koyomi);
                calendar_panel.setTimeControlParams(koyomi);
            }

            calendar_bg_.setVisible(calendar_panel.isVisible());

            debug_info_bg_.setVisible(debug_info_panel.isVisible());

#ifdef PAXS_USING_SIMULATOR
            cached_simulator_ = &simulator;
#endif
            cached_koyomi_ = koyomi;
        }

        /// @brief レンダリング処理
        /// @brief Render
        void render() const override {
            if (!isVisible() || visible_manager_ptr == nullptr) return;

            // 画像の拡大縮小の方式を設定
            const paxg::ScopedSamplerState sampler{ paxg::SamplerState::ClampLinear };

            const paxs::Koyomi& koyomi = cached_koyomi_;

            // 1. バッチ描画開始（Siv3D用）
            PanelBackgroundRenderer::beginBatch();

            // 2. 背景コンポーネントを描画（バッチに登録）
            {
                for (auto* panel : panels) {
                    if (panel && panel->getLayer() == RenderLayer::UIBackground) {
                        panel->render();
                    }
                }
            }

            // 3. バッチ描画終了（すべての背景を一括描画）
            PanelBackgroundRenderer::endBatch();

            // 4. コンテンツコンポーネントを描画
            for (auto* panel : panels) {
                if (panel && panel->getLayer() != RenderLayer::UIBackground) {
                    panel->render();
                }
            }
        }

        bool isHit(int x, int y) const override {
            for (const IWidget* panel : panels) {
                if (panel) {
                    if (panel->isHit(x, y)) {
                        return true;
                    }
                } else {
                    PAXS_WARNING(std::string("UILayer::isHit: ") + panel->getName() + " is nullptr");
                }
            }
            return false;
        }

        EventHandlingResult handleEvent(const MouseEvent& event) override {
            for (IWidget* panel : panels) {
                if (panel) {
                    if (panel->isHit(event.x, event.y)) {
                        EventHandlingResult result = panel->handleEvent(event);
                        if (result.handled) {
                            return result;
                        }
                    }
                } else {
                    PAXS_WARNING(std::string("UILayer::handleEvent: ") + panel->getName() + " is nullptr");
                }
            }
            return EventHandlingResult::NotHandled();
        }

#ifdef PAXS_USING_SIMULATOR
        SettlementStatusPanel& getSettlementStatusPanel() { return settlement_status_panel; }
#endif

        bool isVisible() const override {
            return feature_visibility_manager_ptr->isVisible(MurMur3::calcHash(2, "UI"));
        }
        paxg::Rect getRect() const override {
            return paxg::Rect{ 0, 0, 0, 0 };
        }
        const char* getName() const override {
            return "UILayer";
        }
        RenderLayer getLayer() const override {
            return RenderLayer::UIContent;
        }
        void setVisible(bool /*visible*/) override {}
        void setEnabled(bool /*enabled*/) override {}
        bool isEnabled() const override { return true; }
        void setPos(const paxg::Vec2i& /*pos*/) override {}
    };

}

#endif // !PAX_MAHOROBA_UI_LAYER_HPP
