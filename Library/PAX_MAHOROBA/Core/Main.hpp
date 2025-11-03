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
#include <PAX_SAPIENTICA/Simulation/Simulator.hpp>
#endif

#include <PAX_GRAPHICA/Key.hpp>
#include <PAX_GRAPHICA/Mouse.hpp>
#include <PAX_GRAPHICA/TouchInput.hpp>

#include <PAX_MAHOROBA/Core/InitLogo.hpp>
#include <PAX_MAHOROBA/Input/MapViewportInputHandler.hpp>
#include <PAX_MAHOROBA/Map/MapViewport.hpp>
#include <PAX_MAHOROBA/Rendering/GraphicsManager.hpp>

#include <PAX_SAPIENTICA/AppConfig.hpp>
#include <PAX_SAPIENTICA/Calendar/Koyomi.hpp>
#include <PAX_SAPIENTICA/InputStateManager.hpp>
#include <PAX_SAPIENTICA/MurMur3.hpp>

namespace paxs {

    // 主要な実行時定数・変数
    void startMain() { // フォルダ階層
        paxs::PaxSapienticaInit::firstInit(); // 初期化とロゴの表示

        MapViewport map_viewport{};
        MapViewportInputHandler map_viewport_input_handler{}; // 地図ビューポートの入力処理
        paxs::Koyomi koyomi{}; // 暦を管理する
        paxs::GraphicsManager graphics_manager{}; // グラフィック統合管理
        paxs::InputStateManager input_state_manager; // 画面のクリック・タッチを管理する

        graphics_manager.init();

        // MapViewportInputHandlerをGraphicsManagerに登録
        graphics_manager.setMapViewportInputHandler(&map_viewport_input_handler, &map_viewport);

        map_viewport.setWidth(map_viewport.getHeight() / double(paxg::Window::height()) * double(paxg::Window::width()));

        koyomi.init();

#ifdef PAXS_USING_SIMULATOR
        std::unique_ptr<paxs::SettlementSimulator> simulator{};
#endif

        paxs::PaxSapienticaInit::endLoadingScreen();
        /*##########################################################################################
            ループ開始
        ##########################################################################################*/
        while (paxg::Window::update()) {
            paxg::Mouse::getInstance()->calledFirstEveryFrame(); // 入力を更新

            input_state_manager.init(); // タッチ判定を初期化
            const paxg::ScopedSamplerState sampler{ paxg::SamplerState::ClampNearest }; // 画像の拡大縮小の方式を設定

            // 入力処理
            // キーボード入力（座標に依存しない）
            graphics_manager.getInputRouter().routeKeyboardInput(&input_state_manager);

            // マウスホイール入力（座標に依存しない）
            {
                paxg::Mouse* mouse = paxg::Mouse::getInstance();
                int wheel_rotation = mouse->getWheelRotVol();
                graphics_manager.getInputRouter().routeMouseWheelInput(&input_state_manager, wheel_rotation);
            }

            // マウス/タッチ入力（座標ベース）
            paxg::Mouse* mouse = paxg::Mouse::getInstance();
            int mouse_x = mouse->getPosX();
            int mouse_y = mouse->getPosY();

            // InputEventを作成してマウスボタンと修飾キーを設定
            paxs::InputEvent event(&input_state_manager, mouse_x, mouse_y);

            // マウスボタン情報を設定
            if (mouse->getLeft()) event.mouse_buttons |= paxs::InputEvent::MOUSE_BUTTON_LEFT;
            if (mouse->getRight()) event.mouse_buttons |= paxs::InputEvent::MOUSE_BUTTON_RIGHT;
            if (mouse->getMiddle()) event.mouse_buttons |= paxs::InputEvent::MOUSE_BUTTON_MIDDLE;

            // 修飾キー情報を設定
            if (paxs::Key::isShiftPressed()) event.modifier_keys |= paxs::InputEvent::MODIFIER_SHIFT;
            if (paxs::Key::isCtrlPressed()) event.modifier_keys |= paxs::InputEvent::MODIFIER_CTRL;
            if (paxs::Key::isAltPressed()) event.modifier_keys |= paxs::InputEvent::MODIFIER_ALT;
            if (paxs::Key::isCommandPressed()) event.modifier_keys |= paxs::InputEvent::MODIFIER_COMMAND;

            // マウスホイール回転量と前フレーム座標を設定
            event.wheel_rotation = mouse->getWheelRotVol();
            event.prev_x = mouse->getPosXBefore1Frame();
            event.prev_y = mouse->getPosYBefore1Frame();

            graphics_manager.getInputRouter().routeInput(event);

            graphics_manager.update(
                map_viewport,
                koyomi,
#ifdef PAXS_USING_SIMULATOR
                simulator,
#endif
                input_state_manager
            );

            koyomi.update(
#ifdef PAXS_USING_SIMULATOR
                simulator
#endif
            );

            paxg::TouchInput::updateState();
        }
    }
}

#endif // !PAX_MAHOROBA_MAIN_HPP
