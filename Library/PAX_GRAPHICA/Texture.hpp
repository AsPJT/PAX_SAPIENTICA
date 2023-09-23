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

#if defined(PAXS_USING_SIV3D)
#include <Siv3D.hpp>
#elif defined(PAXS_USING_SFML)
#include <SFML/Graphics.hpp>
#elif defined(PAXS_USING_DXLIB)
#include <DxLib.h>
#endif

#include <PAX_GRAPHICA/IDrawable.hpp>
#include <PAX_GRAPHICA/Image.hpp>
#include <PAX_GRAPHICA/Window.hpp>

namespace paxg {

    struct Texture : public paxg::IDrawable
    {
        Texture() = default;
#if defined(PAXS_USING_SIV3D)
        s3d::Texture texture{};
        Texture(const paxg::Image& image) : texture(image) {}
        Texture(const paxg::String& path) : texture(path.string) {}
        Texture(const std::string& path) : texture(static_cast<paxg::String>(path).string) {}
        operator s3d::Texture() const { return texture; }
        operator bool() const { return (!!texture); }

#elif defined(PAXS_USING_SFML)
        sf::Texture texture{};
        Texture(const paxg::Image& image) { texture.loadFromImage(image); }
        Texture(const paxg::String& path) { texture.loadFromFile(path); }
        Texture(const std::string& path) {
            texture.loadFromFile(static_cast<paxg::String>(path));
        }
        operator sf::Texture() const { return texture; }
#elif defined(PAXS_USING_DXLIB)
        int texture = -1;

        Texture(const paxg::String& path) {
            texture = DxLib::LoadGraph(path.string.c_str());
        }
        Texture(const std::string& path) {
            texture = DxLib::LoadGraph(static_cast<paxg::String>(path).string.c_str());
        }
        operator bool() const { return (texture != -1); }
#else
        constexpr Texture(const paxg::String&) {}
        Texture([[maybe_unused]] const std::string& path) {}
        operator bool() const { return false; }
#endif
        void draw() const override {
#if defined(PAXS_USING_SIV3D)
            texture.draw();
#elif defined(PAXS_USING_DXLIB)
            DxLib::DrawGraph(0, 0, texture, TRUE);
#endif
        }
        int width() const {
#if defined(PAXS_USING_SIV3D)
            return texture.width();
#elif defined(PAXS_USING_DXLIB)
            int width = 0, height = 0;
            DxLib::GetGraphSize(texture, &width, &height);
            return width;
#else
            return 0;
#endif
        }
        int height() const {
#if defined(PAXS_USING_SIV3D)
            return texture.height();
#elif defined(PAXS_USING_DXLIB)
            int width = 0, height = 0;
            DxLib::GetGraphSize(texture, &width, &height);
            return height;
#else
            return 0;
#endif
        }
        // 画像が読み込まれているか
        bool operator!() const {
#if defined(PAXS_USING_SIV3D)
            return !texture;
#elif defined(PAXS_USING_DXLIB)
            return texture == -1;
#else
            return true;
#endif
        }

        void drawAt(const paxg::Vec2f& pos) const override
        {
#if defined(PAXS_USING_SIV3D)
            texture.drawAt(pos.x(), pos.y());
#elif defined(PAXS_USING_DXLIB)
            DxLib::DrawGraph(
                static_cast<int>(pos.x() - (width() / 2)),
                static_cast<int>(pos.y() - (height() / 2)), texture, TRUE);
#elif defined(PAXS_USING_SFML)
            sf::Sprite sprite(texture);
            sprite.setPosition(pos);
            paxg::Window::window.draw(sprite);
#endif
        }
        void drawAt(const paxg::Vec2i& pos) const override
        {
#if defined(PAXS_USING_SIV3D)
            texture.drawAt(pos.x(), pos.y());
#elif defined(PAXS_USING_DXLIB)
            DxLib::DrawGraph(pos.x() - (width() / 2), pos.y() - (height() / 2), texture, TRUE);
#elif defined(PAXS_USING_SFML)
            sf::Sprite sprite(texture);
            sprite.setPosition(pos);
            paxg::Window::window.draw(sprite);
#endif
        }

