/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_INPUT_DEBUG_INPUT_HANDLER_HPP
#define PAX_MAHOROBA_INPUT_DEBUG_INPUT_HANDLER_HPP

#include <map>

#include <PAX_GRAPHICA/Key.hpp>

#include <PAX_MAHOROBA/Input/IInputHandler.hpp>
#include <PAX_MAHOROBA/Input/InputEvents.hpp>
#include <PAX_MAHOROBA/UI/Debug/DebugLayer.hpp>

#include <PAX_SAPIENTICA/Utility/Logger.hpp>

namespace paxs {

/// @brief デバッグレイヤー用の入力ハンドラー
/// @brief Input handler for debug layer
/// @details デバッグコンソールやデバッグ通知のマウス/キーボード操作を処理
class DebugInputHandler : public IInputHandler {
private:
    DebugLayer* debug_layer_ = nullptr;

    // キー状態管理（エッジ検出用）
    bool tab_key_was_pressed_ = false;
    bool p_key_was_pressed_ = false;
    bool v_key_was_pressed_ = false;
    bool enter_key_was_pressed_ = false;
    bool backspace_key_was_pressed_ = false;
    bool up_key_was_pressed_ = false;
    bool down_key_was_pressed_ = false;
    bool left_key_was_pressed_ = false;
    bool right_key_was_pressed_ = false;
    int test_notification_counter_ = 0;

    // 文字入力用のキー状態（簡易実装）
    std::map<int, bool> key_states_;

    // テスト用変数
    int test_int_value_ = 42;
    double test_float_value_ = 3.14159;
    bool test_bool_value_ = true;
    std::string test_string_value_ = "Debug Test";

public:
    DebugInputHandler() = default;
    explicit DebugInputHandler(DebugLayer* debug_layer)
        : debug_layer_(debug_layer) {
        // テスト用変数を登録
        if (debug_layer_ != nullptr) {
            auto& watcher = debug_layer_->getVariableWatcher();
            watcher.addIntVariable("test_notification_counter", &test_notification_counter_);
            watcher.addIntVariable("test_int_value", &test_int_value_);
            watcher.addFloatVariable("test_float_value", &test_float_value_, 5);
            watcher.addBoolVariable("test_bool_value", &test_bool_value_);
            watcher.addStringVariable("test_string", &test_string_value_);
        }
    }

    ~DebugInputHandler() override = default;

    void setDebugLayer(DebugLayer* debug_layer) {
        debug_layer_ = debug_layer;
    }

