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
#include <PAX_GRAPHICA/Window.hpp>

#include <PAX_MAHOROBA/Rendering/IWidget.hpp>
#include <PAX_MAHOROBA/UI/PanelBackground.hpp>
#include <PAX_MAHOROBA/UI/UILayout.hpp>

namespace paxs {

    /// @brief UIパネルの背景を描画するクラス
    /// @brief UI panel background rendering class
    class UIPanelBackground : public IWidget {
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
            , layout_(layout)
        {}

        /// @brief 高さを設定（画面幅全体を使用するパネル用）
        /// @brief Set height (for panels that use full screen width)
        void setHeight(int height) {
            (void)height;
        }

        const char* getName() const override {
            return name_;
        }

        RenderLayer getLayer() const override {
            return RenderLayer::UIBackground;
        }

        void setEnabled(bool enabled) override {
            enabled_ = enabled;
        }

        bool isEnabled() const override {
            return enabled_;
        }

        paxg::Rect getRect() const override {
            if (layout_) {
                return paxg::Rect{
                    static_cast<float>(layout_->x),
                    static_cast<float>(layout_->y),
                    static_cast<float>(layout_->width),
                    static_cast<float>(layout_->height)
                };
            }
            else {
                PAXS_WARNING("UIPanelBackground::getRect: layout is not set, returning full width rect");
                return paxg::Rect{
                    0.0f,
                    0.0f,
                    0.0f,
                    0.0f
                };
            }
        }

        void setPos(const paxg::Vec2i& pos) override {
            (void)pos;
        }

        void setVisible(bool visible) override {
            visible_ = visible;
        }

        bool isVisible() const override {
            return visible_;
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

            if (layout_) {
                if (layout_->width <= 0 || layout_->height <= 0) return;

                background_.draw(
                    layout_->x,
                    layout_->y,
                    layout_->width,
                    layout_->height,
                    corner_radius_,
                    bg_color_
                );
            }
            else {
                PAXS_WARNING(std::string("UIPanelBackground::render: ") + name_ + " layout is not set, skipping render");
            }
        }

    private:
        const char* name_;                  // パネル名
        PanelBackground background_;        // 背景描画ヘルパー（低レベル描画）
        const PanelLayout* layout_;                // レイアウト情報
        paxg::Color bg_color_;              // 背景色
        int corner_radius_;                 // 角の丸み半径
        bool visible_ = true;               // 可視性
        bool enabled_ = true;               // 有効性
    };

} // namespace paxs

#endif // !PAX_MAHOROBA_UI_UI_PANEL_BACKGROUND_HPP
