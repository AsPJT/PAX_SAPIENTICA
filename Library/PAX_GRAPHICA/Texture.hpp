/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_TEXTURE_HPP
#define PAX_GRAPHICA_TEXTURE_HPP

#if defined(PAXS_USING_SIV3D)
#include <Siv3D.hpp>
#elif defined(PAXS_USING_DXLIB)
#include <DxLib.h>
#elif defined(PAXS_USING_SFML)
#include <SFML/Graphics.hpp>
#endif

#include <PAX_GRAPHICA/IDrawable.hpp>
#include <PAX_GRAPHICA/Image.hpp>
#include <PAX_GRAPHICA/Window.hpp>
#include <PAX_SAPIENTICA/Logger.hpp>
#include <PAX_SAPIENTICA/StringExtensions.hpp>

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

#elif defined(PAXS_USING_DXLIB)
        int texture = -1;
        int x = 1;
        int y = 1;

        Texture(const paxg::String& path) {
            // svgの場合は読み込めないので、pngに拡張子を変換する
            std::string path_str = path.string;
            paxs::StringExtensions::replace(path_str, ".svg", ".png");

            texture = DxLib::LoadGraph(path_str.c_str());
            DxLib::GetGraphSize(texture, &x, &y);
        }
        Texture(const std::string& path) {
            // svgの場合は読み込めないので、pngに拡張子を変換する
            std::string path_str = path;
            paxs::StringExtensions::replace(path_str, ".svg", ".png");

            texture = DxLib::LoadGraph(path_str.c_str());
            DxLib::GetGraphSize(texture, &x, &y);
        }
        operator bool() const { return (texture != -1); }

#elif defined(PAXS_USING_SFML)
        sf::Texture texture{};
        Texture(const paxg::Image& image) {
            if (!texture.loadFromImage(image)) {
                PAXS_WARNING("Failed to load image");
            }
        }
        Texture(const paxg::String& path) {
            // svgの場合は読み込めないので、pngに拡張子を変換する
            std::string path_str = path.string;
            paxs::StringExtensions::replace(path_str, ".svg", ".png");

            if (!texture.loadFromFile(path_str)) {
                PAXS_WARNING("Failed to load texture: " + path_str);
            }
        }
        Texture(std::string path) {
            // svgの場合は読み込めないので、pngに拡張子を変換する
            paxs::StringExtensions::replace(path, ".svg", ".png");
            if (!texture.loadFromFile(static_cast<paxg::String>(path).string)) {
                PAXS_WARNING("Failed to load texture: " + path);
            }
        }
        operator sf::Texture() const { return texture; }

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

#elif defined(PAXS_USING_SFML)
            sf::Sprite sprite(texture);
            paxg::Window::window().draw(sprite);

#endif
        }

        int width() const {
#if defined(PAXS_USING_SIV3D)
            return texture.width();

#elif defined(PAXS_USING_DXLIB)
            int width = 0, height = 0;
            DxLib::GetGraphSize(texture, &width, &height);
            return width;

#elif defined(PAXS_USING_SFML)
            return texture.getSize().x;

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

#elif defined(PAXS_USING_SFML)
            return texture.getSize().y;

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

#elif defined(PAXS_USING_SFML)
            return !texture.getSize().x;

#else
            return true;
#endif
        }

#if defined(PAXS_USING_SIV3D)
        void drawAt(const paxg::Vec2f& pos) const override {
            texture.drawAt(pos.x(), pos.y());
        }
#elif defined(PAXS_USING_DXLIB)
        void drawAt(const paxg::Vec2f& pos) const override {
            DxLib::DrawGraph(
                static_cast<int>(pos.x() - (width() / 2)),
                static_cast<int>(pos.y() - (height() / 2)), texture, TRUE);
        }
#elif defined(PAXS_USING_SFML)
        void drawAt(const paxg::Vec2f& pos) const override {
            sf::Sprite sprite(texture);
            sprite.setPosition({ static_cast<float>(pos.x() - (width() / 2)), static_cast<float>(pos.y() - (height() / 2)) });
            paxg::Window::window().draw(sprite);
        }
#else
        void drawAt(const paxg::Vec2f&) const override {}
#endif

#if defined(PAXS_USING_SIV3D)
        void drawAt(const paxg::Vec2i& pos) const override {
            texture.drawAt(pos.x(), pos.y());
        }
#elif defined(PAXS_USING_DXLIB)
        void drawAt(const paxg::Vec2i& pos) const override {
            DxLib::DrawGraph(pos.x() - (width() / 2), pos.y() - (height() / 2), texture, TRUE);
        }
#elif defined(PAXS_USING_SFML)
        void drawAt(const paxg::Vec2i& pos) const override {
            sf::Sprite sprite(texture);
            sprite.setPosition({ static_cast<float>(pos.x() - (width() / 2)), static_cast<float>(pos.y() - (height() / 2)) });
            paxg::Window::window().draw(sprite);
        }
