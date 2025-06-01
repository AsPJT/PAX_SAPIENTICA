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
#include <PAX_SAPIENTICA/InputFile.hpp>
#include <PAX_SAPIENTICA/InputFile/KeyValueTSV.hpp>
#include <PAX_SAPIENTICA/StringExtensions.hpp>
#include <PAX_SAPIENTICA/TouchManager.hpp>


namespace paxs {

    class StringViewerSiv3D {
    private:
#ifdef PAXS_USING_SIMULATOR
        void simulationInit(
            std::unique_ptr<paxs::SettlementSimulator>& simulator, // コンパイル時の分岐により使わない場合あり
            paxs::KoyomiSiv3D& koyomi_siv
        ) const {
            const std::string model_name =
                (simulation_model_index >= simulation_model_name.size()) ?
                "Sample" : simulation_model_name[simulation_model_index];

            simulator->init();
            koyomi_siv.steps.setDay(0); // ステップ数を 0 にする
            koyomi_siv.jdn.setDay(static_cast<double>(SimulationConstants::getInstance(model_name)->start_julian_day)); // シミュレーション初期時の日付に設定
            koyomi_siv.calcDate();
            koyomi_siv.is_agent_update = false;
            koyomi_siv.move_forward_in_time = false; // 一時停止
            koyomi_siv.go_back_in_time = false;
        }

