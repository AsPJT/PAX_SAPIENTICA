/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_UI_MANAGER_HPP
#define PAX_MAHOROBA_UI_MANAGER_HPP

#include <limits>
#include <string>
#include <variant>
#include <vector>

#ifdef PAXS_USING_SIMULATOR
#include <PAX_MAHOROBA/UI/SimulationPanel.hpp>
#endif

#include <PAX_GRAPHICA/Key.hpp>
#include <PAX_GRAPHICA/RenderTexture.hpp>
#include <PAX_GRAPHICA/RoundRect.hpp>
#include <PAX_GRAPHICA/ScopedRenderState.hpp>
#include <PAX_GRAPHICA/Shader.hpp>
#include <PAX_GRAPHICA/System.hpp>
#include <PAX_GRAPHICA/Texture.hpp>

#include <PAX_MAHOROBA/UI/Calendar/CalendarRenderer.hpp>
#include <PAX_MAHOROBA/UI/Calendar/CalendarUILayout.hpp>
#include <PAX_MAHOROBA/UI/DebugInfoPanel.hpp>
#include <PAX_MAHOROBA/UI/HeaderPanel.hpp>
#include <PAX_MAHOROBA/UI/IUIWidget.hpp>
#include <PAX_MAHOROBA/Rendering/LanguageFonts.hpp>
#include <PAX_MAHOROBA/Map/MapViewport.hpp>
#include <PAX_MAHOROBA/Rendering/ShadowRenderer.hpp>
#include <PAX_MAHOROBA/Rendering/FontManager.hpp>
#include <PAX_MAHOROBA/UI/TimeControlPanel.hpp>

#include <PAX_SAPIENTICA/AppConfig.hpp>
#include <PAX_SAPIENTICA/Calendar/Date.hpp>
#include <PAX_SAPIENTICA/Calendar/Koyomi.hpp>
#include <PAX_SAPIENTICA/FeatureVisibilityManager.hpp>
#include <PAX_SAPIENTICA/FontConfig.hpp>
#include <PAX_SAPIENTICA/InputFile/KeyValueTSV.hpp>
#include <PAX_SAPIENTICA/Key/LanguageKeys.hpp>
#include <PAX_SAPIENTICA/Key/MenuKeys.hpp>
#include <PAX_SAPIENTICA/Language.hpp>
#include <PAX_SAPIENTICA/MurMur3.hpp>
#include <PAX_SAPIENTICA/InputStateManager.hpp>


namespace paxs {

    // UIの統合管理を担当するクラス
    class UIManager {
    private:
        FontManager* font_manager_ = nullptr; // 文字表示専用クラス（依存性注入）

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
        paxs::TimeControlPanel time_control_panel;
        paxs::CalendarUILayout ui_layout;
        paxs::CalendarRenderer calendar_renderer;  // カレンダー描画
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
            // FontManagerの参照を保存（初期化はGraphicsManagerで実施済み）
            font_manager_ = &font_manager;

            // CalendarRendererを初期化
            calendar_renderer.init(font_manager_->language_fonts);

            // DebugInfoPanelを初期化
            debug_info_panel.init(font_manager_->language_fonts);
            map_viewport_width_str_index = (MurMur3::calcHash(25, "debug_magnification_power"));
            map_viewport_center_x_str_index = (MurMur3::calcHash(24, "debug_mercator_longitude"));
            map_viewport_center_y_str_index = (MurMur3::calcHash(23, "debug_mercator_latitude"));
            map_viewport_center_lat_str_index = (MurMur3::calcHash(14, "debug_latitude"));
            xyz_tile_z_str_index = (MurMur3::calcHash(17, "debug_xyz_tiles_z"));

            // HeaderPanelを初期化
            header_panel.init(&select_language, &language_text, font_manager_->language_fonts, static_cast<std::uint_least8_t>(FontConfig::PULLDOWN_FONT_SIZE), static_cast<std::uint_least8_t>(FontConfig::PULLDOWN_FONT_BUFFER_THICKNESS));

            // 暦の時間操作のアイコン
            key_value_tsv.input(paxs::AppConfig::getInstance()->getRootPath() + "Data/MenuIcon/MenuIcons.tsv", [&](const std::string& value_) { return paxg::Texture{ value_ }; });

#ifdef PAXS_USING_SIMULATOR
            simulation_panel.init(select_language, simulation_text, font_manager_->language_fonts, FontConfig::PULLDOWN_FONT_SIZE, FontConfig::PULLDOWN_FONT_BUFFER_THICKNESS);
#endif

