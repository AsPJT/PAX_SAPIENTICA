/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_GRAPHICS_MANAGER_HPP
#define PAX_MAHOROBA_GRAPHICS_MANAGER_HPP

#include <PAX_GRAPHICA/Window.hpp>

#include <PAX_MAHOROBA/Core/AppStateManager.hpp>
#include <PAX_MAHOROBA/Core/EventBus.hpp>
#include <PAX_MAHOROBA/Map/MapContentLayer.hpp>
#include <PAX_MAHOROBA/Map/Tile/MapTileLayer.hpp>
#include <PAX_MAHOROBA/Rendering/Photo360Layer.hpp>
#include <PAX_MAHOROBA/Rendering/RenderLayerManager.hpp>
#include <PAX_MAHOROBA/UI/MenuBar/MenuBar.hpp>
#include <PAX_MAHOROBA/UI/UILayer.hpp>

namespace paxs {

    /// @brief グラフィック統合管理クラス
    /// @brief Graphics integrated management class
    /// @details 責務を描画のみに限定し、入力処理や状態管理はInputManagerとAppStateManagerに委譲
    class GraphicsManager {
    private:
        EventBus& event_bus_;
        AppStateManager& app_state_;

        RenderLayerManager render_layer_manager_;

        // 各レイヤー（イベント駆動版、将来的に更新予定）
        // Each layer (event-driven version, to be updated in future)
        MapTileLayer map_tile_layer_;
        MapContentLayer map_content_layer_;
        Photo360Layer photo360_layer_;
        UILayer ui_layer_;
        MenuBar menu_bar_;

    public:
        /// @brief コンストラクタ
        /// @brief Constructor
        /// @param event_bus EventBus参照 / EventBus reference
        /// @param app_state AppStateManager参照 / AppStateManager reference
        GraphicsManager(EventBus& event_bus, AppStateManager& app_state)
            : event_bus_(event_bus)
            , app_state_(app_state)
            , render_layer_manager_()
            , map_tile_layer_()
            , map_content_layer_(&app_state.getMapViewport())
            , photo360_layer_()
            , ui_layer_(
                &app_state.getVisibilityManager(),
                &app_state.getMapViewport(),
                &event_bus,
                &app_state
              )
            , menu_bar_() {

            // FeatureVisibilityManagerの初期化
            menu_bar_.initializeVisibility(&app_state_.getVisibilityManager());

            // AppStateManagerを設定
            menu_bar_.setAppStateManager(&app_state);
            map_content_layer_.setAppStateManager(&app_state);
            map_tile_layer_.setAppStateManager(&app_state);

            // UILayerの初期化（一度のみ）
            ui_layer_.initialize();

            // レイヤーシステムに各コンポーネントを登録
            render_layer_manager_.registerRenderable(&map_tile_layer_);
            render_layer_manager_.registerRenderable(&map_content_layer_);
            render_layer_manager_.registerRenderable(&photo360_layer_);
            render_layer_manager_.registerRenderable(&ui_layer_);
            render_layer_manager_.registerRenderable(&menu_bar_);
        }

        /// @brief 描画処理のみを実行
        /// @details データはAppStateManagerから直接取得し、イベント駆動で更新される
        void render() {
            // AppStateManagerから最新データを取得
            auto& visible_manager = app_state_.getVisibilityManager();

#ifdef PAXS_USING_SIMULATOR
            auto& simulation_manager = app_state_.getSimulationManager();
#endif


#ifdef PAXS_USING_SIMULATOR
            // SettlementStatusPanel の表示モードを更新
            ui_layer_.getSettlementStatusPanel().setSelectDraw(
                map_content_layer_.getSettlementInputHandler().getSelectDraw()
            );

            // シミュレーターが初期化されている かつ Simulation可視フラグがONの場合のみ表示
            const bool simulation_visible = visible_manager.isVisible(FeatureVisibilityManager::View::Simulation);
            ui_layer_.getSettlementStatusPanel().setVisible(
                simulation_manager.isActive() && simulation_visible
            );

            // SettlementStatusPanelの背景をパネルと同期
            ui_layer_.syncSettlementStatusBackground();
#endif

            // Photo360Layerの可視性設定
            photo360_layer_.setVisible(visible_manager.isVisible(FeatureVisibilityManager::View::View3D));

            // 3Dモード時は360度写真とメニューバーのみ描画、通常モードは全レイヤー描画
            if (visible_manager.isVisible(FeatureVisibilityManager::View::View3D)) {
                // 3Dモード: 360度写真を描画してからメニューバーを描画
                photo360_layer_.render();
                menu_bar_.render();
            } else {
                // 通常モード: レイヤーベース描画（Z順序自動管理）
                render_layer_manager_.renderAll();
            }
        }

        /// @brief 入力ハンドラーにウィジェットを登録
        template<typename UIInputHandlerType, typename EventRouterType>
        void registerToInputHandlers(UIInputHandlerType& ui_input_handler, EventRouterType& event_router) {
            ui_input_handler.registerWidget(&menu_bar_);
            ui_input_handler.registerWidget(&ui_layer_);

            event_router.registerHandler(&map_content_layer_);
#ifdef PAXS_USING_SIMULATOR
            event_router.registerHandler(&map_content_layer_.getSettlementInputHandler());
#endif
        }
    };
}

#endif // !PAX_MAHOROBA_GRAPHICS_MANAGER_HPP
