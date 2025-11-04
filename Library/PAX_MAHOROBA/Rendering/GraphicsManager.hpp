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

#include <PAX_MAHOROBA/Input/EventRouter.hpp>
#include <PAX_MAHOROBA/Input/MouseEventRouter.hpp>
#include <PAX_MAHOROBA/Input/MapViewportInputHandler.hpp>
#include <PAX_MAHOROBA/Map/MapContentLayer.hpp>
#include <PAX_MAHOROBA/Map/MapViewport.hpp>
#include <PAX_MAHOROBA/Map/Tile/TileManager.hpp>
#include <PAX_MAHOROBA/Rendering/FontManager.hpp>
#include <PAX_MAHOROBA/Rendering/Photo360Layer.hpp>
#include <PAX_MAHOROBA/Rendering/RenderLayerManager.hpp>
#include <PAX_MAHOROBA/UI/UILayer.hpp>

#include <PAX_SAPIENTICA/Calendar/Koyomi.hpp>
#include <PAX_SAPIENTICA/FeatureVisibilityManager.hpp>
#include <PAX_SAPIENTICA/Language.hpp>
#include <PAX_SAPIENTICA/MurMur3.hpp>

#ifdef PAXS_USING_SIMULATOR
#include <PAX_SAPIENTICA/Simulation/SettlementSimulator.hpp>
#endif

namespace paxs {

    /// @brief グラフィック統合管理クラス
    /// @brief Graphics integrated management class
    class GraphicsManager {
    private:
        FontManager font_manager_;
        TileManager tile_manager_;
        UILayer ui_manager_;
        MapContentLayer map_content_layer_;
        Photo360Layer photo360_layer_;  // 360度写真レイヤー

        // レイヤーベースシステム
        RenderLayerManager render_layer_manager_;

        // 入力管理システム
        EventRouter event_router_;     // 座標に依存しないイベント（キーボード、リサイズ等）
        MouseEventRouter mouse_event_router_;     // マウス入力のみ（レイヤーベース）

        // MapViewportの入力処理
        MapViewportInputHandler* map_viewport_input_handler_ = nullptr;

        // 機能の可視性管理
        FeatureVisibilityManager visible_;

        // 選択言語
        SelectLanguage select_language_;

        // 言語選択メニュー用辞書
        paxs::Language language_text;

        // シミュレーションモデル用辞書
        paxs::Language simulation_text;

        // ウィンドウリサイズ検知用
        int last_window_width_ = 0;
        int last_window_height_ = 0;

    public:
        GraphicsManager() {
            // 可視性の初期化
            visible_.emplace(MurMur3::calcHash("Calendar"), true); // 暦
            visible_.emplace(MurMur3::calcHash("Map"), true); // 地図
            visible_.emplace(MurMur3::calcHash("UI"), true); // UI
            visible_.emplace(MurMur3::calcHash("Simulation"), true); // シミュレーション
            visible_.emplace(MurMur3::calcHash("License"), false); // ライセンス
            visible_.emplace(MurMur3::calcHash("Debug"), false); // デバッグ
            visible_.emplace(MurMur3::calcHash("3D"), false); // 360度写真
        }

        /// @brief 初期化
        void init() {
            // 言語辞書を初期化
            AppConfig::getInstance()->calcDataSettings(MurMur3::calcHash("Languages"),
                [&](const std::string& path_) { language_text.add(path_); });
            AppConfig::getInstance()->calcDataSettings(MurMur3::calcHash("SimulationModels"),
                [&](const std::string& path_) { simulation_text.add(path_); });

            font_manager_.init(select_language_);
            ui_manager_.init(font_manager_, select_language_, language_text, simulation_text);
            map_content_layer_.init(font_manager_, select_language_);

            // XYZタイルを初期化
            AppConfig::getInstance()->calcDataSettings(MurMur3::calcHash("XYZTiles"),
                [&](const std::string& path_) { tile_manager_.add(path_); });
            tile_manager_.addGridLine(); // グリッド線を追加（描画順が最後なので最後に追加）

            // レイヤーシステムに各コンポーネントを登録
            render_layer_manager_.registerRenderable(&tile_manager_);
            render_layer_manager_.registerRenderable(&map_content_layer_);
            render_layer_manager_.registerRenderable(&photo360_layer_);
            render_layer_manager_.registerRenderable(&ui_manager_);

            // EventRouterに各コンポーネントを登録（座標に依存しないイベント用）
            event_router_.registerHandler(&map_content_layer_);

            // InputRouterに各コンポーネントを登録（マウス入力専用、UIが優先）
            mouse_event_router_.registerHandler(&ui_manager_);
            mouse_event_router_.registerHandler(&map_content_layer_);

#ifdef PAXS_USING_SIMULATOR
            // SettlementInputHandler を EventRouter に登録（キーボードイベントのみ）
            event_router_.registerHandler(&map_content_layer_.getSettlementInputHandler());
#endif

            // 可視性の初期状態をメニューに反映
            ui_manager_.initializeMenuFromVisibility(visible_);
        }

