/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_UI_INPUT_HANDLER_HPP
#define PAX_MAHOROBA_UI_INPUT_HANDLER_HPP

#include <algorithm>
#include <vector>

#include <PAX_MAHOROBA/Input/IInputHandler.hpp>
#include <PAX_MAHOROBA/Rendering/IWidget.hpp>
#include <PAX_MAHOROBA/Rendering/RenderLayer.hpp>

namespace paxs {

    /// @brief UI入力処理を担当するクラス
    /// @brief Handles input processing for UI
    class UIInputHandler : public IInputHandler {
    private:
        /// @brief 登録されたウィジェットのリスト
        /// @brief List of registered widgets
        std::vector<IInputHandler*> registered_widgets_;

        bool is_sorted_ = false;

        void sortWidgets() {
            if (is_sorted_) return;
            std::sort(registered_widgets_.begin(), registered_widgets_.end(),
                [](IInputHandler* a, IInputHandler* b) {
                    return static_cast<std::uint16_t>(a->getLayer()) > static_cast<std::uint16_t>(b->getLayer());
                });
            is_sorted_ = true;
        }
    public:
        UIInputHandler() = default;

        /// @brief ウィジェットを登録
        /// @brief Register a widget
        /// @param widget 登録するウィジェット / Widget to register
        void registerWidget(IInputHandler* widget) {
            if (widget == nullptr) return;
            registered_widgets_.emplace_back(widget);
            is_sorted_ = false;
        }

        /// @brief マウスイベント処理（IInputHandlerインターフェース）
        /// @brief Handle mouse event (IInputHandler interface)
        /// @param event マウスイベント / Mouse event
        /// @return イベント処理結果 / Event handling result
        EventHandlingResult handleEvent(const MouseEvent& event) override {
            sortWidgets();

            for (IInputHandler* handler : registered_widgets_) {
                if (handler->isHit(event.x, event.y)) {
                    EventHandlingResult result = handler->handleEvent(event);
                    if (result.handled) {
                        return result; // イベントが処理されたら終了
                    }
                }
            }
            return EventHandlingResult::NotHandled();
        }

        /// @brief ヒットテスト
        /// @brief Hit test
        /// @param x X座標 / X coordinate
        /// @param y Y座標 / Y coordinate
        bool isHit(int x, int y) const override {
            for (const IInputHandler* handler : registered_widgets_) {
                if (handler->isHit(x, y)) {
                    return true;
                }
            }
            return false;
        }

        /// @brief レンダリングレイヤーを取得
        /// @brief Get rendering layer
        /// @return UIContentレイヤー / UIContent layer
        RenderLayer getLayer() const override {
            return RenderLayer::UIContent;
        }

        bool isEnabled() const override {
            return true;
        }

        void setEnabled(bool enabled) {
            (void)enabled;
        }
    };

} // namespace paxs

#endif // !PAX_MAHOROBA_UI_INPUT_HANDLER_HPP
