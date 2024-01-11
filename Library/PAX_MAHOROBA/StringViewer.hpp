/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_STRING_VIEWER_HPP
#define PAX_MAHOROBA_STRING_VIEWER_HPP

/*##########################################################################################

##########################################################################################*/

#include <limits>
#include <string>
#include <variant>
#include <vector>

#include <PAX_GRAPHICA/Key.hpp>
#include <PAX_GRAPHICA/RoundRect.hpp>

#include <PAX_MAHOROBA/3DModel.hpp>
#include <PAX_MAHOROBA/Calendar.hpp> // 計算時に必要
#include <PAX_MAHOROBA/LanguageFonts.hpp>
#include <PAX_MAHOROBA/LocationPoint.hpp>
#include <PAX_MAHOROBA/LocationRange.hpp>
#include <PAX_MAHOROBA/Pulldown.hpp>
#include <PAX_MAHOROBA/XYZTiles.hpp>
#include <PAX_MAHOROBA/XYZTilesList.hpp>

#include <PAX_SAPIENTICA/AppConfig.hpp>
#include <PAX_SAPIENTICA/Calendar/Calendars.hpp>
#include <PAX_SAPIENTICA/Calendar/Date.hpp>
#include <PAX_SAPIENTICA/Calendar/JapaneseEra.hpp>
#include <PAX_SAPIENTICA/Calendar/JulianDayNumber.hpp>
#include <PAX_SAPIENTICA/GraphicVisualizationList.hpp> // 暦にないファイル
#include <PAX_SAPIENTICA/Language.hpp>
#include <PAX_SAPIENTICA/MapProjection.hpp> // 地図投影法
#include <PAX_SAPIENTICA/Math.hpp> // 数学定数
#include <PAX_SAPIENTICA/MurMur3.hpp>
#ifdef PAXS_USING_SIMULATOR
#include <PAX_SAPIENTICA/Simulation/SettlementSimulator.hpp>
#include <PAX_SAPIENTICA/Simulation/SimulationConst.hpp>
#include <PAX_SAPIENTICA/Simulation/Simulator.hpp>
#endif
#include <PAX_SAPIENTICA/InputFile/KeyValueTSV.hpp>
#include <PAX_SAPIENTICA/StringExtensions.hpp>
#include <PAX_SAPIENTICA/TouchManager.hpp>


namespace paxs {

    class StringViewerSiv3D {
    private:
#ifdef PAXS_USING_SIMULATOR
        void simulation(
            std::unique_ptr<paxs::SettlementSimulator<int>>& simulator, // コンパイル時の分岐により使わない場合あり
            paxs::TouchManager& tm_,
            paxs::KoyomiSiv3D& koyomi_siv,
            int debug_start_y
        ) {
            const std::unordered_map<std::uint_least32_t, paxg::Texture>& texture_dictionary = key_value_tsv.get();
                const int time_icon_size = 40; // 時間操作アイコンの大きさ

                // シミュレーションが初期化されていない場合
                if (simulator.get() == nullptr) {
                    texture_dictionary.at(MurMur3::calcHash("texture_load_geographic_data2")).resizedDraw(
                        time_icon_size, paxg::Vec2i(paxg::Window::width() - 360, debug_start_y));
                    if (tm_.get(paxg::Rect{ paxg::Vec2i(paxg::Window::width() - 360, debug_start_y), paxg::Vec2i(time_icon_size, time_icon_size) }.leftClicked())) {
                        std::string map_list_path = "Data/Simulations/MapList.tsv";
                        std::string japan_provinces_path = "Data/Simulations/Japan200-725";

                        AppConfig::getInstance()->calcDataSettingsNotPath(MurMur3::calcHash("SimulationXYZTiles"),
                            [&](const std::string& path_) {map_list_path = path_; });
                        AppConfig::getInstance()->calcDataSettingsNotPath(MurMur3::calcHash("SimulationProvincesPath"),
                            [&](const std::string& path_) {japan_provinces_path = path_; });

#ifdef PAXS_USING_SIV3D
                        static bool is_console_open = false;
                        if (!is_console_open) {
                            s3d::detail::Console_impl{}.open(); // コンソールを開く s3d::Console::Open()
                            is_console_open = true;
                        }
#endif

                        std::random_device seed_gen;
                        simulator = std::make_unique<paxs::SettlementSimulator<int>>(
                            map_list_path, japan_provinces_path,
                            SimulationConstants::getInstance()->getZ(),
                            seed_gen());
                        simulator->init();
                        koyomi_siv.steps.setDay(0); // ステップ数を 0 にする
                        koyomi_siv.jdn.setDay(static_cast<double>(SimulationConstants::getInstance()->start_julian_day)); // シミュレーション初期時の日付に設定
                        koyomi_siv.calcDate();

                        koyomi_siv.is_agent_update = false;

                        koyomi_siv.move_forward_in_time = false; // 一時停止
                        koyomi_siv.go_back_in_time = false;
                    }
                }
                // シミュレーションが初期化されている場合
                else {
                    // シミュレーションが再生されている場合
                    if (koyomi_siv.is_agent_update) {
                        // シミュレーションを停止
                        texture_dictionary.at(MurMur3::calcHash("texture_stop")).resizedDraw(
                            time_icon_size, paxg::Vec2i(paxg::Window::width() - 300, debug_start_y));
                        if (tm_.get(paxg::Rect{ paxg::Vec2i(paxg::Window::width() - 300, debug_start_y), paxg::Vec2i(time_icon_size, time_icon_size) }.leftClicked())) {
                            // if (s3d::SimpleGUI::Button(U"Sim Stop", s3d::Vec2{ 330, 60 })) {
                            koyomi_siv.is_agent_update = false;

                            koyomi_siv.move_forward_in_time = false; // 一時停止
                            koyomi_siv.go_back_in_time = false;
                        }
                    }
                    // シミュレーションが再生されていない場合
                    else {
                        // 地形データを削除
                        texture_dictionary.at(MurMur3::calcHash("texture_delete_geographic_data")).resizedDraw(
                            time_icon_size, paxg::Vec2i(paxg::Window::width() - 360, debug_start_y));
                        if (tm_.get(paxg::Rect{ paxg::Vec2i(paxg::Window::width() - 360, debug_start_y), paxg::Vec2i(time_icon_size, time_icon_size) }.leftClicked())) {
                            simulator.reset();

                            koyomi_siv.steps.setDay(0); // ステップ数を 0 にする
                            koyomi_siv.calcDate();
                        }

                        // シミュレーションを再生
                        texture_dictionary.at(MurMur3::calcHash("texture_playback")).resizedDraw(
                            time_icon_size, paxg::Vec2i(paxg::Window::width() - 300, debug_start_y));
                        if (tm_.get(paxg::Rect{ paxg::Vec2i(paxg::Window::width() - 300, debug_start_y), paxg::Vec2i(time_icon_size, time_icon_size) }.leftClicked())) {
                            // if (s3d::SimpleGUI::Button(U"Sim Start", s3d::Vec2{ 190, 60 })) {
                            koyomi_siv.is_agent_update = true;

                            koyomi_siv.move_forward_in_time = true; // 再生
                            koyomi_siv.go_back_in_time = false;
                        }
                        // シミュレーションを 1 Step 実行
                        texture_dictionary.at(MurMur3::calcHash("texture_1step")).resizedDraw(
                            time_icon_size, paxg::Vec2i(paxg::Window::width() - 240, debug_start_y));
                        if (tm_.get(paxg::Rect{ paxg::Vec2i(paxg::Window::width() - 240, debug_start_y), paxg::Vec2i(time_icon_size, time_icon_size) }.leftClicked())) {
                            simulator->step(); // シミュレーションを 1 ステップ実行する
                            koyomi_siv.steps.getDay()++; // ステップ数を増やす
                            koyomi_siv.calcDate();

                            koyomi_siv.move_forward_in_time = false; // 一時停止
                            koyomi_siv.go_back_in_time = false;
                        }
                    }
                }
        }
#endif

