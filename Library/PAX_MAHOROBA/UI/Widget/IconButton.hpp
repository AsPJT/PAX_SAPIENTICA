
/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com		https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_UI_ICON_BUTTON_HPP
#define PAX_MAHOROBA_UI_ICON_BUTTON_HPP

#include <PAX_GRAPHICA/Rect.hpp>
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
            if (!icon_textures.contains(icon_texture_path_hash)) return;

            icon_textures.at(icon_texture_path_hash).resizedDraw(rect.width(),
                paxg::Vec2i{ rect.x(), rect.y() }
            );
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

        inline static paxs::KeyValueTSV<paxg::Texture> icon_textures;

        static void loadIconTextures() {
            icon_textures.input(AppConfig::getInstance().getSettingPath(MurMur3::calcHash("MenuIcons")),
                [&](const std::string& value_) { return paxg::Texture(value_); });
        }
    };
}

#endif // !PAX_MAHOROBA_UI_ICON_BUTTON_HPP
