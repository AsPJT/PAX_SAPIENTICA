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
#include <PAX_GRAPHICA/Mouse.hpp>
#include <PAX_GRAPHICA/TouchInput.hpp>
#include <PAX_GRAPHICA/Window.hpp>

#include <PAX_SAPIENTICA/Core/Type/Rect.hpp>
#include <PAX_SAPIENTICA/Core/Type/Vector2.hpp>

namespace paxg {

    struct Rect {
#if defined(PAXS_USING_SIV3D)
        s3d::RectF rect{};
        constexpr Rect() = default;
        constexpr Rect(const float x, const float y, const float w, const float h) : rect(x, y, w, h) {}
        constexpr Rect(const paxs::Vector2<float>& pos, const paxs::Vector2<float>& size) : rect(pos.x, pos.y, size.x, size.y) {}
        constexpr Rect(const paxs::Vector2<float>& pos, const float w, const float h) : rect(pos.x, pos.y, w, h) {}
        constexpr Rect(const float x, const float y, const paxs::Vector2<float>& size) : rect(x, y, size.x, size.y) {}
        constexpr Rect(const paxs::Rect<int>& r) : rect(static_cast<float>(r.x()), static_cast<float>(r.y()), static_cast<float>(r.width()), static_cast<float>(r.height())) {}
        constexpr operator s3d::RectF() const { return rect; }
        void setX(const float x_) { rect.x = x_; }
        void setY(const float y_) { rect.y = y_; }
        void setW(const float w_) { rect.w = w_; }
        void setH(const float h_) { rect.h = h_; }
        float x() const { return static_cast<float>(rect.x); }
        float y() const { return static_cast<float>(rect.y); }
        float w() const { return static_cast<float>(rect.w); }
        float h() const { return static_cast<float>(rect.h); }
        paxs::Vector2<float> pos() const { return { static_cast<float>(rect.x), static_cast<float>(rect.y) }; }
        paxs::Vector2<float> size() const { return { static_cast<float>(rect.w), static_cast<float>(rect.h) }; }
        void setPos(const float x_, const float y_) {
            rect.x = x_;
            rect.y = y_;
        }
        void setSize(const float w_, const float h_) {
            rect.w = w_;
            rect.h = h_;
        }
        void setPos(const paxs::Vector2<int>& pos_) {
            rect.x = pos_.x;
            rect.y = pos_.y;
        }
        void setSize(const paxs::Vector2<int>& size_) {
            rect.w = size_.x;
            rect.h = size_.y;
        }
        bool contains(const float x_, const float y_) const {
            return (rect.x <= x_) && (x_ <= (rect.x + rect.w)) &&
                (rect.y <= y_) && (y_ <= (rect.y + rect.h));
        }
        bool contains(const paxs::Vector2<int>& pos) const {
            return contains(static_cast<float>(pos.x), static_cast<float>(pos.y));
        }

#elif defined(PAXS_USING_SFML)
        sf::RectangleShape rect{};
        constexpr Rect() = default;
        Rect(const float x, const float y, const float w, const float h) : rect(sf::Vector2f(w, h)) { rect.setPosition({ x, y }); }
        Rect(const paxs::Vector2<float>& pos, const paxs::Vector2<float>& size) : rect(sf::Vector2f(size.x, size.y)) { rect.setPosition({pos.x, pos.y }); }
        Rect(const paxs::Vector2<float>& pos, const float w, const float h) : rect(sf::Vector2f(w, h)) { rect.setPosition({ pos.x, pos.y }); }
        Rect(const float x, const float y, const paxs::Vector2<float>& size) : rect(sf::Vector2f(size.x, size.y)) { rect.setPosition({ x, y }); }
        Rect(const paxs::Rect<int>& r) : rect(sf::Vector2f(static_cast<float>(r.width()), static_cast<float>(r.height()))) { rect.setPosition({ static_cast<float>(r.x()), static_cast<float>(r.y()) }); }
        operator sf::RectangleShape() const { return rect; }
        void setX(const float x_) { rect.setPosition({ x_, rect.getPosition().y }); }
        void setY(const float y_) { rect.setPosition({ rect.getPosition().x, y_ }); }
        void setW(const float w_) { rect.setSize(sf::Vector2f(w_, rect.getSize().y)); }
        void setH(const float h_) { rect.setSize(sf::Vector2f(rect.getSize().x, h_)); }
        float x() const { return rect.getPosition().x; }
        float y() const { return rect.getPosition().y; }
        float w() const { return rect.getSize().x; }
        float h() const { return rect.getSize().y; }
        paxs::Vector2<float> pos() const { return paxs::Vector2<float>(rect.getPosition().x, rect.getPosition().y); }
        paxs::Vector2<float> size() const { return paxs::Vector2<float>(rect.getSize().x, rect.getSize().y); }
        void setPos(const float x_, const float y_) { rect.setPosition({ x_, y_ }); }
        void setSize(const float w_, const float h_) { rect.setSize(sf::Vector2f(w_, h_)); }
        void setPos(const paxs::Vector2<int>& pos_) { rect.setPosition({ static_cast<float>(pos_.x), static_cast<float>(pos_.y) }); }
        void setSize(const paxs::Vector2<int>& size_) { rect.setSize(sf::Vector2f(static_cast<float>(size_.x), static_cast<float>(size_.y))); }
        bool contains(const float x_, const float y_) const {
            return x_ >= rect.getPosition().x && x_ <= (rect.getPosition().x + rect.getSize().x) &&
                   y_ >= rect.getPosition().y && y_ <= (rect.getPosition().y + rect.getSize().y);
        }
        bool contains(const paxs::Vector2<int>& pos) const {
            return contains(static_cast<float>(pos.x), static_cast<float>(pos.y));
        }
#else
        paxs::Vector2<float> pos_{0.0f, 0.0f};
        paxs::Vector2<float> size_{0.0f, 0.0f};
        constexpr Rect() = default;
        constexpr Rect(const float x, const float y, const float w, const float h) :
            pos_{x, y}, size_{w, h} {}
        constexpr Rect(const paxs::Vector2<float>& pos, const paxs::Vector2<float>& size)
            : pos_{pos}, size_{size} {}
        constexpr Rect(const paxs::Vector2<float>& pos, const float w, const float h) :
            pos_{pos}, size_{w, h} {}
        constexpr Rect(const float x, const float y, const paxs::Vector2<float>& size)
            : pos_{x, y}, size_{size} {}
        constexpr Rect(const paxs::Rect<int>& r)
            : pos_{ static_cast<float>(r.x()), static_cast<float>(r.y()) },
              size_{ static_cast<float>(r.width()), static_cast<float>(r.height()) } {}
        void setX(const float x_) { pos_.x = x_; }
        void setY(const float y_) { pos_.y = y_; }
        void setW(const float w_) { size_.x = w_; }
        void setH(const float h_) { size_.y = h_; }
        float x() const { return pos_.x; }
        float y() const { return pos_.y; }
        float w() const { return size_.x; }
        float h() const { return size_.y; }
        paxs::Vector2<float> pos() const { return pos_; }
        paxs::Vector2<float> size() const { return size_; }
        void setPos(const float x_, const float y_) {
            pos_.x = x_;
            pos_.y = y_;
        }
        void setSize(const float w_, const float h_) {
            size_.x = w_;
            size_.y = h_;
        }
        void setPos(const paxs::Vector2<int>& pos) {
            pos_ = paxs::Vector2<float>{ pos };
        }
        void setSize(const paxs::Vector2<int>& size) {
            size_ = paxs::Vector2<float>{ size };
        }
        bool contains(const float x_, const float y_) const {
            return (pos_.x <= x_) && (x_ <= (pos_.x + size_.x)) &&
                   (pos_.y <= y_) && (y_ <= (pos_.y + size_.y));
        }
        bool contains(const paxs::Vector2<int>& pos) const {
            return contains(static_cast<float>(pos.x), static_cast<float>(pos.y));
        }
#endif

