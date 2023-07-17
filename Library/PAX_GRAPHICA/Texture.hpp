/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023 As Project
    [Production]	2023 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_TEXTURE_HPP
#define PAX_GRAPHICA_TEXTURE_HPP

/*##########################################################################################

##########################################################################################*/

#ifdef PAXS_USING_SIV3D
#include <Siv3D.hpp>
#elif defined(PAXS_USING_SFML)
#include <SFML/Graphics.hpp>
#endif

#include <PAX_GRAPHICA/IDrawable.hpp>
#include <PAX_GRAPHICA/Image.hpp>
#include <PAX_GRAPHICA/Window.hpp>

namespace paxg {

    struct Texture : public paxg::IDrawable
    {
#ifdef PAXS_USING_SIV3D
        s3d::Texture texture;
        constexpr Texture(const paxg::Image& image) : texture(image) {}
        constexpr Texture(const paxg::String& path) : texture(path) {}
        constexpr operator s3d::Texture() const { return texture; }
#elif defined(PAXS_USING_SFML)
        sf::Texture texture;
        Texture(const paxg::Image& image) { texture.loadFromImage(image); }
        Texture(const paxg::String& path) { texture.loadFromFile(path); }
        operator sf::Texture() const { return texture; }
#else
        constexpr (const paxg::String&) {}
#endif
        void draw() const override {}

        void drawAt(const paxg::Vec2f& pos) const override
        {
#ifdef PAXS_USING_SIV3D
            texture.draw(pos);
#elif defined(PAXS_USING_SFML)
            sf::Sprite sprite(texture);
            sprite.setPosition(pos);
            paxg::Window::window.draw(sprite);
#endif
        }
    };
}

#endif // !PAX_GRAPHICA_TEXTURE_HPP