        /// @brief MapViewportInputHandlerを設定
        /// @brief Set MapViewportInputHandler
        /// @param handler MapViewportInputHandlerへのポインタ / Pointer to MapViewportInputHandler
        /// @param viewport MapViewportへのポインタ / Pointer to MapViewport
        void setMapViewportInputHandler(MapViewportInputHandler* handler, MapViewport* viewport) {
            map_viewport_input_handler_ = handler;
            if (handler != nullptr) {
                handler->setViewport(viewport);
                // EventRouterに登録（キーボード、マウスホイール用）
                event_router_.registerHandler(handler);
                // InputRouterに登録（マウス入力用、最低優先度）
                mouse_event_router_.registerHandler(handler);
            }
        }

        /// @brief EventRouterへのアクセス
        EventRouter& getEventRouter() { return event_router_; }
        const EventRouter& getEventRouter() const { return event_router_; }

        /// @brief InputRouterへのアクセス
        MouseEventRouter& getInputRouter() { return mouse_event_router_; }
        const MouseEventRouter& getInputRouter() const { return mouse_event_router_; }

        /// @brief MapViewportInputHandlerへのアクセス
        MapViewportInputHandler* getMapViewportInputHandler() const {
            return map_viewport_input_handler_;
        }

        /// @brief 更新・描画処理
        void update(
            MapViewport& map_viewport,
            paxs::Koyomi& koyomi
#ifdef PAXS_USING_SIMULATOR
            , std::unique_ptr<paxs::SettlementSimulator>& simulator
#endif
        ) {
            // ウィンドウサイズ変更を検知してイベントをブロードキャスト
            const int current_width = paxg::Window::width();
            const int current_height = paxg::Window::height();
            if (current_width != last_window_width_ || current_height != last_window_height_) {
                ResizeEvent resize_event(current_width, current_height);
                event_router_.broadcastEvent(resize_event);
                last_window_width_ = current_width;
                last_window_height_ = current_height;
            }

            // メニューから可視性を同期
            ui_manager_.updateLanguage(select_language_);
            ui_manager_.syncVisibilityFromMenu(visible_);

            // データ更新のみ実施
            tile_manager_.updateData(visible_, map_viewport, koyomi.jdn.cgetDay());

            map_content_layer_.updateData(
                map_viewport,
                koyomi,
#ifdef PAXS_USING_SIMULATOR
                simulator,
#endif
                visible_
            );

            ui_manager_.updateData(
                map_viewport,
                select_language_,
                language_text,
#ifdef PAXS_USING_SIMULATOR
                simulator,
#endif
                koyomi,
                visible_
            );

#ifdef PAXS_USING_SIMULATOR
            // SettlementStatusPanel の表示モードを更新
            ui_manager_.getSettlementStatusPanel().setSelectDraw(
                map_content_layer_.getSettlementInputHandler().getSelectDraw()
            );

            // シミュレーターが初期化されている場合のみ表示
            ui_manager_.getSettlementStatusPanel().setVisible(simulator != nullptr);
#endif

            photo360_layer_.setVisible(visible_.isVisible(paxs::MurMur3::calcHash(2, "3D")));

            // 3Dモード時は360度写真とUIのみ描画、通常モードは全レイヤー描画
            if (visible_.isVisible(paxs::MurMur3::calcHash(2, "3D"))) {
                // 3Dモード: 360度写真を描画してからUIを描画
                photo360_layer_.render();
                ui_manager_.render();
            } else {
                // 通常モード: レイヤーベース描画（Z順序自動管理）
                render_layer_manager_.renderAll();
            }
        }
    };
}

#endif // !PAX_MAHOROBA_GRAPHICS_MANAGER_HPP
