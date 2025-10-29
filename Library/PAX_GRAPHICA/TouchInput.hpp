/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]      2023-2024 As Project
    [Production]    2023-2024 As Project
    [Contact Us]    wanotaitei@gmail.com         https://github.com/AsPJT/PAX_SAPIENTICA
    [License]       Distributed under the CC0 1.0. https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_TOUCH_INPUT_HPP
#define PAX_GRAPHICA_TOUCH_INPUT_HPP

#include <PAX_SAPIENTICA/Type/Vector2.hpp>

namespace paxg {

    /// @brief Multi-touch input abstraction class with state tracking
    /// @brief 状態追跡機能付きマルチタッチ入力抽象化クラス
    /// @details Provides a platform-independent interface for retrieving multi-touch input data.
    ///          Currently supports Android via DxLib, with null implementation for other platforms.
    ///          Also tracks previous frame's touch state for click detection.
    class TouchInput {
    private:
        inline static int previous_touch_count = 0;
        inline static paxs::Vector2<int> previous_touch_position{0, 0};

    public:
        /// @brief Get the number of current touch points
        /// @brief 現在のタッチポイント数を取得
        /// @return Number of active touch points (0 if multi-touch is not supported)
        /// @details On Android with DxLib, returns the actual number of simultaneous touches.
        ///          On other platforms, always returns 0.
        static int getTouchCount() {
#if defined(PAXS_USING_DXLIB) && defined(__ANDROID__)
            return DxLib::GetTouchInputNum();
#else
            return 0;
#endif
        }

        /// @brief Get touch position by index
        /// @brief インデックスでタッチ位置を取得
        /// @param index Touch point index (0-based)
        /// @param pos Output parameter to receive the touch position
        /// @return true if the touch position was successfully retrieved, false otherwise
        /// @details Retrieves the screen coordinates of the specified touch point.
        ///          Returns false on platforms that don't support multi-touch or if the index is invalid.
        static bool getTouchPosition(int index, paxs::Vector2<int>& pos) {
#if defined(PAXS_USING_DXLIB) && defined(__ANDROID__)
            int x = 0, y = 0;
            if (DxLib::GetTouchInput(index, &x, &y, NULL, NULL) == 0) {
                pos.x = x;
                pos.y = y;
                return true;
            }
            return false;
#else
            (void)index;
            (void)pos;
            return false;
#endif
        }

        /// @brief Update touch state (call once per frame)
        /// @brief タッチ状態を更新（フレームごとに1回呼び出す）
        /// @details Updates the previous frame's touch count and position for click detection.
        ///          Should be called at the end of each frame.
        static void updateState() {
            previous_touch_count = getTouchCount();
            if (previous_touch_count >= 1) {
                getTouchPosition(0, previous_touch_position);
            }
        }

        /// @brief Get previous frame's touch count
        /// @brief 前フレームのタッチポイント数を取得
        /// @return Number of touch points in the previous frame
        static int getPreviousTouchCount() {
            return previous_touch_count;
        }

        /// @brief Get previous frame's touch position
        /// @brief 前フレームのタッチ位置を取得
        /// @return Touch position from the previous frame
        static const paxs::Vector2<int>& getPreviousTouchPosition() {
            return previous_touch_position;
        }
    };

}

#endif // !PAX_GRAPHICA_TOUCH_INPUT_HPP
