/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/
//#define PAXS_DEVELOPMENT
#ifndef PAX_MAHOROBA_MAIN_HPP
#define PAX_MAHOROBA_MAIN_HPP

/*##########################################################################################

##########################################################################################*/

#if defined(PAXS_USING_DXLIB) && defined(__ANDROID__)
// シミュレーションを使用しない
#else
#define PAXS_USING_SIMULATOR
#endif

#include <PAX_GRAPHICA/Vec2.hpp>

#if defined(PAXS_USING_DXLIB) || defined(PAXS_USING_SFML)
static int old_left_touch = 0;
static paxg::Vec2i old_left_touch_pos = paxg::Vec2i{ 0,0 };
#endif

#include <PAX_MAHOROBA/Init.hpp>
#include <PAX_MAHOROBA/InitLogo.hpp>
#include <PAX_MAHOROBA/LocationRange.hpp>
#include <PAX_MAHOROBA/LocationPoint.hpp>
#include <PAX_MAHOROBA/3DModel.hpp>
#include <PAX_MAHOROBA/Pulldown.hpp>
#include <PAX_SAPIENTICA/Language.hpp>

#ifdef PAXS_USING_SIMULATOR
#include <PAX_SAPIENTICA/Simulation/SimulationConst.hpp>
#include <PAX_SAPIENTICA/Simulation/Simulator.hpp>
#endif
#include <PAX_MAHOROBA/XYZTiles.hpp>
#include <PAX_MAHOROBA/XYZTilesList.hpp>

// シミュレータを使用する

#include <PAX_MAHOROBA/Calendar.hpp> // 暦
#include <PAX_SAPIENTICA/GraphicVisualizationList.hpp> // 可視化一覧
#include <PAX_MAHOROBA/MapViewer.hpp> // 地図
#include <PAX_MAHOROBA/StringViewer.hpp> // 文字

#include <PAX_SAPIENTICA/TouchManager.hpp>

#include <PAX_SAPIENTICA/Math.hpp>

#include <PAX_GRAPHICA/Key.hpp>
#include <PAX_GRAPHICA/Mouse.hpp>

#include <PAX_SAPIENTICA/AppConfig.hpp>
#include <PAX_SAPIENTICA/InputFile/KeyValueTSV.hpp>
#include <PAX_SAPIENTICA/MurMur3.hpp>

namespace paxs {