        void resizedDrawAt(const paxg::Vec2i& resize, const paxg::Vec2i& pos) const
        {
#if defined(PAXS_USING_SIV3D)
            texture.resized(resize.x(), resize.y()).drawAt(pos.x(), pos.y());
#elif defined(PAXS_USING_DXLIB)
            DxLib::DrawExtendGraph(
                pos.x() - (resize.x() / 2), pos.y() - (resize.y() / 2),
                pos.x() + (resize.x() / 2), pos.y() + (resize.y() / 2),
                texture, TRUE);
#elif defined(PAXS_USING_SFML)
            drawAt(pos);
#endif
        }
        void resizedDrawAt(const int resize, const paxg::Vec2i& pos) const
        {
#if defined(PAXS_USING_SIV3D)
            texture.resized(resize).drawAt(pos.x(), pos.y());
#elif defined(PAXS_USING_DXLIB)
            DxLib::DrawExtendGraph(
                pos.x() - (resize / 2), pos.y() - (resize / 2),
                pos.x() + (resize / 2), pos.y() + (resize / 2),
                texture, TRUE);
#elif defined(PAXS_USING_SFML)
            drawAt(pos);
#endif
        }
        void resizedDrawAt(const paxg::Vec2f& resize, const paxg::Vec2f& pos) const
        {
#if defined(PAXS_USING_SIV3D)
            texture.resized(resize.x(), resize.y()).drawAt(pos.x(), pos.y());
#elif defined(PAXS_USING_DXLIB)
            DxLib::DrawExtendGraph(
                pos.x() - (resize.x() / 2), pos.y() - (resize.y() / 2),
                pos.x() + (resize.x() / 2), pos.y() + (resize.y() / 2),
                texture, TRUE);
#elif defined(PAXS_USING_SFML)
            drawAt(pos);
#endif
        }
        void resizedDrawAt(const int resize, const paxg::Vec2f& pos) const
        {
#if defined(PAXS_USING_SIV3D)
            texture.resized(resize).drawAt(pos.x(), pos.y());
#elif defined(PAXS_USING_DXLIB)
            DxLib::DrawExtendGraph(
                pos.x() - (resize / 2), pos.y() - (resize / 2),
                pos.x() + (resize / 2), pos.y() + (resize / 2),
                texture, TRUE);
#elif defined(PAXS_USING_SFML)
            drawAt(pos);
#endif
        }
        void resizedDraw(const paxg::Vec2i& resize, const paxg::Vec2i& pos) const
        {
#if defined(PAXS_USING_SIV3D)
            texture.resized(resize.x(), resize.y()).draw(pos.x(), pos.y());
#elif defined(PAXS_USING_DXLIB)
            DxLib::DrawExtendGraph(
                pos.x(), pos.y(),
                pos.x() + resize.x(), pos.y() + resize.y(),
                texture, TRUE);
#elif defined(PAXS_USING_SFML)
            draw(pos);
#endif
        }
        void resizedDraw(const int resize, const paxg::Vec2i& pos) const
        {
#if defined(PAXS_USING_SIV3D)
            texture.resized(resize).draw(pos.x(), pos.y());
#elif defined(PAXS_USING_DXLIB)
            DxLib::DrawExtendGraph(
                pos.x(), pos.y(),
                pos.x() + resize, pos.y() + resize,
                texture, TRUE);
#elif defined(PAXS_USING_SFML)
            draw(pos);
#endif
        }
        void resizedDraw(const paxg::Vec2f& resize, const paxg::Vec2f& pos) const
        {
#if defined(PAXS_USING_SIV3D)
            texture.resized(resize.x(), resize.y()).draw(pos.x(), pos.y());
#elif defined(PAXS_USING_DXLIB)
            DxLib::DrawExtendGraph(
                pos.x(), pos.y(),
                pos.x() + resize.x(), pos.y() + resize.y(),
                texture, TRUE);
#elif defined(PAXS_USING_SFML)
            draw(pos);
#endif
        }
        void resizedDraw(const int resize, const paxg::Vec2f& pos) const
        {
#if defined(PAXS_USING_SIV3D)
            texture.resized(resize).draw(pos.x(), pos.y());
#elif defined(PAXS_USING_DXLIB)
            DxLib::DrawExtendGraph(
                pos.x(), pos.y(),
                pos.x() + resize, pos.y() + resize,
                texture, TRUE);
#elif defined(PAXS_USING_SFML)
            draw(pos);
#endif
        }
    };




}

#endif // !PAX_GRAPHICA_TEXTURE_HPP
