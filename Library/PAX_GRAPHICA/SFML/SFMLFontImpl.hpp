/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_SFML_FONT_IMPL_HPP
#define PAX_GRAPHICA_SFML_FONT_IMPL_HPP

#ifdef PAXS_USING_SFML
#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/System/Utf.hpp>

#include <PAX_GRAPHICA/Color.hpp>
#include <PAX_GRAPHICA/Interface/FontImpl.hpp>
#include <PAX_GRAPHICA/Window.hpp>

#include <PAX_SAPIENTICA/Core/Type/Vector2.hpp>
#include <PAX_SAPIENTICA/System/AppConfig.hpp>
#include <PAX_SAPIENTICA/Utility/Logger.hpp>

namespace paxg {

    /// @brief SFML font implementation
    /// @brief SFMLフォント実装
    class SFMLFontImpl : public FontImpl {
    private:
        sf::Font font{};
        int size = 0;

    public:
        SFMLFontImpl(int size_, const std::string& path, int /*buffer_thickness*/) {
            size = size_;
            if (path.empty()) return;
            if (!font.openFromFile(paxs::AppConfig::getInstance().getRootPath() + path)) {
                PAXS_WARNING(path + " is missing.");
            }
        }

        void setOutline(double /*inner*/, double /*outer*/, const Color& /*color*/) override {
            // SFML doesn't support outline in this implementation
        }

        void drawBottomLeft(const std::string& str, const paxs::Vector2<int>& pos, const Color& color) const override {
            sf::Text text(font);
            std::wstring wstr;
            sf::Utf<8>::toWide(str.begin(), str.end(), std::back_inserter(wstr));
            text.setString(wstr);
            text.setCharacterSize(size);
            text.setFillColor(color);
            text.setOutlineColor(sf::Color::White);
            text.setOutlineThickness(2.0f);
            text.setPosition({ static_cast<float>(pos.x), static_cast<float>(pos.y - size / 2) });
            Window::window().draw(text);
        }

        void drawTopRight(const std::string& str, const paxs::Vector2<int>& pos, const Color& color) const override {
            sf::Text text(font);
            std::wstring wstr;
            sf::Utf<8>::toWide(str.begin(), str.end(), std::back_inserter(wstr));
            text.setString(wstr);
            text.setCharacterSize(size);
            text.setFillColor(color);
            text.setOutlineColor(sf::Color::White);
            text.setOutlineThickness(2.0f);
            text.setPosition({ static_cast<float>(pos.x - text.getGlobalBounds().size.x), static_cast<float>(pos.y + size / 2) });
            Window::window().draw(text);
        }

        void drawBottomRight(const std::string& str, const paxs::Vector2<int>& pos, const Color& color) const override {
            sf::Text text(font);
            std::wstring wstr;
            sf::Utf<8>::toWide(str.begin(), str.end(), std::back_inserter(wstr));
            text.setString(wstr);
            text.setCharacterSize(size);
            text.setFillColor(color);
            text.setOutlineColor(sf::Color::White);
            text.setOutlineThickness(2.0f);
            text.setPosition({ static_cast<float>(pos.x - text.getGlobalBounds().size.x), static_cast<float>(pos.y - text.getGlobalBounds().size.y) });
            Window::window().draw(text);
        }

        void draw(const std::string& str, const paxs::Vector2<int>& pos, const Color& color) const override {
            sf::Text text(font);
            std::wstring wstr;
            sf::Utf<8>::toWide(str.begin(), str.end(), std::back_inserter(wstr));
            text.setString(wstr);
            text.setCharacterSize(size);
            text.setFillColor(color);
            text.setOutlineColor(sf::Color::White);
            text.setOutlineThickness(2.0f);
            text.setPosition({ static_cast<float>(pos.x), static_cast<float>(pos.y) });
            Window::window().draw(text);
        }

        void drawBottomCenter(const std::string& str, const paxs::Vector2<int>& pos, const Color& color) const override {
            sf::Text text(font);
            std::wstring wstr;
            sf::Utf<8>::toWide(str.begin(), str.end(), std::back_inserter(wstr));
            text.setString(wstr);
            text.setCharacterSize(size);
            text.setFillColor(color);
            text.setOutlineColor(sf::Color::White);
            text.setOutlineThickness(2.0f);
            text.setPosition({ static_cast<float>(pos.x - text.getGlobalBounds().size.x / 2), static_cast<float>(pos.y - size / 2) });
            Window::window().draw(text);
        }

        void drawTopCenter(const std::string& str, const paxs::Vector2<int>& pos, const Color& color) const override {
            sf::Text text(font);
            std::wstring wstr;
            sf::Utf<8>::toWide(str.begin(), str.end(), std::back_inserter(wstr));
            text.setString(wstr);
            text.setCharacterSize(size);
            text.setFillColor(color);
            text.setOutlineColor(sf::Color::White);
            text.setOutlineThickness(2.0f);
            text.setPosition({ static_cast<float>(pos.x - text.getGlobalBounds().size.x / 2), static_cast<float>(pos.y + size / 2) });
            Window::window().draw(text);
        }

        void drawAt(const std::string& str, const paxs::Vector2<int>& pos, const Color& color) const override {
            sf::Text text(font);
            std::wstring wstr;
            sf::Utf<8>::toWide(str.begin(), str.end(), std::back_inserter(wstr));
            text.setString(wstr);
            text.setCharacterSize(size);
            text.setFillColor(color);
            text.setOutlineColor(sf::Color::White);
            text.setOutlineThickness(2.0f);
            const sf::FloatRect bounds = text.getGlobalBounds();
            text.setPosition({
                static_cast<float>(pos.x) - bounds.size.x / 2,
                static_cast<float>(pos.y) - bounds.size.y / 2
            });
            Window::window().draw(text);
        }

        int height() const override {
            return size;
        }

        int width(const std::string& str) override {
            sf::Text text(font);
            std::wstring wstr;
            sf::Utf<8>::toWide(str.begin(), str.end(), std::back_inserter(wstr));
            text.setString(wstr);
            text.setCharacterSize(size);
            return static_cast<int>(text.getGlobalBounds().size.x);
        }
    };

} // namespace paxg

#endif // PAXS_USING_SFML

#endif // !PAX_GRAPHICA_SFML_FONT_IMPL_HPP
