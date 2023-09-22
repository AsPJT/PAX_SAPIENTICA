/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023 As Project
    [Production]	2023 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_RECT_HPP
#define PAX_GRAPHICA_RECT_HPP

/*##########################################################################################

##########################################################################################*/

#if defined(PAXS_USING_SIV3D)
#include <Siv3D.hpp>
#elif defined(PAXS_USING_SFML)
#include <SFML/Graphics.hpp>
#elif defined(PAXS_USING_DXLIB)
#include <DxLib.h>
#endif

#include <PAX_GRAPHICA/Color.hpp>
#include <PAX_GRAPHICA/IDrawable.hpp>
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
        Rect(const float x, const float y, const float w, const float h) : rect(sf::Vector2f(w, h)) { rect.setPosition(x, y); }
        Rect(const sf::Vector2i& pos, const sf::Vector2i& size) : rect(sf::Vector2f(size.x, size.y)) { rect.setPosition(pos.x, pos.y); }
        Rect(const sf::Vector2i& pos, const float w, const float h) : rect(sf::Vector2f(w, h)) { rect.setPosition(pos.x, pos.y); }
        Rect(const float x, const float y, const sf::Vector2i& size) : rect(sf::Vector2f(size.x, size.y)) { rect.setPosition(x, y); }
        operator sf::RectangleShape() const { return rect; }
        void setX(const float x_) { rect.x = x_; }
        void setY(const float y_) { rect.y = y_; }
        void setW(const float w_) { rect.w = w_; }
        void setH(const float h_) { rect.h = h_; }
        float x() const { return rect.x; }
        float y() const { return rect.y; }
        float w() const { return rect.w; }
        float h() const { return rect.h; }
        Vec2i pos() const { return Vec2i(rect.x, rect.y); }
        Vec2i size() const { return Vec2i(rect.w, rect.h); }
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
#elif defined(PAXS_USING_SFML)
            Window::window.draw(rect);
#elif defined(PAXS_USING_DXLIB)
            DxLib::DrawBox(
                static_cast<int>(x0), static_cast<int>(y0), static_cast<int>(x0 + w0), static_cast<int>(y0 + h0),
                DxLib::GetColor(255, 255, 255), TRUE);
#endif
        }
        void draw(const paxg::Color& c_) const {
#if defined(PAXS_USING_SIV3D)
            rect.draw(c_.color);
#elif defined(PAXS_USING_SFML)
            Window::window.draw(c_);
#elif defined(PAXS_USING_DXLIB)
            DxLib::DrawBox(
                static_cast<int>(x0), static_cast<int>(y0), static_cast<int>(x0 + w0), static_cast<int>(y0 + h0),
                DxLib::GetColor(c_.r, c_.g, c_.b), TRUE);
#endif
        }

        void drawFrame(const double inner_thickness, const double outer_thickness, const paxg::Color& color_) const {
#if defined(PAXS_USING_SIV3D)
            rect.drawFrame(inner_thickness, outer_thickness, color_.color);
#elif defined(PAXS_USING_SFML)
            Window::window.draw(color_);
#elif defined(PAXS_USING_DXLIB)
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
#endif
        }

        bool leftClicked() const {
#if defined(PAXS_USING_SIV3D)
            return rect.leftClicked();
#elif defined(PAXS_USING_DXLIB)
            int mx = 0, my = 0;
            if ((DxLib::GetMouseInput() & MOUSE_INPUT_LEFT) != 0) {
                DxLib::GetMousePoint(&mx, &my);
                return (mx >= x0 && my >= y0 && mx < x0 + w0 && my < y0 + h0);
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
#else
            return false;
#endif
        }

        void drawAt(const Vec2f&) const override {}
        void drawAt(const Vec2i&) const override {}
    };
}

#endif // !PAX_GRAPHICA_RECT_HPP
