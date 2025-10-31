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

#if defined(PAXS_USING_DXLIB) && defined(__ANDROID__)
// シミュレーションを使用しない
#else
#define PAXS_USING_SIMULATOR
#endif

#ifdef PAXS_USING_SIMULATOR
#include <PAX_SAPIENTICA/Simulation/SimulationConst.hpp>
#include <PAX_SAPIENTICA/Simulation/Simulator.hpp>
#endif

#include <PAX_GRAPHICA/Vec2.hpp>
#include <PAX_GRAPHICA/3DModel.hpp>
#include <PAX_GRAPHICA/Key.hpp>
#include <PAX_GRAPHICA/Mouse.hpp>
#include <PAX_GRAPHICA/TouchInput.hpp>

#include <PAX_MAHOROBA/Core/InitLogo.hpp>
#include <PAX_MAHOROBA/Map/MapController.hpp>
#include <PAX_MAHOROBA/Map/MapViewport.hpp>
#include <PAX_MAHOROBA/UI/Pulldown.hpp>
#include <PAX_MAHOROBA/Map/Tile/TileManager.hpp>
#include <PAX_MAHOROBA/UI/UIManager.hpp>

#include <PAX_SAPIENTICA/AppConfig.hpp>
#include <PAX_SAPIENTICA/Calendar/Koyomi.hpp>
#include <PAX_SAPIENTICA/FeatureVisibilityManager.hpp>
#include <PAX_SAPIENTICA/Language.hpp>
#include <PAX_SAPIENTICA/InputStateManager.hpp>
#include <PAX_SAPIENTICA/MurMur3.hpp>

namespace paxs {

