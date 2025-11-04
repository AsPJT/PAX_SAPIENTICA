/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_I_MOUSE_EVENT_HANDLER_HPP
#define PAX_MAHOROBA_I_MOUSE_EVENT_HANDLER_HPP

#include <PAX_MAHOROBA/Input/Events.hpp>
#include <PAX_MAHOROBA/Rendering/RenderLayer.hpp>

namespace paxs {

    /// @brief マウスイベントハンドラーのインターフェース
    /// @brief Interface for mouse event handlers
    ///
    /// レイヤーベースのマウス入力ルーティングとヒットテスト機能を提供します。
    /// このインターフェースを実装することで、オブジェクトはMouseEventRouterによって
    /// Z順序の逆順（前面→背面）で入力イベントを受け取るようになります。
    class IMouseEventHandler {
    public:
        /// @brief 仮想デストラクタ
        /// @brief Virtual destructor
        virtual ~IMouseEventHandler() = default;

        /// @brief マウスイベント処理
        /// @brief Handle mouse event
        /// @param event マウスイベント / Mouse event
        /// @return イベント処理結果 / Event handling result
        virtual EventHandlingResult handleEvent(const MouseEvent& event) {
            (void)event;
            return EventHandlingResult::NotHandled();
        }

        /// @brief 指定座標がこのオブジェクトの範囲内かどうかをチェック（ヒットテスト）
        /// @brief Check if the specified coordinates are within this object's bounds (hit test)
        /// @param x X座標 / X coordinate
        /// @param y Y座標 / Y coordinate
        /// @return 範囲内ならtrue / true if within bounds
        virtual bool hitTest(int x, int y) const = 0;

        /// @brief このオブジェクトが属する入力処理レイヤーを取得
        /// @brief Get the input handling layer this object belongs to
        /// @return レンダリングレイヤー（入力優先度として使用）/ Rendering layer (used as input priority)
        virtual RenderLayer getLayer() const = 0;

        /// @brief オブジェクトが入力処理可能かどうかを取得
        /// @brief Check if the object can handle input
        /// @return 入力処理可能ならtrue / true if input handling is enabled
        virtual bool isEnabled() const = 0;
    };

} // namespace paxs

#endif // !PAX_MAHOROBA_I_MOUSE_EVENT_HANDLER_HPP