#else
        void drawAt(const paxg::Vec2i&) const override {}
#endif

#if defined(PAXS_USING_SIV3D)
        void resizedDrawAt(const paxg::Vec2i& resize, const paxg::Vec2i& pos) const {
            texture.resized(resize.x(), resize.y()).drawAt(pos.x(), pos.y());
        }
#elif defined(PAXS_USING_DXLIB)
        void resizedDrawAt(const paxg::Vec2i& resize, const paxg::Vec2i& pos) const {
            DxLib::DrawExtendGraph(
                pos.x() - (resize.x() / 2), pos.y() - (resize.y() / 2),
                pos.x() + (resize.x() / 2), pos.y() + (resize.y() / 2),
                texture, TRUE);
        }
#elif defined(PAXS_USING_SFML)
        void resizedDrawAt(const paxg::Vec2i& resize, const paxg::Vec2i& pos) const {
            sf::Sprite sprite(texture);
            sprite.setScale({ static_cast<float>(resize.x()) / texture.getSize().x, static_cast<float>(resize.y()) / texture.getSize().y });
            sprite.setPosition({ static_cast<float>(pos.x() - (resize.x() / 2)), static_cast<float>(pos.y() - (resize.y() / 2)) });
            paxg::Window::window().draw(sprite);
        }
#else
        void resizedDrawAt(const paxg::Vec2i&, const paxg::Vec2i&) const {}
#endif

#if defined(PAXS_USING_SIV3D)
        void resizedDrawAt(const int resize, const paxg::Vec2i & pos) const {
            texture.resized(resize).drawAt(pos.x(), pos.y());
        }
#elif defined(PAXS_USING_DXLIB)
        void resizedDrawAt(const int resize, const paxg::Vec2i & pos) const {
            const int resize_x = resize * this->x / this->y;
            DxLib::DrawExtendGraph(
                pos.x() - (resize_x / 2), pos.y() - (resize / 2),
                pos.x() + (resize_x / 2), pos.y() + (resize / 2),
                texture, TRUE);
        }
#elif defined(PAXS_USING_SFML)
        void resizedDrawAt(const int resize, const paxg::Vec2i & pos) const {
            sf::Sprite sprite(texture);
            sprite.setScale({ static_cast<float>(resize) / texture.getSize().x, static_cast<float>(resize) / texture.getSize().y });
            sprite.setPosition({ static_cast<float>(pos.x() - (resize / 2)), static_cast<float>(pos.y() - (resize / 2)) });
            paxg::Window::window().draw(sprite);
        }
#else
        void resizedDrawAt(const int, const paxg::Vec2i&) const {}
#endif

#if defined(PAXS_USING_SIV3D)
        void resizedDrawAt(const paxg::Vec2f & resize, const paxg::Vec2f & pos) const {
            texture.resized(resize.x(), resize.y()).drawAt(pos.x(), pos.y());
        }
#elif defined(PAXS_USING_DXLIB)
        void resizedDrawAt(const paxg::Vec2f & resize, const paxg::Vec2f & pos) const {
            DxLib::DrawExtendGraph(
                static_cast<int>(pos.x() - (resize.x() / 2)), static_cast<int>(pos.y() - (resize.y() / 2)),
                static_cast<int>(pos.x() + (resize.x() / 2)), static_cast<int>(pos.y() + (resize.y() / 2)),
                texture, TRUE);
        }
#elif defined(PAXS_USING_SFML)
        void resizedDrawAt(const paxg::Vec2f & resize, const paxg::Vec2f & pos) const {
            sf::Sprite sprite(texture);
            sprite.setScale({ resize.x() / texture.getSize().x, resize.y() / texture.getSize().y });
            sprite.setPosition({ pos.x() - (resize.x() / 2), pos.y() - (resize.y() / 2) });
            paxg::Window::window().draw(sprite);
        }
#else
        void resizedDrawAt(const paxg::Vec2f&, const paxg::Vec2f&) const {}
#endif

#if defined(PAXS_USING_SIV3D)
        void resizedDrawAt(const int resize, const paxg::Vec2f & pos) const {
            texture.resized(resize).drawAt(pos.x(), pos.y());
        }
#elif defined(PAXS_USING_DXLIB)
        void resizedDrawAt(const int resize, const paxg::Vec2f & pos) const {
            DxLib::DrawExtendGraph(
                static_cast<int>(pos.x() - (resize / 2)), static_cast<int>(pos.y() - (resize / 2)),
                static_cast<int>(pos.x() + (resize / 2)), static_cast<int>(pos.y() + (resize / 2)),
                texture, TRUE);
        }
