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
#include <PAX_MAHOROBA/Input/UIInputHandler.hpp>
#include <PAX_MAHOROBA/Map/MapContentLayer.hpp>
#include <PAX_MAHOROBA/Map/MapViewport.hpp>
#include <PAX_MAHOROBA/Map/Tile/TileManager.hpp>
#include <PAX_MAHOROBA/Rendering/FontManager.hpp>
#include <PAX_MAHOROBA/Rendering/Photo360Layer.hpp>
#include <PAX_MAHOROBA/Rendering/RenderLayerManager.hpp>
#include <PAX_MAHOROBA/UI/HeaderPanel.hpp>
#include <PAX_MAHOROBA/UI/UILayer.hpp>

#include <PAX_SAPIENTICA/Calendar/Koyomi.hpp>
#include <PAX_SAPIENTICA/FeatureVisibilityManager.hpp>
#include <PAX_SAPIENTICA/InputFile/KeyValueTSV.hpp>
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
        HeaderPanel header_panel_;      // ヘッダーパネル（メニューバー + 言語選択）
        std::unique_ptr<UILayer> ui_layer_;
        MapContentLayer map_content_layer_;
        Photo360Layer photo360_layer_;  // 360度写真レイヤー

        // レイヤーベースシステム
        RenderLayerManager render_layer_manager_;

        // 入力管理システム
        EventRouter event_router_;     // 座標に依存しないイベント（キーボード、リサイズ等）
        MouseEventRouter mouse_event_router_;     // マウス入力のみ（レイヤーベース）

        // 入力ハンドラー
        MapViewportInputHandler* map_viewport_input_handler_ = nullptr;
        UIInputHandler ui_input_handler_;

        // 機能の可視性管理
        FeatureVisibilityManager visible_manager_;

        // 選択言語
        SelectLanguage select_language_;

        // 言語選択メニュー用辞書
        paxs::Language language_text;

        // シミュレーションモデル用辞書
        paxs::Language simulation_text;

        // ウィンドウリサイズ検知用
        int last_window_width_ = 0;
        int last_window_height_ = 0;

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

    public:
        GraphicsManager()
        {
            // 可視性の初期化
            visible_manager_.emplace(MurMur3::calcHash("Calendar"), true); // 暦
            visible_manager_.emplace(MurMur3::calcHash("Map"), true); // 地図
            visible_manager_.emplace(MurMur3::calcHash("UI"), true); // UI
            visible_manager_.emplace(MurMur3::calcHash("Simulation"), true); // シミュレーション
            visible_manager_.emplace(MurMur3::calcHash("License"), false); // ライセンス
            visible_manager_.emplace(MurMur3::calcHash("Debug"), false); // デバッグ
            visible_manager_.emplace(MurMur3::calcHash("3D"), false); // 360度写真

            // 言語辞書を初期化
            AppConfig::getInstance()->calcDataSettings(MurMur3::calcHash("Languages"),
                [&](const std::string& path_) { language_text.add(path_); });
            AppConfig::getInstance()->calcDataSettings(MurMur3::calcHash("SimulationModels"),
                [&](const std::string& path_) { simulation_text.add(path_); });
        }

        /// @brief 初期化
        void init(MapViewportInputHandler* handler, MapViewport* viewport, UIInputHandler* ui_input_handler) {
            // HeaderPanelを初期化
            header_panel_.init(&select_language_, &language_text, font_manager_.getLanguageFonts());

            // UILayerを依存性注入で初期化
            ui_layer_ = std::make_unique<UILayer>(
                &font_manager_,
                &select_language_,
                &language_text,
                &simulation_text,
                &visible_manager_,
                viewport
            );

            map_content_layer_.init(&font_manager_, &select_language_, viewport);

            // XYZタイルを初期化
            AppConfig::getInstance()->calcDataSettings(MurMur3::calcHash("XYZTiles"),
                [&](const std::string& path_) { tile_manager_.add(path_); });
            tile_manager_.addGridLine(); // グリッド線を追加（描画順が最後なので最後に追加）

            // レイヤーシステムに各コンポーネントを登録
            render_layer_manager_.registerRenderable(&tile_manager_);
            render_layer_manager_.registerRenderable(&map_content_layer_);
            render_layer_manager_.registerRenderable(&photo360_layer_);
            render_layer_manager_.registerRenderable(&header_panel_);
            render_layer_manager_.registerRenderable(ui_layer_.get());

            // UIInputHandlerにUILayer内のウィジェットを登録
            ui_input_handler->registerWidget(&header_panel_);
            ui_input_handler->registerWidget(ui_layer_.get());

            event_router_.registerHandler(&map_content_layer_);
            mouse_event_router_.registerHandler(ui_input_handler);
#ifdef PAXS_USING_SIMULATOR
            // SettlementInputHandler を EventRouter に登録（キーボードイベントのみ）
            event_router_.registerHandler(&map_content_layer_.getSettlementInputHandler());
#endif

            // 可視性の初期状態をメニューに反映
            header_panel_.initializeMenuFromVisibility(&visible_manager_);

            setMapViewportInputHandler(handler, viewport);
        }

        /// @brief EventRouterへのアクセス
        EventRouter& getEventRouter() { return event_router_; }

        /// @brief InputRouterへのアクセス
        MouseEventRouter& getInputRouter() { return mouse_event_router_; }

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

            // 言語選択を更新（HeaderPanelから取得）
            select_language_.set(header_panel_.getLanguageIndex());
            select_language_.setKey(std::uint_least32_t(header_panel_.getLanguageKey()));

            // メニューから可視性を同期
            header_panel_.syncVisibilityFromMenu(&visible_manager_);

            // データ更新のみ実施
            tile_manager_.updateData(visible_manager_, map_viewport, koyomi.jdn.cgetDay());

            map_content_layer_.updateData(
                koyomi,
#ifdef PAXS_USING_SIMULATOR
                simulator,
#endif
                visible_manager_
            );

            ui_layer_->updateData(
                select_language_,
                language_text,
#ifdef PAXS_USING_SIMULATOR
                simulator,
#endif
                koyomi
            );

#ifdef PAXS_USING_SIMULATOR
            // SettlementStatusPanel の表示モードを更新
            ui_layer_->getSettlementStatusPanel().setSelectDraw(
                map_content_layer_.getSettlementInputHandler().getSelectDraw()
            );

            // シミュレーターが初期化されている場合のみ表示
            ui_layer_->getSettlementStatusPanel().setVisible(simulator != nullptr);
#endif

            photo360_layer_.setVisible(visible_manager_.isVisible(paxs::MurMur3::calcHash(2, "3D")));

            // 3Dモード時は360度写真とUIのみ描画、通常モードは全レイヤー描画
            if (visible_manager_.isVisible(paxs::MurMur3::calcHash(2, "3D"))) {
                // 3Dモード: 360度写真を描画してからUIを描画
                photo360_layer_.render();
                ui_layer_->render();
            } else {
                // 通常モード: レイヤーベース描画（Z順序自動管理）
                render_layer_manager_.renderAll();
            }
        }
    };
}

#endif // !PAX_MAHOROBA_GRAPHICS_MANAGER_HPP