        void draw() const {
#if defined(PAXS_USING_SIV3D)
            rect.draw();

#elif defined(PAXS_USING_DXLIB)
            DxLib::DrawBox(
                static_cast<int>(pos_.x), static_cast<int>(pos_.y), static_cast<int>(pos_.x + size_.x), static_cast<int>(pos_.y + size_.y),
                DxLib::GetColor(255, 255, 255), TRUE);

#elif defined(PAXS_USING_SFML)
            Window::window().draw(rect);

#endif
        }

#if defined(PAXS_USING_SIV3D)
        void draw(const paxg::Color& c_) const {
            rect.draw(c_.color);
        }
#elif defined(PAXS_USING_DXLIB)
        void draw(const paxg::Color& c_) const {
            DxLib::DrawBox(
                static_cast<int>(pos_.x), static_cast<int>(pos_.y), static_cast<int>(pos_.x + size_.x), static_cast<int>(pos_.y + size_.y),
                DxLib::GetColor(c_.r, c_.g, c_.b), TRUE);
        }
#elif defined(PAXS_USING_SFML)
        void draw(const paxg::Color& c_) const {
            sf::RectangleShape rect2 = rect;
            rect2.setFillColor(c_.color);
            Window::window().draw(rect2);
        }
#else
        void draw(const paxg::Color&) const {}
#endif

