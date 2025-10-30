/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_STRING_VIEWER_HPP
#define PAX_MAHOROBA_STRING_VIEWER_HPP

#include <limits>
#include <string>
#include <variant>
#include <vector>

#ifdef PAXS_USING_SIMULATOR
#include <PAX_MAHOROBA/Simulation/SimulationViewer.hpp>
#endif

#include <PAX_GRAPHICA/Key.hpp>
#include <PAX_GRAPHICA/RenderTexture.hpp>
#include <PAX_GRAPHICA/RoundRect.hpp>
#include <PAX_GRAPHICA/ScopedRenderState.hpp>
#include <PAX_GRAPHICA/ScopedRenderTarget.hpp>
#include <PAX_GRAPHICA/ScopedTransform2D.hpp>
#include <PAX_GRAPHICA/Shader.hpp>
#include <PAX_GRAPHICA/System.hpp>
#include <PAX_GRAPHICA/Texture.hpp>

#include <PAX_MAHOROBA/Calendar.hpp>
#include <PAX_MAHOROBA/CalendarRenderer.hpp>
#include <PAX_MAHOROBA/CalendarUILayout.hpp>
#include <PAX_MAHOROBA/DebugInfoPanel.hpp>
#include <PAX_MAHOROBA/LanguageFonts.hpp>
#include <PAX_MAHOROBA/MapView.hpp>
#include <PAX_MAHOROBA/Pulldown.hpp>
#include <PAX_MAHOROBA/ShadowRenderer.hpp>
#include <PAX_MAHOROBA/TimeControlPanel.hpp>

#include <PAX_SAPIENTICA/AppConfig.hpp>
#include <PAX_SAPIENTICA/Calendar/Date.hpp>
#include <PAX_SAPIENTICA/GraphicVisualizationList.hpp>
#include <PAX_SAPIENTICA/InputFile/KeyValueTSV.hpp>
#include <PAX_SAPIENTICA/Key/LanguageKeys.hpp>
#include <PAX_SAPIENTICA/Key/MenuKeys.hpp>
#include <PAX_SAPIENTICA/Language.hpp>
#include <PAX_SAPIENTICA/MurMur3.hpp>
#include <PAX_SAPIENTICA/TouchStateManager.hpp>


namespace paxs {

    class StringViewer {
    public:
        LanguageFonts language_fonts;

        std::vector<std::string> path_list = {
            "Data/Font/noto-sans-jp/NotoSansJP-Regular.otf",
            "Data/Font/noto-sans-jp/NotoSansJP-Regular.otf",
            "Data/Font/noto-sans-sc/NotoSansSC-Regular.otf",
            "Data/Font/noto-sans-sc/NotoSansSC-Regular.otf",
            "Data/Font/noto-sans-kr/NotoSansKR-Regular.otf",
            "Data/Font/noto-sans-jp/NotoSansJP-Regular.otf",
            "Data/Font/noto-sans-jp/NotoSansJP-Regular.otf",
            "Data/Font/noto-sans-jp/NotoSansJP-Regular.otf",
            "Data/Font/noto-sans-jp/NotoSansJP-Regular.otf",
            "Data/Font/noto-sans-jp/NotoSansJP-Regular.otf",
            "Data/Font/noto-sans-jp/NotoSansJP-Regular.otf",
            "Data/Font/noto-sans/NotoSans-Regular.ttf",
            "Data/Font/noto-sans/NotoSans-Regular.ttf",
            "Data/Font/noto-sans-jp/NotoSansJP-Regular.otf",
            "Data/Font/noto-sans-jp/NotoSansJP-Regular.otf",
            "Data/Font/noto-sans/NotoSans-Regular.ttf",
            "Data/Font/noto-sans/NotoSans-Regular.ttf",
            "Data/Font/noto-sans-jp/NotoSansJP-Regular.otf",
            "Data/Font/noto-sans-jp/NotoSansJP-Regular.otf",
            "Data/Font/noto-sans-ar/NotoNaskhArabic-VariableFont_wght.ttf",
            "Data/Font/noto-sans-ar/NotoNaskhArabic-VariableFont_wght.ttf",
            "Data/Font/noto-sans-jp/NotoSansJP-Regular.otf"
        };

        // プルダウンのフォントサイズ
        int pulldown_font_size =
#if defined(PAXS_USING_DXLIB) && defined(__ANDROID__)
            40;
#else
            24;
#endif

        int pulldown_font_buffer_thickness_size = 3; // プルダウンのフォント太さ

        int koyomi_font_size = 22; // 暦のフォントサイズ
        int koyomi_font_buffer_thickness_size = 3; // 暦のフォント太さ

        paxg::Font pin_font{};
        paxg::Font en_font{};

        std::size_t map_view_width_str_index;
        std::size_t map_view_center_x_str_index;
        std::size_t map_view_center_y_str_index;
        std::size_t map_view_center_lat_str_index;
        std::size_t xyz_tile_z_str_index;

