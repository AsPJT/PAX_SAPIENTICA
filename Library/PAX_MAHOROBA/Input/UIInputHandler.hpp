/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]      2023-2024 As Project
    [Production]    2023-2024 As Project
    [Contact Us]    wanotaitei@gmail.com            https://github.com/AsPJT/PAX_SAPIENTICA
    [License]       Distributed under the CC0 1.0.  https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_UI_INPUT_HANDLER_HPP
#define PAX_MAHOROBA_UI_INPUT_HANDLER_HPP

#include <PAX_MAHOROBA/Input/IMouseEventHandler.hpp>
#include <PAX_MAHOROBA/Rendering/RenderLayer.hpp>
#include <PAX_MAHOROBA/UI/UILayer.hpp>

namespace paxs {

    /// @brief UI入力処理を担当するクラス
    /// @brief Handles input processing for UI
    class UIInputHandler : public IMouseEventHandler {
    private:
        bool enabled_ = true; // 入力処理の有効/無効
        UILayer* ui_layer_ = nullptr; // UILayerへの参照（入力を委譲）

    public:
        UIInputHandler() = default;

        /// @brief UILayerを設定
        /// @brief Set UILayer
        /// @param ui_layer UILayerへのポインタ / Pointer to UILayer
        void setUILayer(UILayer* ui_layer) {
            ui_layer_ = ui_layer;
        }

        // IInputHandler の実装
        // IInputHandler implementation

        /// @brief マウスイベント処理（IInputHandlerインターフェース）
        /// @brief Handle mouse event (IInputHandler interface)
        /// @param event マウスイベント / Mouse event
        /// @return イベント処理結果 / Event handling result
        EventHandlingResult handleEvent(const MouseEvent& event) override {
            if (!enabled_ || ui_layer_ == nullptr) {
                return EventHandlingResult::NotHandled();
            }

            // UILayerに入力処理を委譲
            return ui_layer_->handleMouseInput(event);
        }

        /// @brief ヒットテスト（画面全体を対象）
        /// @brief Hit test (targets entire screen)
        /// @param x X座標 / X coordinate
        /// @param y Y座標 / Y coordinate
        /// @return 常にtrue（UI層は画面全体をカバー）/ Always true (UI layer covers entire screen)
        bool hitTest(int x, int y) const override {
            (void)x;
            (void)y;
            return true; // UI層は画面全体をカバー
        }

        /// @brief レンダリングレイヤーを取得
        /// @brief Get rendering layer
        /// @return UIContentレイヤー / UIContent layer
        RenderLayer getLayer() const override {
            return RenderLayer::UIContent;
        }

        /// @brief 入力処理が有効かどうかを取得
        /// @brief Check if input handling is enabled
        /// @return 入力処理が有効ならtrue / true if input handling is enabled
        bool isEnabled() const override {
            return enabled_;
        }

        /// @brief 入力処理の有効/無効を設定
        /// @brief Set enabled state
        /// @param enabled 有効にする場合true / true to enable
        void setEnabled(bool enabled) {
            enabled_ = enabled;
        }
    };

} // namespace paxs

#endif // !PAX_MAHOROBA_UI_INPUT_HANDLER_HPP
