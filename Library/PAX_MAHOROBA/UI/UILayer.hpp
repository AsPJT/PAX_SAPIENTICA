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
#include <vector>

#ifdef PAXS_USING_SIMULATOR
#include <PAX_MAHOROBA/UI/Simulation/SettlementStatusPanel.hpp>
#include <PAX_MAHOROBA/UI/Simulation/SimulationPanel.hpp>
#endif

#include <PAX_GRAPHICA/ScopedRenderState.hpp>

#include <PAX_MAHOROBA/Core/AppStateManager.hpp>
#include <PAX_MAHOROBA/Events/FeatureEvents.hpp>
#include <PAX_MAHOROBA/Map/MapViewport.hpp>
#include <PAX_MAHOROBA/Rendering/IWidget.hpp>
#include <PAX_MAHOROBA/UI/Calendar/CalendarPanel.hpp>
#include <PAX_MAHOROBA/UI/DebugInfoPanel.hpp>
#include <PAX_MAHOROBA/UI/FeatureDetailPanel.hpp>
#include <PAX_MAHOROBA/UI/UILayout.hpp>
#include <PAX_MAHOROBA/UI/Widget/UIPanelBackground.hpp>

#include <PAX_SAPIENTICA/Calendar/Date.hpp>
#include <PAX_SAPIENTICA/Calendar/Koyomi.hpp>
#include <PAX_SAPIENTICA/Simulation/Config/SimulationState.hpp>
#include <PAX_SAPIENTICA/System/AppConst.hpp>
#include <PAX_SAPIENTICA/System/ApplicationEvents.hpp>
#include <PAX_SAPIENTICA/System/EventBus.hpp>
#include <PAX_SAPIENTICA/System/FeatureVisibilityManager.hpp>
#include <PAX_SAPIENTICA/Utility/Logger.hpp>

namespace paxs {

    /// @brief UIレイヤーの統合管理を担当するクラス
    /// @brief Integrated management class for UI layer
    class UILayer : public IWidget{
    private:
        const paxs::FeatureVisibilityManager& visible_manager;

        paxs::UILayout ui_layout;

        paxs::CalendarPanel calendar_panel;
        paxs::UIPanelBackground calendar_bg_;
        paxs::DebugInfoPanel debug_info_panel;
        paxs::UIPanelBackground debug_info_bg_;
        paxs::FeatureDetailPanel feature_detail_panel;
        paxs::UIPanelBackground feature_detail_bg_;
#ifdef PAXS_USING_SIMULATOR
        paxs::SimulationPanel simulation_panel;
        paxs::UIPanelBackground simulation_bg_;
        paxs::SettlementStatusPanel settlement_status_panel;
        paxs::UIPanelBackground settlement_status_bg_;
#endif
        std::vector<IWidget*> panels;

        std::size_t koyomi_date_list_size = 0;

        void sortPanelsByLayer() {
            std::sort(panels.begin(), panels.end(),
                [](IWidget* a, IWidget* b) {
                    // 降順
                    return a->getLayer() > b->getLayer();
                });
        }

        /// @brief イベントを購読
        void subscribeToEvents() {
            paxs::EventBus& event_bus = paxs::EventBus::getInstance();
            // ウィンドウリサイズイベントの購読
            event_bus.subscribe<WindowResizedEvent>(
                [this](const WindowResizedEvent& event) {
                    (void)event;
                    // UIレイアウトを再計算
                    ui_layout.calculate(koyomi_date_list_size, calendar_panel.getTimeControlHeight());
                    calendar_panel.updateButtonLayout();
#ifdef PAXS_USING_SIMULATOR
                    simulation_panel.calculateLayout();
#endif
                }
            );

            // 言語変更イベントの購読
            event_bus.subscribe<LanguageChangedEvent>(
                [this](const LanguageChangedEvent& event) {
                    (void)event;
                    // 言語変更時はレイアウト再計算が必要
                    ui_layout.calculate(koyomi_date_list_size, calendar_panel.getTimeControlHeight());
#ifdef PAXS_USING_SIMULATOR
                    simulation_panel.calculateLayout();
#endif
                }
            );

            // 機能可視性変更イベントの購読
            event_bus.subscribe<FeatureVisibilityChangedEvent>(
                [this](const FeatureVisibilityChangedEvent& event) {
                    (void)event;
                    // 背景の可視性をパネルと同期
                    calendar_bg_.setVisible(calendar_panel.isVisible());
                    debug_info_bg_.setVisible(debug_info_panel.isVisible());
                    feature_detail_bg_.setVisible(feature_detail_panel.isVisible());
#ifdef PAXS_USING_SIMULATOR
                    simulation_bg_.setVisible(simulation_panel.isVisible());
                    settlement_status_bg_.setVisible(settlement_status_panel.isVisible());
#endif
                }
            );

            // Feature選択イベントの購読（背景の可視性を更新）
            event_bus.subscribe<FeatureSelectedEvent>(
                [this](const FeatureSelectedEvent& event) {
                    (void)event;
                    // FeatureDetailPanelが表示されるので背景も表示
                    feature_detail_bg_.setVisible(true);
                }
            );

            // Feature選択解除イベントの購読（背景の可視性を更新）
            event_bus.subscribe<FeatureDeselectedEvent>(
                [this](const FeatureDeselectedEvent& event) {
                    (void)event;
                    // FeatureDetailPanelが非表示になるので背景も非表示
                    feature_detail_bg_.setVisible(false);
                }
            );

#ifdef PAXS_USING_SIMULATOR
            // 集落表示設定変更イベントの購読
            event_bus.subscribe<SettlementDisplayChangedEvent>(
                [this](const SettlementDisplayChangedEvent& event) {
                    // SettlementStatusPanelの表示モードを更新
                    settlement_status_panel.setSelectDraw(event.select_draw);
                }
            );

            // シミュレーション状態変更イベントの購読
            event_bus.subscribe<SimulationStateChangedEvent>(
                [this](const SimulationStateChangedEvent& event) {
                    // シミュレーションが停止状態になったら表示
                    if (event.new_state == SimulationState::Stopped) {
                        settlement_status_panel.setVisible(true);
                        settlement_status_bg_.setVisible(settlement_status_panel.isVisible());
                    }
                    // 初期化前になったら非表示
                    else if (event.new_state == SimulationState::Uninitialized) {
                        settlement_status_panel.setVisible(false);
                        settlement_status_bg_.setVisible(false);
                    }
                }
            );
#endif
        }

