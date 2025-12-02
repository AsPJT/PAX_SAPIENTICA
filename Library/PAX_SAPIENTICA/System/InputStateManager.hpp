/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_INPUT_STATE_MANAGER_HPP
#define PAX_SAPIENTICA_INPUT_STATE_MANAGER_HPP

#include <cstdint>

namespace paxs {

    /// @brief 入力状態を管理するクラス (Infrastructure Layer)
    /// @brief A class that manages input state to properly detect Down/Up/Held/None transitions
    /// @details マウスボタンやキーボードの状態遷移を管理し、Down（押された瞬間）、Held（押され続けている）、Up（離された瞬間）、None（押されていない）を正確に検出します
    /// @details Manages input state transitions (mouse/keyboard) to accurately detect Down (just pressed), Held (being held), Up (just released), None (not pressed)
    struct InputStateManager {
    public:
        /// @brief 入力の状態
        /// @brief Input state
        enum class State : std::uint8_t {
            None,   ///< 押されていない / Not pressed
            Down,   ///< 押された瞬間（1フレームのみ）/ Just pressed (only 1 frame)
            Held,   ///< 押され続けている / Being held
            Up      ///< 離された瞬間（1フレームのみ）/ Just released (only 1 frame)
        };

    private:
        // 前フレームのボタン状態
        bool previous_state_ = false;
        // 現在のボタン状態
        bool current_state_ = false;

    public:
        /// @brief 状態を初期化する
        constexpr void init() noexcept {
            previous_state_ = false;
            current_state_ = false;
        }

        /// @brief 現在のボタン状態を更新して状態を取得する
        /// @brief Update current button state and get the state
        /// @param is_pressed 現在のボタン状態 (true = 押されている, false = 押されていない)
        /// @param is_pressed Current button state (true if pressed, false otherwise)
        /// @return ボタンの状態 / Button state
        /// @details
        /// 状態遷移:
        /// - None → Down: ボタンが押された瞬間（false → true）
        /// - Down → Held: ボタンが押され続けている（true → true、前回Down）
        /// - Held → Held: ボタンが押され続けている（true → true、前回Held）
        /// - Down/Held → Up: ボタンが離された瞬間（true → false）
        /// - Up → None: ボタンが離されたまま（false → false、前回Up）
        /// - None → None: ボタンが離されたまま（false → false、前回None）
        constexpr State update(const bool is_pressed) noexcept {
            State result = State::None;

            if (!previous_state_ && is_pressed) {
                // 前フレーム: 押されていない、現フレーム: 押されている → Down（押された瞬間）
                result = State::Down;
            }
            else if (previous_state_ && is_pressed) {
                // 前フレーム: 押されている、現フレーム: 押されている → Held（押され続けている）
                result = State::Held;
            }
            else if (previous_state_ && !is_pressed) {
                // 前フレーム: 押されている、現フレーム: 押されていない → Up（離された瞬間）
                result = State::Up;
            }
            else {
                // 前フレーム: 押されていない、現フレーム: 押されていない → None（押されていない）
                result = State::None;
            }

            // 次フレームのために状態を保存
            previous_state_ = is_pressed;
            current_state_ = is_pressed;

            return result;
        }

        /// @brief 現在の状態を取得（更新なし）
        /// @brief Get current state (without update)
        /// @return 現在のボタン状態 / Current button state
        [[nodiscard]] constexpr bool getCurrentState() const noexcept {
            return current_state_;
        }

        /// @brief 前フレームの状態を取得
        /// @brief Get previous frame state
        /// @return 前フレームのボタン状態 / Previous frame button state
        [[nodiscard]] constexpr bool getPreviousState() const noexcept {
            return previous_state_;
        }
    };
}

#endif // !PAX_SAPIENTICA_INPUT_STATE_MANAGER_HPP