        /// @brief Draw shadow with blur effect
        /// @brief ぼかし効果付きの影を描画
        /// @param offset Shadow offset (影のオフセット)
        /// @param blur_size Blur size (ぼかしサイズ)
        /// @param spread Spread size (広がりサイズ)
        /// @return Reference to this for method chaining (メソッドチェーン用の自身への参照)
#if defined(PAXS_USING_SIV3D)
        const Rect& drawShadow(const paxs::Vector2<int>& offset, int blur_size, int spread) const {
            rect.drawShadow({offset.x, offset.y}, blur_size, spread);
            return *this;
        }
#elif defined(PAXS_USING_SFML)
        const Rect& drawShadow(const paxs::Vector2<int>& offset, int blur_size, int spread) const {
            // SFML: Simple shadow using semi-transparent rectangles
            // 複数の半透明矩形を重ねて簡易的な影を表現
            sf::RectangleShape shadow = rect;
            const int shadow_alpha = 40; // Base opacity

            for (int i = spread + blur_size; i >= 0; --i) {
                shadow.setPosition(sf::Vector2f(
                    rect.getPosition().x + offset.x + i,
                    rect.getPosition().y + offset.y + i
                ));
                shadow.setSize(sf::Vector2f(
                    rect.getSize().x + i * 2,
                    rect.getSize().y + i * 2
                ));

                int alpha = shadow_alpha * (spread + blur_size - i + 1) / (spread + blur_size + 1);
                shadow.setFillColor(sf::Color(0, 0, 0, static_cast<uint8_t>(alpha)));
                Window::window().draw(shadow);
            }
            return *this;
        }
#elif defined(PAXS_USING_DXLIB)
        const Rect& drawShadow(const paxs::Vector2<int>& offset, int blur_size, int spread) const {
            // DxLib: Simple shadow using semi-transparent rectangles
            // 複数の半透明矩形を重ねて簡易的な影を表現
            DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, 40);

            for (int i = spread + blur_size; i >= 0; --i) {
                int alpha = 40 * (spread + blur_size - i + 1) / (spread + blur_size + 1);
                DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

                DxLib::DrawBox(
                    static_cast<int>(pos_.x + offset.x + i),
                    static_cast<int>(pos_.y + offset.y + i),
                    static_cast<int>(pos_.x + size_.x + i),
                    static_cast<int>(pos_.y + size_.y + i),
                    DxLib::GetColor(0, 0, 0),
                    TRUE
                );
            }

            DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
            return *this;
        }
#else
        const Rect& drawShadow(const paxs::Vector2<int>&, int, int) const {
            return *this;
        }
#endif

