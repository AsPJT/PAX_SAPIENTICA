/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_SFML_TEXTURE_IMPL_HPP
#define PAX_GRAPHICA_SFML_TEXTURE_IMPL_HPP

#if defined(PAXS_USING_SFML)

#include <SFML/Graphics.hpp>
#include <PAX_GRAPHICA/Image.hpp>
#include <PAX_GRAPHICA/TextureImpl.hpp>
#include <PAX_GRAPHICA/Window.hpp>
#include <PAX_SAPIENTICA/AppConfig.hpp>

#include <algorithm> // std::min を使用するために追加
#include <cmath>     // std::min がfloatで曖昧な場合のため (fminでも可)

namespace paxg {

    class SFMLTextureImpl : public TextureImpl {
    private:
        sf::Texture texture;

    public:
        SFMLTextureImpl() = default;

        SFMLTextureImpl(const paxg::Image& image) {
            if (!texture.loadFromImage(image)) {
                PAXS_WARNING("Failed to load image");
            }
        }

        SFMLTextureImpl(const paxg::String& path) {
            std::string path_str = convertSvgToPng(path.string);
            if (!texture.loadFromFile(paxs::AppConfig::getInstance()->getRootPath() + path_str)) {
                PAXS_WARNING("Failed to load texture: " + path_str);
            }
        }

        SFMLTextureImpl(const std::string& path) {
            std::string path_str = convertSvgToPng(path);
            if (!texture.loadFromFile(paxs::AppConfig::getInstance()->getRootPath() + path_str)) {
                PAXS_WARNING("Failed to load texture: " + path_str);
            }
        }

        bool isValid() const override {
            return texture.getSize().x > 0;
        }

        int width() const override {
            return static_cast<int>(texture.getSize().x);
        }

        int height() const override {
            return static_cast<int>(texture.getSize().y);
        }

        void draw() const override {
            sf::Sprite sprite(texture);
            paxg::Window::window().draw(sprite);
        }

        void drawAt(const Vec2f& pos) const override {
            sf::Sprite sprite(texture);
            sprite.setPosition({ pos.x() - (width() / 2.0f), pos.y() - (height() / 2.0f) });
            paxg::Window::window().draw(sprite);
        }

        void drawAt(const Vec2i& pos) const override {
            sf::Sprite sprite(texture);
            sprite.setPosition({ static_cast<float>(pos.x() - (width() / 2)),
                                static_cast<float>(pos.y() - (height() / 2)) });
            paxg::Window::window().draw(sprite);
        }

        void resizedDrawAt(const Vec2i& resize, const Vec2i& pos) const override {
            sf::Sprite sprite(texture);
            sprite.setScale({ static_cast<float>(resize.x()) / texture.getSize().x,
                             static_cast<float>(resize.y()) / texture.getSize().y });
            sprite.setPosition({ static_cast<float>(pos.x() - (resize.x() / 2)),
                                static_cast<float>(pos.y() - (resize.y() / 2)) });
            paxg::Window::window().draw(sprite);
        }

        void resizedDrawAt(int resize, const Vec2i& pos) const override {
            sf::Sprite sprite(texture);

            // アスペクト比を維持して 'resize x resize' に収めるスケールを計算
            float scale_x = static_cast<float>(resize) / texture.getSize().x;
            float scale_y = static_cast<float>(resize) / texture.getSize().y;
            float scale = std::min(scale_x, scale_y);
            sprite.setScale({ scale, scale });

            // スケーリング後の実際の幅と高さでセンタリングする
            float new_width = static_cast<float>(width()) * scale;
            float new_height = static_cast<float>(height()) * scale;

            sprite.setPosition({ static_cast<float>(pos.x()) - (new_width / 2.0f),
                                static_cast<float>(pos.y()) - (new_height / 2.0f) });
            paxg::Window::window().draw(sprite);
        }

        void resizedDrawAt(const Vec2f& resize, const Vec2f& pos) const override {
            sf::Sprite sprite(texture);
            sprite.setScale({ resize.x() / texture.getSize().x, resize.y() / texture.getSize().y });
            sprite.setPosition({ pos.x() - (resize.x() / 2), pos.y() - (resize.y() / 2) });
            paxg::Window::window().draw(sprite);
        }

        void resizedDrawAt(int resize, const Vec2f& pos) const override {
            sf::Sprite sprite(texture);

            // アスペクト比を維持して 'resize x resize' に収めるスケールを計算
            float scale_x = static_cast<float>(resize) / texture.getSize().x;
            float scale_y = static_cast<float>(resize) / texture.getSize().y;
            float scale = std::min(scale_x, scale_y);
            sprite.setScale({ scale, scale });

            // スケーリング後の実際の幅と高さでセンタリングする
            float new_width = static_cast<float>(width()) * scale;
            float new_height = static_cast<float>(height()) * scale;

            sprite.setPosition({ pos.x() - (new_width / 2.0f),
                                pos.y() - (new_height / 2.0f) });
            paxg::Window::window().draw(sprite);
        }

        void resizedDraw(const Vec2i& resize, const Vec2i& pos) const override {
            sf::Sprite sprite(texture);
            sprite.setScale({ static_cast<float>(resize.x()) / texture.getSize().x,
                             static_cast<float>(resize.y()) / texture.getSize().y });
            sprite.setPosition({ static_cast<float>(pos.x()), static_cast<float>(pos.y()) });
            paxg::Window::window().draw(sprite);
        }

        void resizedDraw(int resize, const Vec2i& pos) const override {
            sf::Sprite sprite(texture);

            // アスペクト比を維持して 'resize x resize' に収めるスケールを計算
            float scale_x = static_cast<float>(resize) / texture.getSize().x;
            float scale_y = static_cast<float>(resize) / texture.getSize().y;
            float scale = std::min(scale_x, scale_y);
            sprite.setScale({ scale, scale });

            sprite.setPosition({ static_cast<float>(pos.x()), static_cast<float>(pos.y()) });
            paxg::Window::window().draw(sprite);
        }

        void resizedDraw(const Vec2f& resize, const Vec2f& pos) const override {
            sf::Sprite sprite(texture);
            sprite.setScale({ resize.x() / texture.getSize().x, resize.y() / texture.getSize().y });
            sprite.setPosition({ pos.x(), pos.y() });
            paxg::Window::window().draw(sprite);
        }

        void resizedDraw(int resize, const Vec2f& pos) const override {
            sf::Sprite sprite(texture);

            // アスペクト比を維持して 'resize x resize' に収めるスケールを計算
            float scale_x = static_cast<float>(resize) / texture.getSize().x;
            float scale_y = static_cast<float>(resize) / texture.getSize().y;
            float scale = std::min(scale_x, scale_y);
            sprite.setScale({ scale, scale });

            sprite.setPosition({ pos.x(), pos.y() });
            paxg::Window::window().draw(sprite);
        }

        const sf::Texture& getNativeTexture() const { return texture; }
    };

} // namespace paxg

#endif // defined(PAXS_USING_SFML)

#endif // !PAX_GRAPHICA_SFML_TEXTURE_IMPL_HPP
