/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023-2024 As Project
	[Production]	2023-2024 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_UI_HEADER_PANEL_BACKGROUND_HPP
#define PAX_MAHOROBA_UI_HEADER_PANEL_BACKGROUND_HPP

#include <PAX_MAHOROBA/Rendering/IWidget.hpp>
#include <PAX_MAHOROBA/UI/PanelBackground.hpp>

namespace paxs {

    /// @brief ヘッダーパネルの背景
    /// @brief Header panel background
    class HeaderPanelBackground : public IWidget {
    public:
        HeaderPanelBackground() = default;

        /// @brief 影用のテクスチャを設定
        /// @brief Set textures for shadow rendering
        void setShadowTextures(paxg::RenderTexture& shadow_tex, paxg::RenderTexture& internal_tex) {
            background_.setShadowTextures(shadow_tex, internal_tex);
        }

        /// @brief 高さを設定
        /// @brief Set height
        void setHeight(int height) {
            height_ = height;
        }

        // IWidget インターフェース

        const char* getName() const override {
            return "HeaderPanelBackground";
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
                0.0f,
                0.0f,
                static_cast<float>(paxg::Window::width()),
                static_cast<float>(height_)
            };
        }

        void setPos(const paxg::Vec2i& pos) override {
            (void)pos;  // ヘッダーは常に画面上部なので位置指定は無視
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
            if (height_ <= 0) return;

            background_.drawRect(
                0, 0,
                paxg::Window::width(),
                height_,
                paxg::Color{243, 243, 243}
            );
        }

    private:
        PanelBackground background_;
        int height_ = 0;
        bool visible_ = true;
        bool enabled_ = true;
    };

} // namespace paxs

#endif // !PAX_MAHOROBA_UI_HEADER_PANEL_BACKGROUND_HPP
