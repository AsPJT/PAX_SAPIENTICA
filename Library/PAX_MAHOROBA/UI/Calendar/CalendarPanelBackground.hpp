/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023-2024 As Project
	[Production]	2023-2024 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_UI_CALENDAR_PANEL_BACKGROUND_HPP
#define PAX_MAHOROBA_UI_CALENDAR_PANEL_BACKGROUND_HPP

#include <PAX_MAHOROBA/Rendering/IWidget.hpp>
#include <PAX_MAHOROBA/UI/PanelBackground.hpp>
#include <PAX_MAHOROBA/UI/UILayout.hpp>

namespace paxs {

    /// @brief カレンダーパネルの背景
    /// @brief Calendar panel background
    class CalendarPanelBackground : public IWidget {
    public:
        CalendarPanelBackground() = default;

        /// @brief 影用のテクスチャを設定
        /// @brief Set textures for shadow rendering
        void setShadowTextures(paxg::RenderTexture& shadow_tex, paxg::RenderTexture& internal_tex) {
            background_.setShadowTextures(shadow_tex, internal_tex);
        }

        /// @brief レイアウトを設定
        /// @brief Set layout
        void setLayout(const PanelLayout& layout) {
            layout_ = layout;
        }

        // IWidget インターフェース

        const char* getName() const override {
            return "CalendarPanelBackground";
        }

        RenderLayer getLayer() const override {
            return RenderLayer::UIBackground;  // 背景レイヤー
        }

        bool isAvailable() const override {
            return true;
        }

        void setEnabled(bool enabled) override {
            enabled_ = enabled;
        }

        bool isEnabled() const override {
            return enabled_;
        }

        paxg::Rect getRect() const override {
            return paxg::Rect{
                static_cast<float>(layout_.x),
                static_cast<float>(layout_.y),
                static_cast<float>(layout_.width),
                static_cast<float>(layout_.height)
            };
        }

        void setPos(const paxg::Vec2i& pos) override {
            layout_.x = pos.x();
            layout_.y = pos.y();
        }

        void setVisible(bool visible) override {
            visible_ = visible;
        }

        bool isVisible() const override {
            return visible_;
        }

        bool handleInput(const InputEvent&) override {
            return false;  // 背景は入力を処理しない
        }

        void render() override {
            if (!visible_) return;
            if (layout_.width <= 0 || layout_.height <= 0) return;

            // 背景を描画（バッチに登録 or 即座描画）
            background_.draw(
                layout_.x,
                layout_.y,
                layout_.width,
                layout_.height,
                10,  // corner_radius
                paxg::Color{255, 255, 255}  // bg_color
            );
        }

    private:
        PanelBackground background_;
        PanelLayout layout_;
        bool visible_ = true;
        bool enabled_ = true;
    };

} // namespace paxs

#endif // !PAX_MAHOROBA_UI_CALENDAR_PANEL_BACKGROUND_HPP
