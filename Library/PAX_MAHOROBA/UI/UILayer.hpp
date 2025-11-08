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
#include <PAX_MAHOROBA/UI/SimulationPanel.hpp>
#include <PAX_MAHOROBA/UI/SettlementStatusPanel.hpp>
#endif

#include <PAX_GRAPHICA/ScopedRenderState.hpp>

#include <PAX_MAHOROBA/Core/ApplicationEvents.hpp>
#include <PAX_MAHOROBA/Core/AppStateManager.hpp>
#include <PAX_MAHOROBA/Core/EventBus.hpp>
#include <PAX_MAHOROBA/UI/Calendar/CalendarPanel.hpp>
#include <PAX_MAHOROBA/UI/UILayout.hpp>
#include <PAX_MAHOROBA/UI/DebugInfoPanel.hpp>
#include <PAX_MAHOROBA/UI/UIPanelBackground.hpp>
#include <PAX_MAHOROBA/Map/MapViewport.hpp>
#include <PAX_MAHOROBA/Rendering/IWidget.hpp>

#include <PAX_SAPIENTICA/AppConst.hpp>
#include <PAX_SAPIENTICA/Calendar/Date.hpp>
#include <PAX_SAPIENTICA/Calendar/Koyomi.hpp>
#include <PAX_SAPIENTICA/FeatureVisibilityManager.hpp>
#include <PAX_SAPIENTICA/Logger.hpp>

namespace paxs {

    /// @brief UIレイヤーの統合管理を担当するクラス
    /// @brief Integrated management class for UI layer
    class UILayer : public IWidget{
    private:
        paxs::FeatureVisibilityManager* visible_manager_ptr = nullptr;

        paxs::UILayout ui_layout;

        // イベント駆動用（オプション）
        EventBus* event_bus_ = nullptr;
        AppStateManager* app_state_manager_ = nullptr;

        paxs::CalendarPanel calendar_panel;
        paxs::UIPanelBackground calendar_bg_;
        paxs::DebugInfoPanel debug_info_panel;
        paxs::UIPanelBackground debug_info_bg_;
#ifdef PAXS_USING_SIMULATOR
        paxs::SimulationPanel simulation_panel;
        paxs::UIPanelBackground simulation_bg_;
        paxs::SettlementStatusPanel settlement_status_panel;
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
            const MapViewport* map_viewport,
            EventBus* event_bus = nullptr,
            AppStateManager* app_state_manager = nullptr)
            : visible_manager_ptr(visible_manager),
              event_bus_(event_bus),
              app_state_manager_(app_state_manager),
              calendar_panel(ui_layout, visible_manager),
              debug_info_panel(ui_layout, visible_manager, map_viewport),
#ifdef PAXS_USING_SIMULATOR
              settlement_status_panel(visible_manager),
              simulation_panel(
                  visible_manager,
                  event_bus ? *event_bus : EventBus::getInstance(),
                  *app_state_manager  // 必須パラメータ（nullptrチェックは後で追加予定）
              ),
#endif
              calendar_bg_("CalendarBackground", &ui_layout.calendar_panel),
              debug_info_bg_("DebugInfoBackground", &ui_layout.debug_info_panel)
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
#ifdef PAXS_USING_SIMULATOR
            panels.emplace_back(&simulation_panel);
            panels.emplace_back(&simulation_bg_);
            panels.emplace_back(&settlement_status_panel);
            panels.emplace_back(&settlement_status_bg_);
#endif
            sortPanelsByLayer();

