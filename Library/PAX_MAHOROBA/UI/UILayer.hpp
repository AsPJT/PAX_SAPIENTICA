/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_UI_LAYER_HPP
#define PAX_MAHOROBA_UI_LAYER_HPP

#include <limits>
#include <string>
#include <variant>
#include <vector>

#ifdef PAXS_USING_SIMULATOR
#include <PAX_MAHOROBA/UI/SimulationPanel.hpp>
#include <PAX_MAHOROBA/UI/SettlementStatusPanel.hpp>
#endif

#include <PAX_GRAPHICA/RenderTexture.hpp>
#include <PAX_GRAPHICA/ScopedRenderState.hpp>
#include <PAX_GRAPHICA/System.hpp>
#include <PAX_GRAPHICA/Texture.hpp>

#include <PAX_MAHOROBA/UI/Calendar/CalendarPanel.hpp>
#include <PAX_MAHOROBA/UI/UILayout.hpp>
#include <PAX_MAHOROBA/UI/DebugInfoPanel.hpp>
#include <PAX_MAHOROBA/UI/HeaderPanel.hpp>
#include <PAX_MAHOROBA/UI/UIPanelBackground.hpp>
#include <PAX_MAHOROBA/Rendering/IWidget.hpp>
#include <PAX_MAHOROBA/Map/MapViewport.hpp>
#include <PAX_MAHOROBA/Rendering/FontManager.hpp>
#include <PAX_MAHOROBA/Input/IInputHandler.hpp>
#include <PAX_MAHOROBA/Rendering/IRenderable.hpp>

#include <PAX_SAPIENTICA/AppConfig.hpp>
#include <PAX_SAPIENTICA/Calendar/Date.hpp>
#include <PAX_SAPIENTICA/Calendar/Koyomi.hpp>
#include <PAX_SAPIENTICA/FeatureVisibilityManager.hpp>
#include <PAX_SAPIENTICA/InputFile/KeyValueTSV.hpp>
#include <PAX_SAPIENTICA/InputStateManager.hpp>
#include <PAX_SAPIENTICA/Language.hpp>
#include <PAX_SAPIENTICA/MurMur3.hpp>


namespace paxs {

    /// @brief UIレイヤーの統合管理を担当するクラス
    /// @brief Integrated management class for UI layer
    class UILayer : public IRenderable, public IInputHandler {
    private:
        bool visible_ = true;
        bool enabled_ = true;
        FontManager* font_manager_ = nullptr; // 文字表示専用クラス（依存性注入）

        // 描画に必要なデータをキャッシュ（updateData()で更新、render()で使用）
        MapViewport cached_map_viewport_;
        const SelectLanguage* cached_select_language_ = nullptr;
        const paxs::Language* cached_language_text_ = nullptr;
#ifdef PAXS_USING_SIMULATOR
        std::unique_ptr<paxs::SettlementSimulator>* cached_simulator_ = nullptr;
#endif
        paxs::InputStateManager* cached_input_state_manager_ = nullptr;
        paxs::Koyomi cached_koyomi_;
        paxs::FeatureVisibilityManager* cached_visible_ = nullptr;

        std::size_t map_viewport_width_str_index = 0;
        std::size_t map_viewport_center_x_str_index = 0;
        std::size_t map_viewport_center_y_str_index = 0;
        std::size_t map_viewport_center_lat_str_index = 0;
        std::size_t xyz_tile_z_str_index = 0;

        // UI の影
        paxg::RenderTexture shadow_texture{};
        paxg::RenderTexture internal_texture{};

        paxs::HeaderPanel header_panel;  // ヘッダーパネル（メニューバー + 言語選択）

        // 背景コンポーネント
        paxs::UIPanelBackground header_bg_{"HeaderBackground"};
        paxs::UIPanelBackground calendar_bg_{"CalendarBackground"};
#ifdef PAXS_USING_SIMULATOR
        paxs::UIPanelBackground simulation_bg_{"SimulationBackground"};
        paxs::UIPanelBackground settlement_status_bg_{"SettlementStatusBackground"};
#endif