    // 主要な実行時定数・変数
    void startMain() { // フォルダ階層

        // 可視化一覧
        GraphicVisualizationList visible{};
        visible.emplace(MurMur3::calcHash("Calendar"), true); // 暦
        visible.emplace(MurMur3::calcHash("Map"), true); // 地図
        visible.emplace(MurMur3::calcHash("UI"), true); // UI
        //visible.emplace(MurMur3::calcHash("License"), false); // ライセンス
        visible.emplace(MurMur3::calcHash("3D"), false); // 3D

        MapView map_view{};
        XYZTilesList xyz_tile_list; // 描画する XYZ タイルを管理
        paxs::KoyomiSiv3D koyomi_siv{}; // 暦を管理する
        paxs::StringViewerSiv3D string_siv{}; // 文字を管理する
        SelectLanguage select_language{}; // 選択言語
        paxs::Language language_text;
        paxs::MapViewerSiv3D map_siv{}; // 地図を管理する
        paxs::TouchManager tm; // 画面のクリック・タッチを管理する

        paxs::PaxSapienticaInitSiv3D::firstInit(); // 初期化とロゴの表示

        // XYZ タイルを初期化
        AppConfig::getInstance()->calcDataSettings(MurMur3::calcHash("XYZTiles"),
            [&](const std::string& path_) {xyz_tile_list.add(path_); });

        xyz_tile_list.addGridLine(); // グリッド線を追加 （描画順が最後なので最後に追加）
        map_view.setWidth(map_view.getHeight() / double(paxg::Window::height()) * double(paxg::Window::width()));
        xyz_tile_list.update(string_siv.menu_bar, map_view, koyomi_siv.jdn.cgetDay()); // 地図の辞書を更新
        paxg::Window::update();
#ifdef PAXS_USING_SFML
        xyz_tile_list.update(string_siv.menu_bar, map_view, koyomi_siv.jdn.cgetDay()); // 地図の辞書を更新
        paxg::Window::update();
#endif
        // 言語を初期化（テキストの多言語対応クラス）
        AppConfig::getInstance()->calcDataSettings(MurMur3::calcHash("Languages"),
            [&](const std::string& path_) {language_text.add(path_); });
        //language_text.add(AppConfig::getInstance()->getRootPath() + "Data/Settings/Languages.tsv");
        string_siv.init(select_language, language_text);

        int old_width = paxg::Window::width(); // 1 フレーム前の幅
        int old_height = paxg::Window::height(); // 1 フレーム前の高さ

        int size_change_count = 0; // サイズを更新するカウンタ

        map_siv.init();
        koyomi_siv.init();

        xyz_tile_list.update(string_siv.menu_bar, map_view, koyomi_siv.jdn.cgetDay()); // 地図の辞書を更新

        paxs::Graphics3DModel g3d_model; // 3D モデル

#ifdef PAXS_USING_SIMULATOR
        std::unique_ptr<paxs::SettlementSimulator> simulator{};

        SimulationRange sr;
        // シミュレーションの範囲を設定
        AppConfig::getInstance()->calcDataSettings(MurMur3::calcHash("SimulationRange"),
            [&](const std::string& path_) {sr.input(path_); });
#endif

        /*##########################################################################################
            ループ開始
        ##########################################################################################*/
        while (paxg::Window::update()) {
            paxg::Mouse::getInstance()->calledFirstEveryFrame(); // 入力を更新

            tm.init(); // タッチ判定を初期化
#ifdef PAXS_USING_SIV3D
            const s3d::ScopedRenderStates2D sampler{ s3d::SamplerState::ClampNearest }; // 画像の拡大縮小の方式を設定
#endif
            /*##########################################################################################
                更新処理関連
            ##########################################################################################*/

            // 画面サイズの変更に合わせて地図の幅を変える
            if (old_height != paxg::Window::height()) {
                map_view.setHeight(paxg::Window::height() * map_view.getHeight() / old_height);
                map_view.setWidth(map_view.getHeight() / double(paxg::Window::height()) * double(paxg::Window::width()));
            }
            if (old_width != paxg::Window::width()) {
                map_view.setWidth(map_view.getHeight() / double(paxg::Window::height()) * double(paxg::Window::width()));
            }
            if (old_width != paxg::Window::width() ||
                old_height != paxg::Window::height()) {
                // 影を定義
                if (size_change_count < 1) {
#ifdef PAXS_USING_SIV3D
                    string_siv.shadow_texture = s3d::RenderTexture{ s3d::Scene::Size(), s3d::ColorF{ 1.0, 0.0 } };
                    string_siv.internal_texture = s3d::RenderTexture{ string_siv.shadow_texture.size() };
#endif
                }
                if (size_change_count >= 100) size_change_count = 100;
                ++size_change_count;
            }
            else size_change_count = 0;

            old_width = paxg::Window::width();
            old_height = paxg::Window::height();


            if (visible[MurMur3::calcHash(2, "3D")]) {
                map_view.update(); // キーボード入力を更新
            }

            // プルダウンを更新
            string_siv.pulldown.setPos(paxg::Vec2i{ static_cast<int>(paxg::Window::width() - string_siv.pulldown.getRect().w()), 0 });
            string_siv.pulldown.update(tm);
            select_language.set(std::size_t(string_siv.pulldown.getIndex())); // 選択言語を更新
            select_language.setKey(std::uint_least32_t(string_siv.pulldown.getKey())); // 選択言語を更新
            string_siv.menu_bar.update(tm);

            // 表示の可視化を更新
            //Calendar Map UI Simulation License Debug 3D
            visible.set(MurMur3::calcHash("Calendar"), string_siv.menu_bar.getPulldown(MurMur3::calcHash("view")).getIsItems(0));
            visible.set(MurMur3::calcHash("Map"), string_siv.menu_bar.getPulldown(MurMur3::calcHash("view")).getIsItems(1));
            visible.set(MurMur3::calcHash("UI"), string_siv.menu_bar.getPulldown(MurMur3::calcHash("view")).getIsItems(2));
            visible.set(MurMur3::calcHash("Simulation"), string_siv.menu_bar.getPulldown(MurMur3::calcHash("view")).getIsItems(3));
            //visible.set(MurMur3::calcHash("License"), string_siv.menu_bar.getPulldown(MurMur3::calcHash("view")).getIsItems(4));
            //visible.set(MurMur3::calcHash("Debug"), string_siv.menu_bar.getPulldown(MurMur3::calcHash("view")).getIsItems(5));
            visible.set(MurMur3::calcHash("3D"), string_siv.menu_bar.getPulldown(MurMur3::calcHash("view")).getIsItems(6));

            visible.set(MurMur3::calcHash("tomb"), string_siv.menu_bar.getPulldown(MurMur3::calcHash("place_names")).getIsItems(0));
            visible.set(MurMur3::calcHash("stone_coffin"), string_siv.menu_bar.getPulldown(MurMur3::calcHash("place_names")).getIsItems(1));
            visible.set(MurMur3::calcHash("dotaku"), string_siv.menu_bar.getPulldown(MurMur3::calcHash("place_names")).getIsItems(2));
            visible.set(MurMur3::calcHash("kamekanbo"), string_siv.menu_bar.getPulldown(MurMur3::calcHash("place_names")).getIsItems(3));
            visible.set(MurMur3::calcHash("place_name"), string_siv.menu_bar.getPulldown(MurMur3::calcHash("place_names")).getIsItems(4));
            visible.set(MurMur3::calcHash("site"), string_siv.menu_bar.getPulldown(MurMur3::calcHash("place_names")).getIsItems(5));
            visible.set(MurMur3::calcHash("mtdna"), string_siv.menu_bar.getPulldown(MurMur3::calcHash("place_names")).getIsItems(6));
            visible.set(MurMur3::calcHash("doken"), string_siv.menu_bar.getPulldown(MurMur3::calcHash("place_names")).getIsItems(7));

            if (visible[MurMur3::calcHash(2, "3D")]) {

            xyz_tile_list.update(string_siv.menu_bar, map_view, koyomi_siv.jdn.cgetDay()); // 地図の辞書を更新
                // 地図を更新
                map_siv.update(
                    map_view,
                    select_language,
                    koyomi_siv,
                    string_siv,
#ifdef PAXS_USING_SIMULATOR
                    simulator,
#endif
                    visible
                );
                // 暦を更新
                koyomi_siv.update(
#ifdef PAXS_USING_SIMULATOR
                    simulator
#endif
                );
            }
            else if (!visible[MurMur3::calcHash(2, "3D")]) {
                g3d_model.updateRotation(); // 3D モデルを回転させる
            }

            // 文字を更新
            string_siv.update(
                map_view,
                select_language,
                language_text,
#ifdef PAXS_USING_SIMULATOR
                simulator,
#endif
                tm,
                koyomi_siv,
                visible
            );

#ifdef PAXS_USING_DXLIB
            old_left_touch = DxLib::GetTouchInputNum();
            if (old_left_touch >= 1) {
                int pos_x = 0, pos_y = 0;
                DxLib::GetTouchInput(0, &pos_x, &pos_y, NULL, NULL);
                old_left_touch_pos = paxg::Vec2i(pos_x, pos_y);
            }
#endif
        }
    }
}

#endif // !PAX_MAHOROBA_MAIN_HPP
