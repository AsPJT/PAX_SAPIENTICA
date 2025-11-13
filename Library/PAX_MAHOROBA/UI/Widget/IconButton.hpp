
/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com		https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_UI_ICON_BUTTON_HPP
#define PAX_MAHOROBA_UI_ICON_BUTTON_HPP

#include <PAX_GRAPHICA/Texture.hpp>

#include <PAX_MAHOROBA/Rendering/IWidget.hpp>

#include <PAX_SAPIENTICA/IO/Data/KeyValueTSV.hpp>
#include <PAX_SAPIENTICA/System/AppConfig.hpp>

namespace paxs {

    /// @brief アイコンボタンウィジェット
    class IconButton : public IWidget {
    public:
        IconButton(const char* button_name, const std::uint_least32_t icon_texture_path_hash)
            : name(button_name), icon_texture_path_hash(icon_texture_path_hash) {}

        void render() const override {
            if (!isVisible()) return;
            if (!icon_textures.isLoaded()) {
                loadIconTextures();
            }
            if (!icon_textures.isSuccessfullyLoaded()) return;
            if (!icon_textures.contains(icon_texture_path_hash)) return;

            icon_textures[icon_texture_path_hash].resizedDraw(static_cast<int>(rect.w()), paxg::Vec2i{
                static_cast<int>(rect.x()),
                static_cast<int>(rect.y())
            });
        }

        paxg::Rect getRect() const override { return rect; }
        const char* getName() const override { return name; }
        bool isVisible() const override { return true; }
        void setPos(const paxg::Vec2i& pos) override { rect.setPos(pos); }

    protected:
        void setSize(const paxg::Vec2i& size) { rect.setSize(size); }

    private:
        const std::uint_least32_t icon_texture_path_hash;
        const char* name;

        paxg::Rect rect;

        inline static paxs::KeyValueTSV<paxg::Texture> icon_textures;

        static void loadIconTextures() {
            icon_textures.input("Data/MenuIcon/MenuIcons.tsv",
                [&](const std::string& value_) { return paxg::Texture(value_); });
        }
    };
}

#endif // !PAX_MAHOROBA_UI_ICON_BUTTON_HPP
