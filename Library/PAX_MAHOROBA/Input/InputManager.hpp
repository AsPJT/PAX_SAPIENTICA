/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_INPUT_INPUT_MANAGER_HPP
#define PAX_MAHOROBA_INPUT_INPUT_MANAGER_HPP

#include <PAX_GRAPHICA/Key.hpp>
#include <PAX_GRAPHICA/Mouse.hpp>
#include <PAX_GRAPHICA/Window.hpp>

#include <PAX_MAHOROBA/Input/InputEvents.hpp>
#include <PAX_MAHOROBA/Input/InputRouter.hpp>

#include <PAX_SAPIENTICA/Core/Type/Vector2.hpp>
#include <PAX_SAPIENTICA/System/ApplicationEvents.hpp>
#include <PAX_SAPIENTICA/System/EventBus.hpp>
#include <PAX_SAPIENTICA/System/InputStateManager.hpp>

namespace paxs {

/// @brief 入力管理クラス
/// @brief Input manager class
/// @details AppComponentManagerから入力処理を分離し、入力イベントの収集と配信を担当
///          Separates input processing from AppComponentManager and handles input event collection and distribution
class InputManager {
public:
    InputManager()
        : last_window_size_(paxg::Window::width(), paxg::Window::height()) {
    }

    /// @brief InputRouterを取得
    /// @brief Get InputRouter
    /// @return InputRouter reference
    InputRouter& getInputRouter() { return input_router_; }

    /// @brief 入力ハンドラーを登録
    /// @brief Register input handler
    /// @param handler 登録するハンドラー / Handler to register
    ///
    /// IInputHandlerを実装したハンドラーを登録します。
    /// 全ての入力イベント（キーボード、マウス、マウスホイール）が統一されたルーターを通じて配信されます。
    /// Registers a handler that implements IInputHandler.
    /// All input events (keyboard, mouse, mouse wheel) are distributed through the unified router.
    void registerHandler(IInputHandler* handler) {
        input_router_.registerHandler(handler);
    }

    /// @brief ウィンドウリサイズを検知してイベントを発行
    /// @brief Detect window resize and publish event
    void detectWindowResize() {
        const Vector2<int> current_size(paxg::Window::width(), paxg::Window::height());

        if (current_size != last_window_size_) {
            EventBus::getInstance().publish(WindowResizedEvent(current_size));

            last_window_size_ = current_size;
        }
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

        // 2. キーボードイベント（InputRouterを通じて配信）
        KeyboardEvent keyboard_event;
        input_router_.routeEvent(keyboard_event);

        // 3. マウスホイールイベント（InputRouterを通じて配信）
        paxg::Mouse* mouse = paxg::Mouse::getInstance();
        const int wheel_rotation = mouse->getWheelRotVol();
        if (wheel_rotation != 0) {
            MouseWheelEvent wheel_event(wheel_rotation);
            input_router_.routeEvent(wheel_event);
        }

        // 4. マウスイベント（InputRouterを通じて配信）
        const Vector2<int> mouse_pos = mouse->getPos();
        const bool current_left_button = mouse->getLeft();

        // 左ボタンの状態を更新してイベントを発行
        InputStateManager::State left_state = left_button_state_manager_.update(current_left_button);
        if (left_state != InputStateManager::State::None) {
            MouseEvent event(mouse_pos);

            // 修飾キーの設定
            if (paxg::Key::isShiftPressed()) event.modifier_keys |= MouseEvent::MODIFIER_SHIFT;
            if (paxg::Key::isCtrlPressed()) event.modifier_keys |= MouseEvent::MODIFIER_CTRL;
            if (paxg::Key::isAltPressed()) event.modifier_keys |= MouseEvent::MODIFIER_ALT;
            if (paxg::Key::isCommandPressed()) event.modifier_keys |= MouseEvent::MODIFIER_COMMAND;

            // 前フレームの座標を設定
            event.prev_pos = mouse->getPosBefore1Frame();

            // ボタン状態の設定
            if (left_state == InputStateManager::State::Down) {
                event.left_button_state = MouseButtonState::Pressed;
            }
            else if (left_state == InputStateManager::State::Held) {
                event.left_button_state = MouseButtonState::Held;
            }
            else if (left_state == InputStateManager::State::Up) {
                event.left_button_state = MouseButtonState::Released;
            }

            input_router_.routeEvent(event);
        }
    }

private:
    // 統合入力ルーティングシステム
    InputRouter input_router_{};  // 全ての入力イベント（キーボード、マウス、マウスホイール）

    // マウスボタン状態管理
    InputStateManager left_button_state_manager_;

    // ウィンドウサイズキャッシュ
    Vector2<int> last_window_size_;
};

} // namespace paxs

#endif // !PAX_MAHOROBA_INPUT_INPUT_MANAGER_HPP