    // IInputHandler実装
    EventHandlingResult handleEvent(const KeyboardEvent& /*event*/) override {
        if (debug_layer_ == nullptr) {
            return EventHandlingResult::NotHandled();
        }

        // Tab key: コンソールの表示/非表示を切り替え
        {
            bool tab_key_is_pressed = paxg::Key(paxg::PAXG_KEY_TAB).isPressed();
            if (tab_key_is_pressed && !tab_key_was_pressed_) {
                debug_layer_->toggleConsole();
            }
            tab_key_was_pressed_ = tab_key_is_pressed;
        }

        // P key: パフォーマンスモニターの表示/非表示を切り替え（コンソール非表示時のみ）
        {
            bool p_key_is_pressed = paxg::Key(paxg::PAXG_KEY_P).isPressed();
            if (p_key_is_pressed && !p_key_was_pressed_ && !debug_layer_->getConsole().isVisible()) {
                debug_layer_->togglePerformanceMonitor();
            }
            p_key_was_pressed_ = p_key_is_pressed;
        }

        // V key: 変数ウォッチャーの表示/非表示を切り替え（コンソール非表示時のみ）
        {
            bool v_key_is_pressed = paxg::Key(paxg::PAXG_KEY_V).isPressed();
            if (v_key_is_pressed && !v_key_was_pressed_ && !debug_layer_->getConsole().isVisible()) {
                debug_layer_->toggleVariableWatcher();
            }
            v_key_was_pressed_ = v_key_is_pressed;
        }

#ifdef PAXS_DEVELOPMENT
        // コンソールが表示されている場合、コマンドライン入力を処理
        if (debug_layer_->getConsole().isVisible() && debug_layer_->getConsole().isInputFocused()) {
            // Enter key: コマンド実行
            {
                bool enter_pressed = paxg::Key(paxg::PAXG_KEY_ENTER).isPressed();
                if (enter_pressed && !enter_key_was_pressed_) {
                    debug_layer_->getConsole().handleKeyInput(13);
                }
                enter_key_was_pressed_ = enter_pressed;
            }

            // Backspace key: 文字削除
            {
                bool backspace_pressed = paxg::Key(paxg::PAXG_KEY_BACKSPACE).isPressed();
                if (backspace_pressed && !backspace_key_was_pressed_) {
                    debug_layer_->getConsole().handleKeyInput(8);
                }
                backspace_key_was_pressed_ = backspace_pressed;
            }

            // Up key: 履歴を遡る
            {
                bool up_pressed = paxg::Key(paxg::PAXG_KEY_UP).isPressed();
                if (up_pressed && !up_key_was_pressed_) {
                    debug_layer_->getConsole().handleKeyInput(38);
                }
                up_key_was_pressed_ = up_pressed;
            }

            // Down key: 履歴を進める
            {
                bool down_pressed = paxg::Key(paxg::PAXG_KEY_DOWN).isPressed();
                if (down_pressed && !down_key_was_pressed_) {
                    debug_layer_->getConsole().handleKeyInput(40);
                }
                down_key_was_pressed_ = down_pressed;
            }

            // Left key: カーソルを左に移動
            {
                bool left_pressed = paxg::Key(paxg::PAXG_KEY_LEFT).isPressed();
                if (left_pressed && !left_key_was_pressed_) {
                    debug_layer_->getConsole().handleKeyInput(37);
                }
                left_key_was_pressed_ = left_pressed;
            }

            // Right key: カーソルを右に移動
            {
                bool right_pressed = paxg::Key(paxg::PAXG_KEY_RIGHT).isPressed();
                if (right_pressed && !right_key_was_pressed_) {
                    debug_layer_->getConsole().handleKeyInput(39);
                }
                right_key_was_pressed_ = right_pressed;
            }

            // 英数字と記号の入力（簡易実装）
            handleTextInput();

            return EventHandlingResult::Handled();
        }
#endif // PAXS_DEVELOPMENT

        return EventHandlingResult::NotHandled();
    }

private:
#ifdef PAXS_DEVELOPMENT
    /// @brief テキスト入力を処理（英数字と記号）
    void handleTextInput() {
        // アルファベット（小文字）
        const char letters[] = "abcdefghijklmnopqrstuvwxyz";
        const paxg::InputKey* letter_keys[] = {
            &paxg::PAXG_KEY_A, &paxg::PAXG_KEY_B, &paxg::PAXG_KEY_C, &paxg::PAXG_KEY_D,
            &paxg::PAXG_KEY_E, &paxg::PAXG_KEY_F, &paxg::PAXG_KEY_G, &paxg::PAXG_KEY_H,
            &paxg::PAXG_KEY_I, &paxg::PAXG_KEY_J, &paxg::PAXG_KEY_K, &paxg::PAXG_KEY_L,
            &paxg::PAXG_KEY_M, &paxg::PAXG_KEY_N, &paxg::PAXG_KEY_O, &paxg::PAXG_KEY_P,
            &paxg::PAXG_KEY_Q, &paxg::PAXG_KEY_R, &paxg::PAXG_KEY_S, &paxg::PAXG_KEY_T,
            &paxg::PAXG_KEY_U, &paxg::PAXG_KEY_V, &paxg::PAXG_KEY_W, &paxg::PAXG_KEY_X,
            &paxg::PAXG_KEY_Y, &paxg::PAXG_KEY_Z
        };

        for (std::size_t i = 0; i < 26; ++i) {
            bool is_pressed = paxg::Key(*letter_keys[i]).isPressed();
            int key_code = static_cast<int>(letters[i]);

            if (is_pressed && !key_states_[key_code]) {
                debug_layer_->getConsole().handleTextInput(letters[i]);
            }
            key_states_[key_code] = is_pressed;
        }

        // 数字
        const char digits[] = "0123456789";
        const paxg::InputKey* digit_keys[] = {
            &paxg::PAXG_KEY_0, &paxg::PAXG_KEY_1, &paxg::PAXG_KEY_2, &paxg::PAXG_KEY_3,
            &paxg::PAXG_KEY_4, &paxg::PAXG_KEY_5, &paxg::PAXG_KEY_6, &paxg::PAXG_KEY_7,
            &paxg::PAXG_KEY_8, &paxg::PAXG_KEY_9
        };

        for (std::size_t i = 0; i < 10; ++i) {
            bool is_pressed = paxg::Key(*digit_keys[i]).isPressed();
            int key_code = static_cast<int>(digits[i]);

            if (is_pressed && !key_states_[key_code]) {
                debug_layer_->getConsole().handleTextInput(digits[i]);
            }
            key_states_[key_code] = is_pressed;
        }

        // スペース（ASCII 32）
        {
            bool is_pressed = paxg::Key(paxg::PAXG_KEY_SPACE).isPressed();
            int key_code = static_cast<int>(' ');

            if (is_pressed && !key_states_[key_code]) {
                debug_layer_->getConsole().handleTextInput(' ');
            }
            key_states_[key_code] = is_pressed;
        }
        // マイナス記号（ASCII 45）
        {
            bool is_pressed = paxg::Key(paxg::PAXG_KEY_MINUS).isPressed();
            int key_code = static_cast<int>('-');

            if (is_pressed && !key_states_[key_code]) {
                debug_layer_->getConsole().handleTextInput('-');
            }
            key_states_[key_code] = is_pressed;
        }
    }
#endif // PAXS_DEVELOPMENT

public:

    EventHandlingResult handleEvent(const MouseEvent& event) override {
        if (debug_layer_ == nullptr) {
            return EventHandlingResult::NotHandled();
        }

        // マウス移動は常に処理（ホバー状態の更新）
        debug_layer_->handleMouseMove(event.pos);

        // マウスクリックの処理
        if (event.left_button_state == MouseButtonState::Pressed) {
            debug_layer_->handleMouseClick(event.pos);
            // デバッグレイヤーでイベントが処理された場合でも他のハンドラーにも伝播
            return EventHandlingResult::NotHandled();
        }

        return EventHandlingResult::NotHandled();
    }

    EventHandlingResult handleEvent(const MouseWheelEvent& event) override {
        if (debug_layer_ == nullptr) {
            return EventHandlingResult::NotHandled();
        }

        // コンソールが表示されている場合のみスクロール処理
        if (debug_layer_->getConsole().isVisible()) {
            if (event.wheel_rotation > 0) {
                // 上にスクロール（古いログを表示）
                debug_layer_->getConsole().scrollUp();
            } else if (event.wheel_rotation < 0) {
                // 下にスクロール（新しいログを表示）
                debug_layer_->getConsole().scrollDown();
            }
            return EventHandlingResult::Handled();
        }

        return EventHandlingResult::NotHandled();
    }

    RenderLayer getLayer() const override {
        return RenderLayer::Debug;
    }
};

} // namespace paxs

#endif // !PAX_MAHOROBA_INPUT_DEBUG_INPUT_HANDLER_HPP