    public:

        /*##########################################################################################
            フォント
        ##########################################################################################*/

        LanguageFonts language_fonts;

        std::vector<std::uint_least32_t> language_key = {
MurMur3::calcHash("en-US"), MurMur3::calcHash("ja-JP"), MurMur3::calcHash("zh-TW"), MurMur3::calcHash("zh-CN"), MurMur3::calcHash("ko-KR"), MurMur3::calcHash("es-ES"), MurMur3::calcHash("pt-BR"), MurMur3::calcHash("de-DE"), MurMur3::calcHash("fr-FR"), MurMur3::calcHash("it-IT"), MurMur3::calcHash("tr-TR"), MurMur3::calcHash("pl-PL"), MurMur3::calcHash("el-GR"), MurMur3::calcHash("nl-NL"), MurMur3::calcHash("cs-CZ"), MurMur3::calcHash("uk-UA"), MurMur3::calcHash("ru-RU"), MurMur3::calcHash("id-ID"), MurMur3::calcHash("fa-IR"), MurMur3::calcHash("ar-SA")
        };
        std::vector<std::string> path_list = {
            "Data/Font/noto-sans-jp/NotoSansJP-Regular.otf", "Data/Font/noto-sans-jp/NotoSansJP-Regular.otf", "Data/Font/noto-sans-sc/NotoSansSC-Regular.otf", "Data/Font/noto-sans-sc/NotoSansSC-Regular.otf", "Data/Font/noto-sans-kr/NotoSansKR-Regular.otf", "Data/Font/noto-sans-jp/NotoSansJP-Regular.otf", "Data/Font/noto-sans-jp/NotoSansJP-Regular.otf", "Data/Font/noto-sans-jp/NotoSansJP-Regular.otf", "Data/Font/noto-sans-jp/NotoSansJP-Regular.otf", "Data/Font/noto-sans-jp/NotoSansJP-Regular.otf", "Data/Font/noto-sans/NotoSans-Regular.ttf", "Data/Font/noto-sans/NotoSans-Regular.ttf", "Data/Font/noto-sans-jp/NotoSansJP-Regular.otf", "Data/Font/noto-sans-jp/NotoSansJP-Regular.otf", "Data/Font/noto-sans/NotoSans-Regular.ttf", "Data/Font/noto-sans/NotoSans-Regular.ttf", "Data/Font/noto-sans-jp/NotoSansJP-Regular.otf", "Data/Font/noto-sans-jp/NotoSansJP-Regular.otf", "Data/Font/noto-sans-ar/NotoNaskhArabic-VariableFont_wght.ttf", "Data/Font/noto-sans-ar/NotoNaskhArabic-VariableFont_wght.ttf"
        };

        // プルダウンのフォントサイズ
        int pulldown_font_size =
#if defined(PAXS_USING_DXLIB) && defined(__ANDROID__)
            40;
#else
            24;
#endif

        int pulldown_font_buffer_thickness_size = 3; // プルダウンのフォント太さ

        int koyomi_font_size = 19; // 暦のフォントサイズ
        int koyomi_font_buffer_thickness_size = 3; // 暦のフォント太さ

        paxg::Font pin_font{};
        paxg::Font en_font{};

        std::size_t map_view_width_str_index;
        std::size_t map_view_center_x_str_index;
        std::size_t map_view_center_y_str_index;
        std::size_t map_view_center_lat_str_index;
        std::size_t xyz_tile_z_str_index;
#ifdef PAXS_USING_SIV3D
        // UI の影
        s3d::RenderTexture shadow_texture{};
        s3d::RenderTexture internal_texture{};
#endif
        paxs::Pulldown pulldown;
        paxs::MenuBar menu_bar;

        paxs::KeyValueTSV<paxg::Texture> key_value_tsv;

        //const std::string map_license_name = U"Maptiles by\n淺野孝利 2023「古墳時代の『常総の内海』水域復原に関する一試論」\n研究代表者 荒井啓汰『埋葬施設からみた常総地域の地域構造』\n特別研究員奨励費報告書 筑波大学大学院 人文社会科学研究科";
        //std::string map_license_name = reinterpret_cast<const char*>(u8"Maptiles by\n農研機構農業環境研究部門, under CC BY 2.1 JP.\n20万分の1シームレス地質図V2.\nOpenStreetMap contributors, under ODbL.");
        //const std::string map_license_name = U"Maptiles by MIERUNE, under CC BY. Data by OpenStreetMap contributors, under ODbL.\nMaptiles by 農研機構農業環境研究部門, under CC BY 2.1 JP";

        paxs::Graphics3DModel g3d_model;

