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

#include <PAX_SAPIENTICA/Core/Type/Vector2.hpp>

namespace paxs {

    /// @brief マウスボタンの状態
    /// @brief Mouse button state
    enum class MouseButtonState : std::uint8_t {
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
        /// @brief マウス座標
        /// @brief Mouse position
        Vector2<int> pos{};
        /// @brief 前フレームのマウス座標
        /// @brief Previous frame mouse position
        Vector2<int> prev_pos{};

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
        MouseEvent(const Vector2<int>& position) : pos(position), prev_pos(0, 0) {}
    };

    /// @brief マウスホイールイベント
    /// @brief Mouse wheel event
    struct MouseWheelEvent {
        /// @brief ホイール回転量
        int wheel_rotation = 0;

        MouseWheelEvent() = default;
        explicit MouseWheelEvent(int rotation) : wheel_rotation(rotation) {}
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