    // 主要な実行時定数・変数
    void startMain() { // フォルダ階層
        paxs::PaxSapienticaInit::firstInit(); // 初期化とロゴの表示

        // 可視化一覧
        FeatureVisibilityManager visible{};
        visible.emplace(MurMur3::calcHash("Calendar"), true); // 暦
        visible.emplace(MurMur3::calcHash("Map"), true); // 地図
        visible.emplace(MurMur3::calcHash("UI"), true); // UI
        visible.emplace(MurMur3::calcHash("Simulation"), true); // シミュレーション
        visible.emplace(MurMur3::calcHash("License"), false); // ライセンス
        visible.emplace(MurMur3::calcHash("Debug"), false); // デバッグ
        visible.emplace(MurMur3::calcHash("3D"), false); // 3D

        MapViewport map_viewport{};
        TileManager tile_manager; // 描画する タイルを管理
        paxs::Koyomi koyomi{}; // 暦を管理する
        paxs::UIManager ui_manager{}; // UIを統合管理する
        SelectLanguage select_language{}; // 選択言語
        paxs::Language language_text;
        paxs::Language simulation_text;
        paxs::MapController map_controller{}; // 地図を統合管理する
        paxs::InputStateManager input_state_manager; // 画面のクリック・タッチを管理する


        // XYZ タイルを初期化
        AppConfig::getInstance()->calcDataSettings(MurMur3::calcHash("XYZTiles"),
            [&](const std::string& path_) {tile_manager.add(path_); });

        tile_manager.addGridLine(); // グリッド線を追加 （描画順が最後なので最後に追加）
        map_viewport.setWidth(map_viewport.getHeight() / double(paxg::Window::height()) * double(paxg::Window::width()));
        tile_manager.update(ui_manager.menu_bar, map_viewport, koyomi.jdn.cgetDay()); // 地図の辞書を更新
        // 言語を初期化（テキストの多言語対応クラス）
        AppConfig::getInstance()->calcDataSettings(MurMur3::calcHash("Languages"),
            [&](const std::string& path_) {language_text.add(path_); });
        // シミュレーションのモデル用テキスト
        AppConfig::getInstance()->calcDataSettings(MurMur3::calcHash("SimulationModels"),
            [&](const std::string& path_) {simulation_text.add(path_); });

        ui_manager.init(select_language, language_text, simulation_text);

        int old_width = paxg::Window::width(); // 1 フレーム前の幅
        int old_height = paxg::Window::height(); // 1 フレーム前の高さ

        int size_change_count = 0; // サイズを更新するカウンタ

        map_controller.init();
        koyomi.init();

        tile_manager.update(ui_manager.menu_bar, map_viewport, koyomi.jdn.cgetDay()); // 地図の辞書を更新

        paxg::Graphics3DModel g3d_model; // 3D モデル

#ifdef PAXS_USING_SIMULATOR
        std::unique_ptr<paxs::SettlementSimulator> simulator{};

        SimulationRange sr;
        // シミュレーションの範囲を設定
        AppConfig::getInstance()->calcDataSettings(MurMur3::calcHash("SimulationRange"),
            [&](const std::string& path_) {sr.input(path_); });
#endif
        {
            auto* view_menu = ui_manager.menu_bar.getMenuItem(MurMur3::calcHash("view"));
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

        paxs::PaxSapienticaInit::endLoadingScreen();
        /*##########################################################################################
            ループ開始
        ##########################################################################################*/
        while (paxg::Window::update()) {
            paxg::Mouse::getInstance()->calledFirstEveryFrame(); // 入力を更新

            input_state_manager.init(); // タッチ判定を初期化
            const paxg::ScopedSamplerState sampler{ paxg::SamplerState::ClampNearest }; // 画像の拡大縮小の方式を設定
            /*##########################################################################################
                更新処理関連
            ##########################################################################################*/

            // 画面サイズの変更に合わせて地図の幅を変える
            if (old_height != paxg::Window::height()) {
                map_viewport.setHeight(paxg::Window::height() * map_viewport.getHeight() / old_height);
                map_viewport.setWidth(map_viewport.getHeight() / double(paxg::Window::height()) * double(paxg::Window::width()));
            }
            if (old_width != paxg::Window::width()) {
                map_viewport.setWidth(map_viewport.getHeight() / double(paxg::Window::height()) * double(paxg::Window::width()));
            }
            if (old_width != paxg::Window::width() ||
                old_height != paxg::Window::height()) {
                // 影を定義
                if (size_change_count < 1) {
                    ui_manager.shadow_texture = paxg::RenderTexture{ paxg::Window::Size(), paxg::ColorF{ 1.0, 0.0 } };
                    ui_manager.internal_texture = paxg::RenderTexture{ ui_manager.shadow_texture.size() };
                }
                if (size_change_count >= 100) size_change_count = 100;
                ++size_change_count;
            }
            else size_change_count = 0;

            old_width = paxg::Window::width();
            old_height = paxg::Window::height();


            if (!visible[MurMur3::calcHash(2, "3D")]) {
                map_viewport.update(); // キーボード入力を更新
            }

            // 選択言語のプルダウンを更新
            ui_manager.pulldown.setPos(paxg::Vec2i{ static_cast<int>(paxg::Window::width() - ui_manager.pulldown.getRect().w()), 0 });
            ui_manager.pulldown.update(input_state_manager);
            select_language.set(std::size_t(ui_manager.pulldown.getIndex())); // 選択言語を更新
            select_language.setKey(std::uint_least32_t(ui_manager.pulldown.getKey())); // 選択言語を更新
            ui_manager.menu_bar.update(input_state_manager);

            // 表示の可視化を更新
            //Calendar Map UI Simulation License Debug 3D
            {
                auto* view_menu = ui_manager.menu_bar.getMenuItem(MurMur3::calcHash("view"));
                if (view_menu) {
                    visible.set(MurMur3::calcHash("Calendar"), view_menu->getIsItems(std::size_t(0)));
                    visible.set(MurMur3::calcHash("Map"), view_menu->getIsItems(std::size_t(1)));
                    visible.set(MurMur3::calcHash("UI"), view_menu->getIsItems(std::size_t(2)));
                    visible.set(MurMur3::calcHash("Simulation"), view_menu->getIsItems(std::size_t(3)));
                    //visible.set(MurMur3::calcHash("License"), view_menu->getIsItems(std::size_t(4)));
                    //visible.set(MurMur3::calcHash("Debug"), view_menu->getIsItems(std::size_t(5)));
                    visible.set(MurMur3::calcHash("3D"), view_menu->getIsItems(std::size_t(6)));
                }
            }

            {
                auto* place_names_menu = ui_manager.menu_bar.getMenuItem(MurMur3::calcHash("place_names"));
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
            }

            if (!visible[MurMur3::calcHash(2, "3D")]) {

            tile_manager.update(ui_manager.menu_bar, map_viewport, koyomi.jdn.cgetDay()); // 地図の辞書を更新
                // 地図を更新
                // フォントを取得
                paxg::Font* main_font = ui_manager.font_manager.language_fonts.getAndAdd(
                    select_language.cgetKey(),
                    static_cast<std::uint_least8_t>(ui_manager.koyomi_font_size),
                    static_cast<std::uint_least8_t>(ui_manager.koyomi_font_buffer_thickness_size)
                );
                map_controller.update(
                    map_viewport,
                    koyomi,
                    (main_font == nullptr) ? ui_manager.font_manager.pin_font : (*main_font),
                    ui_manager.font_manager.en_font,
                    ui_manager.font_manager.pin_font,
#ifdef PAXS_USING_SIMULATOR
                    simulator,
#endif
                    visible
                );
                // 暦を更新
                koyomi.update(
#ifdef PAXS_USING_SIMULATOR
                    simulator
#endif
                );
            }
            else if (visible[MurMur3::calcHash(2, "3D")]) {
                g3d_model.updateRotation(); // 3D モデルを回転させる
            }

            // 文字を更新
            ui_manager.update(
                map_viewport,
                select_language,
                language_text,
#ifdef PAXS_USING_SIMULATOR
                simulator,
#endif
                input_state_manager,
                koyomi,
                visible
            );
            paxg::TouchInput::updateState();
        }
    }
}

#endif // !PAX_MAHOROBA_MAIN_HPP
