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
#endif

#include <PAX_GRAPHICA/RenderTexture.hpp>
#include <PAX_GRAPHICA/ScopedRenderState.hpp>
#include <PAX_GRAPHICA/System.hpp>
#include <PAX_GRAPHICA/Texture.hpp>

#include <PAX_MAHOROBA/UI/Calendar/CalendarPanel.hpp>
#include <PAX_MAHOROBA/UI/UILayout.hpp>
#include <PAX_MAHOROBA/UI/DebugInfoPanel.hpp>
#include <PAX_MAHOROBA/UI/HeaderPanel.hpp>
#include <PAX_MAHOROBA/UI/IUIWidget.hpp>
#include <PAX_MAHOROBA/Map/MapViewport.hpp>
#include <PAX_MAHOROBA/Rendering/FontManager.hpp>
#include <PAX_MAHOROBA/Input/IInputHandler.hpp>
#include <PAX_MAHOROBA/Rendering/IRenderable.hpp>

#include <PAX_SAPIENTICA/AppConfig.hpp>
#include <PAX_SAPIENTICA/Calendar/Date.hpp>
#include <PAX_SAPIENTICA/Calendar/Koyomi.hpp>
#include <PAX_SAPIENTICA/FeatureVisibilityManager.hpp>
#include <PAX_SAPIENTICA/FontConfig.hpp>
#include <PAX_SAPIENTICA/InputFile/KeyValueTSV.hpp>
#include <PAX_SAPIENTICA/InputStateManager.hpp>
#include <PAX_SAPIENTICA/Language.hpp>
#include <PAX_SAPIENTICA/MurMur3.hpp>


namespace paxs {

    /// @brief UIレイヤーの統合管理を担当するクラス
    /// @brief Integrated management class for UI layer
    ///
    /// IRenderable と IInputHandler を継承し、レイヤーベースシステムに対応します。
    /// 子ウィジェットへの処理委譲を行います。
    /// Inherits IRenderable and IInputHandler to support layer-based system.
    /// Delegates processing to child widgets.
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

        std::size_t map_viewport_width_str_index;
        std::size_t map_viewport_center_x_str_index;
        std::size_t map_viewport_center_y_str_index;
        std::size_t map_viewport_center_lat_str_index;
        std::size_t xyz_tile_z_str_index;

        // UI の影
        paxg::RenderTexture shadow_texture{};
        paxg::RenderTexture internal_texture{};
        int size_change_count_ = 0;

        paxs::HeaderPanel header_panel;  // ヘッダーパネル（メニューバー + 言語選択）

        // IUIWidget を実装したウィジェットの統合管理
        std::vector<IUIWidget*> widgets;

        paxs::KeyValueTSV<paxg::Texture> key_value_tsv;
        paxs::UILayout ui_layout;
        paxs::CalendarPanel calendar_panel;        // カレンダーパネル（時間操作 + カレンダー表示）
        paxs::DebugInfoPanel debug_info_panel;     // デバッグ情報パネル

#ifdef PAXS_USING_SIMULATOR
        paxs::SimulationPanel simulation_panel;
#endif

    public:
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
            key_value_tsv.input(paxs::AppConfig::getInstance()->getRootPath() + "Data/MenuIcon/MenuIcons.tsv", [&](const std::string& value_) { return paxg::Texture{ value_ }; });

            // HeaderPanelにGitHubアイコンを設定
            header_panel.setGitHubTexture(&key_value_tsv.get().at(MurMur3::calcHash("texture_github")));

#ifdef PAXS_USING_SIMULATOR
            simulation_panel.init(select_language, simulation_text, font_manager_->getLanguageFonts());
#endif

            // 影
            shadow_texture = paxg::RenderTexture{ paxg::Window::Size(), paxg::ColorF{ 1.0, 0.0 } };
            internal_texture = paxg::RenderTexture{ shadow_texture.size() };