        // IWidget を実装したウィジェットの統合管理
        std::vector<IWidget*> widgets;

        paxs::KeyValueTSV<paxg::Texture> key_value_tsv;
        paxs::UILayout ui_layout;
        paxs::CalendarPanel calendar_panel;        // カレンダーパネル（時間操作 + カレンダー表示）
        paxs::DebugInfoPanel debug_info_panel;     // デバッグ情報パネル

#ifdef PAXS_USING_SIMULATOR
        paxs::SimulationPanel simulation_panel;    // シミュレーションパネル
        paxs::SettlementStatusPanel settlement_status_panel;  // Settlement 表示モードステータス
#endif

    public:
        UILayer() = default;

        void init(
            FontManager& font_manager,
            const SelectLanguage& select_language,
            const paxs::Language& language_text,
            const paxs::Language& simulation_text
        ) {
#ifndef PAXS_USING_SIMULATOR
            (void)simulation_text; // シミュレーター未使用時の警告を抑制
#endif
            // FontManagerの参照を保存
            font_manager_ = &font_manager;

            calendar_panel.init(font_manager_->getLanguageFonts());

            debug_info_panel.init(font_manager_->getLanguageFonts());
            map_viewport_width_str_index = (MurMur3::calcHash(25, "debug_magnification_power"));
            map_viewport_center_x_str_index = (MurMur3::calcHash(24, "debug_mercator_longitude"));
            map_viewport_center_y_str_index = (MurMur3::calcHash(23, "debug_mercator_latitude"));
            map_viewport_center_lat_str_index = (MurMur3::calcHash(14, "debug_latitude"));
            xyz_tile_z_str_index = (MurMur3::calcHash(17, "debug_xyz_tiles_z"));

            header_panel.init(&select_language, &language_text, font_manager_->getLanguageFonts());

            // 暦の時間操作のアイコン
            if (!key_value_tsv.input(paxs::AppConfig::getInstance()->getRootPath() + "Data/MenuIcon/MenuIcons.tsv", [&](const std::string& value_) { return paxg::Texture{ value_ }; })) {
                PAXS_ERROR("Failed to load texture KeyValueTSV: Data/MenuIcon/MenuIcons.tsv");
            }

            // HeaderPanelにGitHubアイコンを設定
            header_panel.setGitHubTexture(&key_value_tsv.get().at(MurMur3::calcHash("texture_github")));

#ifdef PAXS_USING_SIMULATOR
            simulation_panel.init(select_language, simulation_text, font_manager_->getLanguageFonts());
            settlement_status_panel.init(font_manager_->getLanguageFonts(), select_language);
#endif

            // 影
            shadow_texture = paxg::RenderTexture{ paxg::Window::Size(), paxg::ColorF{ 1.0, 0.0 } };
            internal_texture = paxg::RenderTexture{ shadow_texture.size() };

            // IWidget を実装したウィジェットを登録
            widgets.clear();
            // 背景コンポーネント（RenderLayer::UIBackground = 300）
            widgets.push_back(&header_bg_);
            widgets.push_back(&calendar_bg_);
#ifdef PAXS_USING_SIMULATOR
            widgets.push_back(&simulation_bg_);
            widgets.push_back(&settlement_status_bg_);
#endif
            // コンテンツコンポーネント（RenderLayer::UIContent = 400）
            widgets.push_back(&calendar_panel);
            widgets.push_back(&debug_info_panel);
#ifdef PAXS_USING_SIMULATOR
            widgets.push_back(&simulation_panel);
            widgets.push_back(&settlement_status_panel);
#endif
            widgets.push_back(&header_panel);

            // TimeControlWidgetに必要な参照を設定
            // 注: texture_dictionaryとkoyomiは後で設定する必要がある
        }