        // UI の影
        paxg::RenderTexture shadow_texture{};
        paxg::RenderTexture internal_texture{};

        paxs::Pulldown pulldown;
        paxs::MenuBar menu_bar;

        paxs::KeyValueTSV<paxg::Texture> key_value_tsv;
        paxs::TimeControlPanel time_control_panel;
        paxs::CalendarUILayout ui_layout;

#ifdef PAXS_USING_SIMULATOR
        paxs::SimulationViewer simulation_viewer;
#endif

        void init(
            const SelectLanguage& select_language,
            const paxs::Language& language_text,
            const paxs::Language& simulation_text
        ) {
#ifndef PAXS_USING_SIMULATOR
            (void)simulation_text; // シミュレーター未使用時の警告を抑制
#endif
            language_fonts.setDefaultPath("Data/Font/noto-sans-sc/NotoSansSC-Regular.otf");
            setLanguageFont(pulldown_font_size, AppConfig::getInstance()->getRootPath(), pulldown_font_buffer_thickness_size);
            //koyomi_font = setFont(koyomi_font_size, AppConfig::getInstance()->getRootPath(), koyomi_font_buffer_thickness_size);
            map_view_width_str_index = (MurMur3::calcHash(25, "debug_magnification_power"));
            map_view_center_x_str_index = (MurMur3::calcHash(24, "debug_mercator_longitude"));
            map_view_center_y_str_index = (MurMur3::calcHash(23, "debug_mercator_latitude"));
            map_view_center_lat_str_index = (MurMur3::calcHash(14, "debug_latitude"));
            xyz_tile_z_str_index = (MurMur3::calcHash(17, "debug_xyz_tiles_z"));

            pulldown = paxs::Pulldown(&select_language, &language_text, paxs::LanguageKeys::ALL_LANGUAGE_HASHES, language_fonts, static_cast<std::uint_least8_t>(pulldown_font_size), static_cast<std::uint_least8_t>(pulldown_font_buffer_thickness_size), paxg::Vec2i{ 3000, 0 }, 0, true);
            pulldown.setPos(paxg::Vec2i{ static_cast<int>(paxg::Window::width() - pulldown.getRect().w()), 0 });

            menu_bar.add(&select_language, &language_text, paxs::MenuBarKeys::VIEW_MENU_HASHES, language_fonts, static_cast<std::uint_least8_t>(pulldown_font_size), static_cast<std::uint_least8_t>(pulldown_font_buffer_thickness_size), MurMur3::calcHash("view"));
            menu_bar.add(&select_language, &language_text, paxs::MenuBarKeys::FEATURE_MENU_HASHES, language_fonts, static_cast<std::uint_least8_t>(pulldown_font_size), static_cast<std::uint_least8_t>(pulldown_font_buffer_thickness_size), MurMur3::calcHash("place_names"));
            menu_bar.add(&select_language, &language_text, paxs::MenuBarKeys::MAP_MENU_HASHES, language_fonts, static_cast<std::uint_least8_t>(pulldown_font_size), static_cast<std::uint_least8_t>(pulldown_font_buffer_thickness_size), MurMur3::calcHash("map"));

            const std::string path = (AppConfig::getInstance()->getRootPath());
            // 暦の時間操作のアイコン
            key_value_tsv.input(path + "Data/MenuIcon/MenuIcons.tsv", [&](const std::string& value_) { return paxg::Texture{ path + value_ }; });

#ifdef PAXS_USING_SIMULATOR
            simulation_viewer.init(select_language, simulation_text, language_fonts, pulldown_font_size, pulldown_font_buffer_thickness_size, path);
#endif

            //koyomi_font = setFont(koyomi_font_size, AppConfig::getInstance()->getRootPath(), 2);

            pin_font = paxg::Font{ 14 /*, Typeface::Bold*/
                , (path + "Data/Font/noto-sans-jp/NotoSansJP-Regular.otf"), 2 };

            //pin_font = paxg::Font{ 18 /*, Typeface::Bold*/
            //    , (path + "Data/Font/noto-sans-jp/NotoSansJP-Regular.otf"), 2 };

            // 英語用フォント
            en_font = paxg::Font{ 20 /*, Typeface::Bold*/
    , (path + "Data/Font/noto-sans-jp/NotoSansJP-Regular.otf"), 2 };

            // 影
            shadow_texture = paxg::RenderTexture{ paxg::Window::Size(), paxg::ColorF{ 1.0, 0.0 } };
            internal_texture = paxg::RenderTexture{ shadow_texture.size() };
        }

