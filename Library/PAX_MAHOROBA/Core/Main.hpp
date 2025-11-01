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

        // MapViewportInputHandlerをGraphicsManagerに登録
        // Register MapViewportInputHandler to GraphicsManager
        graphics_manager.setMapViewportInputHandler(&map_viewport_input_handler, &map_viewport);

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

            graphics_manager.getUILayer().updateLanguage(select_language);
            graphics_manager.getUILayer().syncVisibilityFromMenu(visible);

            // 入力処理（InputRouterを使用した統合的な入力処理）
            // Input processing (unified input processing using InputRouter)
            if (!visible.isVisible(MurMur3::calcHash(2, "3D"))) {
                // キーボード入力（座標に依存しない）
                // Keyboard input (coordinate-independent)
                // 優先順位: UI (400) → MapContentManager (200) → MapViewportInputHandler (0)
                graphics_manager.getInputRouter().routeKeyboardInput(&input_state_manager);

                // マウスホイール入力（座標に依存しない）
                // Mouse wheel input (coordinate-independent)
                // 優先順位: UI (400) → MapContentManager (200) → MapViewportInputHandler (0)
                graphics_manager.getInputRouter().routeMouseWheelInput(&input_state_manager);

                // マウス/タッチ入力（座標ベース）
                // Mouse/Touch input (coordinate-based)
                paxg::Mouse* mouse = paxg::Mouse::getInstance();
                int mouse_x = mouse->getPosX();
                int mouse_y = mouse->getPosY();

                // InputEventを作成してマウスボタンと修飾キーを設定
                // Create InputEvent and set mouse buttons and modifier keys
                paxs::InputEvent event(&input_state_manager, mouse_x, mouse_y);

                // マウスボタン情報を設定
                // Set mouse button information
                if (mouse->getLeft()) event.mouse_buttons |= paxs::InputEvent::MOUSE_BUTTON_LEFT;
                if (mouse->getRight()) event.mouse_buttons |= paxs::InputEvent::MOUSE_BUTTON_RIGHT;
                if (mouse->getMiddle()) event.mouse_buttons |= paxs::InputEvent::MOUSE_BUTTON_MIDDLE;

                // 修飾キー情報を設定
                // Set modifier key information
                if (paxs::Key::isShiftPressed()) event.modifier_keys |= paxs::InputEvent::MODIFIER_SHIFT;
                if (paxs::Key::isCtrlPressed()) event.modifier_keys |= paxs::InputEvent::MODIFIER_CTRL;
                if (paxs::Key::isAltPressed()) event.modifier_keys |= paxs::InputEvent::MODIFIER_ALT;
                if (paxs::Key::isCommandPressed()) event.modifier_keys |= paxs::InputEvent::MODIFIER_COMMAND;

                // マウスホイール回転量と前フレーム座標を設定
                // Set mouse wheel rotation and previous frame coordinates
                event.wheel_rotation = mouse->getWheelRotVol();
                event.prev_x = mouse->getPosXBefore1Frame();
                event.prev_y = mouse->getPosYBefore1Frame();

                // 優先順位: UI (400) → MapContentManager (200) → MapViewportInputHandler (0)
                graphics_manager.getInputRouter().routeInput(event);
            }

            if (!visible.isVisible(MurMur3::calcHash(2, "3D"))) {
                // ウィンドウリサイズイベントをInputRouter経由でルーティング
                // Route window resize event through InputRouter
                int current_width = paxg::Window::width();
                int current_height = paxg::Window::height();
                graphics_manager.getInputRouter().routeWindowResizeEvent(current_width, current_height);

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
            else if (visible.isVisible(MurMur3::calcHash(2, "3D"))) {
                g3d_model.updateRotation(); // 3D モデルを回転させる
            }
            paxg::TouchInput::updateState();
        }
    }
}

#endif // !PAX_MAHOROBA_MAIN_HPP