        void drawAt() const {
#if defined(PAXS_USING_SIV3D)
            // rect.draw();

#elif defined(PAXS_USING_DXLIB)
            DxLib::DrawBox(
                static_cast<int>(pos_.x - size_.x / 2), static_cast<int>(pos_.y - size_.y / 2), static_cast<int>(pos_.x + size_.x / 2), static_cast<int>(pos_.y + size_.y / 2),
                DxLib::GetColor(255, 255, 255), TRUE);

#elif defined(PAXS_USING_SFML)
            Window::window().draw(rect);

#endif
        }

#if defined(PAXS_USING_SIV3D)
        void drawAt(const paxg::Color&) const {
            // rect.draw(c_.color);
        }
#elif defined(PAXS_USING_DXLIB)
        void drawAt(const paxg::Color& c_) const {
            DxLib::DrawBox(
                static_cast<int>(pos_.x - size_.x / 2), static_cast<int>(pos_.y - size_.y / 2), static_cast<int>(pos_.x + size_.x / 2), static_cast<int>(pos_.y + size_.y / 2),
                DxLib::GetColor(c_.r, c_.g, c_.b), TRUE);
        }
#elif defined(PAXS_USING_SFML)
        void drawAt(const paxg::Color& c_) const {
            sf::RectangleShape rect2 = rect;
            rect2.setFillColor(c_.color);
            Window::window().draw(rect2);
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
                static_cast<int>(pos_.x - outer_thickness), static_cast<int>(pos_.y - outer_thickness),
                static_cast<int>(pos_.x + size_.x + outer_thickness), static_cast<int>(pos_.y + inner_thickness),
                DxLib::GetColor(color_.r, color_.g, color_.b), TRUE);
            DxLib::DrawBox(
                static_cast<int>(pos_.x - outer_thickness), static_cast<int>(pos_.y + size_.y - inner_thickness),
                static_cast<int>(pos_.x + size_.x + outer_thickness), static_cast<int>(pos_.y + size_.y + outer_thickness),
                DxLib::GetColor(color_.r, color_.g, color_.b), TRUE);
            DxLib::DrawBox(
                static_cast<int>(pos_.x - outer_thickness), static_cast<int>(pos_.y - outer_thickness),
                static_cast<int>(pos_.x + inner_thickness), static_cast<int>(pos_.y + size_.y + outer_thickness),
                DxLib::GetColor(color_.r, color_.g, color_.b), TRUE);
            DxLib::DrawBox(
                static_cast<int>(pos_.x + size_.x - inner_thickness), static_cast<int>(pos_.y - outer_thickness),
                static_cast<int>(pos_.x + size_.x + outer_thickness), static_cast<int>(pos_.y + size_.y + outer_thickness),
                DxLib::GetColor(color_.r, color_.g, color_.b), TRUE);

        }
#elif defined(PAXS_USING_SFML)
        void drawFrame(const double inner_thickness, const double outer_thickness, const paxg::Color& c_) const {

            sf::RectangleShape rect1(sf::Vector2f(
                rect.getSize().x + static_cast<float>(outer_thickness * 2), static_cast<float>(outer_thickness + inner_thickness)));
            rect1.setPosition({ rect.getPosition().x - static_cast<float>(outer_thickness), rect.getPosition().y - static_cast<float>(outer_thickness) });
            rect1.setFillColor(c_.color);
            Window::window().draw(rect1);

            sf::RectangleShape rect2(sf::Vector2f(
                rect.getSize().x + static_cast<float>(outer_thickness * 2), static_cast<float>(outer_thickness + inner_thickness)));
            rect2.setPosition({ rect.getPosition().x - static_cast<float>(outer_thickness), rect.getPosition().y + rect.getSize().y - static_cast<float>(inner_thickness) });
            rect2.setFillColor(c_.color);
            Window::window().draw(rect2);

            sf::RectangleShape rect3(sf::Vector2f(
                static_cast<float>(outer_thickness + inner_thickness), static_cast<float>(rect.getSize().y + outer_thickness * 2)));
            rect3.setPosition({ static_cast<float>(rect.getPosition().x - outer_thickness), static_cast<float>(rect.getPosition().y - outer_thickness) });
            rect3.setFillColor(c_.color);
            Window::window().draw(rect3);

            sf::RectangleShape rect4(sf::Vector2f(
                static_cast<float>(outer_thickness + inner_thickness), static_cast<float>(rect.getSize().y + outer_thickness * 2)));
            rect4.setPosition({ static_cast<float>(rect.getPosition().x + rect.getSize().x - inner_thickness), static_cast<float>(rect.getPosition().y - outer_thickness) });
            rect4.setFillColor(c_.color);
            Window::window().draw(rect4);

        }
#else
        void drawFrame(const double, const double, const paxg::Color&) const {}
#endif

        bool mouseOver() const {
#if defined(PAXS_USING_SIV3D)
            return rect.mouseOver();

#elif defined(PAXS_USING_DXLIB)
            int mx = 0, my = 0;
            DxLib::GetMousePoint(&mx, &my);
            return (mx >= pos_.x && my >= pos_.y &&
                mx < pos_.x + size_.x && my < pos_.y + size_.y);

#elif defined(PAXS_USING_SFML)
            return rect.getGlobalBounds().contains(
                { static_cast<float>(sf::Mouse::getPosition(Window::window()).x),
            static_cast<float>(sf::Mouse::getPosition(Window::window()).y) });

#else
            return false;
#endif
        }
    };
}

#endif // !PAX_GRAPHICA_RECT_HPP
