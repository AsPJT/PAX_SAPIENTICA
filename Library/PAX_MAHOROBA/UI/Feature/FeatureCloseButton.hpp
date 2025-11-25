/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_UI_FEATURE_CLOSE_BUTTON_HPP
#define PAX_MAHOROBA_UI_FEATURE_CLOSE_BUTTON_HPP

#include <functional>

#include <PAX_MAHOROBA/UI/Widget/IconButton.hpp>

#include <PAX_SAPIENTICA/Utility/MurMur3.hpp>

namespace paxs {

    /// @brief Feature詳細パネルの閉じるボタン
    /// @brief Close button for Feature info panel
    class FeatureCloseButton : public IconButton {
    public:
        using ClickCallback = std::function<void()>;

        FeatureCloseButton()
            : IconButton("FeatureCloseButton", MurMur3::calcHash("texture_close")) {
            // デフォルトのボタンサイズを設定
            setSize(Vector2<int>{ 20, 20 });
        }

        RenderLayer getLayer() const override { return RenderLayer::UIContent; }

        void setOnClick(ClickCallback callback) {
            on_click_ = std::move(callback);
        }

        EventHandlingResult handleEvent(const MouseEvent& event) override {
            // 左クリックが押されたら
            if (event.left_button_state == MouseButtonState::Pressed) {
                if (on_click_) {
                    on_click_();
                    return EventHandlingResult::Handled();
                }
            }
            return EventHandlingResult::Handled();
        }

    private:
        ClickCallback on_click_;
    };

} // namespace paxs

#endif // !PAX_MAHOROBA_UI_FEATURE_CLOSE_BUTTON_HPP