        /// @brief 可視性状態をメニューに反映（初期化時に呼び出し）
        void initializeMenuFromVisibility(paxs::FeatureVisibilityManager& visible) {
            auto* view_menu = header_panel.getMenuBar().getMenuItem(MurMur3::calcHash("view"));
            if (view_menu) {
                view_menu->setIsItems(std::size_t(0), visible.isVisible(MurMur3::calcHash("Calendar")));
                view_menu->setIsItems(std::size_t(1), visible.isVisible(MurMur3::calcHash("Map")));
                view_menu->setIsItems(std::size_t(2), visible.isVisible(MurMur3::calcHash("UI")));
                view_menu->setIsItems(std::size_t(3), visible.isVisible(MurMur3::calcHash("Simulation")));
                view_menu->setIsItems(std::size_t(4), visible.isVisible(MurMur3::calcHash("License")));
                view_menu->setIsItems(std::size_t(5), visible.isVisible(MurMur3::calcHash("Debug")));
                view_menu->setIsItems(std::size_t(6), visible.isVisible(MurMur3::calcHash("3D")));
            }
        }

        /// @brief 選択言語を更新（HeaderPanelから取得）
        void updateLanguage(SelectLanguage& select_language) {
            select_language.set(std::size_t(header_panel.getLanguageIndex()));
            select_language.setKey(std::uint_least32_t(header_panel.getLanguageKey()));
        }

        /// @brief メニュー状態を可視性に反映（毎フレーム呼び出し）
        void syncVisibilityFromMenu(paxs::FeatureVisibilityManager& visible) {
            // View メニューの状態を同期
            auto* view_menu = header_panel.getMenuBar().getMenuItem(MurMur3::calcHash("view"));
            if (view_menu) {
                visible.setVisibility(MurMur3::calcHash("Calendar"), view_menu->getIsItems(std::size_t(0)));
                visible.setVisibility(MurMur3::calcHash("Map"), view_menu->getIsItems(std::size_t(1)));
                visible.setVisibility(MurMur3::calcHash("UI"), view_menu->getIsItems(std::size_t(2)));
                visible.setVisibility(MurMur3::calcHash("Simulation"), view_menu->getIsItems(std::size_t(3)));
                visible.setVisibility(MurMur3::calcHash("License"), view_menu->getIsItems(std::size_t(4)));
                visible.setVisibility(MurMur3::calcHash("Debug"), view_menu->getIsItems(std::size_t(5)));
                visible.setVisibility(MurMur3::calcHash("3D"), view_menu->getIsItems(std::size_t(6)));
            }

            // Place Names メニューの状態を同期
            auto* place_names_menu = header_panel.getMenuBar().getMenuItem(MurMur3::calcHash("place_names"));
            if (place_names_menu) {
                visible.setVisibility(MurMur3::calcHash("place_name"), place_names_menu->getIsItems(std::size_t(0)));
                visible.setVisibility(MurMur3::calcHash("site"), place_names_menu->getIsItems(std::size_t(1)));
                visible.setVisibility(MurMur3::calcHash("tumulus"), place_names_menu->getIsItems(std::size_t(2)));
                visible.setVisibility(MurMur3::calcHash("dolmen"), place_names_menu->getIsItems(std::size_t(3)));
                visible.setVisibility(MurMur3::calcHash("kamekanbo"), place_names_menu->getIsItems(std::size_t(4)));
                visible.setVisibility(MurMur3::calcHash("stone_coffin"), place_names_menu->getIsItems(std::size_t(5)));
                visible.setVisibility(MurMur3::calcHash("doken"), place_names_menu->getIsItems(std::size_t(6)));
                visible.setVisibility(MurMur3::calcHash("dotaku"), place_names_menu->getIsItems(std::size_t(7)));
                visible.setVisibility(MurMur3::calcHash("bronze_mirror"), place_names_menu->getIsItems(std::size_t(8)));
                visible.setVisibility(MurMur3::calcHash("human_bone"), place_names_menu->getIsItems(std::size_t(9)));
                visible.setVisibility(MurMur3::calcHash("mtdna"), place_names_menu->getIsItems(std::size_t(10)));
                visible.setVisibility(MurMur3::calcHash("ydna"), place_names_menu->getIsItems(std::size_t(11)));
            }

            // Map メニューの状態を同期
            auto* map_menu = header_panel.getMenuBar().getMenuItem(MurMur3::calcHash("map"));
            if (map_menu) {
                visible.setVisibility(MurMur3::calcHash("menu_bar_map_base"), map_menu->getIsItems(std::size_t(0)));
                visible.setVisibility(MurMur3::calcHash("menu_bar_map_land_and_sea"), map_menu->getIsItems(std::size_t(1)));
                visible.setVisibility(MurMur3::calcHash("menu_bar_map_land_and_water"), map_menu->getIsItems(std::size_t(2)));
                visible.setVisibility(MurMur3::calcHash("menu_bar_map_soil"), map_menu->getIsItems(std::size_t(3)));
                visible.setVisibility(MurMur3::calcHash("menu_bar_map_soil_temperature"), map_menu->getIsItems(std::size_t(4)));
                visible.setVisibility(MurMur3::calcHash("menu_bar_map_ryosei_country"), map_menu->getIsItems(std::size_t(5)));
                visible.setVisibility(MurMur3::calcHash("menu_bar_map_ryosei_line"), map_menu->getIsItems(std::size_t(6)));
                visible.setVisibility(MurMur3::calcHash("menu_bar_map_slope"), map_menu->getIsItems(std::size_t(7)));
                visible.setVisibility(MurMur3::calcHash("menu_bar_map_lakes_and_rivers1"), map_menu->getIsItems(std::size_t(8)));
                visible.setVisibility(MurMur3::calcHash("menu_bar_map_lakes_and_rivers2"), map_menu->getIsItems(std::size_t(9)));
                visible.setVisibility(MurMur3::calcHash("menu_bar_map_line1"), map_menu->getIsItems(std::size_t(10)));
                visible.setVisibility(MurMur3::calcHash("menu_bar_map_line2"), map_menu->getIsItems(std::size_t(11)));
            }
        }