            // イベント購読を設定
            if (event_bus_ != nullptr) {
                subscribeToEvents();
            }
        }

        // コピー・ムーブ禁止（メンバー変数へのポインタをvector<IWidget*>に格納しているため）
        UILayer(const UILayer&) = delete;
        UILayer& operator=(const UILayer&) = delete;
        UILayer(UILayer&&) = delete;
        UILayer& operator=(UILayer&&) = delete;

        /// @brief イベントバスを設定してイベント駆動を有効化
        /// @brief Set EventBus to enable event-driven updates
        /// @param event_bus EventBusへのポインタ
        void setEventBus(EventBus* event_bus) {
            event_bus_ = event_bus;
            if (event_bus_ != nullptr) {
                subscribeToEvents();
            }
        }

        // TODO: イベントベースに移行
        /// @brief SettlementStatusPanelの背景可視性を同期
        void syncSettlementStatusBackground() {
#ifdef PAXS_USING_SIMULATOR
            settlement_status_bg_.setVisible(settlement_status_panel.isVisible());
#endif
        }

        /// @brief UILayerの初期化（一度だけ呼び出す）
        void initialize() {
            if (!app_state_manager_) return;

            const auto& koyomi = app_state_manager_->getKoyomi();

            // UIレイアウトを初期計算
            ui_layout.calculate(koyomi.date_list.size(), calendar_panel.getTimeControlHeight());

#ifdef PAXS_USING_SIMULATOR
            // シミュレーションパネルの参照を設定（一度のみ）
            const auto& simulation_manager = app_state_manager_->getSimulationManager();
            const auto& koyomi_sim = app_state_manager_->getKoyomi();
            simulation_panel.getControlButtons().setReferences(&simulation_manager, &koyomi_sim,
                ui_layout.koyomi_font_y + ui_layout.next_rect_start_y + 20);
            simulation_panel.setupCallback();

            // 背景の可視性をパネルと同期
            simulation_bg_.setVisible(simulation_panel.isVisible());
#endif

            // CalendarPanelの初期設定
            if (calendar_panel.isVisible()) {
                calendar_panel.setCalendarParams(koyomi);
                calendar_panel.setTimeControlParams(koyomi, app_state_manager_);
            }

            // 背景の可視性をパネルと同期
            calendar_bg_.setVisible(calendar_panel.isVisible());
            debug_info_bg_.setVisible(debug_info_panel.isVisible());
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
            return visible_manager_ptr->isVisible(FeatureVisibilityManager::View::UI);
        }
        void setEnabled(bool /*enabled*/) override {}
        void setVisible(bool /*visible*/) override {}
        bool isEnabled() const override { return true; }
        void setPos(const paxg::Vec2i& /*pos*/) override {}
        paxg::Rect getRect() const override { return paxg::Rect{}; }
        const char* getName() const override { return "UILayer"; }
        RenderLayer getLayer() const override { return RenderLayer::UIContent; }

    private:
        /// @brief イベントを購読
        /// @brief Subscribe to events
        void subscribeToEvents() {
            if (event_bus_ == nullptr) return;
            // ウィンドウリサイズイベントの購読
            event_bus_->subscribe<WindowResizedEvent>(
                [this](const WindowResizedEvent& event) {
                    (void)event;
                    // UIレイアウトを再計算
                    if (app_state_manager_) {
                        const auto& koyomi = app_state_manager_->getKoyomi();
                        ui_layout.calculate(koyomi.date_list.size(), calendar_panel.getTimeControlHeight());
                    }
                }
            );

            // 日付変更イベントの購読
            event_bus_->subscribe<DateChangedEvent>(
                [this](const DateChangedEvent& event) {
                    (void)event;
                    // CalendarPanelの日付表示を更新
                    if (calendar_panel.isVisible() && app_state_manager_) {
                        const auto& koyomi = app_state_manager_->getKoyomi();
                        calendar_panel.setCalendarParams(koyomi);
                        calendar_panel.setTimeControlParams(koyomi, app_state_manager_);

                        // レイアウトも再計算（日付リストのサイズが変わる可能性があるため）
                        ui_layout.calculate(koyomi.date_list.size(), calendar_panel.getTimeControlHeight());
                    }
                }
            );

            // 言語変更イベントの購読
            event_bus_->subscribe<LanguageChangedEvent>(
                [this](const LanguageChangedEvent& event) {
                    (void)event;
                    // 言語変更時はレイアウト再計算が必要
                    if (app_state_manager_) {
                        const auto& koyomi = app_state_manager_->getKoyomi();
                        ui_layout.calculate(koyomi.date_list.size(), calendar_panel.getTimeControlHeight());
                    }
                }
            );

            // 機能可視性変更イベントの購読
            event_bus_->subscribe<FeatureVisibilityChangedEvent>(
                [this](const FeatureVisibilityChangedEvent& event) {
                    (void)event;
                    // 背景の可視性をパネルと同期
                    // UILayer全体の可視性はrender()でチェックされるため、ここでは個別パネルの可視性のみ考慮
                    calendar_bg_.setVisible(calendar_panel.isVisible());
                    debug_info_bg_.setVisible(debug_info_panel.isVisible());
#ifdef PAXS_USING_SIMULATOR
                    simulation_bg_.setVisible(simulation_panel.isVisible());
                    // settlement_status_bg_はGraphicsManagerで制御
#endif
                }
            );

#ifdef PAXS_USING_SIMULATOR
            // シミュレーション状態変更イベントの購読
            event_bus_->subscribe<SimulationStateChangedEvent>(
                [this](const SimulationStateChangedEvent& event) {
                    (void)event;
                    // シミュレーション状態変更時の処理
                    if (app_state_manager_) {
                        const auto& koyomi = app_state_manager_->getKoyomi();
                        ui_layout.calculate(koyomi.date_list.size(), calendar_panel.getTimeControlHeight());


                        // 背景の可視性をパネルと同期
                        simulation_bg_.setVisible(simulation_panel.isVisible());
                    }
                }
            );

            // シミュレーションステップ実行イベントの購読
            event_bus_->subscribe<SimulationStepExecutedEvent>(
                [this](const SimulationStepExecutedEvent& event) {
                    (void)event;
                }
            );
#endif
        }
    };

}

#endif // !PAX_MAHOROBA_UI_LAYER_HPP
