/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023 As Project
    [Production]	2023 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SIV3D_MAIN_HPP
#define PAX_SAPIENTICA_SIV3D_MAIN_HPP

/*##########################################################################################

##########################################################################################*/


#include <PAX_SAPIENTICA/Siv3D/Init.hpp>
#include <PAX_SAPIENTICA/Siv3D/InitLogo.hpp>
#include <PAX_SAPIENTICA/Siv3D/LocationRange.hpp>
#include <PAX_SAPIENTICA/Siv3D/LocationPoint.hpp>
#include <PAX_SAPIENTICA/Siv3D/3DModel.hpp>
#include <PAX_SAPIENTICA/Siv3D/Pulldown.hpp>
#include <PAX_SAPIENTICA/Siv3D/Language.hpp>
#include <PAX_SAPIENTICA/Siv3D/Key.hpp>
#include <PAX_SAPIENTICA/Simulation/Simulator.hpp>
#include <PAX_SAPIENTICA/Siv3D/XYZTiles.hpp>
#include <PAX_SAPIENTICA/Siv3D/XYZTilesList.hpp>

// シミュレータを使用する
#define PAXS_USING_SIMULATOR
#include <PAX_SAPIENTICA/Siv3D/Calendar.hpp> // 暦
#include <PAX_SAPIENTICA/GraphicVisualizationList.hpp> // 可視化一覧
#include <PAX_SAPIENTICA/Siv3D/MapViewer.hpp> // 地図
#include <PAX_SAPIENTICA/Siv3D/StringViewer.hpp> // 文字

#include <PAX_SAPIENTICA/TouchManager.hpp>

#include <PAX_SAPIENTICA/Math.hpp>

namespace paxs {

    void startMain() {

        // 主要な実行時定数・変数
        const std::string path8 = "./../../../../../"; // フォルダ階層
        SelectLanguage select_language{}; // 選択言語

        paxs::PaxSapienticaInitSiv3D::firstInit(); // 初期化とロゴの表示
        s3d::detail::Console_impl{}.open(); // コンソールを開く s3d::Console::Open()
        paxs::Language language_text(path8 + "Data/Language/Text.txt"); // テキストの多言語対応クラス

        // 可視化一覧
        GraphicVisualizationList visible{};
        visible.emplace("Calendar", true); // 暦
        visible.emplace("Map", true); // 地図
        visible.emplace("UI", true); // UI
        visible.emplace("License", false); // ライセンス
        visible.emplace("3D", false); // 3D

        //#ifdef PAXS_USING_SIMULATOR
        paxs::Simulator<int> simlator;
        // 対馬のみ
        //paxs::Vector2<int> start_position = paxs::Vector2<int>{ 879, 406 };
        //paxs::Vector2<int> end_position = paxs::Vector2<int>{ 881, 409 };
        // 本州
        paxs::Vector2<int> start_position = paxs::Vector2<int>{ 877, 381 };
        paxs::Vector2<int> end_position = paxs::Vector2<int>{ 917, 422 };
        //#endif
        int old_width = s3d::Scene::Width(); // 1 フレーム前の幅
        int old_height = s3d::Scene::Height(); // 1 フレーム前の高さ

        int size_change_count = 0; // サイズを更新するカウンタ

        paxs::MapViewerSiv3D map_siv{}; // 地図を管理する
        map_siv.init(path8);

        paxs::KoyomiSiv3D koyomi_siv{}; // 暦を管理する
        koyomi_siv.init(language_text, path8);

        paxs::StringViewerSiv3D string_siv{}; // 文字を管理する
        string_siv.init(language_text, path8);

        paxs::TouchManager tm; // 画面のクリック・タッチを管理する

        /*##########################################################################################
            ループ開始
        ##########################################################################################*/
        while (s3d::System::Update()) {
            tm.init(); // タッチ判定を初期化
            const s3d::ScopedRenderStates2D sampler{ s3d::SamplerState::ClampNearest }; // 画像の拡大縮小の方式を設定
            /*##########################################################################################
                更新処理関連
            ##########################################################################################*/

            // 画面サイズの変更に合わせて地図の幅を変える
            if (old_width != s3d::Scene::Width()) {
                map_siv.map_view->setWidth(s3d::Scene::Width() * map_siv.map_view->getWidth() / old_width);
                map_siv.map_view->setHeight(map_siv.map_view->getWidth() / double(s3d::Scene::Width()) * double(s3d::Scene::Height()));
            }
            if (old_height != s3d::Scene::Height()) {
                map_siv.map_view->setHeight(map_siv.map_view->getWidth() / double(s3d::Scene::Width()) * double(s3d::Scene::Height()));
            }
            if (old_width != s3d::Scene::Width() ||
                old_height != s3d::Scene::Height()) {
                // 影を定義
                if (size_change_count < 1) {
                    string_siv.shadow_texture = s3d::RenderTexture{ s3d::Scene::Size(), s3d::ColorF{ 1.0, 0.0 } };
                    string_siv.internal_texture = s3d::RenderTexture{ string_siv.shadow_texture.size() };
                }
                if (size_change_count >= 100) size_change_count = 100;
                ++size_change_count;
            }
            else size_change_count = 0;

            old_width = s3d::Scene::Width();
            old_height = s3d::Scene::Height();

            // プルダウンを更新
            string_siv.pulldown.setPos(s3d::Point{ s3d::Scene::Width() - string_siv.pulldown.getRect().w, 0 });
            string_siv.pulldown.update(SelectLanguage{}, tm);
            select_language.set(std::size_t(string_siv.pulldown.getIndex())); // 選択言語を更新
            string_siv.menu_bar.update(select_language, tm);

            // 表示の可視化を更新
            //Calendar Map UI Simulation License Debug 3D
            visible.set("Calendar", string_siv.menu_bar.getPulldown(MenuBarType::view).getIsItems(0));
            visible.set("Map", string_siv.menu_bar.getPulldown(MenuBarType::view).getIsItems(1));
            visible.set("UI", string_siv.menu_bar.getPulldown(MenuBarType::view).getIsItems(2));
            visible.set("Simulation", string_siv.menu_bar.getPulldown(MenuBarType::view).getIsItems(3));
            visible.set("License", string_siv.menu_bar.getPulldown(MenuBarType::view).getIsItems(4));
            visible.set("Debug", string_siv.menu_bar.getPulldown(MenuBarType::view).getIsItems(5));
            visible.set("3D", string_siv.menu_bar.getPulldown(MenuBarType::view).getIsItems(6));

            // 地図を更新
            map_siv.update(
                select_language,
                koyomi_siv,
                string_siv,
                simlator,
                start_position,
                visible
            );
            // 暦を更新
            koyomi_siv.update(
                language_text,
                simlator
            );
            // 文字を更新
            string_siv.update(
                map_siv.map_view,
                select_language,
                language_text,
                simlator,
                start_position,
                end_position,
                path8,
                tm,
                koyomi_siv,
                visible
            );
            paxs::PaxSapienticaInitSiv3D::secondInit(); // ソフトウェアを実行した最初のフレームの一番最後に実行
        }
    }
}

#endif // !PAX_SAPIENTICA_SIV3D_MAIN_HPP
