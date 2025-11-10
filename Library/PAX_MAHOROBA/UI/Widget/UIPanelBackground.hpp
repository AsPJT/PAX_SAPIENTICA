/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_UI_UI_PANEL_BACKGROUND_HPP
#define PAX_MAHOROBA_UI_UI_PANEL_BACKGROUND_HPP

#include <PAX_GRAPHICA/Color.hpp>
#include <PAX_GRAPHICA/Rect.hpp>
#include <PAX_GRAPHICA/Vec2.hpp>

#include <PAX_MAHOROBA/Rendering/IWidget.hpp>
#include <PAX_MAHOROBA/UI/UILayout.hpp>
#include <PAX_MAHOROBA/UI/Widget/PanelBackgroundRenderer.hpp>

namespace paxs {

    /// @brief UIパネルの背景を描画するクラス
    /// @brief UI panel background rendering class
    class UIPanelBackground : public IWidget {
    private:
        const char* name_;
        const PanelLayout* layout;
        const paxg::Color bg_color_;
        const int corner_radius_;
        bool visible_ = true;

    public:
        /// @brief コンストラクタ
        /// @param name パネル名（"HeaderBackground"等）
        /// @param bg_color 背景色（デフォルト: 薄灰色 {243, 243, 243}）
        /// @param corner_radius 角の丸み半径（デフォルト: 10）
        UIPanelBackground(
            const char* name,
            const PanelLayout* layout,
            const paxg::Color& bg_color = paxg::Color{243, 243, 243},
            int corner_radius = 10
        )
            : name_(name)
            , bg_color_(bg_color)
            , corner_radius_(corner_radius)
            , layout(layout)
        {}

        paxg::Rect getRect() const override {
            if (layout) {
                return layout->getRect();
            }
            PAXS_WARNING("UIPanelBackground::getRect: layout is not set, returning full width rect");
            return paxg::Rect{};
        }

        EventHandlingResult handleEvent(const MouseEvent& event) override {
            // パネル背景上のすべてのマウスイベントを消費（背後へのクリック防止）
            if (event.left_button_state == MouseButtonState::Pressed ||
                event.left_button_state == MouseButtonState::Held ||
                event.left_button_state == MouseButtonState::Released) {
                return EventHandlingResult::Handled();
            }
            return EventHandlingResult::NotHandled();
        }

        void render() const override {
            if (!visible_) return;
            if (!layout) {
                PAXS_WARNING(std::string("UIPanelBackground::render: ") + name_ + " layout is not set, skipping render");
                return;
            }

            if (layout->width <= 0 || layout->height <= 0) {
                PAXS_WARNING(std::string("UIPanelBackground::render: ") + name_ + " has non-positive dimensions, skipping render");
                return;
            }

            PanelBackgroundRenderer::draw(
                layout->x, layout->y,
                layout->width, layout->height,
                corner_radius_, bg_color_
            );
        }

        const char* getName() const override {
            return name_;
        }
        RenderLayer getLayer() const override {
            return RenderLayer::UIBackground;
        }
        void setVisible(bool visible) override {
            visible_ = visible;
        }
        bool isVisible() const override {
            return visible_;
        }

        void setPos(const paxg::Vec2i& /*pos*/) override {}
        void setEnabled(bool /*enabled*/) override {}
        bool isEnabled() const override { return true; }
};

} // namespace paxs

#endif // !PAX_MAHOROBA_UI_UI_PANEL_BACKGROUND_HPP