            // IUIWidget を実装したウィジェットを登録
            widgets.clear();
            widgets.push_back(&calendar_panel);
            widgets.push_back(&debug_info_panel);
#ifdef PAXS_USING_SIMULATOR
            widgets.push_back(&simulation_panel);
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
                visible.set(MurMur3::calcHash("Calendar"), view_menu->getIsItems(std::size_t(0)));
                visible.set(MurMur3::calcHash("Map"), view_menu->getIsItems(std::size_t(1)));
                visible.set(MurMur3::calcHash("UI"), view_menu->getIsItems(std::size_t(2)));
                visible.set(MurMur3::calcHash("Simulation"), view_menu->getIsItems(std::size_t(3)));
                visible.set(MurMur3::calcHash("License"), view_menu->getIsItems(std::size_t(4)));
                visible.set(MurMur3::calcHash("Debug"), view_menu->getIsItems(std::size_t(5)));
                visible.set(MurMur3::calcHash("3D"), view_menu->getIsItems(std::size_t(6)));
            }

            // Place Names メニューの状態を同期
            auto* place_names_menu = header_panel.getMenuBar().getMenuItem(MurMur3::calcHash("place_names"));
            if (place_names_menu) {
                visible.set(MurMur3::calcHash("place_name"), place_names_menu->getIsItems(std::size_t(0)));
                visible.set(MurMur3::calcHash("site"), place_names_menu->getIsItems(std::size_t(1)));
                visible.set(MurMur3::calcHash("tumulus"), place_names_menu->getIsItems(std::size_t(2)));
                visible.set(MurMur3::calcHash("dolmen"), place_names_menu->getIsItems(std::size_t(3)));
                visible.set(MurMur3::calcHash("kamekanbo"), place_names_menu->getIsItems(std::size_t(4)));
                visible.set(MurMur3::calcHash("stone_coffin"), place_names_menu->getIsItems(std::size_t(5)));
                visible.set(MurMur3::calcHash("doken"), place_names_menu->getIsItems(std::size_t(6)));
                visible.set(MurMur3::calcHash("dotaku"), place_names_menu->getIsItems(std::size_t(7)));
                visible.set(MurMur3::calcHash("bronze_mirror"), place_names_menu->getIsItems(std::size_t(8)));
                visible.set(MurMur3::calcHash("human_bone"), place_names_menu->getIsItems(std::size_t(9)));
                visible.set(MurMur3::calcHash("mtdna"), place_names_menu->getIsItems(std::size_t(10)));
                visible.set(MurMur3::calcHash("ydna"), place_names_menu->getIsItems(std::size_t(11)));
            }

            // Map メニューの状態を同期
            auto* map_menu = header_panel.getMenuBar().getMenuItem(MurMur3::calcHash("map"));
            if (map_menu) {
                visible.set(MurMur3::calcHash("menu_bar_map_base"), map_menu->getIsItems(std::size_t(0)));
                visible.set(MurMur3::calcHash("menu_bar_map_land_and_sea"), map_menu->getIsItems(std::size_t(1)));
                visible.set(MurMur3::calcHash("menu_bar_map_land_and_water"), map_menu->getIsItems(std::size_t(2)));
                visible.set(MurMur3::calcHash("menu_bar_map_soil"), map_menu->getIsItems(std::size_t(3)));
                visible.set(MurMur3::calcHash("menu_bar_map_soil_temperature"), map_menu->getIsItems(std::size_t(4)));
                visible.set(MurMur3::calcHash("menu_bar_map_ryosei_country"), map_menu->getIsItems(std::size_t(5)));
                visible.set(MurMur3::calcHash("menu_bar_map_ryosei_line"), map_menu->getIsItems(std::size_t(6)));
                visible.set(MurMur3::calcHash("menu_bar_map_slope"), map_menu->getIsItems(std::size_t(7)));
                visible.set(MurMur3::calcHash("menu_bar_map_lakes_and_rivers1"), map_menu->getIsItems(std::size_t(8)));
                visible.set(MurMur3::calcHash("menu_bar_map_lakes_and_rivers2"), map_menu->getIsItems(std::size_t(9)));
                visible.set(MurMur3::calcHash("menu_bar_map_line1"), map_menu->getIsItems(std::size_t(10)));
                visible.set(MurMur3::calcHash("menu_bar_map_line2"), map_menu->getIsItems(std::size_t(11)));
            }
        }

        /// @brief ウィンドウサイズ変更時のテクスチャ再初期化
        void handleWindowResize() {
            if (size_change_count_ < 1) {
                shadow_texture = paxg::RenderTexture{ paxg::Window::Size(), paxg::ColorF{ 1.0, 0.0 } };
                internal_texture = paxg::RenderTexture{ shadow_texture.size() };
            }
            if (size_change_count_ >= 100) size_change_count_ = 100;
            ++size_change_count_;
        }

        void resetSizeChangeCount() {
            size_change_count_ = 0;
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

            // 各パネルに影テクスチャを設定
            header_panel.setShadowTextures(shadow_texture, internal_texture);
#ifdef PAXS_USING_SIMULATOR
            simulation_panel.setShadowTextures(shadow_texture, internal_texture);
            simulation_panel.setReferences(simulator, input_state_manager, koyomi, visible,
                map_viewport, font_manager_->getLanguageFonts(), select_language, language_text,
                ui_layout.koyomi_font_y + ui_layout.next_rect_start_y + 20);
#endif

            // CalendarPanelの可視性と設定
            if (visible[MurMur3::calcHash(8, "Calendar")] && visible[MurMur3::calcHash(2, "UI")]) {
#ifdef PAXS_USING_SIMULATOR
                bool is_simulator_active = (simulator != nullptr);
#else
                bool is_simulator_active = false;
#endif
                calendar_panel.setShadowTextures(shadow_texture, internal_texture);
                calendar_panel.setLayout(ui_layout, key_value_tsv.get());
                calendar_panel.setCalendarParams(koyomi, select_language, language_text, is_simulator_active);
                calendar_panel.setTimeControlParams(koyomi);
                calendar_panel.setVisible(true);
            } else {
                calendar_panel.setVisible(false);
            }

            // DebugInfoPanelの可視性と設定
            if (visible[MurMur3::calcHash(8, "Calendar")] && visible[MurMur3::calcHash(2, "UI")]) {
                int debug_start_y = ui_layout.getDebugStartY();
                debug_info_panel.setShadowTextures(shadow_texture, internal_texture);
                debug_info_panel.setBackgroundRect(
                    ui_layout.rect_start_x,
                    debug_start_y - 15,
                    ui_layout.rect_len_x,
                    ui_layout.next_rect_end_y
                );
                debug_info_panel.setVisible(true);
            } else {
                debug_info_panel.setVisible(false);
            }

            // ウィジェットを更新（入力処理）
            for (auto* widget : widgets) {
                if (widget) {
                    widget->update(input_state_manager);
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

        /// @brief 既存のupdate()メソッド（後方互換性のため維持）
        /// @brief Existing update() method (kept for backward compatibility)
        void update(
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
            updateData(map_viewport, select_language, language_text,
#ifdef PAXS_USING_SIMULATOR
                simulator,
#endif
                input_state_manager, koyomi, visible);
            render();
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

            // マップ情報とシミュレーション統計を描画
            if (visible[MurMur3::calcHash(8, "Calendar")] && visible[MurMur3::calcHash(2, "UI")]) {
                int debug_start_y = ui_layout.getDebugStartY();
                debug_info_panel.renderMapAndSimulationInfo(
                    map_viewport, debug_start_y, select_language, language_text, visible
#ifdef PAXS_USING_SIMULATOR
                    , *cached_simulator_
#endif
                );
            }

#ifdef PAXS_USING_SIMULATOR
            if (cached_simulator_ && *cached_simulator_) {
                simulation_panel.drawPulldownBackground(*cached_simulator_, visible);
            }
#endif

            // ウィジェットを描画
            for (auto* widget : widgets) {
                if (widget) {
                    widget->draw();
                }
            }

            // 考古学的遺物の型式情報を描画
            if (visible[MurMur3::calcHash(8, "Calendar")] && visible[MurMur3::calcHash(2, "UI")]
#ifdef PAXS_USING_SIMULATOR
                && (!cached_simulator_ || *cached_simulator_ == nullptr)
#endif
                ) {
                int debug_start_y = ui_layout.getDebugStartY();
                debug_info_panel.renderArchaeologicalInfo(
                    koyomi, ui_layout, debug_start_y, select_language, language_text
                );
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

        // IInputHandler の実装
        // IInputHandler implementation

        /// @brief 入力処理（子ウィジェットに委譲）
        /// @brief Handle input (delegate to child widgets)
        /// @param event 入力イベント / Input event
        /// @return 処理した場合true / true if handled
        bool handleInput(const InputEvent& event) override {
            if (!enabled_ || !visible_) return false;

            // 座標に依存しないイベント（キーボード、マウスホイール、ウィンドウイベント）はスキップ
            // Skip coordinate-independent events (Keyboard, MouseWheel, Window events)
            if (event.type == InputEventType::Keyboard ||
                event.type == InputEventType::MouseWheel ||
                event.type == InputEventType::WindowResize ||
                event.type == InputEventType::WindowFocus) {
                return false;
            }

            // 子ウィジェットに順番に入力イベントを渡す（マウス/タッチのみ）
            // Pass input event to child widgets in order (Mouse/Touch only)
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
            // HeaderPanel always attempts to process (menu bar is fixed at top of screen)
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
            // Return true if any child widget is hit
            for (const auto* widget : widgets) {
                if (widget && widget->isVisible() && widget->isEnabled()) {
                    if (widget->hitTest(x, y)) {
                        return true;
                    }
                }
            }

            // HeaderPanelのヒットテスト
            // HeaderPanel hit test
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