    public:
        UILayer(const AppStateManager& app_state_manager)
            : visible_manager(app_state_manager.getVisibilityManager()),
              calendar_panel(ui_layout, visible_manager, app_state_manager),
              debug_info_panel(ui_layout, visible_manager, app_state_manager.getMapViewport(), app_state_manager.getKoyomi()),
              feature_detail_panel(ui_layout, visible_manager),
#ifdef PAXS_USING_SIMULATOR
              settlement_status_panel(visible_manager),
              simulation_panel(visible_manager, app_state_manager, ui_layout),
#endif
              calendar_bg_("CalendarBackground", &ui_layout.calendar_panel),
              debug_info_bg_("DebugInfoBackground", &ui_layout.debug_info_panel),
              feature_detail_bg_("FeatureDetailBackground", &feature_detail_panel)
#ifdef PAXS_USING_SIMULATOR
              , simulation_bg_("SimulationBackground", &ui_layout.simulation_panel)
              , settlement_status_bg_("SettlementStatusBackground", &ui_layout.settlement_status_panel)
#endif
        {
            // 影描画用のRenderTextureを最大画面サイズで初期化（一回のみ）
            PanelBackgroundRenderer::initShadowTextures(paxs::AppConst::max_window_size);

            panels.emplace_back(&calendar_panel);
            panels.emplace_back(&calendar_bg_);
            panels.emplace_back(&debug_info_panel);
            panels.emplace_back(&debug_info_bg_);
            panels.emplace_back(&feature_detail_panel);
            panels.emplace_back(&feature_detail_bg_);
#ifdef PAXS_USING_SIMULATOR
            panels.emplace_back(&simulation_panel);
            panels.emplace_back(&simulation_bg_);
            panels.emplace_back(&settlement_status_panel);
            panels.emplace_back(&settlement_status_bg_);
#endif
            sortPanelsByLayer();

            const auto& koyomi = app_state_manager.getKoyomi();
            koyomi_date_list_size = koyomi.date_list.size();
            subscribeToEvents();

            // UIレイアウトを初期計算
            ui_layout.calculate(koyomi_date_list_size, calendar_panel.getTimeControlHeight());
            calendar_panel.updateButtonLayout();

#ifdef PAXS_USING_SIMULATOR
            simulation_panel.calculateLayout();
#endif
        }

        // コピー・ムーブ禁止（メンバー変数へのポインタをvector<IWidget*>に格納しているため）
        UILayer(const UILayer&) = delete;
        UILayer& operator=(const UILayer&) = delete;
        UILayer(UILayer&&) = delete;
        UILayer& operator=(UILayer&&) = delete;

        /// @brief UILayerの可視性を初期化
        void initializeVisibility() {
#ifdef PAXS_USING_SIMULATOR
            // 背景の可視性をパネルと同期
            simulation_bg_.setVisible(simulation_panel.isVisible());
            settlement_status_bg_.setVisible(settlement_status_panel.isVisible());
#endif
            // 背景の可視性をパネルと同期
            calendar_bg_.setVisible(calendar_panel.isVisible());
            debug_info_bg_.setVisible(debug_info_panel.isVisible());
            feature_detail_bg_.setVisible(feature_detail_panel.isVisible());
        }

        void render() const override {
            if (!isVisible()) return;

            // 画像の拡大縮小の方式を設定
            const paxg::ScopedSamplerState sampler{ paxg::SamplerState::ClampLinear };

            // 1. バッチ描画開始（Siv3D用）
            PanelBackgroundRenderer::beginBatch();

            // 2. 背景コンポーネントを描画（バッチに登録）
            for (auto* panel : panels) {
                if (panel && panel->getLayer() == RenderLayer::UIBackground) {
                    panel->render();
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
            if (!isVisible()) return false;
            for (const IWidget* panel : panels) {
                if (panel) {
                    if (panel->isHit(x, y)) {
                        return true;
                    }
                } else {
                    PAXS_WARNING("UILayer::isHit: panel is nullptr");
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
                    PAXS_WARNING("UILayer::handleEvent: panel is nullptr");
                }
            }
            return EventHandlingResult::NotHandled();
        }

#ifdef PAXS_USING_SIMULATOR
        SettlementStatusPanel& getSettlementStatusPanel() { return settlement_status_panel; }
#endif

        bool isVisible() const override {
            return visible_manager.isVisible(ViewMenu::ui);
        }
        void setPos(const paxg::Vec2i& /*pos*/) override {}
        paxg::Rect getRect() const override { return paxg::Rect{}; }
        const char* getName() const override { return "UILayer"; }
        RenderLayer getLayer() const override { return RenderLayer::UIContent; }
    };

}

#endif // !PAX_MAHOROBA_UI_LAYER_HPP
