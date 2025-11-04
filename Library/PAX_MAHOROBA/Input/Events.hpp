/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_EVENTS_HPP
#define PAX_MAHOROBA_EVENTS_HPP

#include <cstdint>

namespace paxs {

    /// @brief マウスボタンの状態
    /// @brief Mouse button state
    enum class MouseButtonState {
        None,       ///< 押されていない / Not pressed
        Pressed,    ///< 押された瞬間 / Just pressed
        Released,   ///< 離された瞬間 / Just released
        Held        ///< 押され続けている / Being held
    };

    /// @brief キーボードイベント
    /// @brief Keyboard event
    struct KeyboardEvent {
        /// @brief 修飾キーフラグ
        /// @brief Modifier key flags
        std::uint8_t modifier_keys = 0;

        static constexpr std::uint8_t MODIFIER_SHIFT = 0x01;
        static constexpr std::uint8_t MODIFIER_CTRL = 0x02;
        static constexpr std::uint8_t MODIFIER_ALT = 0x04;
        static constexpr std::uint8_t MODIFIER_COMMAND = 0x08;

        KeyboardEvent() = default;
    };

    /// @brief マウスイベント
    /// @brief Mouse event
    struct MouseEvent {
        /// @brief X座標
        int x = 0;
        /// @brief Y座標
        int y = 0;
        /// @brief 前フレームのX座標
        int prev_x = 0;
        /// @brief 前フレームのY座標
        int prev_y = 0;

        /// @brief 左ボタンの状態
        MouseButtonState left_button_state = MouseButtonState::None;
        /// @brief 右ボタンの状態
        MouseButtonState right_button_state = MouseButtonState::None;
        /// @brief 中ボタンの状態
        MouseButtonState middle_button_state = MouseButtonState::None;

        /// @brief 修飾キーフラグ
        std::uint8_t modifier_keys = 0;

        static constexpr std::uint8_t MODIFIER_SHIFT = 0x01;
        static constexpr std::uint8_t MODIFIER_CTRL = 0x02;
        static constexpr std::uint8_t MODIFIER_ALT = 0x04;
        static constexpr std::uint8_t MODIFIER_COMMAND = 0x08;

        MouseEvent() = default;
        MouseEvent(int x_, int y_) : x(x_), y(y_) {}
    };

    /// @brief マウスホイールイベント
    /// @brief Mouse wheel event
    struct MouseWheelEvent {
        /// @brief ホイール回転量
        int wheel_rotation = 0;

        MouseWheelEvent() = default;
        explicit MouseWheelEvent(int rotation) : wheel_rotation(rotation) {}
    };

    /// @brief ウィンドウリサイズイベント
    /// @brief Window resize event
    struct ResizeEvent {
        /// @brief 新しいウィンドウ幅
        int width = 0;
        /// @brief 新しいウィンドウ高さ
        int height = 0;

        ResizeEvent() = default;
        ResizeEvent(int w, int h) : width(w), height(h) {}
    };

    /// @brief ウィンドウフォーカスイベント
    /// @brief Window focus event
    struct FocusEvent {
        /// @brief フォーカス状態
        bool has_focus = false;

        FocusEvent() = default;
        explicit FocusEvent(bool focus) : has_focus(focus) {}
    };

    /// @brief イベント処理結果
    /// @brief Event handling result
    struct EventHandlingResult {
        /// @brief イベントが処理されたか
        bool handled = false;
        /// @brief キャプチャを要求するか
        bool request_capture = false;

        EventHandlingResult() = default;
        EventHandlingResult(bool h, bool c) : handled(h), request_capture(c) {}

        /// @brief 処理済みを返す
        static EventHandlingResult Handled() { return EventHandlingResult(true, false); }
        /// @brief 処理済み＋キャプチャを返す
        static EventHandlingResult HandledWithCapture() { return EventHandlingResult(true, true); }
        /// @brief 未処理を返す
        static EventHandlingResult NotHandled() { return EventHandlingResult(false, false); }
    };

} // namespace paxs

#endif // !PAX_MAHOROBA_EVENTS_HPP