        void simulation(
            std::unique_ptr<paxs::SettlementSimulator>& simulator, // コンパイル時の分岐により使わない場合あり
            paxs::TouchManager& tm_,
            paxs::KoyomiSiv3D& koyomi_siv,
            int debug_start_y
        ) {
            const std::unordered_map<std::uint_least32_t, paxg::Texture>& texture_dictionary = key_value_tsv.get();
                const int time_icon_size = 40; // 時間操作アイコンの大きさ

                const std::string model_name =
                    (simulation_model_index >= simulation_model_name.size()) ?
                    "Sample" : simulation_model_name[simulation_model_index];

                std::string map_list_path = "Data/Simulations/" + model_name + "/MapList.tsv";
                std::string japan_provinces_path = "Data/Simulations/" + model_name;

                // シミュレーションが初期化されていない場合
                if (simulator.get() == nullptr) {
                    texture_dictionary.at(MurMur3::calcHash("texture_load_geographic_data2")).resizedDraw(
                        time_icon_size, paxg::Vec2i(paxg::Window::width() - 360, debug_start_y));
                    if (tm_.get(paxg::Rect{ paxg::Vec2i(paxg::Window::width() - 360, debug_start_y), paxg::Vec2i(time_icon_size, time_icon_size) }.leftClicked())) {

                        AppConfig::getInstance()->calcDataSettingsNotPath(MurMur3::calcHash("SimulationXYZTiles"),
                            [&](const std::string& path_) {map_list_path = path_; });
                        AppConfig::getInstance()->calcDataSettingsNotPath(MurMur3::calcHash("SimulationProvincesPath"),
                            [&](const std::string& path_) {japan_provinces_path = path_; });
                        // Sample を選択モデル名に置換
                        paxs::StringExtensions::replace(map_list_path, "Sample", model_name);
                        paxs::StringExtensions::replace(japan_provinces_path, "Sample", model_name);
                        // シミュレーション変数を初期化
                        SimulationConstants::getInstance(model_name)->init(model_name);
#ifdef PAXS_USING_SIV3D
                        static bool is_console_open = false;
                        if (!is_console_open) {
                            s3d::detail::Console_impl{}.open(); // コンソールを開く s3d::Console::Open()
                            is_console_open = true;
                        }
#endif
                        std::random_device seed_gen;
                        simulator = std::make_unique<paxs::SettlementSimulator>(
                            map_list_path, japan_provinces_path,
                            seed_gen());
                        simulationInit(simulator, koyomi_siv);
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
                        // シミュレーション入力データを初期化
                        texture_dictionary.at(MurMur3::calcHash("texture_reload")).resizedDraw(
                            time_icon_size, paxg::Vec2i(paxg::Window::width() - 420, debug_start_y + 60));
                        if (tm_.get(paxg::Rect{ paxg::Vec2i(paxg::Window::width() - 420, debug_start_y + 60), paxg::Vec2i(time_icon_size, time_icon_size) }.leftClicked())) {
                            SimulationConstants::getInstance(model_name)->init(model_name);
                        }
                        // 人間データを初期化
                        texture_dictionary.at(MurMur3::calcHash("texture_load_agent_data2")).resizedDraw(
                            time_icon_size, paxg::Vec2i(paxg::Window::width() - 420, debug_start_y));
                        if (tm_.get(paxg::Rect{ paxg::Vec2i(paxg::Window::width() - 420, debug_start_y), paxg::Vec2i(time_icon_size, time_icon_size) }.leftClicked())) {
                            simulationInit(simulator, koyomi_siv);

                            koyomi_siv.steps.setDay(0); // ステップ数を 0 にする
                            koyomi_siv.calcDate();
                        }
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
MurMur3::calcHash("en-US"),
MurMur3::calcHash("ja-JP"),
MurMur3::calcHash("zh-TW"),
MurMur3::calcHash("zh-CN"),
MurMur3::calcHash("ko-KR"),
MurMur3::calcHash("hiragana"),
MurMur3::calcHash("es-ES"),
MurMur3::calcHash("pt-BR"),
MurMur3::calcHash("de-DE"),
MurMur3::calcHash("fr-FR"),
MurMur3::calcHash("it-IT"),
MurMur3::calcHash("tr-TR"),
MurMur3::calcHash("pl-PL"),
MurMur3::calcHash("el-GR"),
MurMur3::calcHash("nl-NL"),
MurMur3::calcHash("cs-CZ"),
MurMur3::calcHash("uk-UA"),
MurMur3::calcHash("ru-RU"),
MurMur3::calcHash("id-ID"),
MurMur3::calcHash("fa-IR"),
MurMur3::calcHash("ar-SA"),
MurMur3::calcHash("ain")
        };

        // シミュレーションの Key
        std::vector<std::uint_least32_t> simulation_key;
        // シミュレーションモデル名
        std::vector<std::string> simulation_model_name;

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

        // シミュレーションのモデル番号
        std::size_t simulation_model_index = 0;
#ifdef PAXS_USING_SIV3D
        // UI の影
        s3d::RenderTexture shadow_texture{};
        s3d::RenderTexture internal_texture{};
#endif
        paxs::Pulldown pulldown;
#ifdef PAXS_USING_SIMULATOR
        paxs::Pulldown simulation_pulldown;
#endif
        paxs::MenuBar menu_bar;

        paxs::KeyValueTSV<paxg::Texture> key_value_tsv;

        //const std::string map_license_name = U"Maptiles by\n淺野孝利 2023「古墳時代の『常総の内海』水域復原に関する一試論」\n研究代表者 荒井啓汰『埋葬施設からみた常総地域の地域構造』\n特別研究員奨励費報告書 筑波大学大学院 人文社会科学研究科";
        //std::string map_license_name = reinterpret_cast<const char*>(u8"Maptiles by\n農研機構農業環境研究部門, under CC BY 2.1 JP.\n20万分の1シームレス地質図V2.\nOpenStreetMap contributors, under ODbL.");
        //const std::string map_license_name = U"Maptiles by MIERUNE, under CC BY. Data by OpenStreetMap contributors, under ODbL.\nMaptiles by 農研機構農業環境研究部門, under CC BY 2.1 JP";

        void init(
            const SelectLanguage& select_language,
            const paxs::Language& language_text,
            const paxs::Language& simulation_text
        ) {
            language_fonts.setDefaultPath("Data/Font/noto-sans-sc/NotoSansSC-Regular.otf");
            setLanguageFont(pulldown_font_size, AppConfig::getInstance()->getRootPath(), pulldown_font_buffer_thickness_size);
            //koyomi_font = setFont(koyomi_font_size, AppConfig::getInstance()->getRootPath(), koyomi_font_buffer_thickness_size);
            map_view_width_str_index = (MurMur3::calcHash(25, "debug_magnification_power"));
            map_view_center_x_str_index = (MurMur3::calcHash(24, "debug_mercator_longitude"));
            map_view_center_y_str_index = (MurMur3::calcHash(23, "debug_mercator_latitude"));
            map_view_center_lat_str_index = (MurMur3::calcHash(14, "debug_latitude"));
            xyz_tile_z_str_index = (MurMur3::calcHash(17, "debug_xyz_tiles_z"));

            pulldown = paxs::Pulldown(&select_language, &language_text, language_key, language_fonts, static_cast<std::uint_least8_t>(pulldown_font_size), static_cast<std::uint_least8_t>(pulldown_font_buffer_thickness_size), paxg::Vec2i{ 3000, 0 }, 0, true);
            pulldown.setPos(paxg::Vec2i{ static_cast<int>(paxg::Window::width() - pulldown.getRect().w()), 0 });
#ifdef PAXS_USING_SIMULATOR
            {
                // シミュレーションモデルのファイルを読み込む
                const std::string models_path = "Data/Simulations/Models.txt";
                paxs::InputFile models_tsv(AppConfig::getInstance()->getRootPath() + models_path);
                if (models_tsv.fail()) {
                    PAXS_WARNING("Failed to read Models TXT file: " + models_path);
                    simulation_model_name.emplace_back("Sample");
                    simulation_key.emplace_back(MurMur3::calcHash("Sample"));
                }
                else {
                    // 1 行目を読み込む
                    if (!(models_tsv.getLine())) {
                        simulation_model_name.emplace_back("Sample");
                        simulation_key.emplace_back(MurMur3::calcHash("Sample"));
                    }
                    else {
                        // BOM を削除
                        models_tsv.deleteBOM();
                        // 1 行目を分割する
                        simulation_model_name.emplace_back(models_tsv.pline);
                        simulation_key.emplace_back(MurMur3::calcHash(models_tsv.pline.c_str()));
                        // 1 行ずつ読み込み（区切りはタブ）
                        while (models_tsv.getLine()) {
                            simulation_model_name.emplace_back(models_tsv.pline);
                            simulation_key.emplace_back(MurMur3::calcHash(models_tsv.pline.c_str()));
                        }
                    }
                }
            }
            // シミュレーションモデルのプルダウンメニューを初期化
            simulation_pulldown = paxs::Pulldown(&select_language, &simulation_text, simulation_key, language_fonts, static_cast<std::uint_least8_t>(pulldown_font_size), static_cast<std::uint_least8_t>(pulldown_font_buffer_thickness_size), paxg::Vec2i{ 3000, 0 }, 0, false);
            simulation_pulldown.setPos(paxg::Vec2i{ static_cast<int>(paxg::Window::width() - simulation_pulldown.getRect().w() - 200), 600 });
#endif
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


            std::vector<std::uint_least32_t> list_test6 = {
            MurMur3::calcHash("place_names"),
                MurMur3::calcHash("place_names_place_name"),
                MurMur3::calcHash("place_names_site"),
                MurMur3::calcHash("place_names_tumulus"),
                MurMur3::calcHash("place_names_dolmen"),
                MurMur3::calcHash("place_names_kamekanbo"),
                MurMur3::calcHash("place_names_stone_coffin"),
                MurMur3::calcHash("place_names_doken"),
                MurMur3::calcHash("place_names_dotaku"),
                MurMur3::calcHash("place_names_bronze_mirror"),
                MurMur3::calcHash("place_names_human_bone"),
                MurMur3::calcHash("place_names_mtdna"),
                MurMur3::calcHash("place_names_ydna")
            };
            //std::vector<std::uint_least32_t> list_test4 = {
            //MurMur3::calcHash("menu_bar_calendar"),
            //    MurMur3::calcHash("menu_bar_calendar_japan"),
            //    MurMur3::calcHash("menu_bar_calendar_gregorian"),
            //    MurMur3::calcHash("menu_bar_calendar_julian"),
            //    MurMur3::calcHash("menu_bar_calendar_julian_day"),
            //    MurMur3::calcHash("menu_bar_calendar_hijri")
            //};
            std::vector<std::uint_least32_t> list_test5 = {
            MurMur3::calcHash("menu_bar_map"),
                MurMur3::calcHash("menu_bar_map_base"),
                MurMur3::calcHash("menu_bar_map_land_and_sea"),
                MurMur3::calcHash("menu_bar_map_land_and_water"),
                MurMur3::calcHash("menu_bar_map_soil"),
                MurMur3::calcHash("menu_bar_map_soil_temperature"),
                MurMur3::calcHash("menu_bar_map_ryosei_country"),
                MurMur3::calcHash("menu_bar_map_ryosei_line"),
                MurMur3::calcHash("menu_bar_map_slope"),
                MurMur3::calcHash("menu_bar_map_lakes_and_rivers1"),
                MurMur3::calcHash("menu_bar_map_lakes_and_rivers2"),
                MurMur3::calcHash("menu_bar_map_line1"),
                MurMur3::calcHash("menu_bar_map_line2")
            };
            //menu_bar.add(&select_language, &language_text, list_test1, language_fonts, static_cast<std::uint_least8_t>(pulldown_font_size), static_cast<std::uint_least8_t>(pulldown_font_buffer_thickness_size), MurMur3::calcHash("file"));
            //menu_bar.add(&select_language, &language_text, list_test2, language_fonts, static_cast<std::uint_least8_t>(pulldown_font_size), static_cast<std::uint_least8_t>(pulldown_font_buffer_thickness_size), MurMur3::calcHash("edit"));
            menu_bar.add(&select_language, &language_text, list_test3, language_fonts, static_cast<std::uint_least8_t>(pulldown_font_size), static_cast<std::uint_least8_t>(pulldown_font_buffer_thickness_size), MurMur3::calcHash("view"));
            menu_bar.add(&select_language, &language_text, list_test6, language_fonts, static_cast<std::uint_least8_t>(pulldown_font_size), static_cast<std::uint_least8_t>(pulldown_font_buffer_thickness_size), MurMur3::calcHash("place_names"));
            //menu_bar.add(&select_language, &language_text, list_test4, language_fonts, static_cast<std::uint_least8_t>(pulldown_font_size), static_cast<std::uint_least8_t>(pulldown_font_buffer_thickness_size), MurMur3::calcHash("calendar"));
            menu_bar.add(&select_language, &language_text, list_test5, language_fonts, static_cast<std::uint_least8_t>(pulldown_font_size), static_cast<std::uint_least8_t>(pulldown_font_buffer_thickness_size), MurMur3::calcHash("map"));

            const std::string path = (AppConfig::getInstance()->getRootPath());
            // 暦の時間操作のアイコン
            key_value_tsv.input(path + "Data/MenuIcon/MenuIcons.tsv", [&](const std::string& value_) { return paxg::Texture{ path + value_ }; });

            //koyomi_font = setFont(koyomi_font_size, AppConfig::getInstance()->getRootPath(), 2);

            pin_font = paxg::Font{ 14 /*, Typeface::Bold*/
                , (path + "Data/Font/noto-sans-jp/NotoSansJP-Regular.otf"), 2 };

            //pin_font = paxg::Font{ 18 /*, Typeface::Bold*/
            //    , (path + "Data/Font/noto-sans-jp/NotoSansJP-Regular.otf"), 2 };

            // 英語用フォント
            en_font = paxg::Font{ 20 /*, Typeface::Bold*/
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
            std::unique_ptr<paxs::SettlementSimulator>& simulator, // コンパイル時の分岐により使わない場合あり
#endif
            paxs::TouchManager& tm_,
            paxs::KoyomiSiv3D& koyomi_siv,
            paxs::GraphicVisualizationList& visible
            ) {
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

            koyomi_font_x = paxg::Window::width() - 270;
            koyomi_font_en_x = koyomi_font_x - 20;
            int rect_start_x = koyomi_font_en_x - 165;
            int rect_len_x = paxg::Window::width() - rect_start_x - 15;

            int koyomi_height = static_cast<int>(koyomi_siv.date_list.size()) * (koyomi_font_size * 4 / 3); // 暦の縦の幅

            const int arrow_time_icon_size = 40; // 時間操作アイコンの大きさ
            const int time_icon_size = 50; // 時間操作アイコンの大きさ
            int icon_const_start_x = rect_len_x - 10;

            int icon_start_x = icon_const_start_x;

            int sum_icon_height = arrow_time_icon_size + time_icon_size * 2;

            int icon_start_y = koyomi_height + 40;// 80 + 30;
            const int arrow_icon_move_x = int(arrow_time_icon_size * 1.2);// int(time_icon_size * 1.4);
            const int icon_move_x = int(time_icon_size * 1.1);// int(time_icon_size * 1.4);

            const int arrow_icon_move_y = int(arrow_time_icon_size * 1.1);
            const int icon_move_y = int(time_icon_size * 1.1);

            int next_rect_start_y = icon_start_y + sum_icon_height + 50;//230;
            int next_rect_end_y = 280;//380;

#ifdef PAXS_USING_SIV3D
                // 影を作る図形を shadow_texture に描く
            {
                const s3d::ScopedRenderTarget2D target{ shadow_texture.clear(s3d::ColorF{ 1.0, 0.0 }) };
                const s3d::ScopedRenderStates2D blend{ s3d::BlendState::MaxAlpha };
                const s3d::Transformer2D transform{ s3d::Mat3x2::Translate(3, 3) };
                paxg::Rect{ 0, 0, static_cast<float>(paxg::Window::width()), static_cast<float>(pulldown.getRect().h()) }.draw(); // メニューバー

                if (visible[MurMur3::calcHash(8, "Calendar")] && visible[MurMur3::calcHash(2, "UI")]) {
                    paxg::RoundRect{ rect_start_x, koyomi_font_y - 15, rect_len_x, next_rect_start_y, 10 }.draw();
                    paxg::RoundRect{ rect_start_x, koyomi_font_y + next_rect_start_y + 5, rect_len_x, next_rect_end_y, 10 }.draw();
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
                paxg::RoundRect{ rect_start_x, koyomi_font_y - 15, rect_len_x, next_rect_start_y, 10 }.draw(paxg::Color{ 255, 255, 255 }/*s3d::Palette::White*/);
                paxg::RoundRect{ rect_start_x, koyomi_font_y + next_rect_start_y + 5, rect_len_x, next_rect_end_y, 10 }.draw(paxg::Color{ 255, 255, 255 }/*s3d::Palette::White*/);
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

                            if (text_str != nullptr) (*one_font).drawTopRight(*text_str//std::string(koyomi_siv.date_list[i].calendar_name[select_language.cget() + 1 /* 言語位置調整 */])
                                , paxg::Vec2i(static_cast<int>(koyomi_font_en_x), static_cast<int>(koyomi_font_en_y + i * (koyomi_font_size * 4 / 3))), paxg::Color(0, 0, 0));

                            (*one_font).drawTopRight(",", paxg::Vec2i(static_cast<int>(int(95 * koyomi_font_size / 30.0) + koyomi_font_en_x), static_cast<int>(koyomi_font_en_y + i * (koyomi_font_size * 4 / 3))), paxg::Color(0, 0, 0));
                            (*one_font).drawTopRight(",", paxg::Vec2i(static_cast<int>(int(235 * koyomi_font_size / 30.0) + koyomi_font_en_x), static_cast<int>(koyomi_font_en_y + i * (koyomi_font_size * 4 / 3))), paxg::Color(0, 0, 0));
                            (*one_font).drawTopRight("th", paxg::Vec2i(static_cast<int>(int(315 * koyomi_font_size / 30.0) + koyomi_font_en_x), static_cast<int>(koyomi_font_en_y + i * (koyomi_font_size * 4 / 3))), paxg::Color(0, 0, 0));

                            (*one_font).drawTopRight(std::to_string(date_year), paxg::Vec2i(static_cast<int>(int(en_cal_name_pos_x * koyomi_font_size / 30.0) + koyomi_font_en_x), static_cast<int>(koyomi_font_en_y + i * (koyomi_font_size * 4 / 3))), paxg::Color(0, 0, 0));
                            (*one_font).drawTopRight(std::string(koyomi_siv.month_name[date_month]), paxg::Vec2i(static_cast<int>(int(220 * koyomi_font_size / 30.0) + koyomi_font_en_x), static_cast<int>(koyomi_font_en_y + i * (koyomi_font_size * 4 / 3))), paxg::Color(0, 0, 0));
                            (*one_font).drawTopRight(std::to_string(date_day), paxg::Vec2i(static_cast<int>(int(280 * koyomi_font_size / 30.0) + koyomi_font_en_x), static_cast<int>(koyomi_font_en_y + i * (koyomi_font_size * 4 / 3))), paxg::Color(0, 0, 0));
                            if (simulator == nullptr) {
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

                        // マップの高さ
                        (*one_font).setOutline(0, 0.6, paxg::Color(255, 255, 255));
                        (*one_font).drawTopRight(((select_language.cgetKey() == MurMur3::calcHash("ja-JP")) ?
                            reinterpret_cast<const char*>(u8"拡大率") :
                            "Zoom rate"),
                            paxg::Vec2i(paxg::Window::width() - 40, debug_start_y), paxg::Color(0, 0, 0));
                        (*one_font).drawTopRight(std::to_string(map_view.getHeight()),
                            paxg::Vec2i(paxg::Window::width() - 40, debug_start_y + 30), paxg::Color(0, 0, 0));

                        //map_view.setHeight(11.0);
                        //map_view.setCenterX(134.0);
                        //map_view.setCenterY(36.8);

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

            // メニューバー
            paxg::Rect{ 0, 0, static_cast<float>(paxg::Window::width()), static_cast<float>(pulldown.getRect().h()) }.draw(paxg::Color{ 243, 243, 243 });
#ifdef PAXS_USING_SIMULATOR            // シミュレーションのボタン
            if (visible[MurMur3::calcHash("Simulation")] && visible[MurMur3::calcHash("UI")] && visible[MurMur3::calcHash("Calendar")]) {
                if (simulator == nullptr) {
                    paxg::Rect{ 0, 0, static_cast<float>(paxg::Window::width()), static_cast<float>(simulation_pulldown.getRect().h()) }.draw(paxg::Color{ 243, 243, 243 });
                }
            }
#endif

#ifdef PAXS_USING_SIV3D
            const std::unordered_map<std::uint_least32_t, paxg::Texture>& texture_dictionary = key_value_tsv.get();

            texture_dictionary.at(MurMur3::calcHash(14, "texture_github")).resizedDraw(24, paxg::Vec2i{ paxg::Window::width() - 280, 3 });
            if (tm_.get(s3d::Rect(paxg::Window::width() - 280, 3, 28).leftClicked())) {
                // Web ページをブラウザで開く
                s3d::System::LaunchBrowser(U"https://github.com/AsPJT/PAX_SAPIENTICA");
            }
#endif
#ifdef PAXS_USING_SIMULATOR
            // シミュレーションのボタン
            if (visible[MurMur3::calcHash("Simulation")] && visible[MurMur3::calcHash("UI")] && visible[MurMur3::calcHash("Calendar")]) {
                if (simulator == nullptr) {
                    simulation_pulldown.draw(); // シミュレーション選択
                }
            }
#endif
            pulldown.draw(); // 言語選択
            menu_bar.draw(); // 左上メニューバー

            //if (visible[MurMur3::calcHash("UI")])

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
                    language_key[i],
                    static_cast<std::uint_least8_t>(font_size_),
                    static_cast<std::uint_least8_t>(buffer_thickness)
                );
            }
        }

    };

}

#endif // !PAX_MAHOROBA_STRING_VIEWER_HPP
