/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023-2024 As Project
	[Production]	2023-2024 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_UI_SETTLEMENT_STATUS_PANEL_BACKGROUND_HPP
#define PAX_MAHOROBA_UI_SETTLEMENT_STATUS_PANEL_BACKGROUND_HPP

#include <PAX_MAHOROBA/Rendering/IWidget.hpp>
#include <PAX_MAHOROBA/UI/PanelBackground.hpp>
#include <PAX_MAHOROBA/UI/UILayout.hpp>

namespace paxs {

    /// @brief Settlement ステータスパネルの背景
    /// @brief Settlement status panel background
    class SettlementStatusPanelBackground : public IWidget {
    public:
        SettlementStatusPanelBackground() = default;

        void setShadowTextures(paxg::RenderTexture& shadow_tex, paxg::RenderTexture& internal_tex) {
            background_.setShadowTextures(shadow_tex, internal_tex);
        }

        void setLayout(const PanelLayout& layout) {
            layout_ = layout;
        }

        // IWidget インターフェース

        const char* getName() const override {
            return "SettlementStatusPanelBackground";
        }

        RenderLayer getLayer() const override {
            return RenderLayer::UIBackground;
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
            return false;
        }

        void render() override {
            if (!visible_) return;
            if (layout_.width <= 0 || layout_.height <= 0) return;

            background_.draw(
                layout_.x,
                layout_.y,
                layout_.width,
                layout_.height,
                10,
                paxg::Color{255, 255, 255}
            );
        }

    private:
        PanelBackground background_;
        PanelLayout layout_;
        bool visible_ = true;
        bool enabled_ = true;
    };

} // namespace paxs

#endif // !PAX_MAHOROBA_UI_SETTLEMENT_STATUS_PANEL_BACKGROUND_HPP
