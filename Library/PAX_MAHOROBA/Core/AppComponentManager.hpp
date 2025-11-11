/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_APP_COMPONENT_MANAGER_HPP
#define PAX_MAHOROBA_APP_COMPONENT_MANAGER_HPP

#include <PAX_GRAPHICA/Window.hpp>

#include <PAX_MAHOROBA/Core/AppStateManager.hpp>
#include <PAX_MAHOROBA/Core/EventBus.hpp>
#include <PAX_MAHOROBA/Input/MapContentInputHandler.hpp>
#include <PAX_MAHOROBA/Input/Photo360InputHandler.hpp>
#include <PAX_MAHOROBA/Map/MapContentLayer.hpp>
#include <PAX_MAHOROBA/Map/Tile/MapTileLayer.hpp>
#include <PAX_MAHOROBA/Rendering/Photo360Layer.hpp>
#include <PAX_MAHOROBA/Rendering/RenderLayerManager.hpp>
#include <PAX_MAHOROBA/UI/MenuBar/MenuBar.hpp>
#include <PAX_MAHOROBA/UI/UILayer.hpp>

namespace paxs {

    /// @brief アプリケーションコンポーネント統合管理クラス
    /// @brief Application component integrated management class
    /// @details レンダリングレイヤー、UIレイヤー、入力ハンドラーを統合管理
    /// @details Manages rendering layers, UI layers, and input handlers
    /// @note 旧名: GraphicsManager（Rendering層からCore層に昇格）
    /// @note Former name: GraphicsManager (promoted from Rendering layer to Core layer)
    class AppComponentManager {
    private:
        EventBus& event_bus_;
        AppStateManager& app_state_;

        RenderLayerManager render_layer_manager_;

        // 各レイヤー（イベント駆動版、将来的に更新予定）
        // Each layer (event-driven version, to be updated in future)
        MapTileLayer map_tile_layer_;
        MapContentLayer map_content_layer_;
        Photo360Layer photo360_layer_;
        Photo360InputHandler photo360_input_handler_;
        MapContentInputHandler map_content_input_handler_;
        UILayer ui_layer_;
        MenuBar menu_bar_;

    public:
        /// @brief コンストラクタ
        /// @brief Constructor
        /// @param event_bus EventBus参照 / EventBus reference
        /// @param app_state AppStateManager参照 / AppStateManager reference
        AppComponentManager(EventBus& event_bus, AppStateManager& app_state)
            : event_bus_(event_bus)
            , app_state_(app_state)
            , render_layer_manager_()
            , map_tile_layer_()
            , map_content_layer_(&app_state.getMapViewport())
            , photo360_layer_()
            , photo360_input_handler_(
                &photo360_layer_,
                &app_state.getVisibilityManager()
              )
            , map_content_input_handler_(
                nullptr,  // 後で設定（map_content_layer_初期化後）
                nullptr,  // 後で設定（map_content_layer_初期化後）
                &app_state.getVisibilityManager()
              )
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

            // MapContentInputHandlerにFeaturesとRenderContextを設定
            // （map_content_layer_の初期化後に設定する必要があるため、ここで代入）
            map_content_input_handler_ = MapContentInputHandler(
                &map_content_layer_.getFeatures(),
                &map_content_layer_.getRenderContext(),
                &app_state.getVisibilityManager()
            );

            // UILayerの初期化（一度のみ）
            ui_layer_.initialize();

            // レイヤーシステムに各コンポーネントを登録
            render_layer_manager_.registerRenderable(&map_tile_layer_);
            render_layer_manager_.registerRenderable(&map_content_layer_);
            render_layer_manager_.registerRenderable(&photo360_layer_);
            render_layer_manager_.registerRenderable(&ui_layer_);
            render_layer_manager_.registerRenderable(&menu_bar_);
        }

        // コピー・ムーブ禁止（メンバー変数へのポインタをRenderLayerManagerに登録しているため）
        AppComponentManager(const AppComponentManager&) = delete;
        AppComponentManager& operator=(const AppComponentManager&) = delete;
        AppComponentManager(AppComponentManager&&) = delete;
        AppComponentManager& operator=(AppComponentManager&&) = delete;

        /// @brief タイルデータを更新（描画前に呼び出し）
        /// @brief Update tile data (called before rendering)
        void updateTiles() {
            map_tile_layer_.updateTileData();
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
            // シミュレーターが初期化されている かつ Simulation可視フラグがONの場合のみ表示
            const bool simulation_visible = visible_manager.isVisible(ViewMenu::simulation);
            ui_layer_.getSettlementStatusPanel().setVisible(
                simulation_manager.isActive() && simulation_visible
            );

            // SettlementStatusPanelの背景をパネルと同期
            ui_layer_.syncSettlementStatusBackground();
#endif

            // Photo360Layerの可視性設定
            photo360_layer_.setVisible(visible_manager.isVisible(ViewMenu::view_3d));

            // 3Dモード時は360度写真とメニューバーのみ描画、通常モードは全レイヤー描画
            if (visible_manager.isVisible(ViewMenu::view_3d)) {
                // 3Dモード: 360度写真を描画してからメニューバーを描画
                photo360_layer_.render();
                menu_bar_.render();
            } else {
                // 通常モード: レイヤーベース描画（Z順序自動管理）
                render_layer_manager_.renderAll();
            }
        }

        /// @brief 入力ハンドラーにウィジェットを登録
        /// @brief Register widgets to input handlers
        /// @param ui_input_handler UI入力ハンドラー / UI input handler
        /// @param input_router 統合入力ルーター / Unified input router
        ///
        /// 入力優先度:
        /// Input priority:
        /// - キーボード / Keyboard: Photo360 (Photo360:300) > Settlement (MapContent:200) > Map (MapTile:100)
        /// - マウス / Mouse: UI (UIContent:500) > MapContent (MapContent:200) > MapViewport (MapTile:100)
        template<typename UIInputHandlerType, typename InputRouterType>
        void registerToInputHandlers(UIInputHandlerType& ui_input_handler, InputRouterType& input_router) {
            // UIウィジェット登録（マウス入力最高優先度）
            ui_input_handler.registerWidget(&menu_bar_);
            ui_input_handler.registerWidget(&ui_layer_);

            // 統合入力ルーターに登録（レイヤーベース優先度制御）
            input_router.registerHandler(&photo360_input_handler_);
            input_router.registerHandler(&map_content_input_handler_);
#ifdef PAXS_USING_SIMULATOR
            input_router.registerHandler(&map_content_layer_.getSettlementInputHandler());
#endif
        }
    };
}

#endif // !PAX_MAHOROBA_APP_COMPONENT_MANAGER_HPP
