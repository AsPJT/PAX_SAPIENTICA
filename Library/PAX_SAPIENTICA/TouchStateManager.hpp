/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023-2024 As Project
	[Production]	2023-2024 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_TOUCH_STATE_MANAGER_HPP
#define PAX_SAPIENTICA_TOUCH_STATE_MANAGER_HPP

namespace paxs {

    /// @brief A class that manages touch state to prevent duplicate processing
    /// @brief タッチの状態を管理して二重処理を防ぐクラス
    /// @details Tracks whether a touch event has been processed to avoid handling the same touch multiple times
    struct TouchStateManager {
    private:
        // タッチされているか
        bool is_touch = false;
    public:
        /// @brief Initialize the touch state
        /// @brief タッチ状態を初期化する
        /// @details Resets the internal touch flag to allow new touch events
        constexpr void init() noexcept { is_touch = false; }

        /// @brief Check and update touch state
        /// @brief タッチ状態を確認して更新する
        /// @param is_touch_ Current touch status (true if touched, false otherwise)
        /// @return true if this is a new touch event (not yet processed), false if already processed or not touched
        /// @details Returns true only on the first frame when touch begins, preventing duplicate processing
        constexpr bool get(const bool is_touch_) noexcept {

            // まだタッチされていない場合
            if (!is_touch) {
                // 今回もタッチされていないので不可
                if (!is_touch_) {
                    return false;
                }
                // 今回初めてタッチされたので成功
                else {
                    is_touch = true;
                    return true;
                }
            }
            // 既にタッチされている場合
            else {
                // 二重処理を防ぐため、不可
                return false;
            }
        }
    };
}

#endif // !PAX_SAPIENTICA_TOUCH_STATE_MANAGER_HPP
