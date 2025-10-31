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
#include <PAX_MAHOROBA/Map/MapViewport.hpp>
#include <PAX_MAHOROBA/Map/Input/MapViewportInputHandler.hpp>
#include <PAX_MAHOROBA/Rendering/GraphicsManager.hpp>

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
        MapViewportInputHandler map_viewport_input_handler{}; // 地図ビューポートの入力処理
        paxs::Koyomi koyomi{}; // 暦を管理する
        paxs::GraphicsManager graphics_manager{}; // グラフィック統合管理
        SelectLanguage select_language{}; // 選択言語
        paxs::Language language_text;
        paxs::Language simulation_text;
        paxs::InputStateManager input_state_manager; // 画面のクリック・タッチを管理する


        // 言語を初期化（テキストの多言語対応クラス）
        AppConfig::getInstance()->calcDataSettings(MurMur3::calcHash("Languages"),
            [&](const std::string& path_) {language_text.add(path_); });
        // シミュレーションのモデル用テキスト
        AppConfig::getInstance()->calcDataSettings(MurMur3::calcHash("SimulationModels"),
            [&](const std::string& path_) {simulation_text.add(path_); });

        // GraphicsManagerを初期化
        graphics_manager.init(select_language, language_text, simulation_text);

        // XYZ タイルを初期化
        AppConfig::getInstance()->calcDataSettings(MurMur3::calcHash("XYZTiles"),
            [&](const std::string& path_) {graphics_manager.getTileManager().add(path_); });

        graphics_manager.getTileManager().addGridLine(); // グリッド線を追加 （描画順が最後なので最後に追加）
        map_viewport.setWidth(map_viewport.getHeight() / double(paxg::Window::height()) * double(paxg::Window::width()));

        koyomi.init();

        paxg::Graphics3DModel g3d_model; // 3D モデル

#ifdef PAXS_USING_SIMULATOR
        std::unique_ptr<paxs::SettlementSimulator> simulator{};

        SimulationRange sr;
        // シミュレーションの範囲を設定
        AppConfig::getInstance()->calcDataSettings(MurMur3::calcHash("SimulationRange"),
            [&](const std::string& path_) {sr.input(path_); });
#endif
        // 可視性の初期状態をメニューに反映
        graphics_manager.getUILayer().initializeMenuFromVisibility(visible);

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

            if (!visible[MurMur3::calcHash(2, "3D")]) {
                map_viewport_input_handler.update(map_viewport); // 入力処理を更新
            }

            graphics_manager.getUILayer().updateLanguage(select_language);
            graphics_manager.getUILayer().syncVisibilityFromMenu(visible);

            if (!visible[MurMur3::calcHash(2, "3D")]) {
                // グラフィック統合更新（タイル + UI）
                graphics_manager.update(
                    map_viewport,
                    select_language,
                    language_text,
                    koyomi,
#ifdef PAXS_USING_SIMULATOR
                    simulator,
#endif
                    input_state_manager,
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
            paxg::TouchInput::updateState();
        }
    }
}

#endif // !PAX_MAHOROBA_MAIN_HPP