        /// @brief ウィンドウサイズ変更時のテクスチャ再初期化
        void handleWindowResize() {
            // ウィンドウサイズ変更時に影テクスチャを再作成
            shadow_texture = paxg::RenderTexture{ paxg::Window::Size(), paxg::ColorF{ 1.0, 0.0 } };
            internal_texture = paxg::RenderTexture{ shadow_texture.size() };
        }

        /// @brief データ更新（描画は行わない）
        /// @brief Update data (no drawing)
        void updateData(
            MapViewport& map_viewport,
            const SelectLanguage& select_language,
            const paxs::Language& language_text,
#ifdef PAXS_USING_SIMULATOR
            std::unique_ptr<paxs::SettlementSimulator>& simulator,
#endif
            paxs::InputStateManager& input_state_manager,
            paxs::Koyomi& koyomi,
            paxs::FeatureVisibilityManager& visible
            ) {
            map_viewport.getCoordinate().toEquirectangularDegY();

            // UIレイアウトを計算
            ui_layout.calculate(koyomi.date_list.size(), calendar_panel.getTimeControlHeight());

            // 背景コンポーネントに影テクスチャとレイアウトを設定
            header_bg_.setShadowTextures(shadow_texture, internal_texture);
            header_bg_.setLayout(nullptr);  // 画面幅全体を使用
            header_bg_.setHeight(header_panel.getHeight());

            calendar_bg_.setShadowTextures(shadow_texture, internal_texture);
            calendar_bg_.setLayout(&ui_layout.calendar_panel);

#ifdef PAXS_USING_SIMULATOR
            simulation_bg_.setShadowTextures(shadow_texture, internal_texture);
            simulation_bg_.setLayout(&ui_layout.simulation_panel);

            settlement_status_bg_.setShadowTextures(shadow_texture, internal_texture);
            // SettlementStatusPanelのレイアウト（固定位置）
            // TODO: UILayoutに統合
            PanelLayout settlement_status_layout;
            settlement_status_layout.x = 40;
            settlement_status_layout.y = 80;
            settlement_status_layout.width = 300;   // テキスト幅 + パディング
            settlement_status_layout.height = 60;   // テキスト高さ + パディング
            settlement_status_bg_.setLayout(&settlement_status_layout);
#endif
#ifdef PAXS_USING_SIMULATOR
            bool simulation_visible = visible.isVisible(MurMur3::calcHash("Simulation")) &&
                                    visible.isVisible(MurMur3::calcHash(2, "UI"));
            simulation_panel.setVisible(simulation_visible);

            simulation_panel.setReferences(simulator, input_state_manager, koyomi, visible,
                font_manager_->getLanguageFonts(), select_language, language_text,
                ui_layout.koyomi_font_y + ui_layout.next_rect_start_y + 20);

            simulation_bg_.setVisible(simulation_panel.isVisible());

            settlement_status_bg_.setVisible(settlement_status_panel.isVisible());
#endif

            // CalendarPanelの可視性と設定
            bool calendar_visible = visible.isVisible(MurMur3::calcHash(8, "Calendar")) && visible.isVisible(MurMur3::calcHash(2, "UI"));
            if (calendar_visible) {
                calendar_panel.setLayout(ui_layout, key_value_tsv.get());
                calendar_panel.setCalendarParams(koyomi, select_language, language_text);
                calendar_panel.setTimeControlParams(koyomi);
                calendar_panel.setVisible(true);
                calendar_bg_.setVisible(true);
            } else {
                calendar_panel.setVisible(false);
                calendar_bg_.setVisible(false);
            }

            debug_info_panel.setVisible(visible.isVisible(MurMur3::calcHash("UI")) && visible.isVisible(MurMur3::calcHash("Debug")));

            // ウィジェットを更新（入力処理）
            InputEvent event;
            event.input_state_manager = &input_state_manager;
            for (auto* widget : widgets) {
                if (widget) {
                    widget->handleInput(event);
                }
            }

            // 描画用にデータをキャッシュ
            cached_map_viewport_ = map_viewport;
            cached_select_language_ = &select_language;
            cached_language_text_ = &language_text;
#ifdef PAXS_USING_SIMULATOR
            cached_simulator_ = &simulator;
#endif
            cached_input_state_manager_ = &input_state_manager;
            cached_koyomi_ = koyomi;
            cached_visible_ = &visible;
        }

