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

#include <PAX_SAPIENTICA/MurMur3.hpp>

namespace paxs {

    /// @brief グラフィック統合管理クラス（イベント駆動版）
    /// @brief Graphics integrated management class (Event-driven version)
    /// @details 責務を描画のみに限定し、入力処理や状態管理はInputManagerとAppStateManagerに委譲
    ///          Responsibilities limited to rendering only, input and state management delegated to InputManager and AppStateManager
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
            , ui_layer_(const_cast<FeatureVisibilityManager*>(&app_state.getVisibilityManager()), &app_state.getMapViewport())
            , menu_bar_() {

            // FeatureVisibilityManagerの初期化
            menu_bar_.initializeVisibility(const_cast<FeatureVisibilityManager*>(&app_state_.getVisibilityManager()));

            // レイヤーシステムに各コンポーネントを登録
            render_layer_manager_.registerRenderable(&map_tile_layer_);
            render_layer_manager_.registerRenderable(&map_content_layer_);
            render_layer_manager_.registerRenderable(&photo360_layer_);
            render_layer_manager_.registerRenderable(&ui_layer_);
            render_layer_manager_.registerRenderable(&menu_bar_);
        }

        /// @brief 描画処理のみを実行
        /// @brief Render only (no update logic)
        /// @details データはAppStateManagerから直接取得し、イベント駆動で更新される
        ///          Data is obtained directly from AppStateManager and updated via events
        void render() {
            // AppStateManagerから最新データを取得
            auto& visible_manager = const_cast<FeatureVisibilityManager&>(app_state_.getVisibilityManager());
            const auto& map_viewport = app_state_.getMapViewport();
            auto& koyomi = const_cast<Koyomi&>(app_state_.getKoyomi());

#ifdef PAXS_USING_SIMULATOR
            auto& simulation_manager = const_cast<SimulationManager&>(app_state_.getSimulationManager());
#endif

            // MenuBarの状態をFeatureVisibilityManagerに同期
            // Sync MenuBar state to FeatureVisibilityManager
            menu_bar_.syncVisibilityFromMenu(&visible_manager);

            // データ更新（将来的にはイベント駆動に移行予定）
            // Data update (to be migrated to event-driven in future)
            map_tile_layer_.updateData(visible_manager, map_viewport, koyomi.jdn.cgetDay());

            map_content_layer_.updateData(
                koyomi,
#ifdef PAXS_USING_SIMULATOR
                simulation_manager,
#endif
                visible_manager
            );

            ui_layer_.updateData(
#ifdef PAXS_USING_SIMULATOR
                simulation_manager,
#endif
                koyomi
            );

#ifdef PAXS_USING_SIMULATOR
            // SettlementStatusPanel の表示モードを更新
            ui_layer_.getSettlementStatusPanel().setSelectDraw(
                map_content_layer_.getSettlementInputHandler().getSelectDraw()
            );

            // シミュレーターが初期化されている場合のみ表示
            ui_layer_.getSettlementStatusPanel().setVisible(simulation_manager.isActive());
#endif

            photo360_layer_.setVisible(visible_manager.isVisible(paxs::MurMur3::calcHash(2, "3D")));

            // 3Dモード時は360度写真とUIのみ描画、通常モードは全レイヤー描画
            // Render 360-degree photo and UI in 3D mode, all layers in normal mode
            if (visible_manager.isVisible(paxs::MurMur3::calcHash(2, "3D"))) {
                // 3Dモード: 360度写真を描画してからUIを描画
                // 3D mode: Render 360-degree photo then UI
                photo360_layer_.render();
                menu_bar_.render();
            } else {
                // 通常モード: レイヤーベース描画（Z順序自動管理）
                // Normal mode: Layer-based rendering (automatic Z-order management)
                render_layer_manager_.renderAll();
            }
        }

        /// @brief MenuBarへのアクセス（後方互換性のため）
        /// @brief Access to MenuBar (for backward compatibility)
        MenuBar& getMenuBar() { return menu_bar_; }

        /// @brief UILayerへのアクセス（後方互換性のため）
        /// @brief Access to UILayer (for backward compatibility)
        UILayer& getUILayer() { return ui_layer_; }

        /// @brief MapContentLayerへのアクセス（後方互換性のため）
        /// @brief Access to MapContentLayer (for backward compatibility)
        MapContentLayer& getMapContentLayer() { return map_content_layer_; }
    };
}

#endif // !PAX_MAHOROBA_GRAPHICS_MANAGER_HPP
