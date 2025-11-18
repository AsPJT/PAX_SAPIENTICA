/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_ROUND_RECT_HPP
#define PAX_GRAPHICA_ROUND_RECT_HPP

#if defined(PAXS_USING_SIV3D)
#include <Siv3D.hpp>
#elif defined(PAXS_USING_DXLIB)
#include <DxLib.h>
#elif defined(PAXS_USING_SFML)
#include <SFML/Graphics.hpp>
#include <algorithm> // std::min, std::max (SFMLで使用)
#endif

#include <PAX_GRAPHICA/Color.hpp>
#include <PAX_GRAPHICA/Mouse.hpp>
#include <PAX_GRAPHICA/TouchInput.hpp>
#include <PAX_GRAPHICA/Vec2.hpp>
#include <PAX_GRAPHICA/Window.hpp>

namespace paxg {

    struct RoundRect {
#if defined(PAXS_USING_SIV3D)
        s3d::RoundRect rect{};
        constexpr RoundRect() = default;
        constexpr RoundRect(const int x, const int y, const int w, const int h) : rect(x, y, w, h, w / 5) {}
        constexpr RoundRect(const int x, const int y, const int w, const int h, const int r) : rect(x, y, w, h, r) {}
        constexpr RoundRect(const Vec2i& pos, const Vec2i& size) : rect(pos.x(), pos.y(), size.x(), size.y(), size.x() / 5) {}
        constexpr RoundRect(const Vec2i& pos, const Vec2i& size, const int r) : rect(pos.x(), pos.y(), size.x(), size.y(), r) {}
        constexpr RoundRect(const Vec2i& pos, const int w, const int h) : rect(pos.x(), pos.y(), w, h, w / 5) {}
        constexpr RoundRect(const Vec2i& pos, const int w, const int h, const int r) : rect(pos.x(), pos.y(), w, h, r) {}
        constexpr RoundRect(const int x, const int y, const Vec2i& size) : rect(x, y, size.x(), size.y(), size.x() / 5) {}
        constexpr RoundRect(const int x, const int y, const Vec2i& size, const int r) : rect(x, y, size.x(), size.y(), r) {}
        constexpr operator s3d::RoundRect() const { return rect; }
        void setX(const int x_) { rect.x = x_; }
        void setY(const int y_) { rect.y = y_; }
        void setW(const int w_) { rect.w = w_; }
        void setH(const int h_) { rect.h = h_; }
        void setR(const int r_) { rect.r = r_; }
        int x() const { return static_cast<int>(rect.x); }
        int y() const { return static_cast<int>(rect.y); }
        int w() const { return static_cast<int>(rect.w); }
        int h() const { return static_cast<int>(rect.h); }
        int r() const { return static_cast<int>(rect.r); }
        Vec2i pos() const { return Vec2i(static_cast<int>(rect.x), static_cast<int>(rect.y)); }
        Vec2i size() const { return Vec2i(static_cast<int>(rect.w), static_cast<int>(rect.h)); }
        void setPos(const int x_, const int y_) {
            rect.x = x_;
            rect.y = y_;
        }
        void setSize(const int w_, const int h_) {
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
        int x0{}, y0{}, w0{}, h0{}, r0{};
        constexpr RoundRect() = default;
        RoundRect(const int x, const int y, const int w, const int h) :
            x0(x), y0(y), w0(w), h0(h), r0(w / 5) {}
        RoundRect(const int x, const int y, const int w, const int h, const int r) :
            x0(x), y0(y), w0(w), h0(h), r0(r) {}
        RoundRect(const Vec2i& pos, const Vec2i& size)
            : x0(static_cast<int>(pos.x())), y0(static_cast<int>(pos.y())),
            w0(static_cast<int>(size.x())), h0(static_cast<int>(size.y())), r0(w0 / 5) {}
        RoundRect(const Vec2i& pos, const Vec2i& size, const int r)
            : x0(static_cast<int>(pos.x())), y0(static_cast<int>(pos.y())),
            w0(static_cast<int>(size.x())), h0(static_cast<int>(size.y())), r0(r) {}
        RoundRect(const Vec2i& pos, const int w, const int h) :
            x0(static_cast<int>(pos.x())), y0(static_cast<int>(pos.y())), w0(w), h0(h), r0(w0 / 5) {}
        RoundRect(const Vec2i& pos, const int w, const int h, const int r) :
            x0(static_cast<int>(pos.x())), y0(static_cast<int>(pos.y())), w0(w), h0(h), r0(r) {}
        RoundRect(const int x, const int y, const Vec2i& size)
            : x0(x), y0(y), w0(static_cast<int>(size.x())), h0(static_cast<int>(size.y())), r0(w0 / 5) {}
        RoundRect(const int x, const int y, const Vec2i& size, const int r)
            : x0(x), y0(y), w0(static_cast<int>(size.x())), h0(static_cast<int>(size.y())), r0(r) {}

        // operator sf::RectangleShape() const { return rect; } // [削除] rect メンバはもうない

        void setX(const int x_) { x0 = x_; }
        void setY(const int y_) { y0 = y_; }
        void setW(const int w_) { w0 = w_; }
        void setH(const int h_) { h0 = h_; }
        void setR(const int r_) { r0 = r_; }
        int x() const { return x0; }
        int y() const { return y0; }
        int w() const { return w0; }
        int h() const { return h0; }
        int r() const { return r0; }
        Vec2i pos() const { return Vec2i(static_cast<int>(x0), static_cast<int>(y0)); }
        Vec2i size() const { return Vec2i(static_cast<int>(w0), static_cast<int>(h0)); }
        void setPos(const int x_, const int y_) {
            x0 = x_;
            y0 = y_;
        }
        void setSize(const int w_, const int h_) {
            w0 = w_;
            h0 = h_;
        }
        void setPos(const Vec2i& pos_) {
            x0 = static_cast<int>(pos_.x());
            y0 = static_cast<int>(pos_.y());
        }
        void setSize(const Vec2i& size_) {
            w0 = static_cast<int>(size_.x());
            h0 = static_cast<int>(size_.y());
        }
#else
        int x0{}, y0{}, w0{}, h0{}, r0{};
        constexpr RoundRect() = default;
        constexpr RoundRect(const int x, const int y, const int w, const int h) :
            x0(x), y0(y), w0(w), h0(h), r0(w0 / 5) {}
        constexpr RoundRect(const int x, const int y, const int w, const int h, const int r) :
            x0(x), y0(y), w0(w), h0(h), r0(r) {}
        constexpr RoundRect(const Vec2i& pos, const Vec2i& size)
            : x0(static_cast<int>(pos.x())), y0(static_cast<int>(pos.y())),
            w0(static_cast<int>(size.x())), h0(static_cast<int>(size.y())), r0(w0 / 5) {}
        constexpr RoundRect(const Vec2i& pos, const Vec2i& size, const int r)
            : x0(static_cast<int>(pos.x())), y0(static_cast<int>(pos.y())),
            w0(static_cast<int>(size.x())), h0(static_cast<int>(size.y())), r0(r) {}
        constexpr RoundRect(const Vec2i& pos, const int w, const int h) :
            x0(static_cast<int>(pos.x())), y0(static_cast<int>(pos.y())), w0(w), h0(h), r0(w0 / 5) {}
        constexpr RoundRect(const Vec2i& pos, const int w, const int h, const int r) :
            x0(static_cast<int>(pos.x())), y0(static_cast<int>(pos.y())), w0(w), h0(h), r0(r) {}
        constexpr RoundRect(const int x, const int y, const Vec2i& size)
            : x0(x), y0(y), w0(static_cast<int>(size.x())), h0(static_cast<int>(size.y())), r0(w0 / 5) {}
        constexpr RoundRect(const int x, const int y, const Vec2i& size, const int r)
            : x0(x), y0(y), w0(static_cast<int>(size.x())), h0(static_cast<int>(size.y())), r0(r) {}
        void setX(const int x_) { x0 = x_; }
        void setY(const int y_) { y0 = y_; }
        void setW(const int w_) { w0 = w_; }
        void setH(const int h_) { h0 = h_; }
        void setR(const int r_) { r0 = r_; }
        int x() const { return x0; }
        int y() const { return y0; }
        int w() const { return w0; }
        int h() const { return h0; }
        int r() const { return r0; }
        Vec2i pos() const { return Vec2i(static_cast<int>(x0), static_cast<int>(y0)); }
        Vec2i size() const { return Vec2i(static_cast<int>(w0), static_cast<int>(h0)); }
        void setPos(const int x_, const int y_) {
            x0 = x_;
            y0 = y_;
        }
        void setSize(const int w_, const int h_) {
            w0 = w_;
            h0 = h_;
        }
        void setPos(const Vec2i& pos_) {
            x0 = static_cast<int>(pos_.x());
            y0 = static_cast<int>(pos_.y());
        }
        void setSize(const Vec2i& size_) {
            w0 = static_cast<int>(size_.x());
            h0 = static_cast<int>(size_.y());
        }
#endif

        // SFML描画用のヘルパー関数
#if defined(PAXS_USING_SFML)
    private:
        unsigned calcCornerPointCount(float r) const {
            // r が小さいならそこまで分割しない。大きいなら増やす。
            // 好きに調整してOK
            if (r < 6.f)  return 4;   // ほぼ角の丸み分かる程度
            if (r < 12.f) return 6;
            if (r < 24.f) return 8;
            if (r < 48.f) return 12;
            return 16;                // ここから上はかなりなめらか
        }
        void drawInternal(float x, float y, float w, float h, float r, const sf::Color& c) const
        {
            r = std::min(r, w * 0.5f);
            r = std::min(r, h * 0.5f);

            if (r <= 0.f) {
                sf::RectangleShape rect({w, h});
                rect.setPosition({x, y});
                rect.setFillColor(c);
                Window::window().draw(rect);
                return;
            }

            // 半径に応じて分割数を決める
            const unsigned cornerPointCount = calcCornerPointCount(r);
            const unsigned totalPoints = 4 * (cornerPointCount + 1);

            sf::ConvexShape shape;
            shape.setPointCount(totalPoints);
            shape.setFillColor(c);

            const float left   = x;
            const float top    = y;
            const float right  = x + w;
            const float bottom = y + h;

            const sf::Vector2f c_tl(left + r,  top + r);
            const sf::Vector2f c_tr(right - r, top + r);
            const sf::Vector2f c_br(right - r, bottom - r);
            const sf::Vector2f c_bl(left + r,  bottom - r);

            auto putCorner = [&](unsigned startIndex,
                                const sf::Vector2f& center,
                                float startDeg)
            {
                for (unsigned i = 0; i <= cornerPointCount; ++i) {
                    float ang = startDeg + 90.f * (static_cast<float>(i) / cornerPointCount);
                    float rad = ang * 3.14159265358979323846f / 180.f;
                    float px = center.x + std::cos(rad) * r;
                    float py = center.y + std::sin(rad) * r;
                    shape.setPoint(startIndex + i, {px, py});
                }
            };

            unsigned idx = 0;
            putCorner(idx, c_tl, 180.f);
            idx += cornerPointCount + 1;
            putCorner(idx, c_tr, 270.f);
            idx += cornerPointCount + 1;
            putCorner(idx, c_br,   0.f);
            idx += cornerPointCount + 1;
            putCorner(idx, c_bl,  90.f);

            Window::window().draw(shape);
        }

    public:
#endif

        void draw() const {
#if defined(PAXS_USING_SIV3D)
            rect.draw();

#elif defined(PAXS_USING_DXLIB)
            DxLib::DrawRoundRect(
                static_cast<int>(x0), static_cast<int>(y0), static_cast<int>(x0 + w0), static_cast<int>(y0 + h0), r0, r0,
                DxLib::GetColor(255, 255, 255), TRUE);

#elif defined(PAXS_USING_SFML)
            drawInternal(static_cast<float>(x0), static_cast<float>(y0),
                static_cast<float>(w0), static_cast<float>(h0),
                static_cast<float>(r0), sf::Color::White);
#endif
        }

#if defined(PAXS_USING_SIV3D)
        void draw(const paxg::Color& c_) const {
            rect.draw(c_.color);
        }
#elif defined(PAXS_USING_DXLIB)
        void draw(const paxg::Color& c_) const {
            DxLib::DrawRoundRect(
                static_cast<int>(x0), static_cast<int>(y0), static_cast<int>(x0 + w0), static_cast<int>(y0 + h0), r0, r0,
                DxLib::GetColor(c_.r, c_.g, c_.b), TRUE);
        }
#elif defined(PAXS_USING_SFML)
        void draw(const paxg::Color& c_) const {
            drawInternal(static_cast<float>(x0), static_cast<float>(y0),
                static_cast<float>(w0), static_cast<float>(h0),
                static_cast<float>(r0), c_.color);
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
        const RoundRect& drawShadow(const Vec2i& offset, int blur_size, int spread) const {
            rect.drawShadow({ offset.x(), offset.y() }, blur_size, spread);
            return *this;
        }
#elif defined(PAXS_USING_SFML)
        const RoundRect& drawShadow(const Vec2i& offset, int blur_size, int spread) const {
            const float base_x = static_cast<float>(x0);
            const float base_y = static_cast<float>(y0);
            const float base_w = static_cast<float>(w0);
            const float base_h = static_cast<float>(h0);
            const float base_r = static_cast<float>(r0);

            const int shadow_alpha = 40; // Base opacity

            for (int i = spread + blur_size; i >= 0; --i) {
                // 影の位置とサイズ、角の丸みを計算
                // (影は元の図形より 2*i 大きく、-i オフセットして中央揃えにする)
                const float x = base_x + offset.x() - i;
                const float y = base_y + offset.y() - i;
                const float w = base_w + 2.0f * i;
                const float h = base_h + 2.0f * i;
                const float r = base_r + i;

                int alpha = shadow_alpha * (spread + blur_size - i + 1) / (spread + blur_size + 1);
                sf::Color shadow_color(0, 0, 0, static_cast<uint8_t>(alpha));

                // ヘルパー関数で角丸の影を描画
                drawInternal(x, y, w, h, r, shadow_color);
            }
            return *this;
        }
#elif defined(PAXS_USING_DXLIB)
        const RoundRect& drawShadow(const Vec2i& offset, int blur_size, int spread) const {
            // DxLib: Simple shadow using semi-transparent rectangles
            // 複数の半透明矩形を重ねて簡易的な影を表現
            DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, 40);

            for (int i = spread + blur_size; i >= 0; --i) {
                int alpha = 40 * (spread + blur_size - i + 1) / (spread + blur_size + 1);
                DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

                // DxLibの実装も影が広がるように
                DxLib::DrawRoundRect(
                    static_cast<int>(x0 + offset.x() - i),
                    static_cast<int>(y0 + offset.y() - i),
                    static_cast<int>(x0 + w0 + offset.x() + i),
                    static_cast<int>(y0 + h0 + offset.y() + i),
                    r0 + i, r0 + i, // 角の丸みも広げる
                    DxLib::GetColor(0, 0, 0),
                    TRUE
                );
            }

            DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
            return *this;
        }
#else
        const RoundRect& drawShadow(const Vec2i&, int, int) const {
            // No platform defined: empty implementation
            return *this;
        }
#endif

        void drawAt() const {
#if defined(PAXS_USING_SIV3D)
            // rect.draw(); // 元のコードでもコメントアウトされていた
#elif defined(PAXS_USING_DXLIB)
            DxLib::DrawRoundRect(
                static_cast<int>(x0 - w0 / 2), static_cast<int>(y0 - h0 / 2), static_cast<int>(x0 + w0 / 2), static_cast<int>(y0 + h0 / 2), r0, r0,
                DxLib::GetColor(255, 255, 255), TRUE);

#elif defined(PAXS_USING_SFML)
            // (x0, y0) が中心になるように座標を計算してヘルパーを呼び出す
            drawInternal(static_cast<float>(x0) - static_cast<float>(w0) / 2.0f,
                static_cast<float>(y0) - static_cast<float>(h0) / 2.0f,
                static_cast<float>(w0), static_cast<float>(h0),
                static_cast<float>(r0), sf::Color::White);
#endif
        }

#if defined(PAXS_USING_SIV3D)
        void drawAt(const paxg::Color&) const {
            // rect.draw(c_.color); // 元のコードでもコメントアウトされていた
        }
#elif defined(PAXS_USING_DXLIB)
        void drawAt(const paxg::Color& c_) const {
            DxLib::DrawRoundRect(
                static_cast<int>(x0 - w0 / 2), static_cast<int>(y0 - h0 / 2), static_cast<int>(x0 + w0 / 2), static_cast<int>(y0 + h0 / 2), r0, r0,
                DxLib::GetColor(c_.r, c_.g, c_.b), TRUE);
        }
#elif defined(PAXS_USING_SFML)
        void drawAt(const paxg::Color& c_) const {
            // (x0, y0) が中心になるように座標を計算してヘルパーを呼び出す
            drawInternal(static_cast<float>(x0) - static_cast<float>(w0) / 2.0f,
                static_cast<float>(y0) - static_cast<float>(h0) / 2.0f,
                static_cast<float>(w0), static_cast<float>(h0),
                static_cast<float>(r0), c_.color);
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
            const float x = static_cast<float>(x0);
            const float y = static_cast<float>(y0);
            const float w = static_cast<float>(w0);
            const float h = static_cast<float>(h0);

            sf::RectangleShape rect1(sf::Vector2f(
                static_cast<float>(w + outer_thickness * 2), static_cast<float>(outer_thickness + inner_thickness)));
            rect1.setPosition({ x - static_cast<float>(outer_thickness), y - static_cast<float>(outer_thickness) });
            rect1.setFillColor(c_.color);
            Window::window().draw(rect1);

            sf::RectangleShape rect2(sf::Vector2f(
                static_cast<float>(w + outer_thickness * 2), static_cast<float>(outer_thickness + inner_thickness)));
            rect2.setPosition({ x - static_cast<float>(outer_thickness), y + h - static_cast<float>(inner_thickness) });
            rect2.setFillColor(c_.color);
            Window::window().draw(rect2);

            sf::RectangleShape rect3(sf::Vector2f(
                static_cast<float>(outer_thickness + inner_thickness), static_cast<float>(h + outer_thickness * 2)));
            rect3.setPosition({ x - static_cast<float>(outer_thickness), y - static_cast<float>(outer_thickness) });
            rect3.setFillColor(c_.color);
            Window::window().draw(rect3);

            sf::RectangleShape rect4(sf::Vector2f(
                static_cast<float>(outer_thickness + inner_thickness), static_cast<float>(h + outer_thickness * 2)));
            rect4.setPosition({ x + static_cast<float>(w - inner_thickness), y - static_cast<float>(outer_thickness) });
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
            return (mx >= x0 && my >= y0 && mx < x0 + w0 && my < y0 + h0);

#elif defined(PAXS_USING_SFML)
            return sf::RectangleShape{}.getGlobalBounds().contains(
                { static_cast<float>(sf::Mouse::getPosition(Window::window()).x),
                  static_cast<float>(sf::Mouse::getPosition(Window::window()).y) });
#else
            return false;
#endif
        }
    };
}

#endif // !PAX_GRAPHICA_ROUND_RECT_HPP