        // IRenderable の実装
        // IRenderable implementation

        /// @brief レンダリング処理
        /// @brief Render
        void render() override {
            if (!visible_ || cached_visible_ == nullptr) return;

            // 画像の拡大縮小の方式を設定
            const paxg::ScopedSamplerState sampler{ paxg::SamplerState::ClampLinear };

            paxs::FeatureVisibilityManager& visible = *cached_visible_;
            MapViewport& map_viewport = cached_map_viewport_;
            const SelectLanguage& select_language = *cached_select_language_;
            const paxs::Language& language_text = *cached_language_text_;
            paxs::Koyomi& koyomi = cached_koyomi_;

            // 1. バッチ描画開始（Siv3D用）
            PanelBackground::beginBatch();

            // 2. 背景ウィジェットを描画（バッチに登録）
            for (auto* widget : widgets) {
                if (widget && widget->getLayer() == RenderLayer::UIBackground) {
                    widget->render();
                }
            }

            // 3. バッチ描画終了（すべての背景を一括描画）
            PanelBackground::endBatch(&shadow_texture, &internal_texture);

            // 4. UIコンテンツを描画
            // デバッグ情報パネルのコンテンツを描画
            if (visible.isVisible(MurMur3::calcHash("Debug"))) {
#ifdef PAXS_USING_SIMULATOR
                bool is_simulator_active = (*cached_simulator_ != nullptr);
                debug_info_panel.renderMapAndSimulationInfo(
                    map_viewport, select_language, language_text, *cached_simulator_, &koyomi, is_simulator_active
                );
#else
                debug_info_panel.renderMapAndSimulationInfo(
                    map_viewport, select_language, language_text, &koyomi, false
                );
#endif
            }

            // コンテンツウィジェットを描画
            for (auto* widget : widgets) {
                if (widget && widget->getLayer() != RenderLayer::UIBackground) {
                    widget->render();
                }
            }
        }

