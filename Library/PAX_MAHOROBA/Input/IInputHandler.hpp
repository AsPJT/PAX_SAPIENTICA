/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_I_INPUT_HANDLER_HPP
#define PAX_MAHOROBA_I_INPUT_HANDLER_HPP

#include <PAX_SAPIENTICA/Core/Type/Vector2.hpp>

#include <PAX_MAHOROBA/Input/InputEvents.hpp>
#include <PAX_MAHOROBA/Rendering/RenderLayer.hpp>

namespace paxs {

    /// @brief 統合入力ハンドラーインターフェース
    /// @brief Unified input handler interface
    ///
    /// キーボード、マウス、マウスホイールなど、全ての入力イベントを処理するための統一インターフェース。
    /// レイヤーベースの優先度管理とヒットテストをサポートします。
    class IInputHandler {
    public:
        virtual ~IInputHandler() = default;

        /// @brief キーボードイベント処理
        /// @brief Handle keyboard event
        /// @param event キーボードイベント / Keyboard event
        /// @return イベント処理結果 / Event handling result
        ///
        /// キーボードイベントは座標に依存しないため、isHit()は無視されます。
        virtual EventHandlingResult handleEvent(const KeyboardEvent& event) {
            (void)event;
            return EventHandlingResult::NotHandled();
        }

        /// @brief マウスイベント処理
        /// @brief Handle mouse event
        /// @param event マウスイベント / Mouse event
        /// @return イベント処理結果 / Event handling result
        ///
        /// マウスイベントは座標に依存するため、isHit()が事前にチェックされます。
        virtual EventHandlingResult handleEvent(const MouseEvent& event) {
            (void)event;
            return EventHandlingResult::NotHandled();
        }

        /// @brief マウスホイールイベント処理
        /// @brief Handle mouse wheel event
        /// @param event マウスホイールイベント / Mouse wheel event
        /// @return イベント処理結果 / Event handling result
        ///
        /// マウスホイールイベントは座標に依存しないため、isHit()は無視されます。
        virtual EventHandlingResult handleEvent(const MouseWheelEvent& event) {
            (void)event;
            return EventHandlingResult::NotHandled();
        }

        /// @brief 指定座標がこのオブジェクトの範囲内かどうかをチェック（ヒットテスト）
        /// @brief Check if the specified coordinates are within this object's bounds (hit test)
        /// @param pos マウス座標 / Mouse position
        /// @return 範囲内ならtrue / true if within bounds
        ///
        /// マウスイベント処理時のみ使用されます。
        /// キーボードやマウスホイールイベントでは呼び出されません。
        virtual bool isHit(const paxs::Vector2<int>& pos) const {
            (void)pos;
            return true;  // デフォルトは常にヒット / Default: always hit
        }

        /// @brief このオブジェクトが属する入力処理レイヤーを取得
        /// @brief Get the input handling layer this object belongs to
        /// @return レンダリングレイヤー（入力優先度として使用）/ Rendering layer (used as input priority)
        virtual RenderLayer getLayer() const = 0;
    };

} // namespace paxs

#endif // !PAX_MAHOROBA_I_INPUT_HANDLER_HPP
