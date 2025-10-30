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
#include <PAX_MAHOROBA/LanguageFonts.hpp>
#include <PAX_MAHOROBA/MapView.hpp>
#include <PAX_MAHOROBA/Pulldown.hpp>
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

            // 暦の位置
            int koyomi_font_y = pulldown_font_size + 43;
            int koyomi_font_en_y = pulldown_font_size + 43;

            int koyomi_font_x = paxg::Window::width() - 270;
            int koyomi_font_en_x = koyomi_font_x - 20;
            int rect_start_x = koyomi_font_en_x - 165;
            int rect_len_x = paxg::Window::width() - rect_start_x - 15;

            int koyomi_height = static_cast<int>(koyomi_siv.date_list.size()) * (koyomi_font_size * 4 / 3); // 暦の縦の幅

            // 時間操作パネルの位置とサイズ
            int time_control_base_x = rect_len_x - 10;
            int time_control_base_y = koyomi_height + 40;
            int time_control_height = time_control_panel.getHeight();

            int next_rect_start_y = time_control_base_y + time_control_height + 50;
            int next_rect_end_y = 280;

            // 影を作る図形を shadow_texture に描く
            {
                const paxg::ScopedRenderTarget target{ shadow_texture, paxg::ColorF{ 1.0, 0.0 } };
                const paxg::ScopedBlendState blend{ paxg::BlendState::MaxAlpha };
                const paxg::ScopedTransform2D transform{ 3.0, 3.0 };
                paxg::Rect{ 0, 0, static_cast<float>(paxg::Window::width()), static_cast<float>(pulldown.getRect().h()) }.draw(); // メニューバー

                if (visible[MurMur3::calcHash(8, "Calendar")] && visible[MurMur3::calcHash(2, "UI")]) {
                    paxg::RoundRect{ rect_start_x, koyomi_font_y - 15, rect_len_x, next_rect_start_y, 10 }.draw();
                    paxg::RoundRect{ rect_start_x, koyomi_font_y + next_rect_start_y + 5, rect_len_x, next_rect_end_y, 10 }.draw();
                }
            }
                // shadow_texture を 2 回ガウスぼかし
            {
                paxg::Shader::GaussianBlur(shadow_texture, internal_texture, shadow_texture);
                paxg::Shader::GaussianBlur(shadow_texture, internal_texture, shadow_texture);
            }
            shadow_texture.draw(paxg::ColorF{ 0.0, 0.5 });

            if (visible[MurMur3::calcHash(8, "Calendar")] && visible[MurMur3::calcHash(2, "UI")]) {
                // 暦表示の範囲に白背景を追加
                paxg::RoundRect{ rect_start_x, koyomi_font_y - 15, rect_len_x, next_rect_start_y, 10 }.draw(paxg::Color{ 255, 255, 255 });
                paxg::RoundRect{ rect_start_x, koyomi_font_y + next_rect_start_y + 5, rect_len_x, next_rect_end_y, 10 }.draw(paxg::Color{ 255, 255, 255 });
            }

#ifdef PAXS_USING_SIMULATOR
            // シミュレーションのボタン
            simulation_viewer.update(simulator, tm_, koyomi_siv, koyomi_font_y + next_rect_start_y + 20, visible);
