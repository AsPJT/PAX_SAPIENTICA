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
#include <PAX_MAHOROBA/Input/MapContentInputHandler.hpp>
#include <PAX_MAHOROBA/Input/Photo360InputHandler.hpp>
#include <PAX_MAHOROBA/Map/MapContentLayer.hpp>
#include <PAX_MAHOROBA/Map/Tile/MapTileLayer.hpp>
#include <PAX_MAHOROBA/Rendering/Photo360Layer.hpp>
#include <PAX_MAHOROBA/Rendering/RenderLayerManager.hpp>
#include <PAX_MAHOROBA/UI/LoadingProgressBar.hpp>
#include <PAX_MAHOROBA/UI/MenuBar/MenuBar.hpp>
#include <PAX_MAHOROBA/UI/UILayer.hpp>

#ifdef PAXS_DEVELOPMENT
#include <PAX_MAHOROBA/UI/Debug/DebugLayer.hpp>
#endif

namespace paxs {

    /// @brief アプリケーションコンポーネント統合管理クラス
    /// @brief Application component integrated management class
    /// @details レンダリングレイヤー、UIレイヤー、入力ハンドラーを統合管理
    class AppComponentManager {
    private:
        AppStateManager& app_state_;
        RenderLayerManager render_layer_manager_;
        MapTileLayer map_tile_layer_;
        MapContentLayer map_content_layer_;
        MapContentInputHandler map_content_input_handler_;
        Photo360Layer photo360_layer_;
        Photo360InputHandler photo360_input_handler_;
        UILayer ui_layer_;
        MenuBar menu_bar_;

#ifdef PAXS_USING_SIMULATOR
        LoadingProgressBar<bool> loading_progress_bar_;  ///< ロード進捗バー / Loading progress bar
#endif

#ifdef PAXS_DEVELOPMENT
        DebugLayer debug_layer_;
#endif

    public:
        AppComponentManager(AppStateManager& app_state)
            : app_state_(app_state)
            , map_tile_layer_(app_state)
            , map_content_layer_(app_state)
            , photo360_layer_(app_state.getVisibilityManager())
            , photo360_input_handler_(photo360_layer_)
            , ui_layer_(app_state)
            , menu_bar_(app_state_.getVisibilityManager())
            , map_content_input_handler_(
                map_content_layer_.getFeatures(),
                map_content_layer_.getRenderContext(),
                app_state.getVisibilityManager()
            )
#ifdef PAXS_USING_SIMULATOR
            , loading_progress_bar_(
                &app_state.getLoadingHandle(),
                paxg::Window::width() / 2 - 200,
                paxg::Window::height() / 2 - 15,
                400,
                30,
                Fonts().getFont(FontProfiles::UI_MEDIUM)
            )
#endif
#ifdef PAXS_DEVELOPMENT
            , debug_layer_()
#endif
        {
            // menubar生成時に可視性が初期化されるため、UILayerの可視性初期化はここで実行
            ui_layer_.initializeVisibility();

            // レイヤーシステムに各コンポーネントを登録
            render_layer_manager_.registerRenderable(&map_tile_layer_);
            render_layer_manager_.registerRenderable(&map_content_layer_);
            render_layer_manager_.registerRenderable(&photo360_layer_);
            render_layer_manager_.registerRenderable(&ui_layer_);
            render_layer_manager_.registerRenderable(&menu_bar_);

#ifdef PAXS_DEVELOPMENT
            render_layer_manager_.registerRenderable(&debug_layer_);
#endif
        }

        // コピー・ムーブ禁止（メンバー変数へのポインタをRenderLayerManagerに登録しているため）
        ~AppComponentManager() = default;
        AppComponentManager(const AppComponentManager&) = delete;
        AppComponentManager& operator=(const AppComponentManager&) = delete;
        AppComponentManager(AppComponentManager&&) = delete;
        AppComponentManager& operator=(AppComponentManager&&) = delete;

        void render() {
            // 3Dモード時は360度写真とメニューバーのみ描画、通常モードは全レイヤー描画
            if (photo360_layer_.isVisible()) {
                // 3Dモード: 360度写真を描画してからメニューバーを描画
                photo360_layer_.render();
                menu_bar_.render();
            } else {
                // 通常モード: レイヤーベース描画（Z順序自動管理）
                render_layer_manager_.renderAll();
            }
        }

#ifdef PAXS_USING_SIMULATOR
        /// @brief ロード中モードでの描画（タイル・メニューバー・進捗バーのみ）
        /// @brief Render in loading mode (tiles, menubar, and progress bar only)
        void renderLoadingMode() {
            // タイルレイヤーを描画
            map_tile_layer_.render();

            // メニューバーを描画
            menu_bar_.render();

            // ロード進捗バーを描画
            loading_progress_bar_.render();
        }
#endif

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

#ifdef PAXS_DEVELOPMENT
        DebugLayer& getDebugLayer() {
            return debug_layer_;
        }
#endif
    };
}

#endif // !PAX_MAHOROBA_APP_COMPONENT_MANAGER_HPP
