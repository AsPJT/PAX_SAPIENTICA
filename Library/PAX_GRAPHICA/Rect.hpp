/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_RECT_HPP
#define PAX_GRAPHICA_RECT_HPP

#if defined(PAXS_USING_SIV3D)
#include <Siv3D.hpp>
#elif defined(PAXS_USING_DXLIB)
#include <DxLib.h>
#elif defined(PAXS_USING_SFML)
#include <SFML/Graphics.hpp>
#endif

#include <PAX_GRAPHICA/Color.hpp>
#include <PAX_GRAPHICA/IDrawable.hpp>
#include <PAX_GRAPHICA/Mouse.hpp>
#include <PAX_GRAPHICA/Vec2.hpp>
#include <PAX_GRAPHICA/Window.hpp>

namespace paxg {

    struct Rect : public paxg::IDrawable {
#if defined(PAXS_USING_SIV3D)
        s3d::RectF rect{};
        constexpr Rect() = default;
        constexpr Rect(const float x, const float y, const float w, const float h) : rect(x, y, w, h) {}
        constexpr Rect(const Vec2i& pos, const Vec2i& size) : rect(pos.x(), pos.y(), size.x(), size.y()) {}
        constexpr Rect(const Vec2i& pos, const float w, const float h) : rect(pos.x(), pos.y(), w, h) {}
        constexpr Rect(const float x, const float y, const Vec2i& size) : rect(x, y, size.x(), size.y()) {}
        constexpr operator s3d::RectF() const { return rect; }
        void setX(const float x_) { rect.x = x_; }
        void setY(const float y_) { rect.y = y_; }
        void setW(const float w_) { rect.w = w_; }
        void setH(const float h_) { rect.h = h_; }
        float x() const { return static_cast<float>(rect.x); }
        float y() const { return static_cast<float>(rect.y); }
        float w() const { return static_cast<float>(rect.w); }
        float h() const { return static_cast<float>(rect.h); }
        Vec2i pos() const { return Vec2i(static_cast<int>(rect.x), static_cast<int>(rect.y)); }
        Vec2i size() const { return Vec2i(static_cast<int>(rect.w), static_cast<int>(rect.h)); }
        void setPos(const float x_, const float y_) {
            rect.x = x_;
            rect.y = y_;
        }
        void setSize(const float w_, const float h_) {
            rect.w = w_;
            rect.h = h_;
        }
        void setPos(const Vec2i& pos_) {
            rect.x = pos_.x();
            rect.y = pos_.y();
        }
        void setSize(const Vec2i& size_) {
            rect.w = size_.x();
            rect.h = size_.y();
        }

#elif defined(PAXS_USING_SFML)
        sf::RectangleShape rect{};
        constexpr Rect() = default;
        Rect(const float x, const float y, const float w, const float h) : rect(sf::Vector2f(w, h)) { rect.setPosition({ x, y }); }
        Rect(const sf::Vector2i& pos, const sf::Vector2i& size) : rect(sf::Vector2f(static_cast<float>(size.x), static_cast<float>(size.y))) { rect.setPosition({ static_cast<float>(pos.x), static_cast<float>(pos.y) }); }
        Rect(const sf::Vector2i& pos, const float w, const float h) : rect(sf::Vector2f(w, h)) { rect.setPosition({ static_cast<float>(pos.x), static_cast<float>(pos.y) }); }
        Rect(const float x, const float y, const sf::Vector2i& size) : rect(sf::Vector2f(static_cast<float>(size.x), static_cast<float>(size.y))) { rect.setPosition({ x, y }); }
        operator sf::RectangleShape() const { return rect; }
        void setX(const float x_) { rect.setPosition({ x_, rect.getPosition().y }); }
        void setY(const float y_) { rect.setPosition({ rect.getPosition().x, y_ }); }
        void setW(const float w_) { rect.setSize(sf::Vector2f(w_, rect.getSize().y)); }
        void setH(const float h_) { rect.setSize(sf::Vector2f(rect.getSize().x, h_)); }
        float x() const { return rect.getPosition().x; }
        float y() const { return rect.getPosition().y; }
        float w() const { return rect.getSize().x; }
        float h() const { return rect.getSize().y; }
        Vec2i pos() const { return Vec2i(static_cast<int>(rect.getPosition().x), static_cast<int>(rect.getPosition().y)); }
        Vec2i size() const { return Vec2i(static_cast<int>(rect.getSize().x), static_cast<int>(rect.getSize().y)); }
        void setPos(const float x_, const float y_) { rect.setPosition({ x_, y_ }); }
        void setSize(const float w_, const float h_) { rect.setSize(sf::Vector2f(w_, h_)); }
        void setPos(const Vec2i& pos_) { rect.setPosition({ static_cast<float>(pos_.x()), static_cast<float>(pos_.y()) }); }
        void setSize(const Vec2i& size_) { rect.setSize(sf::Vector2f(static_cast<float>(size_.x()), static_cast<float>(size_.y()))); }
#else
        float x0{}, y0{}, w0{}, h0{};
        constexpr Rect() = default;
        constexpr Rect(const float x, const float y, const float w, const float h) :
            x0(x), y0(y), w0(w), h0(h) {}
        constexpr Rect(const Vec2i& pos, const Vec2i& size)
            : x0(static_cast<float>(pos.x())), y0(static_cast<float>(pos.y())),
            w0(static_cast<float>(size.x())), h0(static_cast<float>(size.y())) {}
        constexpr Rect(const Vec2i& pos, const float w, const float h) :
            x0(static_cast<float>(pos.x())), y0(static_cast<float>(pos.y())), w0(w), h0(h) {}
        constexpr Rect(const float x, const float y, const Vec2i& size)
            : x0(x), y0(y), w0(static_cast<float>(size.x())), h0(static_cast<float>(size.y())) {}
        void setX(const float x_) { x0 = x_; }
        void setY(const float y_) { y0 = y_; }
        void setW(const float w_) { w0 = w_; }
        void setH(const float h_) { h0 = h_; }
        float x() const { return x0; }
        float y() const { return y0; }
        float w() const { return w0; }
        float h() const { return h0; }
        Vec2i pos() const { return Vec2i(static_cast<int>(x0), static_cast<int>(y0)); }
        Vec2i size() const { return Vec2i(static_cast<int>(w0), static_cast<int>(h0)); }
        void setPos(const float x_, const float y_) {
            x0 = x_;
            y0 = y_;
        }
        void setSize(const float w_, const float h_) {
            w0 = w_;
            h0 = h_;
        }
        void setPos(const Vec2i& pos_) {
            x0 = static_cast<float>(pos_.x());
            y0 = static_cast<float>(pos_.y());
        }
        void setSize(const Vec2i& size_) {
            w0 = static_cast<float>(size_.x());
            h0 = static_cast<float>(size_.y());
        }
#endif

