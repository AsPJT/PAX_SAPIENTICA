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

#include <PAX_SAPIENTICA/Calendar/Koyomi.hpp>
#include <PAX_SAPIENTICA/MouseClickStateManager.hpp>
#include <memory>

namespace paxs {

    // 主要な実行時定数・変数
    void startMain() { // フォルダ階層
        paxs::PaxSapienticaInit::firstInit(); // 初期化とロゴの表示
        MapViewport map_viewport{};
        std::unique_ptr<paxs::MapViewportInputHandler> map_viewport_input_handler = std::make_unique<MapViewportInputHandler>(); // 地図ビューポートの入力処理
        std::unique_ptr<paxs::UIInputHandler> ui_input_handler = std::make_unique<paxs::UIInputHandler>(); // UIの入力処理
        paxs::Koyomi koyomi{}; // 暦を管理する
        paxs::GraphicsManager graphics_manager{}; // グラフィック統合管理

        graphics_manager.init(map_viewport_input_handler.get(), &map_viewport, ui_input_handler.get());

        map_viewport.setWidth(map_viewport.getHeight() / double(paxg::Window::height()) * double(paxg::Window::width()));

        koyomi.init();

#ifdef PAXS_USING_SIMULATOR
        std::unique_ptr<paxs::SettlementSimulator> simulator{};
#endif

        paxs::PaxSapienticaInit::endLoadingScreen();

        // マウスボタンの状態管理
        paxs::MouseClickStateManager left_button_state_manager;

        /*##########################################################################################
            ループ開始
        ##########################################################################################*/
        while (paxg::Window::update()) {
            paxg::Mouse::getInstance()->calledFirstEveryFrame(); // 入力を更新

            const paxg::ScopedSamplerState sampler{ paxg::SamplerState::ClampNearest }; // 画像の拡大縮小の方式を設定

            // キーボード入力イベントをブロードキャスト（全ハンドラーに通知）
            paxs::KeyboardEvent keyboard_event;
            graphics_manager.getEventRouter().broadcastEvent(keyboard_event);

            // マウスホイール入力イベントをブロードキャスト（全ハンドラーに通知）
            paxg::Mouse* mouse = paxg::Mouse::getInstance();
            int wheel_rotation = mouse->getWheelRotVol();
            if (wheel_rotation != 0) {
                paxs::MouseWheelEvent wheel_event(wheel_rotation);
                graphics_manager.getEventRouter().broadcastEvent(wheel_event);
            }

            // マウス/タッチ入力（座標ベース）
            int mouse_x = mouse->getPosX();
            int mouse_y = mouse->getPosY();

            // 現在のボタン状態を取得
            bool current_left_button = mouse->getLeft();
            bool current_right_button = mouse->getRight();
            bool current_middle_button = mouse->getMiddle();

            // 左ボタンの状態を更新してイベントを発行
            paxs::MouseClickStateManager::State left_state = left_button_state_manager.update(current_left_button);
            if (left_state != paxs::MouseClickStateManager::State::None) {
                paxs::MouseEvent event(mouse_x, mouse_y);
                if (paxs::Key::isShiftPressed()) event.modifier_keys |= paxs::MouseEvent::MODIFIER_SHIFT;
                if (paxs::Key::isCtrlPressed()) event.modifier_keys |= paxs::MouseEvent::MODIFIER_CTRL;
                if (paxs::Key::isAltPressed()) event.modifier_keys |= paxs::MouseEvent::MODIFIER_ALT;
                if (paxs::Key::isCommandPressed()) event.modifier_keys |= paxs::MouseEvent::MODIFIER_COMMAND;
                event.prev_x = mouse->getPosXBefore1Frame();
                event.prev_y = mouse->getPosYBefore1Frame();

                if (left_state == paxs::MouseClickStateManager::State::Down) {
                    event.left_button_state = paxs::MouseButtonState::Pressed;
                }
                else if (left_state == paxs::MouseClickStateManager::State::Held) {
                    event.left_button_state = paxs::MouseButtonState::Held;
                }
                else if (left_state == paxs::MouseClickStateManager::State::Up) {
                    event.left_button_state = paxs::MouseButtonState::Released;
                }

                graphics_manager.getInputRouter().routeEvent(event);
            }

            graphics_manager.update(
                map_viewport,
                koyomi
#ifdef PAXS_USING_SIMULATOR
                , simulator
#endif
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