            // 影
            shadow_texture = paxg::RenderTexture{ paxg::Window::Size(), paxg::ColorF{ 1.0, 0.0 } };
            internal_texture = paxg::RenderTexture{ shadow_texture.size() };

            // IUIWidget を実装したウィジェットを登録
            widgets.clear();
            widgets.push_back(&time_control_panel);
            widgets.push_back(&calendar_renderer);
            widgets.push_back(&debug_info_panel);
#ifdef PAXS_USING_SIMULATOR
            widgets.push_back(&simulation_panel);
#endif
            widgets.push_back(&header_panel);

            // TimeControlPanelに必要な参照を設定
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

        void update(
            MapViewport& map_viewport,
            const SelectLanguage& select_language,
            const paxs::Language& language_text,
#ifdef PAXS_USING_SIMULATOR
            std::unique_ptr<paxs::SettlementSimulator>& simulator, // コンパイル時の分岐により使わない場合あり
#endif
            paxs::InputStateManager& input_state_manager,
            paxs::Koyomi& koyomi,
            paxs::FeatureVisibilityManager& visible
            ) {
            map_viewport.getCoordinate().toEquirectangularDegY();

            // 画像の拡大縮小の方式を設定
            const paxg::ScopedSamplerState sampler{ paxg::SamplerState::ClampLinear };

            // UIレイアウトを計算
            ui_layout.calculate(FontConfig::PULLDOWN_FONT_SIZE, FontConfig::KOYOMI_FONT_SIZE, koyomi.date_list.size(), time_control_panel.getHeight());

            // 影とパネルを描画
#ifdef PAXS_USING_SIV3D
            // Siv3D: Use high-quality shadow renderer with Gaussian blur
            paxs::ShadowRenderer::renderShadowWithPanels(
                shadow_texture,
                internal_texture,
                [&]() {
                    // 影の形状を描画
                    paxg::Rect{ 0, 0, static_cast<float>(paxg::Window::width()), static_cast<float>(header_panel.getHeight()) }.draw(); // メニューバー

                    if (visible[MurMur3::calcHash(8, "Calendar")] && visible[MurMur3::calcHash(2, "UI")]) {
                        paxg::RoundRect{ ui_layout.rect_start_x, ui_layout.koyomi_font_y - 15, ui_layout.rect_len_x, ui_layout.next_rect_start_y, 10 }.draw();
                        paxg::RoundRect{ ui_layout.rect_start_x, ui_layout.koyomi_font_y + ui_layout.next_rect_start_y + 5, ui_layout.rect_len_x, ui_layout.next_rect_end_y, 10 }.draw();
                    }
                },
                [&]() {
                    // パネル本体を描画
                    if (visible[MurMur3::calcHash(8, "Calendar")] && visible[MurMur3::calcHash(2, "UI")]) {
                        paxg::RoundRect{ ui_layout.rect_start_x, ui_layout.koyomi_font_y - 15, ui_layout.rect_len_x, ui_layout.next_rect_start_y, 10 }.draw(paxg::Color{ 255, 255, 255 });
                        paxg::RoundRect{ ui_layout.rect_start_x, ui_layout.koyomi_font_y + ui_layout.next_rect_start_y + 5, ui_layout.rect_len_x, ui_layout.next_rect_end_y, 10 }.draw(paxg::Color{ 255, 255, 255 });
                    }
                }
            );
#else
            // SFML/DxLib: Use simple shadow with drawShadow method
            // メニューバーの影
            paxg::Rect{ 0, 0, static_cast<float>(paxg::Window::width()), static_cast<float>(header_panel.getHeight()) }.drawShadow({ 1, 1 }, 4, 1).draw();

            if (visible[MurMur3::calcHash(8, "Calendar")] && visible[MurMur3::calcHash(2, "UI")]) {
                // カレンダーパネルの影と描画
                paxg::RoundRect{ ui_layout.rect_start_x, ui_layout.koyomi_font_y - 15, ui_layout.rect_len_x, ui_layout.next_rect_start_y, 10 }.drawShadow({ 1, 1 }, 4, 1).draw(paxg::Color{ 255, 255, 255 });
                paxg::RoundRect{ ui_layout.rect_start_x, ui_layout.koyomi_font_y + ui_layout.next_rect_start_y + 5, ui_layout.rect_len_x, ui_layout.next_rect_end_y, 10 }.drawShadow({ 1, 1 }, 4, 1).draw(paxg::Color{ 255, 255, 255 });
            }
#endif

#ifdef PAXS_USING_SIMULATOR
            // シミュレーションのボタン
            simulation_panel.setReferences(simulator, input_state_manager, koyomi, visible, ui_layout.koyomi_font_y + ui_layout.next_rect_start_y + 20);
#endif

            if (visible[MurMur3::calcHash(8, "Calendar")] && visible[MurMur3::calcHash(2, "UI")]) {
                // カレンダーを描画
#ifdef PAXS_USING_SIMULATOR
                bool is_simulator_active = (simulator != nullptr);
#else
                bool is_simulator_active = false;
#endif
                // CalendarRendererのレンダリングパラメータを設定
                calendar_renderer.setRenderParams(koyomi, ui_layout, FontConfig::KOYOMI_FONT_SIZE, FontConfig::KOYOMI_FONT_BUFFER_THICKNESS, select_language, language_text, is_simulator_active);
                calendar_renderer.setVisible(true);

                // 時間操作パネルを更新・描画
                const paxs::UnorderedMap<std::uint_least32_t, paxg::Texture>& texture_dictionary = key_value_tsv.get();
                time_control_panel.setReferences(texture_dictionary, koyomi);
                time_control_panel.setPos(paxg::Vec2i{ui_layout.time_control_base_x, ui_layout.koyomi_font_y + ui_layout.time_control_base_y});
                time_control_panel.setVisible(true);
            } else {
                calendar_renderer.setVisible(false);
                time_control_panel.setVisible(false);
            }

            if (visible[MurMur3::calcHash(8, "Calendar")] && visible[MurMur3::calcHash(2, "UI")]) {
                int debug_start_y = ui_layout.getDebugStartY();
                // マップ情報とシミュレーション統計を描画
                debug_info_panel.setVisible(true);
                debug_info_panel.renderMapAndSimulationInfo(
                    map_viewport, debug_start_y, FontConfig::KOYOMI_FONT_SIZE, FontConfig::KOYOMI_FONT_BUFFER_THICKNESS,
                    select_language, language_text, visible
#ifdef PAXS_USING_SIMULATOR
                    , simulator
#endif
                );
            } else {
                debug_info_panel.setVisible(false);
            }

#ifdef PAXS_USING_SIMULATOR
            simulation_panel.drawPulldownBackground(simulator, visible);
#endif

            const paxs::UnorderedMap<std::uint_least32_t, paxg::Texture>& texture_dictionary = key_value_tsv.get();

            texture_dictionary.at(MurMur3::calcHash("texture_github")).resizedDraw(24, paxg::Vec2i{ paxg::Window::width() - 280, 3 });
            if (input_state_manager.get(paxg::Rect(static_cast<float>(paxg::Window::width() - 280), 3.0f, 28.0f, 28.0f).leftClicked())) {
                // Web ページをブラウザで開く
                paxg::System::launchBrowser("https://github.com/AsPJT/PAX_SAPIENTICA");
            }

            // IUIWidget を実装したウィジェットを更新
            for (auto* widget : widgets) {
                if (widget) {
                    widget->update(input_state_manager);
                }
            }

            // IUIWidget を実装したウィジェットを描画
            for (auto* widget : widgets) {
                if (widget) {
                    widget->draw();
                }
            }

            if (visible[MurMur3::calcHash(8, "Calendar")] && visible[MurMur3::calcHash(2, "UI")]
#ifdef PAXS_USING_SIMULATOR
                && simulator == nullptr
#endif
                ) {
                int debug_start_y = ui_layout.getDebugStartY();
                // 考古学的遺物の型式情報を描画
                debug_info_panel.renderArchaeologicalInfo(
                    koyomi, ui_layout, debug_start_y, FontConfig::KOYOMI_FONT_SIZE, FontConfig::KOYOMI_FONT_BUFFER_THICKNESS,
                    select_language, language_text
                );
            }
        }
    };

}

#endif // !PAX_MAHOROBA_UI_MANAGER_HPP
