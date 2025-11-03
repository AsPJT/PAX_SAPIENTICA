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
#include <PAX_GRAPHICA/RenderTexture.hpp>
#include <PAX_GRAPHICA/Rect.hpp>
#include <PAX_GRAPHICA/Vec2.hpp>
#include <PAX_GRAPHICA/Window.hpp>

#include <PAX_MAHOROBA/Rendering/IWidget.hpp>
#include <PAX_MAHOROBA/UI/PanelBackground.hpp>
#include <PAX_MAHOROBA/UI/UILayout.hpp>

namespace paxs {

    /// @brief UIパネルの背景を描画するウィジェット
    /// @brief UI panel background rendering widget
    ///
    /// PanelBackgroundクラス（低レベル描画ヘルパー）をラップし、
    /// IWidgetインターフェースとレイアウト管理機能を提供します。
    /// Wraps PanelBackground class (low-level drawing helper) and provides
    /// IWidget interface and layout management functionality.
    ///
    /// 薄灰色（{243, 243, 243}）を標準背景色として使用します。
    /// Uses light gray ({243, 243, 243}) as the standard background color.
    class UIPanelBackground : public IWidget {
    public:
        /// @brief コンストラクタ
        /// @param name パネル名（"HeaderBackground"等）
        /// @param bg_color 背景色（デフォルト: 薄灰色 {243, 243, 243}）
        /// @param corner_radius 角の丸み半径（デフォルト: 10）
        UIPanelBackground(
            const char* name,
            const paxg::Color& bg_color = paxg::Color{243, 243, 243},
            int corner_radius = 10
        )
            : name_(name)
            , bg_color_(bg_color)
            , corner_radius_(corner_radius)
        {}

        /// @brief レイアウトを設定（通常のパネル用）
        /// @brief Set layout (for normal panels)
        /// @param layout レイアウト情報（nullptrの場合は画面幅全体を使用）
        void setLayout(const PanelLayout* layout) {
            if (layout) {
                layout_ = *layout;
                use_layout_ = true;
            } else {
                use_layout_ = false;
            }
        }

        /// @brief 高さを設定（画面幅全体を使用するパネル用）
        /// @brief Set height (for panels that use full screen width)
        void setHeight(int height) {
            layout_.height = height;
        }

        // IWidget インターフェース実装

        const char* getName() const override {
            return name_;
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
            if (use_layout_) {
                return paxg::Rect{
                    static_cast<float>(layout_.x),
                    static_cast<float>(layout_.y),
                    static_cast<float>(layout_.width),
                    static_cast<float>(layout_.height)
                };
            }
            else {
                // 画面幅全体を使用（ヘッダーパネル等）
                return paxg::Rect{
                    0.0f,
                    0.0f,
                    static_cast<float>(paxg::Window::width()),
                    static_cast<float>(layout_.height)
                };
            }
        }

        void setPos(const paxg::Vec2i& pos) override {
            if (use_layout_) {
                layout_.x = pos.x();
                layout_.y = pos.y();
            }
            // use_layout_ == false の場合は位置指定を無視（常に画面上部）
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

            if (use_layout_) {
                // レイアウトを使用する場合
                if (layout_.width <= 0 || layout_.height <= 0) return;

                background_.draw(
                    layout_.x,
                    layout_.y,
                    layout_.width,
                    layout_.height,
                    corner_radius_,
                    bg_color_
                );
            }
            else {
                // 画面幅全体を使用する場合（ヘッダーパネル等）
                if (layout_.height <= 0) return;

                background_.drawRect(
                    0, 0,
                    paxg::Window::width(),
                    layout_.height,
                    bg_color_
                );
            }
        }

    private:
        const char* name_;                  // パネル名
        PanelBackground background_;        // 背景描画ヘルパー（低レベル描画）
        PanelLayout layout_;                // レイアウト情報
        paxg::Color bg_color_;              // 背景色
        int corner_radius_;                 // 角の丸み半径
        bool use_layout_ = true;            // レイアウト使用フラグ
        bool visible_ = true;               // 可視性
        bool enabled_ = true;               // 有効性
    };

} // namespace paxs

#endif // !PAX_MAHOROBA_UI_UI_PANEL_BACKGROUND_HPP
