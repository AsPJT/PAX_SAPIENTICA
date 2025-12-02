
/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com		https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_UI_ICON_BUTTON_HPP
#define PAX_MAHOROBA_UI_ICON_BUTTON_HPP

#include <PAX_GRAPHICA/Color.hpp>
#include <PAX_GRAPHICA/Rect.hpp>
#include <PAX_GRAPHICA/RoundRect.hpp>
#include <PAX_GRAPHICA/Texture.hpp>
#include <PAX_GRAPHICA/Vec2.hpp>

#include <PAX_MAHOROBA/Rendering/InteractiveUIComponent.hpp>

#include <PAX_SAPIENTICA/IO/Data/KeyValueTSV.hpp>
#include <PAX_SAPIENTICA/System/AppConfig.hpp>

namespace paxs {

    /// @brief アイコンボタンウィジェット
    class IconButton : public InteractiveUIComponent {
    public:
        IconButton(const char* button_name, const std::uint_least32_t icon_texture_path_hash)
            : icon_texture_path_hash(icon_texture_path_hash), name(button_name) {}

        void render() const override {
            if (!isVisible()) return;
            if (!icon_textures.isLoaded()) {
                loadIconTextures();
            }
            if (!icon_textures.isSuccessfullyLoaded()) return;

            const paxg::Texture* const icon_texture_ptr = icon_textures.try_get(icon_texture_path_hash);
            if (icon_texture_ptr == nullptr) return;

            // ボタン状態に応じた背景を描画（3pxのマージン付き）
            constexpr int margin = 3;
            if (is_pressed) {
                // 押された時: 濃い灰色 (RGB: 213, 213, 213)
                paxg::RoundRect background(
                    rect.x() - margin,
                    rect.y() - margin,
                    rect.width() + (margin * 2),
                    rect.height() + (margin * 2)
                );
                background.draw(paxg::Color(213, 213, 213));
            }
            else if (is_hovered) {
                // ホバー時: 薄い灰色 (RGB: 228, 228, 228)
                paxg::RoundRect background(
                    rect.x() - margin,
                    rect.y() - margin,
                    rect.width() + (margin * 2),
                    rect.height() + (margin * 2)
                );
                background.draw(paxg::Color(228, 228, 228));
            }

            // アイコンを描画
            icon_texture_ptr->resizedDraw(rect.width(),
                paxs::Vector2<int>{ rect.x(), rect.y() }
            );

            is_hovered = false;
            is_pressed = false;
        }

        EventHandlingResult handleEvent(const MouseEvent& event) override {
            if (!isVisible()) {
                is_hovered = false;
                is_pressed = false;
                return EventHandlingResult::NotHandled();
            }

            // ホバー状態を更新
            is_hovered = isHit(event.pos);
            is_pressed = is_hovered && (event.left_button_state != MouseButtonState::None);

            return EventHandlingResult::NotHandled();
        }

        Rect<int> getRect() const override { return rect; }
        const char* getName() const override { return name; }
        bool isVisible() const override { return true; }
        void setPos(const Vector2<int>& pos) override { rect.position = pos; }

    protected:
        void setSize(const Vector2<int>& size) { rect.size = size; }

    private:
        const std::uint_least32_t icon_texture_path_hash;
        const char* name;

        paxs::Rect<int> rect;

        mutable bool is_hovered = false;
        mutable bool is_pressed = false;

        inline static paxs::KeyValueTSV<paxg::Texture> icon_textures;

        static void loadIconTextures() {
            icon_textures.input(AppConfig::getInstance().getSettingPath(MurMur3::calcHash("MenuIcons")),
                [&](const std::string& value_) { return paxg::Texture(value_); });
        }
    };
}

#endif // !PAX_MAHOROBA_UI_ICON_BUTTON_HPP