        void update(
            MapView& map_view,
            const SelectLanguage& select_language,
            const paxs::Language& language_text,
#ifdef PAXS_USING_SIMULATOR
            std::unique_ptr<paxs::SettlementSimulator>& simulator, // コンパイル時の分岐により使わない場合あり
#endif
            paxs::TouchStateManager& tm_,
            paxs::KoyomiSiv3D& koyomi_siv,
            paxs::GraphicVisualizationList& visible
            ) {
            map_view.getCoordinate().toEquirectangularDegY();

            // 画像の拡大縮小の方式を設定
            const paxg::ScopedSamplerState sampler{ paxg::SamplerState::ClampLinear };

            // UIレイアウトを計算
            ui_layout.calculate(pulldown_font_size, koyomi_font_size, koyomi_siv.date_list.size(), time_control_panel.getHeight());

            // 影とパネルを描画
            paxs::ShadowRenderer::renderShadowWithPanels(
                shadow_texture,
                internal_texture,
                [&]() {
                    // 影の形状を描画
                    paxg::Rect{ 0, 0, static_cast<float>(paxg::Window::width()), static_cast<float>(pulldown.getRect().h()) }.draw(); // メニューバー

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

#ifdef PAXS_USING_SIMULATOR
            // シミュレーションのボタン
            simulation_viewer.update(simulator, tm_, koyomi_siv, ui_layout.koyomi_font_y + ui_layout.next_rect_start_y + 20, visible);
#endif

            if (visible[MurMur3::calcHash(8, "Calendar")] && visible[MurMur3::calcHash(2, "UI")]) {
                // カレンダーを描画
#ifdef PAXS_USING_SIMULATOR
                bool is_simulator_active = (simulator != nullptr);
#else
                bool is_simulator_active = false;
#endif
                paxs::CalendarRenderer::render(koyomi_siv, ui_layout, koyomi_font_size, koyomi_font_buffer_thickness_size, select_language, language_text, language_fonts, is_simulator_active);

                // 時間操作パネルを更新・描画
                const std::unordered_map<std::uint_least32_t, paxg::Texture>& texture_dictionary = key_value_tsv.get();
                time_control_panel.update(ui_layout.time_control_base_x, ui_layout.koyomi_font_y + ui_layout.time_control_base_y, texture_dictionary, tm_, koyomi_siv);
            }

            if (visible[MurMur3::calcHash(8, "Calendar")] && visible[MurMur3::calcHash(2, "UI")]) {
                int debug_start_y = ui_layout.getDebugStartY();
                // マップ情報とシミュレーション統計を描画
                paxs::DebugInfoPanel::renderMapAndSimulationInfo(
                    map_view, debug_start_y, koyomi_font_size, koyomi_font_buffer_thickness_size,
                    select_language, language_text, language_fonts, visible
#ifdef PAXS_USING_SIMULATOR
                    , simulator
#endif
                );
            }

            // メニューバー
            paxg::Rect{ 0, 0, static_cast<float>(paxg::Window::width()), static_cast<float>(pulldown.getRect().h()) }.draw(paxg::Color{ 243, 243, 243 });
#ifdef PAXS_USING_SIMULATOR            // シミュレーションのボタン
            simulation_viewer.drawPulldownBackground(simulator, visible);
#endif

            const std::unordered_map<std::uint_least32_t, paxg::Texture>& texture_dictionary = key_value_tsv.get();

            texture_dictionary.at(MurMur3::calcHash("texture_github")).resizedDraw(24, paxg::Vec2i{ paxg::Window::width() - 280, 3 });
            if (tm_.get(paxg::Rect(paxg::Window::width() - 280, 3, 28, 28).leftClicked())) {
                // Web ページをブラウザで開く
                paxg::System::launchBrowser("https://github.com/AsPJT/PAX_SAPIENTICA");
            }

#ifdef PAXS_USING_SIMULATOR
            // シミュレーションのボタン
            simulation_viewer.drawPulldown(simulator, visible);
#endif
            pulldown.draw(); // 言語選択
            menu_bar.draw(); // 左上メニューバー

            if (visible[MurMur3::calcHash(8, "Calendar")] && visible[MurMur3::calcHash(2, "UI")]
#ifdef PAXS_USING_SIMULATOR
                && simulator == nullptr
#endif
                ) {
                int debug_start_y = ui_layout.getDebugStartY();
                // 考古学的遺物の型式情報を描画
                paxs::DebugInfoPanel::renderArchaeologicalInfo(
                    koyomi_siv, ui_layout, debug_start_y, koyomi_font_size, koyomi_font_buffer_thickness_size,
                    select_language, language_text, language_fonts
                );
            }
        }
    private:
        void setLanguageFont(
            const int font_size_,
            const std::string& path,
            const int buffer_thickness) {
            for (std::size_t i = 0; i < path_list.size(); ++i) {
                language_fonts.add(
                    (path + path_list[i]),
                    paxs::LanguageKeys::ALL_LANGUAGE_HASHES[i],
                    static_cast<std::uint_least8_t>(font_size_),
                    static_cast<std::uint_least8_t>(buffer_thickness)
                );
            }
        }

    };

}

#endif // !PAX_MAHOROBA_STRING_VIEWER_HPP