#endif

            if (visible[MurMur3::calcHash(8, "Calendar")] && visible[MurMur3::calcHash(2, "UI")]) {
                // 暦の表示（日本語）
                if (
                    select_language.cgetKey() == MurMur3::calcHash("ja-JP")
                    || select_language.cgetKey() == MurMur3::calcHash("zh-TW")
                    || select_language.cgetKey() == MurMur3::calcHash("zh-CN")
                    ) {
                    for (std::size_t i = 0; i < koyomi_siv.date_list.size(); ++i) {

                        cal::DateOutputType output_type = cal::DateOutputType::name_and_value;
                        std::visit([&](const auto& x) { output_type = x.getDateOutputType(); }, koyomi_siv.date_list[i].date);

                        int date_year = 0;
                        int date_month = 0;
                        int date_day = 0;
                        bool date_lm = false;

                        // 暦の読み方を返す
                        const std::string* const text_str = language_text.getStringPtr(
                            koyomi_siv.date_list[i].calendar_name_key,
                            select_language.cgetKey()
                        );
                        if (text_str == nullptr) continue;

                        // 暦描画フォントを指定
                        paxg::Font* one_font = language_fonts.getAndAdd(select_language.cgetKey(), static_cast<std::uint_least8_t>(koyomi_font_size), static_cast<std::uint_least8_t>(koyomi_font_buffer_thickness_size));
                        if (one_font == nullptr) continue;

                        switch (output_type) {
                        case paxs::cal::DateOutputType::name_and_ymd:
                            std::visit([&](const auto& x) {
                                date_year = int(x.cgetYear());
                                date_month = int(x.cgetMonth());
                                date_day = int(x.cgetDay());
                                date_lm = x.isLeapMonth();
                                }, koyomi_siv.date_list[i].date);
                            if (date_day <= 0 || date_month <= 0) break;

                            (*one_font).drawTopRight(*text_str
                                ,
                                paxg::Vec2i(static_cast<int>(koyomi_font_x), static_cast<int>(koyomi_font_y + i * (koyomi_font_size * 4 / 3))), paxg::Color(0, 0, 0));
                            (*one_font).drawTopRight(reinterpret_cast<const char*>(u8"年"), paxg::Vec2i(static_cast<int>(int(120 * koyomi_font_size / 30.0) + koyomi_font_x), static_cast<int>(koyomi_font_y + i * (koyomi_font_size * 4 / 3))), paxg::Color(0, 0, 0));
                            (*one_font).drawTopRight(reinterpret_cast<const char*>(u8"月"), paxg::Vec2i(static_cast<int>(int(220 * koyomi_font_size / 30.0) + koyomi_font_x), static_cast<int>(koyomi_font_y + i * (koyomi_font_size * 4 / 3))), paxg::Color(0, 0, 0));
                            (*one_font).drawTopRight(reinterpret_cast<const char*>(u8"日"), paxg::Vec2i(static_cast<int>(int(300 * koyomi_font_size / 30.0) + koyomi_font_x), static_cast<int>(koyomi_font_y + i * (koyomi_font_size * 4 / 3))), paxg::Color(0, 0, 0));

                            (*one_font).drawTopRight(std::to_string(date_year), paxg::Vec2i(static_cast<int>(int(85 * koyomi_font_size / 30.0) + koyomi_font_x), static_cast<int>(koyomi_font_y + i * (koyomi_font_size * 4 / 3))), paxg::Color(0, 0, 0));
                            (*one_font).drawTopRight(std::to_string(date_month), paxg::Vec2i(static_cast<int>(int(190 * koyomi_font_size / 30.0) + koyomi_font_x), static_cast<int>(koyomi_font_y + i * (koyomi_font_size * 4 / 3))), paxg::Color(0, 0, 0));
                            (*one_font).drawTopRight(std::to_string(date_day), paxg::Vec2i(static_cast<int>(int(270 * koyomi_font_size / 30.0) + koyomi_font_x), static_cast<int>(koyomi_font_y + i * (koyomi_font_size * 4 / 3))), paxg::Color(0, 0, 0));
                            if (date_lm) {
                                (*one_font).drawTopRight(reinterpret_cast<const char*>(u8"閏"), paxg::Vec2i(static_cast<int>((
                                    (date_month < 10) ? int(167 * koyomi_font_size / 30.0) + koyomi_font_x : int(152 * koyomi_font_size / 30.0) + koyomi_font_x
                                    )), static_cast<int>(koyomi_font_y + i * (koyomi_font_size * 4 / 3))), paxg::Color(0, 0, 0));
                            }
                            break;
                        case paxs::cal::DateOutputType::name_and_value:
                            (*one_font).drawTopRight(*text_str
                                , paxg::Vec2i(static_cast<int>(koyomi_font_x), static_cast<int>(koyomi_font_y + i * (koyomi_font_size * 4 / 3))), paxg::Color(0, 0, 0));
                            std::visit([&](const auto& x) {
                                date_day = int(x.cgetDay());
                                }, koyomi_siv.date_list[i].date);
                            (*one_font).drawTopRight(std::to_string(date_day), paxg::Vec2i(static_cast<int>(int(300 * koyomi_font_size / 30.0) + koyomi_font_x), static_cast<int>(koyomi_font_y + i * (koyomi_font_size * 4 / 3))), paxg::Color(0, 0, 0));
                            break;
                        default:
                            break;
                        }
                    }
                }
                else {
                    // 暦の表示（英語）
                    for (std::size_t i = 0; i < koyomi_siv.date_list.size(); ++i) {

                        cal::DateOutputType output_type = cal::DateOutputType::name_and_value;
                        std::visit([&](const auto& x) { output_type = x.getDateOutputType(); }, koyomi_siv.date_list[i].date);

                        int date_year = 0;
                        int date_month = 0;
                        int date_day = 0;
                        bool date_lm = false;
                        const int en_cal_name_pos_x = 85;

                        // 暦描画フォントを指定
                        paxg::Font* one_font = language_fonts.getAndAdd(select_language.cgetKey(), static_cast<std::uint_least8_t>(koyomi_font_size), static_cast<std::uint_least8_t>(koyomi_font_buffer_thickness_size));
                        if (one_font == nullptr) continue;
                        // 年描画フォントを指定
                        paxg::Font* big_year_font = language_fonts.getAndAdd(select_language.cgetKey(), static_cast<std::uint_least8_t>(koyomi_font_size * 3), static_cast<std::uint_least8_t>(koyomi_font_buffer_thickness_size));
                        if (big_year_font == nullptr) continue;

                        // 暦の読み方を返す
                        const std::string* const text_str = language_text.getStringPtr(
                            koyomi_siv.date_list[i].calendar_name_key,
                            select_language.cgetKey()
                        );
                        if (text_str == nullptr) continue;

                        switch (output_type) {
                        case paxs::cal::DateOutputType::name_and_ymd:
                            std::visit([&](const auto& x) {
                                date_year = int(x.cgetYear());
                                date_month = int(x.cgetMonth());
                                date_day = int(x.cgetDay());
                                date_lm = x.isLeapMonth();
                                }, koyomi_siv.date_list[i].date);
                            if (date_day <= 0 || date_month <= 0) break;

                            if (text_str != nullptr) (*one_font).drawTopRight(*text_str
                                , paxg::Vec2i(static_cast<int>(koyomi_font_en_x), static_cast<int>(koyomi_font_en_y + i * (koyomi_font_size * 4 / 3))), paxg::Color(0, 0, 0));

                            (*one_font).drawTopRight(",", paxg::Vec2i(static_cast<int>(int(95 * koyomi_font_size / 30.0) + koyomi_font_en_x), static_cast<int>(koyomi_font_en_y + i * (koyomi_font_size * 4 / 3))), paxg::Color(0, 0, 0));
                            (*one_font).drawTopRight(",", paxg::Vec2i(static_cast<int>(int(235 * koyomi_font_size / 30.0) + koyomi_font_en_x), static_cast<int>(koyomi_font_en_y + i * (koyomi_font_size * 4 / 3))), paxg::Color(0, 0, 0));
                            (*one_font).drawTopRight("th", paxg::Vec2i(static_cast<int>(int(315 * koyomi_font_size / 30.0) + koyomi_font_en_x), static_cast<int>(koyomi_font_en_y + i * (koyomi_font_size * 4 / 3))), paxg::Color(0, 0, 0));

                            (*one_font).drawTopRight(std::to_string(date_year), paxg::Vec2i(static_cast<int>(int(en_cal_name_pos_x * koyomi_font_size / 30.0) + koyomi_font_en_x), static_cast<int>(koyomi_font_en_y + i * (koyomi_font_size * 4 / 3))), paxg::Color(0, 0, 0));
                            (*one_font).drawTopRight(std::string(koyomi_siv.month_name[date_month]), paxg::Vec2i(static_cast<int>(int(220 * koyomi_font_size / 30.0) + koyomi_font_en_x), static_cast<int>(koyomi_font_en_y + i * (koyomi_font_size * 4 / 3))), paxg::Color(0, 0, 0));
                            (*one_font).drawTopRight(std::to_string(date_day), paxg::Vec2i(static_cast<int>(int(280 * koyomi_font_size / 30.0) + koyomi_font_en_x), static_cast<int>(koyomi_font_en_y + i * (koyomi_font_size * 4 / 3))), paxg::Color(0, 0, 0));
#ifdef PAXS_USING_SIMULATOR
                            if (simulator == nullptr) {
#else
                            {
#endif
                                if (i == 1) (*big_year_font).drawTopRight(std::to_string(date_year)/* + " AD"*/, paxg::Vec2i(static_cast<int>(int(en_cal_name_pos_x * koyomi_font_size / 30.0) + koyomi_font_en_x + 100 - 70), static_cast<int>(550 - 30 + koyomi_font_en_y + i * (koyomi_font_size * 4 / 3))), paxg::Color(0, 0, 0));
                            }

                            if (date_lm) {
                                (*one_font).drawTopRight("int.", paxg::Vec2i(static_cast<int>((
                                    int(152 * koyomi_font_size / 30.0) + koyomi_font_en_x
                                    )), static_cast<int>(koyomi_font_en_y + i * (koyomi_font_size * 4 / 3))), paxg::Color(0, 0, 0));
                            }
                            break;
                        case paxs::cal::DateOutputType::name_and_value:

                        {
                            if (text_str != nullptr) (*one_font).drawTopRight(*text_str,
                                paxg::Vec2i(static_cast<int>(koyomi_font_en_x), static_cast<int>(koyomi_font_en_y + i * (koyomi_font_size * 4 / 3))), paxg::Color(0, 0, 0));
                        }
                        std::visit([&](const auto& x) {
                            date_day = int(x.cgetDay());
                            }, koyomi_siv.date_list[i].date);
                        (*one_font).drawTopRight(std::to_string(date_day), paxg::Vec2i(static_cast<int>(int(315 * koyomi_font_size / 30.0) + koyomi_font_en_x), static_cast<int>(koyomi_font_en_y + i * (koyomi_font_size * 4 / 3))), paxg::Color(0, 0, 0));

                        break;
                        default:
                            break;
                        }
                    }
                }
                // 時間操作パネルを更新・描画
                const std::unordered_map<std::uint_least32_t, paxg::Texture>& texture_dictionary = key_value_tsv.get();
                time_control_panel.update(time_control_base_x, koyomi_font_y + time_control_base_y, texture_dictionary, tm_, koyomi_siv);
            }

            if (visible[MurMur3::calcHash(8, "Calendar")] && visible[MurMur3::calcHash(2, "UI")]) {
                int debug_start_y = koyomi_font_y + next_rect_start_y + 10;
                {

                    // 暦描画フォントを指定
                    paxg::Font* one_font = language_fonts.getAndAdd(select_language.cgetKey(), static_cast<std::uint_least8_t>(koyomi_font_size), static_cast<std::uint_least8_t>(koyomi_font_buffer_thickness_size));
                    if (one_font != nullptr) {

                        // マップの高さ
                        (*one_font).setOutline(0, 0.6, paxg::Color(255, 255, 255));
                        (*one_font).drawTopRight(((select_language.cgetKey() == MurMur3::calcHash("ja-JP")) ?
                            reinterpret_cast<const char*>(u8"拡大率") :
                            "Zoom rate"),
                            paxg::Vec2i(paxg::Window::width() - 40, debug_start_y), paxg::Color(0, 0, 0));
                        (*one_font).drawTopRight(std::to_string(map_view.getHeight()),
                            paxg::Vec2i(paxg::Window::width() - 40, debug_start_y + 30), paxg::Color(0, 0, 0));

#ifdef PAXS_USING_SIMULATOR
                        if (simulator == nullptr) {
#else
                            {
#endif
                            if (visible[MurMur3::calcHash(8, "Simulation")]) {
                                (*one_font).drawTopRight(std::to_string(map_view.getCenterX()),
                                    paxg::Vec2i(paxg::Window::width() - 40, debug_start_y + 60), paxg::Color(0, 0, 0));
                                (*one_font).drawTopRight(std::to_string(map_view.getCenterY()),
                                    paxg::Vec2i(paxg::Window::width() - 40, debug_start_y + 90), paxg::Color(0, 0, 0));
                            }
                        }
#ifdef PAXS_USING_SIMULATOR
                        if (simulator != nullptr) {
                        (*one_font).setOutline(0, 0.6, paxg::Color(255, 255, 255));
                        (*one_font).drawTopRight(
                            ((select_language.cgetKey() == MurMur3::calcHash("ja-JP"))?
                                reinterpret_cast<const char*>(u8"人口数") :
                                "Population"),
                            paxg::Vec2i(paxg::Window::width() - 140, debug_start_y + 60), paxg::Color(0, 0, 0));
                        (*one_font).drawTopRight(
                            std::to_string(simulator->cgetPopulationNum()),
                            paxg::Vec2i(paxg::Window::width() - 40, debug_start_y + 60), paxg::Color(0, 0, 0));

                        (*one_font).setOutline(0, 0.6, paxg::Color(255, 255, 255));
                        (*one_font).drawTopRight(
                            ((select_language.cgetKey() == MurMur3::calcHash("ja-JP")) ?
                                reinterpret_cast<const char*>(u8"集落数") :
                                "Settlements"),
                            paxg::Vec2i(paxg::Window::width() - 140, debug_start_y + 90), paxg::Color(0, 0, 0));
                        (*one_font).drawTopRight(
                            std::to_string(simulator->cgetSettlement()),
                            paxg::Vec2i(paxg::Window::width() - 40, debug_start_y + 90), paxg::Color(0, 0, 0));

                        (*one_font).drawTopRight(
                            ((select_language.cgetKey() == MurMur3::calcHash("ja-JP")) ?
                                reinterpret_cast<const char*>(u8"渡来数") :
                                "Total Immigrants"),
                            paxg::Vec2i(paxg::Window::width() - 140, debug_start_y + 120), paxg::Color(0, 0, 0));
                        (*one_font).drawTopRight(
                            std::to_string(simulator->emigrationSize()),
                            paxg::Vec2i(paxg::Window::width() - 40, debug_start_y + 120), paxg::Color(0, 0, 0));

                        (*one_font).drawTopRight(
                            ((select_language.cgetKey() == MurMur3::calcHash("ja-JP")) ?
                                reinterpret_cast<const char*>(u8"全ての処理時間（秒）") :
                                "All Processing Time [s]"),
                            paxg::Vec2i(paxg::Window::width() - 140, debug_start_y + 150), paxg::Color(0, 0, 0));
                        (*one_font).drawTopRight(
                            std::to_string(simulator->cgetProcessingTime()),
                            paxg::Vec2i(paxg::Window::width() - 40, debug_start_y + 150), paxg::Color(0, 0, 0));

                        (*one_font).drawTopRight(
                            ((select_language.cgetKey() == MurMur3::calcHash("ja-JP")) ?
                                reinterpret_cast<const char*>(u8"集団移動の処理時間（秒）") :
                                "Move Processing Time [s]"),
                            paxg::Vec2i(paxg::Window::width() - 140, debug_start_y + 180), paxg::Color(0, 0, 0));
                        (*one_font).drawTopRight(
                            std::to_string(simulator->cgetMoveProcessingTime()),
                            paxg::Vec2i(paxg::Window::width() - 40, debug_start_y + 180), paxg::Color(0, 0, 0));

                        (*one_font).drawTopRight(
                            ((select_language.cgetKey() == MurMur3::calcHash("ja-JP")) ?
                                reinterpret_cast<const char*>(u8"婚姻の処理時間（秒）") :
                                "Marriage Processing Time [s]"),
                            paxg::Vec2i(paxg::Window::width() - 140, debug_start_y + 210), paxg::Color(0, 0, 0));
                        (*one_font).drawTopRight(
                            std::to_string(simulator->cgetMarriageProcessingTime()),
                            paxg::Vec2i(paxg::Window::width() - 40, debug_start_y + 210), paxg::Color(0, 0, 0));


                        }
#endif
                    }
                }
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
                int debug_start_y = koyomi_font_y + next_rect_start_y + 10;
                // 暦描画フォントを指定
                paxg::Font* one_font = language_fonts.getAndAdd(select_language.cgetKey(), static_cast<std::uint_least8_t>(koyomi_font_size), static_cast<std::uint_least8_t>(koyomi_font_buffer_thickness_size));
                if (one_font != nullptr) {
                    {
                        std::string dotaku = "";
                        std::string doken = "";

                        static std::array<int, 12> yayoi_year = { {
                        -230, -190, -150, -100, -50, -20, 10, 40, 80, 120, 160, 200
                        } };
                        static std::array<std::string, 12> dotaku_name = { {
                                "",
                                reinterpret_cast<const char*>(u8"Ⅰ式　菱環紐1式"),
            reinterpret_cast<const char*>(u8"Ⅰ式　菱環紐2式"),
            reinterpret_cast<const char*>(u8"Ⅱ式　外縁付鈕1式"),
            reinterpret_cast<const char*>(u8"Ⅱ式　外縁付鈕2式"),
            reinterpret_cast<const char*>(u8"Ⅲ式　扁平鈕1式"),
            reinterpret_cast<const char*>(u8"Ⅲ式　扁平鈕2式"),
            reinterpret_cast<const char*>(u8"Ⅳ式　突線鈕1式"),
            reinterpret_cast<const char*>(u8"Ⅳ式　突線鈕2式"),
            reinterpret_cast<const char*>(u8"Ⅳ式　突線鈕3式"),
            reinterpret_cast<const char*>(u8"Ⅳ式　突線鈕4式"),
            reinterpret_cast<const char*>(u8"Ⅳ式　突線鈕5式")
                                    } };
                        static std::array<std::string, 12> doken_name = { {
                                "",
                                reinterpret_cast<const char*>(u8"細形"),
            reinterpret_cast<const char*>(u8""),
            reinterpret_cast<const char*>(u8"中細形a類"),
            reinterpret_cast<const char*>(u8"中細形b類"),
            reinterpret_cast<const char*>(u8"中細形c類／平形Ⅰ"),
            reinterpret_cast<const char*>(u8"中広形／平形Ⅱ"),
            reinterpret_cast<const char*>(u8""),
            reinterpret_cast<const char*>(u8""),
            reinterpret_cast<const char*>(u8""),
            reinterpret_cast<const char*>(u8""),
            reinterpret_cast<const char*>(u8"")
                                    } };

                        std::string sueki_nakamura = "";
                        std::string sueki_tanabe = "";

                        static std::array<int, 18> sueki_year = { {
                        1380,1390,1410,1430,1440,1450,1460,1470,1490,1500,1520,1530,1550,1560,1590,1620,1645,1670
                        } };
                        static std::array<std::string, 18> sueki_name = { {
            "","TG232","TK73","TK216","ON46","ON46/TK208","TK208","TK23","TK23/TK47","TK47",
            "MT15","TK10","TK10/MT85","MT85","TK43","TK209",reinterpret_cast<const char*>(u8"TK217古"),reinterpret_cast<const char*>(u8"TK217新")
                                    } };
                        static std::array<std::string, 18> sueki_nakamura_name = { {
            "",reinterpret_cast<const char*>(u8"I-1前"),reinterpret_cast<const char*>(u8"I-1後"),"I-2","I-3","I-3","I-3","I-4","I-4/I-5","I-5",
            "II-1","II-2","II-2/II-3","II-3","II-4","II-5","II-6","III-1"
                                    } };
#ifndef PAXS_USING_SIMULATOR
                        {
                            int date_year = 0;
                            std::visit([&](const auto& x) { date_year = int(x.cgetYear()); }, koyomi_siv.date_list[1].date);
                            for (std::size_t i = 0; i < sueki_year.size(); ++i) {
                                if (date_year < sueki_year[i]) {
                                    sueki_tanabe = sueki_name[i];
                                    sueki_nakamura = sueki_nakamura_name[i];
                                    break;
                                }
                            }
                            dotaku = "";
                            for (std::size_t i = 0; i < yayoi_year.size(); ++i) {
                                if (date_year < yayoi_year[i]) {
                                    dotaku = dotaku_name[i];
                                    doken = doken_name[i];
                                    break;
                                }
                            }
                        }
                        if (dotaku.size() != 0) {
                                (*one_font).setOutline(0, 0.6, paxg::Color(255, 255, 255));
                                (*one_font).draw(reinterpret_cast<const char*>(u8"銅鐸 型式"),
                                    paxg::Vec2i(rect_start_x + 10, debug_start_y + 140), paxg::Color(0, 0, 0));
                            (*one_font).drawTopRight(
                                std::string(dotaku),
                                paxg::Vec2i(paxg::Window::width() - 40, debug_start_y + 140), paxg::Color(0, 0, 0));
                        }
                        if (doken.size() != 0) {
                            (*one_font).setOutline(0, 0.6, paxg::Color(255, 255, 255));
                            (*one_font).draw(reinterpret_cast<const char*>(u8"銅剣 型式"),
                                paxg::Vec2i(rect_start_x + 10, debug_start_y + 170), paxg::Color(0, 0, 0));
                            (*one_font).drawTopRight(
                                std::string(doken),
                                paxg::Vec2i(paxg::Window::width() - 40, debug_start_y + 170), paxg::Color(0, 0, 0));
                        }

                        const std::string* sueki_nakamura_str = language_text.getStringPtr(koyomi_siv.sueki_nakamura_key, select_language.cgetKey());
                        if (sueki_nakamura.size() != 0) {
                            if (sueki_nakamura_str != nullptr) {
                                (*one_font).setOutline(0, 0.6, paxg::Color(255, 255, 255));
                                (*one_font).draw(*sueki_nakamura_str,
                                    paxg::Vec2i(rect_start_x + 10, debug_start_y + 140), paxg::Color(0, 0, 0));
                            }
                            (*one_font).drawTopRight(
                                std::string(sueki_nakamura),
                                paxg::Vec2i(paxg::Window::width() - 40, debug_start_y + 140), paxg::Color(0, 0, 0));
                        }
                        if (sueki_tanabe.size() != 0) {
                            const std::string* sueki_tanabe_str = language_text.getStringPtr(koyomi_siv.sueki_tanabe_key, select_language.cgetKey());
                            if (sueki_nakamura_str != nullptr) {
                                (*one_font).draw(*sueki_tanabe_str,
                                    paxg::Vec2i(rect_start_x + 10, debug_start_y + 170), paxg::Color(0, 0, 0));
                            }
                            (*one_font).setOutline(0, 0.6, paxg::Color(255, 255, 255));
                            (*one_font).drawTopRight(
                                std::string(sueki_tanabe),
                                paxg::Vec2i(paxg::Window::width() - 40, debug_start_y + 170), paxg::Color(0, 0, 0));
                        }
#endif
                    }
                }
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