        /// @brief レイヤーを取得
        /// @brief Get layer
        /// @return UIContentレイヤー / UIContent layer
        RenderLayer getLayer() const override {
            return RenderLayer::UIContent;
        }

        /// @brief 可視性を取得
        /// @brief Get visibility
        bool isVisible() const override {
            return visible_;
        }

        /// @brief 可視性を設定
        /// @brief Set visibility
        void setVisible(bool visible) override {
            visible_ = visible;
        }

#ifdef PAXS_USING_SIMULATOR
        /// @brief SettlementStatusPanelへのアクセス
        /// @brief Access to SettlementStatusPanel
        SettlementStatusPanel& getSettlementStatusPanel() { return settlement_status_panel; }
#endif

        // IInputHandler の実装
        // IInputHandler implementation

        /// @brief 入力処理（子ウィジェットに委譲）
        /// @brief Handle input (delegate to child widgets)
        /// @param event 入力イベント / Input event
        /// @return 処理した場合true / true if handled
        bool handleInput(const InputEvent& event) override {
            if (!enabled_ || !visible_) return false;

            // ウィンドウリサイズイベントを処理
            if (event.type == InputEventType::WindowResize) {
                handleWindowResize();
                return false; // 他のハンドラーにも処理を継続
            }

            // 座標に依存しないイベント（キーボード、マウスホイール、フォーカス）はスキップ
            if (event.type == InputEventType::Keyboard ||
                event.type == InputEventType::MouseWheel ||
                event.type == InputEventType::WindowFocus) {
                return false;
            }

            // 子ウィジェットに順番に入力イベントを渡す（マウス/タッチのみ）
            for (auto* widget : widgets) {
                if (widget && widget->isEnabled() && widget->isVisible()) {
                    if (widget->hitTest(event.x, event.y)) {
                        if (widget->handleInput(event)) {
                            return true;  // 処理された
                        }
                    }
                }
            }

            // HeaderPanelは常に処理を試みる（メニューバーは画面上部に固定）
            if (header_panel.isEnabled() && header_panel.isVisible()) {
                if (header_panel.hitTest(event.x, event.y)) {
                    if (header_panel.handleInput(event)) {
                        return true;
                    }
                }
            }

            return false;
        }

        /// @brief ヒットテスト（子ウィジェットのいずれかにヒット）
        /// @brief Hit test (hit any of child widgets)
        /// @param x X座標 / X coordinate
        /// @param y Y座標 / Y coordinate
        /// @return 範囲内ならtrue / true if within bounds
        bool hitTest(int x, int y) const override {
            if (!visible_ || !enabled_) return false;

            // いずれかの子ウィジェットがヒットすればtrue
            for (const auto* widget : widgets) {
                if (widget && widget->isVisible() && widget->isEnabled()) {
                    if (widget->hitTest(x, y)) {
                        return true;
                    }
                }
            }

            // HeaderPanelのヒットテスト
            if (header_panel.isVisible() && header_panel.isEnabled()) {
                if (header_panel.hitTest(x, y)) {
                    return true;
                }
            }

            return false;
        }

        /// @brief 有効性を取得
        /// @brief Get enabled state
        bool isEnabled() const override {
            return enabled_;
        }

        /// @brief 有効性を設定
        /// @brief Set enabled state
        void setEnabled(bool enabled) {
            enabled_ = enabled;
        }
    };

}

#endif // !PAX_MAHOROBA_UI_LAYER_HPP