        void draw() const override {
#if defined(PAXS_USING_SIV3D)
            rect.draw();

#elif defined(PAXS_USING_DXLIB)
            DxLib::DrawBox(
                static_cast<int>(x0), static_cast<int>(y0), static_cast<int>(x0 + w0), static_cast<int>(y0 + h0),
                DxLib::GetColor(255, 255, 255), TRUE);

#elif defined(PAXS_USING_SFML)
            Window::window.draw(rect);

#endif
        }

#if defined(PAXS_USING_SIV3D)
        void draw(const paxg::Color& c_) const {
            rect.draw(c_.color);
        }
#elif defined(PAXS_USING_DXLIB)
        void draw(const paxg::Color& c_) const {
            DxLib::DrawBox(
                static_cast<int>(x0), static_cast<int>(y0), static_cast<int>(x0 + w0), static_cast<int>(y0 + h0),
                DxLib::GetColor(c_.r, c_.g, c_.b), TRUE);
        }
#elif defined(PAXS_USING_SFML)
        void draw(const paxg::Color& c_) const {
            sf::RectangleShape rect2 = rect;
            rect2.setFillColor(c_.color);
            Window::window.draw(rect2);
        }
#else
        void draw(const paxg::Color&) const {}
#endif

        void drawAt() const {
#if defined(PAXS_USING_SIV3D)
            // rect.draw();

#elif defined(PAXS_USING_DXLIB)
            DxLib::DrawBox(
                static_cast<int>(x0 - w0 / 2), static_cast<int>(y0 - h0 / 2), static_cast<int>(x0 + w0 / 2), static_cast<int>(y0 + h0 / 2),
                DxLib::GetColor(255, 255, 255), TRUE);

#elif defined(PAXS_USING_SFML)
            Window::window.draw(rect);

#endif
        }

#if defined(PAXS_USING_SIV3D)
        void drawAt(const paxg::Color&) const {
            // rect.draw(c_.color);
        }
#elif defined(PAXS_USING_DXLIB)
        void drawAt(const paxg::Color& c_) const {
            DxLib::DrawBox(
                static_cast<int>(x0 - w0 / 2), static_cast<int>(y0 - h0 / 2), static_cast<int>(x0 + w0 / 2), static_cast<int>(y0 + h0 / 2),
                DxLib::GetColor(c_.r, c_.g, c_.b), TRUE);
        }
#elif defined(PAXS_USING_SFML)
        void drawAt(const paxg::Color& c_) const {
            sf::RectangleShape rect2 = rect;
            rect2.setFillColor(c_.color);
            Window::window.draw(rect2);
        }
#else
        void drawAt(const paxg::Color&) const {}
#endif

#if defined(PAXS_USING_SIV3D)
        void drawFrame(const double inner_thickness, const double outer_thickness, const paxg::Color& color_) const {
            rect.drawFrame(inner_thickness, outer_thickness, color_.color);

        }
#elif defined(PAXS_USING_DXLIB)
        void drawFrame(const double inner_thickness, const double outer_thickness, const paxg::Color& color_) const {
            DxLib::DrawBox(
                static_cast<int>(x0 - outer_thickness), static_cast<int>(y0 - outer_thickness),
                static_cast<int>(x0 + w0 + outer_thickness), static_cast<int>(y0 + inner_thickness),
                DxLib::GetColor(color_.r, color_.g, color_.b), TRUE);
            DxLib::DrawBox(
                static_cast<int>(x0 - outer_thickness), static_cast<int>(y0 + h0 - inner_thickness),
                static_cast<int>(x0 + w0 + outer_thickness), static_cast<int>(y0 + h0 + outer_thickness),
                DxLib::GetColor(color_.r, color_.g, color_.b), TRUE);
            DxLib::DrawBox(
                static_cast<int>(x0 - outer_thickness), static_cast<int>(y0 - outer_thickness),
                static_cast<int>(x0 + inner_thickness), static_cast<int>(y0 + h0 + outer_thickness),
                DxLib::GetColor(color_.r, color_.g, color_.b), TRUE);
            DxLib::DrawBox(
                static_cast<int>(x0 + w0 - inner_thickness), static_cast<int>(y0 - outer_thickness),
                static_cast<int>(x0 + w0 + outer_thickness), static_cast<int>(y0 + h0 + outer_thickness),
                DxLib::GetColor(color_.r, color_.g, color_.b), TRUE);

        }
#elif defined(PAXS_USING_SFML)
        void drawFrame(const double inner_thickness, const double outer_thickness, const paxg::Color& c_) const {

            sf::RectangleShape rect1(sf::Vector2f(
                rect.getSize().x + static_cast<float>(outer_thickness * 2), static_cast<float>(outer_thickness + inner_thickness)));
            rect1.setPosition({ rect.getPosition().x - static_cast<float>(outer_thickness), rect.getPosition().y - static_cast<float>(outer_thickness) });
            rect1.setFillColor(c_.color);
            Window::window.draw(rect1);

            sf::RectangleShape rect2(sf::Vector2f(
                rect.getSize().x + static_cast<float>(outer_thickness * 2), static_cast<float>(outer_thickness + inner_thickness)));
            rect2.setPosition({ rect.getPosition().x - static_cast<float>(outer_thickness), rect.getPosition().y + rect.getSize().y - static_cast<float>(inner_thickness) });
            rect2.setFillColor(c_.color);
            Window::window.draw(rect2);

            sf::RectangleShape rect3(sf::Vector2f(
                static_cast<float>(outer_thickness + inner_thickness), static_cast<float>(rect.getSize().y + outer_thickness * 2)));
            rect3.setPosition({ static_cast<float>(rect.getPosition().x - outer_thickness), static_cast<float>(rect.getPosition().y - outer_thickness) });
            rect3.setFillColor(c_.color);
            Window::window.draw(rect3);

            sf::RectangleShape rect4(sf::Vector2f(
                static_cast<float>(outer_thickness + inner_thickness), static_cast<float>(rect.getSize().y + outer_thickness * 2)));
            rect4.setPosition({ static_cast<float>(rect.getPosition().x + rect.getSize().x - inner_thickness), static_cast<float>(rect.getPosition().y - outer_thickness) });
            rect4.setFillColor(c_.color);
            Window::window.draw(rect4);

        }
#else
        void drawFrame(const double, const double, const paxg::Color&) const {}
#endif