        void init(
            const SelectLanguage& select_language,
            const paxs::Language& language_text
        ) {
            language_fonts.setDefaultPath("Data/Font/noto-sans-sc/NotoSansSC-Regular.otf");
            setLanguageFont(pulldown_font_size, AppConfig::getInstance()->getRootPath(), pulldown_font_buffer_thickness_size);
            //koyomi_font = setFont(koyomi_font_size, AppConfig::getInstance()->getRootPath(), koyomi_font_buffer_thickness_size);
            map_view_width_str_index = (MurMur3::calcHash(25, "debug_magnification_power"));
            map_view_center_x_str_index = (MurMur3::calcHash(24, "debug_mercator_longitude"));
            map_view_center_y_str_index = (MurMur3::calcHash(23, "debug_mercator_latitude"));
            map_view_center_lat_str_index = (MurMur3::calcHash(14, "debug_latitude"));
            xyz_tile_z_str_index = (MurMur3::calcHash(17, "debug_xyz_tiles_z"));

            std::vector<std::uint_least32_t> list_test0 = {
            MurMur3::calcHash("en-US"),
                MurMur3::calcHash("ja-JP"),
                MurMur3::calcHash("zh-TW"),
                MurMur3::calcHash("zh-CN"),
                MurMur3::calcHash("ko-KR"),
                MurMur3::calcHash("es-ES"),
                MurMur3::calcHash("pt-BR")
            };

            pulldown = paxs::Pulldown(&select_language, &language_text, list_test0, language_fonts, static_cast<std::uint_least8_t>(pulldown_font_size), static_cast<std::uint_least8_t>(pulldown_font_buffer_thickness_size), paxg::Vec2i{ 3000, 0 }, 0, true);
            pulldown.setPos(paxg::Vec2i{ static_cast<int>(paxg::Window::width() - pulldown.getRect().w()), 0 });

            //std::vector<std::uint_least32_t> list_test1 = {
            //MurMur3::calcHash("menu_bar_file"),
            //    MurMur3::calcHash("menu_bar_file_new"),
            //    MurMur3::calcHash("menu_bar_file_open"),
            //    MurMur3::calcHash("menu_bar_file_save"),
            //    MurMur3::calcHash("menu_bar_file_close"),
            //    MurMur3::calcHash("menu_bar_file_exit")
            //};
            //std::vector<std::uint_least32_t> list_test2 = {
            //MurMur3::calcHash("menu_bar_edit"),
            //    MurMur3::calcHash("menu_bar_edit_cut"),
            //    MurMur3::calcHash("menu_bar_edit_copy"),
            //    MurMur3::calcHash("menu_bar_edit_paste"),
            //    MurMur3::calcHash("menu_bar_edit_delete")
            //};
            std::vector<std::uint_least32_t> list_test3 = {
            MurMur3::calcHash("menu_bar_view"),
                MurMur3::calcHash("menu_bar_view_calendar"),
                MurMur3::calcHash("menu_bar_view_map"),
                MurMur3::calcHash("menu_bar_view_ui"),
                MurMur3::calcHash("menu_bar_view_simulation"),
                MurMur3::calcHash("menu_bar_view_license"),
                MurMur3::calcHash("menu_bar_view_debug"),
                MurMur3::calcHash("menu_bar_view_3d")
            };
            //std::vector<std::uint_least32_t> list_test4 = {
            //MurMur3::calcHash("menu_bar_calendar"),
            //    MurMur3::calcHash("menu_bar_calendar_japan"),
            //    MurMur3::calcHash("menu_bar_calendar_gregorian"),
            //    MurMur3::calcHash("menu_bar_calendar_julian"),
            //    MurMur3::calcHash("menu_bar_calendar_julian_day"),
            //    MurMur3::calcHash("menu_bar_calendar_hijri")
            //};
            //std::vector<std::uint_least32_t> list_test5 = {
            //MurMur3::calcHash("menu_bar_map"),
            //    MurMur3::calcHash("menu_bar_map_base"),
            //    MurMur3::calcHash("menu_bar_map_land_and_sea"),
            //    MurMur3::calcHash("menu_bar_map_land_and_water"),
            //    MurMur3::calcHash("menu_bar_map_soil"),
            //    MurMur3::calcHash("menu_bar_map_soil_temperature"),
            //    MurMur3::calcHash("menu_bar_map_ryosei_country"),
            //    MurMur3::calcHash("menu_bar_map_ryosei_line"),
            //    MurMur3::calcHash("menu_bar_map_slope"),
            //    MurMur3::calcHash("menu_bar_map_lakes_and_rivers1"),
            //    MurMur3::calcHash("menu_bar_map_lakes_and_rivers2"),
            //    MurMur3::calcHash("menu_bar_map_line1"),
            //    MurMur3::calcHash("menu_bar_map_line2")
            //};
            //menu_bar.add(&select_language, &language_text, list_test1, language_fonts, static_cast<std::uint_least8_t>(pulldown_font_size), static_cast<std::uint_least8_t>(pulldown_font_buffer_thickness_size), MurMur3::calcHash("file"));
            //menu_bar.add(&select_language, &language_text, list_test2, language_fonts, static_cast<std::uint_least8_t>(pulldown_font_size), static_cast<std::uint_least8_t>(pulldown_font_buffer_thickness_size), MurMur3::calcHash("edit"));
            menu_bar.add(&select_language, &language_text, list_test3, language_fonts, static_cast<std::uint_least8_t>(pulldown_font_size), static_cast<std::uint_least8_t>(pulldown_font_buffer_thickness_size), MurMur3::calcHash("view"));
            //menu_bar.add(&select_language, &language_text, list_test4, language_fonts, static_cast<std::uint_least8_t>(pulldown_font_size), static_cast<std::uint_least8_t>(pulldown_font_buffer_thickness_size), MurMur3::calcHash("calendar"));
            //menu_bar.add(&select_language, &language_text, list_test5, language_fonts, static_cast<std::uint_least8_t>(pulldown_font_size), static_cast<std::uint_least8_t>(pulldown_font_buffer_thickness_size), MurMur3::calcHash("map"));

            const std::string path = (AppConfig::getInstance()->getRootPath());
            // 暦の時間操作のアイコン
            key_value_tsv.input(path + "Data/MenuIcon/MenuIcons.tsv", [&](const std::string& value_) { return paxg::Texture{ path + value_ }; });

            //koyomi_font = setFont(koyomi_font_size, AppConfig::getInstance()->getRootPath(), 2);

            pin_font = paxg::Font{ 14 /*, Typeface::Bold*/
                , (path + "Data/Font/noto-sans-jp/NotoSansJP-Regular.otf"), 2 };

            pin_font = paxg::Font{ 18 /*, Typeface::Bold*/
                , (path + "Data/Font/noto-sans-jp/NotoSansJP-Regular.otf"), 2 };

            // 英語用フォント
            en_font = paxg::Font{ 16 /*, Typeface::Bold*/
    , (path + "Data/Font/noto-sans-jp/NotoSansJP-Regular.otf"), 2 };

#ifdef PAXS_USING_SIV3D
            // 影
            shadow_texture = s3d::RenderTexture{ s3d::Scene::Size(), s3d::ColorF{ 1.0, 0.0 } };
            internal_texture = s3d::RenderTexture{ shadow_texture.size() };
#endif
        }

