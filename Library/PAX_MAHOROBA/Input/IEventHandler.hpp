/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_I_EVENT_HANDLER_HPP
#define PAX_MAHOROBA_I_EVENT_HANDLER_HPP

#include <PAX_MAHOROBA/Input/Events.hpp>

namespace paxs {

    /// @brief イベントハンドラーの基底インターフェース
    /// @brief Base interface for event handlers
    ///
    /// キーボード、マウスホイール、ウィンドウリサイズ、フォーカスなど、
    /// 各種イベントを処理するための基底インターフェース。
    /// IInputHandlerはこのインターフェースを継承し、レイヤー・ヒットテストを追加します。
    /// Base interface for handling various events (keyboard, mouse wheel, resize).
    /// IInputHandler inherits this and adds layer-based routing and hit testing.
    class IEventHandler {
    public:
        /// @brief 仮想デストラクタ
        /// @brief Virtual destructor
        virtual ~IEventHandler() = default;

        /// @brief キーボードイベント処理
        /// @brief Handle keyboard event
        /// @param event キーボードイベント / Keyboard event
        /// @return イベント処理結果 / Event handling result
        virtual EventHandlingResult handleEvent(const KeyboardEvent& event) {
            (void)event;
            return EventHandlingResult::NotHandled();
        }

        /// @brief マウスホイールイベント処理
        /// @brief Handle mouse wheel event
        /// @param event マウスホイールイベント / Mouse wheel event
        /// @return イベント処理結果 / Event handling result
        virtual EventHandlingResult handleEvent(const MouseWheelEvent& event) {
            (void)event;
            return EventHandlingResult::NotHandled();
        }

        /// @brief オブジェクトがイベント処理可能かどうかを取得
        /// @brief Check if the object can handle events
        /// @return イベント処理可能ならtrue / true if event handling is enabled
        virtual bool isEnabled() const = 0;
    };

} // namespace paxs

#endif // !PAX_MAHOROBA_I_EVENT_HANDLER_HPP