        bool leftClicked() const {
#if defined(PAXS_USING_SIV3D)
            return rect.leftClicked();
#elif defined(PAXS_USING_DXLIB)
            if (old_left_touch == 1) {
                const int touch_num = DxLib::GetTouchInputNum();
                // 1 フレーム前にタッチされている
                if (touch_num == 0) {
                    const auto& mx = old_left_touch_pos.x();
                    const auto& my = old_left_touch_pos.y();
                    return (mx >= x0 && my >= y0 && mx < x0 + w0 && my < y0 + h0);
                }
            }
            // 1 フレーム前にタッチされている
            if (paxg::Mouse::getInstance()->upLeft()) {
                int mx = 0, my = 0;
                DxLib::GetMousePoint(&mx, &my);
                return (mx >= x0 && my >= y0 && mx < x0 + w0 && my < y0 + h0);
            }
            return false;
#elif defined(PAXS_USING_SFML)
            // 1 フレーム前にタッチされている
            if (paxg::Mouse::getInstance()->upLeft()) {
                int mx = sf::Mouse::getPosition(Window::window).x, my = sf::Mouse::getPosition(Window::window).y;
                return (mx >= rect.getPosition().x &&
                    my >= rect.getPosition().y &&
                    mx < rect.getPosition().x + rect.getSize().x &&
                    my < rect.getPosition().y + rect.getSize().y);
            }
            return false;
#else
            return false;
#endif
        }

        bool mouseOver() const {
#if defined(PAXS_USING_SIV3D)
            return rect.mouseOver();

#elif defined(PAXS_USING_DXLIB)
            int mx = 0, my = 0;
            DxLib::GetMousePoint(&mx, &my);
            return (mx >= x0 && my >= y0 && mx < x0 + w0 && my < y0 + h0);

#elif defined(PAXS_USING_SFML)
            return rect.getGlobalBounds().contains(
                { static_cast<float>(sf::Mouse::getPosition(Window::window).x),
            static_cast<float>(sf::Mouse::getPosition(Window::window).y) });

#else
            return false;
#endif
        }

        void drawAt(const Vec2f&) const override {}
        void drawAt(const Vec2i&) const override {}
    };
}

#endif // !PAX_GRAPHICA_RECT_HPP