        void update(
            MapView& map_view,
            const SelectLanguage& select_language,
            const paxs::Language& language_text,
#ifdef PAXS_USING_SIMULATOR
            std::unique_ptr<paxs::SettlementSimulator<int>>& simulator, // コンパイル時の分岐により使わない場合あり
            std::size_t& pop_num, // 人口数
            std::size_t& sat_num, // 集落数
#endif
            paxs::TouchManager& tm_,
            paxs::KoyomiSiv3D& koyomi_siv,
            paxs::GraphicVisualizationList& visible
            ) {
            const double map_view_width = map_view.getWidth();
            // const double map_view_center_lat =
                //paxs::MathF64::radToDeg(std::asin(std::tanh(paxs::MathF64::degToRad(map_view->getCenterY()))));
            map_view.getCoordinate().toEquirectangularDegY();

            // 画像の拡大縮小の方式を設定
            const s3d::ScopedRenderStates2D sampler{ s3d::SamplerState::ClampLinear };

            // 暦の位置
            int koyomi_font_x = 0;//220;
            int koyomi_font_y = pulldown_font_size + 43;
            int koyomi_font_en_x = 0;//820;
            int koyomi_font_en_y = pulldown_font_size + 43;

            koyomi_font_x = paxg::Window::width() - 220;
            koyomi_font_en_x = paxg::Window::width() - 240;
            int rect_start_x = paxg::Window::width() - 395;

            int koyomi_height = static_cast<int>(koyomi_siv.date_list.size()) * (koyomi_font_size * 4 / 3); // 暦の縦の幅

            const int arrow_time_icon_size = 24; // 時間操作アイコンの大きさ
            const int time_icon_size = 40; // 時間操作アイコンの大きさ
            int icon_const_start_x = 360;

            int icon_start_x = icon_const_start_x;

            int sum_icon_height = arrow_time_icon_size + time_icon_size * 2;

            int icon_start_y = koyomi_height + 40;// 80 + 30;
            const int arrow_icon_move_x = int(arrow_time_icon_size * 1.2);// int(time_icon_size * 1.4);
            const int icon_move_x = int(time_icon_size * 1.1);// int(time_icon_size * 1.4);

            const int arrow_icon_move_y = 30;
            const int icon_move_y = 44;

            int next_rect_start_y = icon_start_y + sum_icon_height + 50;//230;
            int next_rect_end_y = 230;//380;

#ifdef PAXS_USING_SIV3D
                // 影を作る図形を shadow_texture に描く
            {
                const s3d::ScopedRenderTarget2D target{ shadow_texture.clear(s3d::ColorF{ 1.0, 0.0 }) };
                const s3d::ScopedRenderStates2D blend{ s3d::BlendState::MaxAlpha };
                const s3d::Transformer2D transform{ s3d::Mat3x2::Translate(3, 3) };
                paxg::Rect{ 0, 0, static_cast<float>(paxg::Window::width()), static_cast<float>(pulldown.getRect().h()) }.draw(); // メニューバー

                if (visible[MurMur3::calcHash(8, "Calendar")] && visible[MurMur3::calcHash(2, "UI")]) {
                    paxg::RoundRect{ rect_start_x, koyomi_font_y - 15, 380, next_rect_start_y, 10 }.draw();
                    paxg::RoundRect{ rect_start_x, koyomi_font_y + next_rect_start_y + 5, 380, next_rect_end_y, 10 }.draw();
                }
            }
                // shadow_texture を 2 回ガウスぼかし
            {
                s3d::Shader::GaussianBlur(shadow_texture, internal_texture, shadow_texture);
                s3d::Shader::GaussianBlur(shadow_texture, internal_texture, shadow_texture);
            }
            shadow_texture.draw(s3d::ColorF{ 0.0, 0.5 });
#endif
            if (visible[MurMur3::calcHash(8, "Calendar")] && visible[MurMur3::calcHash(2, "UI")]) {
                // 暦表示の範囲に白背景を追加
                paxg::RoundRect{ rect_start_x, koyomi_font_y - 15, 380, next_rect_start_y, 10 }.draw(paxg::Color{ 255, 255, 255 }/*s3d::Palette::White*/);
                paxg::RoundRect{ rect_start_x, koyomi_font_y + next_rect_start_y + 5, 380, next_rect_end_y, 10 }.draw(paxg::Color{ 255, 255, 255 }/*s3d::Palette::White*/);
            }

#ifdef PAXS_USING_SIMULATOR
            // シミュレーションのボタン
            if (visible[MurMur3::calcHash("Simulation")] && visible[MurMur3::calcHash("UI")] && visible[MurMur3::calcHash("Calendar")]) {
                simulation(simulator, tm_, koyomi_siv, koyomi_font_y + next_rect_start_y + 20);
            }
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

                            (*one_font).drawTopRight(*text_str//std::string(koyomi_siv.date_list[i].calendar_name[select_language.cget() + 1 /* 言語位置調整 */])
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
                            (*one_font).drawTopRight(*text_str//std::string(koyomi_siv.date_list[i].calendar_name[select_language.cget() + 1 /* 言語位置調整 */])
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

                            if (text_str != nullptr) (*one_font).drawTopRight(*text_str//std::string(koyomi_siv.date_list[i].calendar_name[select_language.cget() + 1 /* 言語位置調整 */])
                                , paxg::Vec2i(static_cast<int>(koyomi_font_en_x), static_cast<int>(koyomi_font_en_y + i * (koyomi_font_size * 4 / 3))), paxg::Color(0, 0, 0));

                            (*one_font).drawTopRight(",", paxg::Vec2i(static_cast<int>(int(95 * koyomi_font_size / 30.0) + koyomi_font_en_x), static_cast<int>(koyomi_font_en_y + i * (koyomi_font_size * 4 / 3))), paxg::Color(0, 0, 0));
                            (*one_font).drawTopRight(",", paxg::Vec2i(static_cast<int>(int(235 * koyomi_font_size / 30.0) + koyomi_font_en_x), static_cast<int>(koyomi_font_en_y + i * (koyomi_font_size * 4 / 3))), paxg::Color(0, 0, 0));
                            (*one_font).drawTopRight("th", paxg::Vec2i(static_cast<int>(int(315 * koyomi_font_size / 30.0) + koyomi_font_en_x), static_cast<int>(koyomi_font_en_y + i * (koyomi_font_size * 4 / 3))), paxg::Color(0, 0, 0));

                            (*one_font).drawTopRight(std::to_string(date_year), paxg::Vec2i(static_cast<int>(int(en_cal_name_pos_x * koyomi_font_size / 30.0) + koyomi_font_en_x), static_cast<int>(koyomi_font_en_y + i * (koyomi_font_size * 4 / 3))), paxg::Color(0, 0, 0));
                            (*one_font).drawTopRight(std::string(koyomi_siv.month_name[date_month]), paxg::Vec2i(static_cast<int>(int(220 * koyomi_font_size / 30.0) + koyomi_font_en_x), static_cast<int>(koyomi_font_en_y + i * (koyomi_font_size * 4 / 3))), paxg::Color(0, 0, 0));
                            (*one_font).drawTopRight(std::to_string(date_day), paxg::Vec2i(static_cast<int>(int(280 * koyomi_font_size / 30.0) + koyomi_font_en_x), static_cast<int>(koyomi_font_en_y + i * (koyomi_font_size * 4 / 3))), paxg::Color(0, 0, 0));
                            if (date_lm) {
                                (*one_font).drawTopRight("int.", paxg::Vec2i(static_cast<int>((
                                    int(152 * koyomi_font_size / 30.0) + koyomi_font_en_x
                                    )), static_cast<int>(koyomi_font_en_y + i * (koyomi_font_size * 4 / 3))), paxg::Color(0, 0, 0));
                            }
                            break;
                        case paxs::cal::DateOutputType::name_and_value:

                        {
                            if (text_str != nullptr) (*one_font).drawTopRight(*text_str//std::string(koyomi_siv.date_list[i].calendar_name[select_language.cget() + 1 /* 言語位置調整 */])
                                ,
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
                const std::unordered_map<std::uint_least32_t, paxg::Texture>& texture_dictionary = key_value_tsv.get();

                texture_dictionary.at(MurMur3::calcHash("texture_reverse_playback")).resizedDraw(arrow_time_icon_size, paxg::Vec2i(paxg::Window::width() - icon_start_x, koyomi_font_y + icon_start_y));
                if (tm_.get(paxg::Rect{ paxg::Vec2i(paxg::Window::width() - icon_start_x, koyomi_font_y + icon_start_y), paxg::Vec2i(arrow_time_icon_size, arrow_time_icon_size) }.leftClicked())) {
                    koyomi_siv.move_forward_in_time = false;
                    koyomi_siv.go_back_in_time = true; // 逆再生
                }
                icon_start_x -= arrow_icon_move_x;
                texture_dictionary.at(MurMur3::calcHash("texture_stop")).resizedDraw(arrow_time_icon_size, paxg::Vec2i(paxg::Window::width() - icon_start_x, koyomi_font_y + icon_start_y));
                if (tm_.get(paxg::Rect{ paxg::Vec2i(paxg::Window::width() - icon_start_x, koyomi_font_y + icon_start_y), paxg::Vec2i(arrow_time_icon_size, arrow_time_icon_size) }.leftClicked())) {
                    koyomi_siv.move_forward_in_time = false; // 一時停止
                    koyomi_siv.go_back_in_time = false;
                }
                icon_start_x -= arrow_icon_move_x;
                texture_dictionary.at(MurMur3::calcHash("texture_playback")).resizedDraw(arrow_time_icon_size, paxg::Vec2i(paxg::Window::width() - icon_start_x, koyomi_font_y + icon_start_y));
                if (tm_.get(paxg::Rect{ paxg::Vec2i(paxg::Window::width() - icon_start_x, koyomi_font_y + icon_start_y), paxg::Vec2i(arrow_time_icon_size, arrow_time_icon_size) }.leftClicked())) {
                    koyomi_siv.move_forward_in_time = true; // 再生
                    koyomi_siv.go_back_in_time = false;
                }
                icon_start_y += arrow_icon_move_y;
                icon_start_x = icon_const_start_x;

                texture_dictionary.at(MurMur3::calcHash("texture_d_l")).resizedDraw(time_icon_size, paxg::Vec2i(paxg::Window::width() - icon_start_x, koyomi_font_y + icon_start_y));
                if (tm_.get(paxg::Rect{ paxg::Vec2i(paxg::Window::width() - icon_start_x, koyomi_font_y + icon_start_y), paxg::Vec2i(time_icon_size, time_icon_size) }.leftClicked())) {
                    koyomi_siv.jdn.getDay() -= 1;
                    koyomi_siv.calcDate();
                }
                icon_start_x -= icon_move_x;
                texture_dictionary.at(MurMur3::calcHash("texture_m_l")).resizedDraw(time_icon_size, paxg::Vec2i(paxg::Window::width() - icon_start_x, koyomi_font_y + icon_start_y));
                if (tm_.get(paxg::Rect{ paxg::Vec2i(paxg::Window::width() - icon_start_x, koyomi_font_y + icon_start_y), paxg::Vec2i(time_icon_size, time_icon_size) }.leftClicked())) {
                    koyomi_siv.jdn.getDay() -= (365.2422 / 12.0);
                    koyomi_siv.calcDate();
                }
                icon_start_x -= icon_move_x;
                texture_dictionary.at(MurMur3::calcHash("texture_y_l")).resizedDraw(time_icon_size, paxg::Vec2i(paxg::Window::width() - icon_start_x, koyomi_font_y + icon_start_y));
                if (tm_.get(paxg::Rect{ paxg::Vec2i(paxg::Window::width() - icon_start_x, koyomi_font_y + icon_start_y), paxg::Vec2i(time_icon_size, time_icon_size) }.leftClicked())) {
                    koyomi_siv.jdn.getDay() -= 365.2422;
                    koyomi_siv.calcDate();
                }
                icon_start_x -= icon_move_x;
                texture_dictionary.at(MurMur3::calcHash("texture_10y_l")).resizedDraw(time_icon_size, paxg::Vec2i(paxg::Window::width() - icon_start_x, koyomi_font_y + icon_start_y));
                if (tm_.get(paxg::Rect{ paxg::Vec2i(paxg::Window::width() - icon_start_x, koyomi_font_y + icon_start_y), paxg::Vec2i(time_icon_size, time_icon_size) }.leftClicked())) {
                    koyomi_siv.jdn.getDay() -= (365.2422 * 10);
                    koyomi_siv.calcDate();
                }
                icon_start_x -= icon_move_x;
                texture_dictionary.at(MurMur3::calcHash("texture_c_l")).resizedDraw(time_icon_size, paxg::Vec2i(paxg::Window::width() - icon_start_x, koyomi_font_y + icon_start_y));
                if (tm_.get(paxg::Rect{ paxg::Vec2i(paxg::Window::width() - icon_start_x, koyomi_font_y + icon_start_y), paxg::Vec2i(time_icon_size, time_icon_size) }.leftClicked())) {
                    koyomi_siv.jdn.getDay() -= (365.2422 * 100);
                    koyomi_siv.calcDate();
                }
                icon_start_x -= icon_move_x;
                texture_dictionary.at(MurMur3::calcHash("texture_10c_l")).resizedDraw(time_icon_size, paxg::Vec2i(paxg::Window::width() - icon_start_x, koyomi_font_y + icon_start_y));
                if (tm_.get(paxg::Rect{ paxg::Vec2i(paxg::Window::width() - icon_start_x, koyomi_font_y + icon_start_y), paxg::Vec2i(time_icon_size, time_icon_size) }.leftClicked())) {
                    koyomi_siv.jdn.getDay() -= (365.2422 * 1000);
                    koyomi_siv.calcDate();
                }
                icon_start_x -= icon_move_x;
                texture_dictionary.at(MurMur3::calcHash("texture_100c_l")).resizedDraw(time_icon_size, paxg::Vec2i(paxg::Window::width() - icon_start_x, koyomi_font_y + icon_start_y));
                if (tm_.get(paxg::Rect{ paxg::Vec2i(paxg::Window::width() - icon_start_x, koyomi_font_y + icon_start_y), paxg::Vec2i(time_icon_size, time_icon_size) }.leftClicked())) {
                    koyomi_siv.jdn.getDay() -= (365.2422 * 10000);
                    koyomi_siv.calcDate();
                }
                icon_start_y += icon_move_y;
                icon_start_x = icon_const_start_x;

                texture_dictionary.at(MurMur3::calcHash("texture_d_r")).resizedDraw(time_icon_size, paxg::Vec2i(paxg::Window::width() - icon_start_x, koyomi_font_y + icon_start_y));
                if (tm_.get(paxg::Rect{ paxg::Vec2i(paxg::Window::width() - icon_start_x, koyomi_font_y + icon_start_y), paxg::Vec2i(time_icon_size, time_icon_size) }.leftClicked())) {
                    koyomi_siv.jdn.getDay() += 1;
                    koyomi_siv.calcDate();
                }
                icon_start_x -= icon_move_x;
                texture_dictionary.at(MurMur3::calcHash("texture_m_r")).resizedDraw(time_icon_size, paxg::Vec2i(paxg::Window::width() - icon_start_x, koyomi_font_y + icon_start_y));
                if (tm_.get(paxg::Rect{ paxg::Vec2i(paxg::Window::width() - icon_start_x, koyomi_font_y + icon_start_y), paxg::Vec2i(time_icon_size, time_icon_size) }.leftClicked())) {
                    koyomi_siv.jdn.getDay() += (365.2422 / 12.0);
                    koyomi_siv.calcDate();
                }
                icon_start_x -= icon_move_x;
                texture_dictionary.at(MurMur3::calcHash("texture_y_r")).resizedDraw(time_icon_size, paxg::Vec2i(paxg::Window::width() - icon_start_x, koyomi_font_y + icon_start_y));
                if (tm_.get(paxg::Rect{ paxg::Vec2i(paxg::Window::width() - icon_start_x, koyomi_font_y + icon_start_y), paxg::Vec2i(time_icon_size, time_icon_size) }.leftClicked())) {
                    koyomi_siv.jdn.getDay() += 365.2422;
                    koyomi_siv.calcDate();
                }
                icon_start_x -= icon_move_x;
                texture_dictionary.at(MurMur3::calcHash("texture_10y_r")).resizedDraw(time_icon_size, paxg::Vec2i(paxg::Window::width() - icon_start_x, koyomi_font_y + icon_start_y));
                if (tm_.get(paxg::Rect{ paxg::Vec2i(paxg::Window::width() - icon_start_x, koyomi_font_y + icon_start_y), paxg::Vec2i(time_icon_size, time_icon_size) }.leftClicked())) {
                    koyomi_siv.jdn.getDay() += (365.2422 * 10);
                    koyomi_siv.calcDate();
                }
                icon_start_x -= icon_move_x;
                texture_dictionary.at(MurMur3::calcHash("texture_c_r")).resizedDraw(time_icon_size, paxg::Vec2i(paxg::Window::width() - icon_start_x, koyomi_font_y + icon_start_y));
                if (tm_.get(paxg::Rect{ paxg::Vec2i(paxg::Window::width() - icon_start_x, koyomi_font_y + icon_start_y), paxg::Vec2i(time_icon_size, time_icon_size) }.leftClicked())) {
                    koyomi_siv.jdn.getDay() += (365.2422 * 100);
                    koyomi_siv.calcDate();
                }
                icon_start_x -= icon_move_x;
                texture_dictionary.at(MurMur3::calcHash("texture_10c_r")).resizedDraw(time_icon_size, paxg::Vec2i(paxg::Window::width() - icon_start_x, koyomi_font_y + icon_start_y));
                if (tm_.get(paxg::Rect{ paxg::Vec2i(paxg::Window::width() - icon_start_x, koyomi_font_y + icon_start_y), paxg::Vec2i(time_icon_size, time_icon_size) }.leftClicked())) {
                    koyomi_siv.jdn.getDay() += (365.2422 * 1000);
                    koyomi_siv.calcDate();
                }
                icon_start_x -= icon_move_x;
                texture_dictionary.at(MurMur3::calcHash("texture_100c_r")).resizedDraw(time_icon_size, paxg::Vec2i(paxg::Window::width() - icon_start_x, koyomi_font_y + icon_start_y));
                if (tm_.get(paxg::Rect{ paxg::Vec2i(paxg::Window::width() - icon_start_x, koyomi_font_y + icon_start_y), paxg::Vec2i(time_icon_size, time_icon_size) }.leftClicked())) {
                    koyomi_siv.jdn.getDay() += (365.2422 * 10000);
                    koyomi_siv.calcDate();
                }

            }
            // 時代区分を選択するラジオボタン
            // static size_t index1 = 2;
            // if (s3d::SimpleGUI::RadioButtons(index1, options, s3d::Vec2{ paxg::Window::width() - 400, 400 })) {
            // jdn = period_jdn[index1];
            // }

            if (visible[MurMur3::calcHash(8, "Calendar")] && visible[MurMur3::calcHash(2, "UI")]) {
                int debug_start_y = koyomi_font_y + next_rect_start_y + 10;
                //int debug_move_y = 25;
                // その他のデバッグ用の変数情報の表示
                //if (visible[MurMur3::calcHash(2, "UI")])
                {

                    // 暦描画フォントを指定
                    paxg::Font* one_font = language_fonts.getAndAdd(select_language.cgetKey(), static_cast<std::uint_least8_t>(koyomi_font_size), static_cast<std::uint_least8_t>(koyomi_font_buffer_thickness_size));
                    if (one_font != nullptr) {

                        // (*one_font)(std::string(language_text.cget()[map_view_center_x_str_index][select_language.cget() + 1 /* 言語位置調整 */]
                        //)).draw(s3d::TextStyle::Outline(0, 0.6, s3d::Palette::White), s3d::Arg::topRight = s3d::Vec2(paxg::Window::width() - 160, debug_start_y), s3d::Palette::Black);
                        // (*one_font)(s3d::ToString(map_view->getCenterX())
                        //    ).draw(s3d::TextStyle::Outline(0, 0.6, s3d::Palette::White), s3d::Vec2(paxg::Window::width() - 110, debug_start_y), s3d::Palette::Black);
                        //debug_start_y += debug_move_y;
                        //// マップ中心座標 Y
                        // (*one_font)(s3d::Unicode::FromUTF8(language_text.cget()[map_view_center_y_str_index][select_language.cget() + 1 /* 言語位置調整 */]
                        //)).draw(s3d::TextStyle::Outline(0, 0.6, s3d::Palette::White), s3d::Arg::topRight = s3d::Vec2(paxg::Window::width() - 160, debug_start_y), s3d::Palette::Black);
                        // (*one_font)(s3d::ToString(map_view->getCenterY())
                        //    ).draw(s3d::TextStyle::Outline(0, 0.6, s3d::Palette::White), s3d::Vec2(paxg::Window::width() - 110, debug_start_y), s3d::Palette::Black);
                        //debug_start_y += debug_move_y;
                        // (*one_font)(s3d::Unicode::FromUTF8(language_text.cget()[map_view_center_lat_str_index][select_language.cget() + 1 /* 言語位置調整 */]
                        //)).draw(s3d::TextStyle::Outline(0, 0.6, s3d::Palette::White), s3d::Arg::topRight = s3d::Vec2(paxg::Window::width() - 160, debug_start_y), s3d::Palette::Black);
                        // (*one_font)(s3d::ToString(map_view_center_lat)
                        //    ).draw(s3d::TextStyle::Outline(0, 0.6, s3d::Palette::White), s3d::Vec2(paxg::Window::width() - 110, debug_start_y), s3d::Palette::Black);
                        //debug_start_y += debug_move_y;
                        // (*one_font)(s3d::Unicode::FromUTF8(language_text.cget()[map_view_width_str_index][select_language.cget() + 1 /* 言語位置調整 */]
                        //)).draw(s3d::TextStyle::Outline(0, 0.6, s3d::Palette::White), s3d::Arg::topRight = s3d::Vec2(paxg::Window::width() - 160, debug_start_y), s3d::Palette::Black);

                        // マップの幅
                        (*one_font).setOutline(0, 0.6, paxg::Color(255, 255, 255));
                        (*one_font).drawTopRight(((select_language.cgetKey() == MurMur3::calcHash("ja-JP")) ?
                            reinterpret_cast<const char*>(u8"拡大率") :
                            "Zoom rate"),
                            paxg::Vec2i(paxg::Window::width() - 40, debug_start_y), paxg::Color(0, 0, 0));
                        (*one_font).drawTopRight(std::to_string(map_view_width),
                            paxg::Vec2i(paxg::Window::width() - 40, debug_start_y + 30), paxg::Color(0, 0, 0));

#ifdef PAXS_USING_SIMULATOR
                        if (simulator != nullptr) {
                        (*one_font).setOutline(0, 0.6, paxg::Color(255, 255, 255));
                        (*one_font).drawTopRight(
                            ((select_language.cgetKey() == MurMur3::calcHash("ja-JP"))?
                                reinterpret_cast<const char*>(u8"人口数") :
                                "Population"),
                            paxg::Vec2i(paxg::Window::width() - 140, debug_start_y + 60), paxg::Color(0, 0, 0));
                        (*one_font).drawTopRight(
                            std::to_string(pop_num),
                            paxg::Vec2i(paxg::Window::width() - 40, debug_start_y + 60), paxg::Color(0, 0, 0));

                        (*one_font).setOutline(0, 0.6, paxg::Color(255, 255, 255));
                        (*one_font).drawTopRight(
                            ((select_language.cgetKey() == MurMur3::calcHash("ja-JP")) ?
                                reinterpret_cast<const char*>(u8"集落数") :
                                "Settlements"),
                            paxg::Vec2i(paxg::Window::width() - 140, debug_start_y + 90), paxg::Color(0, 0, 0));
                        (*one_font).drawTopRight(
                            std::to_string(sat_num),
                            paxg::Vec2i(paxg::Window::width() - 40, debug_start_y + 90), paxg::Color(0, 0, 0));

                        (*one_font).drawTopRight(
                            ((select_language.cgetKey() == MurMur3::calcHash("ja-JP")) ?
                                reinterpret_cast<const char*>(u8"渡来数") :
                                "Immigrants"),
                            paxg::Vec2i(paxg::Window::width() - 140, debug_start_y + 120), paxg::Color(0, 0, 0));
                        (*one_font).drawTopRight(
                            std::to_string(simulator->emigrationSize()),
                            paxg::Vec2i(paxg::Window::width() - 40, debug_start_y + 120), paxg::Color(0, 0, 0));

                        }
#endif
                        //debug_start_y += debug_move_y;
                        // (*one_font)(s3d::Unicode::FromUTF8(language_text.cget()[xyz_tile_z_str_index][select_language.cget() + 1 /* 言語位置調整 */]
                        //)).draw(s3d::TextStyle::Outline(0, 0.6, s3d::Palette::White), s3d::Arg::topRight = s3d::Vec2(paxg::Window::width() - 160, debug_start_y), s3d::Palette::Black);

                        //koyomi_font[language](s3d::ToString(xyz_tile2->getZ())
                        //).draw(s3d::TextStyle::Outline(0, 0.6, s3d::Palette::White), s3d::Vec2(paxg::Window::width() - 110, debug_start_y), s3d::Palette::Black);
                        //debug_start_y += debug_move_y;
                        // koyomi_font[select_language.cget()].setOutline(0, 0.6, paxg::Color(255, 255, 255));
                        // koyomi_font[select_language.cget()].draw(std::to_string(xyz_tile2->getZNum()),
                        //    paxg::Vec2i(paxg::Window::width() - 110, debug_start_y), paxg::Color(0, 0, 0));
                    }
                }
            }
//            if (visible[MurMur3::calcHash(7, "License")]) {
//#ifdef PAXS_USING_SIV3D
//                //s3d::LicenseManager::ShowInBrowser();
//#endif
//            }

            if (!visible[MurMur3::calcHash(2, "3D")]) {
                g3d_model.updateRotation(); // 3D モデルを回転させる
            }

            // メニューバー
            paxg::Rect{ 0, 0, static_cast<float>(paxg::Window::width()), static_cast<float>(pulldown.getRect().h()) }.draw(paxg::Color{ 243, 243, 243 });

            const std::unordered_map<std::uint_least32_t, paxg::Texture>& texture_dictionary = key_value_tsv.get();

#ifdef PAXS_USING_SIV3D
            texture_dictionary.at(MurMur3::calcHash(14, "texture_github")).resizedDraw(24, paxg::Vec2i{ paxg::Window::width() - 280, 3 });
            if (tm_.get(s3d::Rect(paxg::Window::width() - 280, 3, 28).leftClicked())) {
                // Web ページをブラウザで開く
                s3d::System::LaunchBrowser(U"https://github.com/AsPJT/PAX_SAPIENTICA");
            }
#endif
            pulldown.draw(); // 言語選択
            menu_bar.draw(); // 左上メニューバー

            //if (visible[MurMur3::calcHash("UI")])

            if (visible[MurMur3::calcHash(8, "Calendar")] && visible[MurMur3::calcHash(2, "UI")]) {
                int debug_start_y = koyomi_font_y + next_rect_start_y + 10;
                // 暦描画フォントを指定
                paxg::Font* one_font = language_fonts.getAndAdd(select_language.cgetKey(), static_cast<std::uint_least8_t>(koyomi_font_size), static_cast<std::uint_least8_t>(koyomi_font_buffer_thickness_size));
                if (one_font != nullptr) {
                    {
                        std::string sueki_nakamura = "";
                        std::string sueki_tanabe = "";

                        static std::array<int, 18> sueki_year = { {
                        380,390,410,430,440,450,460,470,490,500,520,530,550,560,590,620,645,670
                        } };
                        static std::array<std::string, 18> sueki_name = { {
            "","TG232","TK73","TK216","ON46","ON46/TK208","TK208","TK23","TK23/TK47","TK47",
            "MT15","TK10","TK10/MT85","MT85","TK43","TK209",reinterpret_cast<const char*>(u8"TK217古"),reinterpret_cast<const char*>(u8"TK217新")
                                    } };
                        static std::array<std::string, 18> sueki_nakamura_name = { {
            "",reinterpret_cast<const char*>(u8"I-1前"),reinterpret_cast<const char*>(u8"I-1後"),"I-2","I-3","I-3","I-3","I-4","I-4/I-5","I-5",
            "II-1","II-2","II-2/II-3","II-3","II-4","II-5","II-6","III-1"
                                    } };
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
                        }
                        const std::string* sueki_nakamura_str = language_text.getStringPtr(koyomi_siv.sueki_nakamura_key, select_language.cgetKey());
                        if (sueki_nakamura.size() != 0) {
                            if (sueki_nakamura_str != nullptr) {
                                (*one_font).setOutline(0, 0.6, paxg::Color(255, 255, 255));
                                (*one_font).drawTopRight(*sueki_nakamura_str,
                                    paxg::Vec2i(paxg::Window::width() - 140, debug_start_y + 150), paxg::Color(0, 0, 0));
                            }
                            (*one_font).drawTopRight(
                                std::string(sueki_nakamura),
                                paxg::Vec2i(paxg::Window::width() - 40, debug_start_y + 150), paxg::Color(0, 0, 0));
                        }
                        if (sueki_tanabe.size() != 0) {
                            const std::string* sueki_tanabe_str = language_text.getStringPtr(koyomi_siv.sueki_tanabe_key, select_language.cgetKey());
                            if (sueki_nakamura_str != nullptr) {
                                (*one_font).drawTopRight(*sueki_tanabe_str,
                                    paxg::Vec2i(paxg::Window::width() - 140, debug_start_y + 180), paxg::Color(0, 0, 0));
                            }
                            (*one_font).setOutline(0, 0.6, paxg::Color(255, 255, 255));
                            (*one_font).drawTopRight(
                                std::string(sueki_tanabe),
                                paxg::Vec2i(paxg::Window::width() - 40, debug_start_y + 180), paxg::Color(0, 0, 0));
                        }
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
                    language_key[i],
                    static_cast<std::uint_least8_t>(font_size_),
                    static_cast<std::uint_least8_t>(buffer_thickness)
                );
            }
        }

    };

}

#endif // !PAX_MAHOROBA_STRING_VIEWER_HPP
