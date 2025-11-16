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
        const IWidget* widget_; // 動的なパネル用（優先）
        const paxg::Color bg_color_;
        const int corner_radius_;
        bool visible_ = true;

    public:
        /// @brief コンストラクタ（PanelLayout使用）
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
            , widget_(nullptr)
        {}

        /// @brief コンストラクタ（IWidget使用 - 動的パネル用）
        /// @param name パネル名
        /// @param widget 動的なパネルへのポインタ
        /// @param bg_color 背景色
        /// @param corner_radius 角の丸み半径
        UIPanelBackground(
            const char* name,
            const IWidget* widget,
            const paxg::Color& bg_color = paxg::Color{243, 243, 243},
            int corner_radius = 10
        )
            : name_(name)
            , bg_color_(bg_color)
            , corner_radius_(corner_radius)
            , layout(nullptr)
            , widget_(widget)
        {}

        paxg::Rect getRect() const override {
            // widgetが設定されている場合はそちらを優先（動的パネル）
            if (widget_ != nullptr) {
                return widget_->getRect();
            }
            // 従来のPanelLayout使用
            if (layout != nullptr) {
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
            if (!visible_) {
                return;
            }

            // widget_が設定されている場合（動的パネル）
            if (widget_ != nullptr) {
                const paxg::Rect rect = widget_->getRect();
                if (rect.w() <= 0 || rect.h() <= 0) {
                    PAXS_WARNING(std::string("UIPanelBackground::render: ") + name_ + " (widget) has non-positive dimensions, skipping render");
                    return;
                }
                PanelBackgroundRenderer::draw(
                    static_cast<int>(rect.x()),
                    static_cast<int>(rect.y()),
                    static_cast<int>(rect.w()),
                    static_cast<int>(rect.h()),
                    corner_radius_, bg_color_
                );
                return;
            }

            // layoutが設定されている場合（従来）
            if (layout == nullptr) {
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

        const char* getName() const override { return name_; }
        RenderLayer getLayer() const override { return RenderLayer::UIBackground; }
        void setVisible(bool visible) { visible_ = visible; }
        bool isVisible() const override { return visible_; }
        void setPos(const paxg::Vec2i& /*pos*/) override {}
};

} // namespace paxs

#endif // !PAX_MAHOROBA_UI_UI_PANEL_BACKGROUND_HPP
