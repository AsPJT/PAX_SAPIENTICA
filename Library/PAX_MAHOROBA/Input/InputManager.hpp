/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]      2023-2025 As Project
    [Production]    2023-2025 As Project
    [Contact Us]    wanotaitei@gmail.com         https://github.com/AsPJT/PAX_SAPIENTICA
    [License]       Distributed under the CC0 1.0. https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_INPUT_INPUT_MANAGER_HPP
#define PAX_MAHOROBA_INPUT_INPUT_MANAGER_HPP

#include <PAX_GRAPHICA/Key.hpp>
#include <PAX_GRAPHICA/Mouse.hpp>
#include <PAX_GRAPHICA/Window.hpp>

#include <PAX_MAHOROBA/Core/ApplicationEvents.hpp>
#include <PAX_MAHOROBA/Core/EventBus.hpp>
#include <PAX_MAHOROBA/Input/EventRouter.hpp>
#include <PAX_MAHOROBA/Input/Events.hpp>
#include <PAX_MAHOROBA/Input/MouseEventRouter.hpp>

#include <PAX_SAPIENTICA/MouseClickStateManager.hpp>

namespace paxs {

/// @brief 入力管理クラス
/// @brief Input manager class
/// @details GraphicsManagerから入力処理を分離し、入力イベントの収集と配信を担当
///          Separates input processing from GraphicsManager and handles input event collection and distribution
class InputManager {
public:
    InputManager(EventBus& event_bus)
        : event_bus_(event_bus)
        , event_router_()
        , mouse_event_router_()
        , last_window_width_(paxg::Window::width())
        , last_window_height_(paxg::Window::height()) {
    }

    /// @brief EventRouterを取得
    /// @brief Get EventRouter
    /// @return EventRouter reference
    EventRouter& getEventRouter() { return event_router_; }

    /// @brief MouseEventRouterを取得
    /// @brief Get MouseEventRouter
    /// @return MouseEventRouter reference
    MouseEventRouter& getMouseEventRouter() { return mouse_event_router_; }

    /// @brief 入力ハンドラーを登録
    /// @brief Register input handlers
    /// @tparam T ハンドラーの型（EventHandlerとIInputHandlerを実装）
    /// @param handler 登録するハンドラー
    void registerHandler(auto* handler) {
        // EventRouterに登録（キーボード、マウスホイール用）
        event_router_.registerHandler(handler);
        // MouseEventRouterに登録（マウス入力用）
        mouse_event_router_.registerHandler(handler);
    }

    /// @brief 入力を処理
    /// @brief Process input
    /// @details 1. ウィンドウリサイズ検知
    ///          2. キーボードイベント（フォーカスがある場合のみ）
    ///          3. マウスホイールイベント（フォーカスがある場合のみ）
    ///          4. マウスイベント（フォーカスがある場合のみ）
    void processInput() {
        // マウス入力を更新（毎フレーム最初に呼ぶ必要がある）
        paxg::Mouse::getInstance()->calledFirstEveryFrame();

        // 1. ウィンドウリサイズ検知
        detectWindowResize();

        // ウィンドウがフォーカスを持っていない場合、入力イベントは処理しない
        if (!paxg::Window::hasFocus()) {
            return;
        }

        // 2. キーボードイベント（既存のEventRouterを通じて配信）
        KeyboardEvent keyboard_event;
        event_router_.broadcastEvent(keyboard_event);

        // 3. マウスホイールイベント（既存のEventRouterを通じて配信）
        paxg::Mouse* mouse = paxg::Mouse::getInstance();
        const int wheel_rotation = mouse->getWheelRotVol();
        if (wheel_rotation != 0) {
            MouseWheelEvent wheel_event(wheel_rotation);
            event_router_.broadcastEvent(wheel_event);
        }

        // 4. マウスイベント（既存のMouseEventRouterを通じて配信）
        const int mouse_x = mouse->getPosX();
        const int mouse_y = mouse->getPosY();
        const bool current_left_button = mouse->getLeft();

        // 左ボタンの状態を更新してイベントを発行
        MouseClickStateManager::State left_state = left_button_state_manager_.update(current_left_button);
        if (left_state != MouseClickStateManager::State::None) {
            MouseEvent event(mouse_x, mouse_y);

            // 修飾キーの設定
            if (paxs::Key::isShiftPressed()) event.modifier_keys |= MouseEvent::MODIFIER_SHIFT;
            if (paxs::Key::isCtrlPressed()) event.modifier_keys |= MouseEvent::MODIFIER_CTRL;
            if (paxs::Key::isAltPressed()) event.modifier_keys |= MouseEvent::MODIFIER_ALT;
            if (paxs::Key::isCommandPressed()) event.modifier_keys |= MouseEvent::MODIFIER_COMMAND;

            // 前フレームの座標を設定
            event.prev_x = mouse->getPosXBefore1Frame();
            event.prev_y = mouse->getPosYBefore1Frame();

            // ボタン状態の設定
            if (left_state == MouseClickStateManager::State::Down) {
                event.left_button_state = MouseButtonState::Pressed;
            }
            else if (left_state == MouseClickStateManager::State::Held) {
                event.left_button_state = MouseButtonState::Held;
            }
            else if (left_state == MouseClickStateManager::State::Up) {
                event.left_button_state = MouseButtonState::Released;
            }

            mouse_event_router_.routeEvent(event);
        }
    }

private:
    EventBus& event_bus_;

    // 既存の入力ルーティングシステム
    EventRouter event_router_;         // 座標に依存しないイベント（キーボード、リサイズ等）
    MouseEventRouter mouse_event_router_;  // マウス入力のみ（レイヤーベース）

    // マウスボタン状態管理
    MouseClickStateManager left_button_state_manager_;

    // ウィンドウサイズキャッシュ
    int last_window_width_;
    int last_window_height_;

    /// @brief ウィンドウリサイズを検知してイベントを発行
    /// @brief Detect window resize and publish event
    void detectWindowResize() {
        const int current_width = paxg::Window::width();
        const int current_height = paxg::Window::height();

        if (current_width != last_window_width_ || current_height != last_window_height_) {
            // EventBus経由でWindowResizedEventを発行
            event_bus_.publish(WindowResizedEvent(current_width, current_height));

            last_window_width_ = current_width;
            last_window_height_ = current_height;
        }
    }
};

} // namespace paxs

#endif // !PAX_MAHOROBA_INPUT_INPUT_MANAGER_HPP