#elif defined(PAXS_USING_SFML)
        void resizedDrawAt(const int resize, const paxg::Vec2f & pos) const {
            sf::Sprite sprite(texture);
            sprite.setScale({ static_cast<float>(resize) / texture.getSize().x, static_cast<float>(resize) / texture.getSize().y });
            sprite.setPosition({ pos.x() - (resize / 2), pos.y() - (resize / 2) });
            paxg::Window::window().draw(sprite);
        }
#else
        void resizedDrawAt(const int, const paxg::Vec2f&) const {}
#endif

#if defined(PAXS_USING_SIV3D)
        void resizedDraw(const paxg::Vec2i & resize, const paxg::Vec2i & pos) const {
            texture.resized(resize.x(), resize.y()).draw(pos.x(), pos.y());
        }
#elif defined(PAXS_USING_DXLIB)
        void resizedDraw(const paxg::Vec2i & resize, const paxg::Vec2i & pos) const {
            DxLib::DrawExtendGraph(
                pos.x(), pos.y(),
                pos.x() + resize.x(), pos.y() + resize.y(),
                texture, TRUE);
        }
#elif defined(PAXS_USING_SFML)
        void resizedDraw(const paxg::Vec2i & resize, const paxg::Vec2i & pos) const {
            sf::Sprite sprite(texture);
            sprite.setScale({ static_cast<float>(resize.x()) / texture.getSize().x, static_cast<float>(resize.y()) / texture.getSize().y });
            sprite.setPosition({ static_cast<float>(pos.x()), static_cast<float>(pos.y()) });
            paxg::Window::window().draw(sprite);
        }
#else
        void resizedDraw(const paxg::Vec2i&, const paxg::Vec2i&) const {}
#endif

#if defined(PAXS_USING_SIV3D)
        void resizedDraw(const int resize, const paxg::Vec2i & pos) const {
            texture.resized(resize).draw(pos.x(), pos.y());
        }
#elif defined(PAXS_USING_DXLIB)
        void resizedDraw(const int resize, const paxg::Vec2i & pos) const {
            DxLib::DrawExtendGraph(
                pos.x(), pos.y(),
                pos.x() + resize, pos.y() + resize,
                texture, TRUE);
        }
#elif defined(PAXS_USING_SFML)
        void resizedDraw(const int resize, const paxg::Vec2i & pos) const {
            sf::Sprite sprite(texture);
            sprite.setScale({ static_cast<float>(resize) / texture.getSize().x, static_cast<float>(resize) / texture.getSize().y });
            sprite.setPosition({ static_cast<float>(pos.x()), static_cast<float>(pos.y()) });
            paxg::Window::window().draw(sprite);
        }
#else
        void resizedDraw(const int, const paxg::Vec2i&) const {}
#endif

#if defined(PAXS_USING_SIV3D)
        void resizedDraw(const paxg::Vec2f & resize, const paxg::Vec2f & pos) const {
            texture.resized(resize.x(), resize.y()).draw(pos.x(), pos.y());
        }
#elif defined(PAXS_USING_DXLIB)
        void resizedDraw(const paxg::Vec2f & resize, const paxg::Vec2f & pos) const {
            DxLib::DrawExtendGraph(
                static_cast<int>(pos.x()), static_cast<int>(pos.y()),
                static_cast<int>(pos.x() + resize.x()), static_cast<int>(pos.y() + resize.y()),
                texture, TRUE);
        }
#elif defined(PAXS_USING_SFML)
        void resizedDraw(const paxg::Vec2f & resize, const paxg::Vec2f & pos) const {
            sf::Sprite sprite(texture);
            sprite.setScale({ resize.x() / texture.getSize().x, resize.y() / texture.getSize().y });
            sprite.setPosition({ pos.x(), pos.y() });
            paxg::Window::window().draw(sprite);
        }
#else
        void resizedDraw(const paxg::Vec2f&, const paxg::Vec2f&) const {}
#endif

#if defined(PAXS_USING_SIV3D)
        void resizedDraw(const int resize, const paxg::Vec2f & pos) const {
            texture.resized(resize).draw(pos.x(), pos.y());
        }
#elif defined(PAXS_USING_DXLIB)
        void resizedDraw(const int resize, const paxg::Vec2f & pos) const {
            DxLib::DrawExtendGraph(
                static_cast<int>(pos.x()), static_cast<int>(pos.y()),
                static_cast<int>(pos.x() + resize), static_cast<int>(pos.y() + resize),
                texture, TRUE);
        }
#elif defined(PAXS_USING_SFML)
        void resizedDraw(const int resize, const paxg::Vec2f & pos) const {
            sf::Sprite sprite(texture);
            sprite.setScale({ static_cast<float>(resize) / texture.getSize().x, static_cast<float>(resize) / texture.getSize().y });
            sprite.setPosition({ pos.x(), pos.y() });
            paxg::Window::window().draw(sprite);
        }
#else
        void resizedDraw(const int, const paxg::Vec2f&) const {}
#endif
    };

}

#endif // !PAX_GRAPHICA_TEXTURE_HPP
