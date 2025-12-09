/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_TOGGLE_BUTTON_HPP
#define PAX_GRAPHICA_TOGGLE_BUTTON_HPP

#if defined(PAXS_USING_SIV3D)
#include <Siv3D.hpp>
#elif defined(PAXS_USING_DXLIB)
#include <DxLib.h>
#elif defined(PAXS_USING_SFML)
#include <SFML/Graphics.hpp>
#endif

#include <PAX_GRAPHICA/Color.hpp>
#include <PAX_GRAPHICA/Window.hpp>

#include <PAX_SAPIENTICA/Core/Type/Rect.hpp>
#include <PAX_SAPIENTICA/Core/Type/Vector2.hpp>

namespace paxg {

    /// @brief Toggle button component with sliding knob (inspired by CSS switch design)
    /// @brief スライドするつまみ付きトグルボタンコンポーネント（CSSスイッチデザインに触発）
    ///
    /// Features / 機能:
    /// - Rounded rectangle background (pill shape) / 角丸長方形の背景（ピル型）
    /// - Sliding circular knob / スライドする円形のつまみ
    /// - Smooth animation / スムーズなアニメーション
    /// - Color changes based on state (green=ON, gray=OFF) / 状態に応じた色の変化（緑=ON、灰色=OFF）
    /// - Shadow effect on knob / つまみに影効果
    class ToggleButton {
    private:
        float x_{}, y_{};           // Position (位置)
        float width_{50.0f};        // Width (幅)
        float height_{30.0f};       // Height (高さ)
        bool is_on_{false};         // Toggle state (トグル状態)

        // Colors (色)
        static constexpr paxg::Color color_on_{0, 222, 0};        // Green when ON #00de00 (ONの時の緑)
        static constexpr paxg::Color color_off_{255, 255, 255};   // White when OFF (OFFの時の白色)
        static constexpr paxg::Color color_knob_{255, 255, 255};  // White knob (白いつまみ)
        static constexpr paxg::Color color_shadow_{187, 187, 187};// Shadow color (#bbbbbb) (影の色)

        // Animation (アニメーション)
        float knob_position_{0.0f}; // 0.0 (left) to 1.0 (right)
        static constexpr float animation_speed_{0.2f}; // Smooth transition speed

    public:
        /// @brief Default constructor
        /// @brief デフォルトコンストラクタ
        ToggleButton() = default;

        /// @brief Constructor with position
        /// @brief 位置を指定するコンストラクタ
        /// @param x X position (X座標)
        /// @param y Y position (Y座標)
        constexpr ToggleButton(float x, float y) : x_(x), y_(y) {}

        /// @brief Constructor with position and size
        /// @brief 位置とサイズを指定するコンストラクタ
        /// @param x X position (X座標)
        /// @param y Y position (Y座標)
        /// @param width Width (幅)
        /// @param height Height (高さ)
        constexpr ToggleButton(float x, float y, float width, float height)
            : x_(x), y_(y), width_(width), height_(height) {}

        /// @brief Set position
        /// @brief 位置を設定
        /// @param x X position (X座標)
        /// @param y Y position (Y座標)
        void setPosition(float x, float y) {
            x_ = x;
            y_ = y;
        }

        /// @brief Set size
        /// @brief サイズを設定
        /// @param width Width (幅)
        /// @param height Height (高さ)
        void setSize(float width, float height) {
            width_ = width;
            height_ = height;
        }

        /// @brief Get toggle state
        /// @brief トグル状態を取得
        /// @return true if ON, false if OFF (ONの場合true、OFFの場合false)
        bool isOn() const {
            return is_on_;
        }

        /// @brief Set toggle state
        /// @brief トグル状態を設定
        /// @param on New state (新しい状態)
        void setOn(bool on) {
            is_on_ = on;
            // Immediately update knob position to match state (no animation on manual set)
            knob_position_ = on ? 1.0f : 0.0f;
        }

        /// @brief Toggle the state (ON→OFF or OFF→ON)
        /// @brief 状態を切り替え（ON→OFFまたはOFF→ON）
        void toggle() {
            is_on_ = !is_on_;
        }

        /// @brief Update button state (call every frame in main loop)
        /// @brief ボタンの状態を更新（メインループで毎フレーム呼び出す）
        /// @details Handles animation and click detection / アニメーションとクリック判定を処理
        void update() {
            // Animate knob position (smooth transition)
            const float target_position = is_on_ ? 1.0f : 0.0f;
            if (knob_position_ < target_position) {
                knob_position_ += animation_speed_;
                if (knob_position_ > target_position) knob_position_ = target_position;
            } else if (knob_position_ > target_position) {
                knob_position_ -= animation_speed_;
                if (knob_position_ < target_position) knob_position_ = target_position;
            }
        }

        /// @brief Get interpolated background color based on knob position
        /// @brief つまみの位置に基づいて補間された背景色を取得
        /// @return Interpolated color between OFF and ON / OFFとONの間で補間された色
        paxg::Color getInterpolatedColor() const {
            // Linear interpolation between color_off_ (white: 255,255,255) and color_on_ (green: 0,222,0)
            const unsigned char r = static_cast<unsigned char>(
                255 + (0 - 255) * knob_position_
            );
            const unsigned char g = static_cast<unsigned char>(
                255 + (222 - 255) * knob_position_
            );
            const unsigned char b = static_cast<unsigned char>(
                255 + (0 - 255) * knob_position_
            );
            return paxg::Color{r, g, b};
        }

        /// @brief Draw the toggle button
        /// @brief トグルボタンを描画
        /// @details Draws background, knob, and shadow / 背景、つまみ、影を描画
        void draw() const {
            // Background: two circles + center rectangle for pill shape
            const float radius = height_ / 2.0f;
            const paxg::Color bg_color = getInterpolatedColor();

#if defined(PAXS_USING_SIV3D)
            // Left circle
            s3d::Circle(x_ + radius, y_ + radius, radius).draw(paxg::ColorF(bg_color));

            // Center rectangle
            s3d::Rect(x_ + radius, y_, width_ - height_, height_).draw(paxg::ColorF(bg_color));

            // Right circle
            s3d::Circle(x_ + width_ - radius, y_ + radius, radius).draw(paxg::ColorF(bg_color));

            // Knob
            const float knob_radius = height_ * 0.4f;
            const float knob_x = x_ + radius + (width_ - height_) * knob_position_;
            const float knob_y = y_ + radius;

            // Shadow
            s3d::Circle(knob_x + 2, knob_y + 2, knob_radius).draw(s3d::ColorF(0, 0, 0, 0.2));

            // Knob circle
            s3d::Circle(knob_x, knob_y, knob_radius).draw(paxg::ColorF(color_knob_));

#elif defined(PAXS_USING_DXLIB)
            // Get RGB values from bg_color for DxLib
            const unsigned char bg_r = static_cast<unsigned char>(255 + (0 - 255) * knob_position_);
            const unsigned char bg_g = static_cast<unsigned char>(255 + (222 - 255) * knob_position_);
            const unsigned char bg_b = static_cast<unsigned char>(255 + (0 - 255) * knob_position_);

            // Left circle
            DxLib::DrawCircle(
                static_cast<int>(x_ + radius),
                static_cast<int>(y_ + radius),
                static_cast<int>(radius),
                DxLib::GetColor(bg_r, bg_g, bg_b),
                TRUE
            );

            // Center rectangle
            DxLib::DrawBox(
                static_cast<int>(x_ + radius),
                static_cast<int>(y_),
                static_cast<int>(x_ + width_ - radius),
                static_cast<int>(y_ + height_),
                DxLib::GetColor(bg_r, bg_g, bg_b),
                TRUE
            );

            // Right circle
            DxLib::DrawCircle(
                static_cast<int>(x_ + width_ - radius),
                static_cast<int>(y_ + radius),
                static_cast<int>(radius),
                DxLib::GetColor(bg_r, bg_g, bg_b),
                TRUE
            );

            // Border for mostly OFF state
            if (knob_position_ < 0.5f) {
                const unsigned char border_alpha = static_cast<unsigned char>(214 * (1.0f - knob_position_ * 2.0f));
                // Left circle border
                DxLib::DrawCircle(
                    static_cast<int>(x_ + radius),
                    static_cast<int>(y_ + radius),
                    static_cast<int>(radius),
                    DxLib::GetColor(border_alpha, border_alpha, border_alpha),
                    FALSE
                );
                // Right circle border
                DxLib::DrawCircle(
                    static_cast<int>(x_ + width_ - radius),
                    static_cast<int>(y_ + radius),
                    static_cast<int>(radius),
                    DxLib::GetColor(border_alpha, border_alpha, border_alpha),
                    FALSE
                );
                // Top and bottom borders
                DxLib::DrawLine(
                    static_cast<int>(x_ + radius),
                    static_cast<int>(y_),
                    static_cast<int>(x_ + width_ - radius),
                    static_cast<int>(y_),
                    DxLib::GetColor(border_alpha, border_alpha, border_alpha)
                );
                DxLib::DrawLine(
                    static_cast<int>(x_ + radius),
                    static_cast<int>(y_ + height_),
                    static_cast<int>(x_ + width_ - radius),
                    static_cast<int>(y_ + height_),
                    DxLib::GetColor(border_alpha, border_alpha, border_alpha)
                );
            }

            // Knob
            const float knob_radius = height_ * 0.4f;
            const float knob_x = x_ + radius + (width_ - height_) * knob_position_;
            const float knob_y = y_ + radius;

            // Shadow
            DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, 51);
            DxLib::DrawCircle(
                static_cast<int>(knob_x + 2),
                static_cast<int>(knob_y + 2),
                static_cast<int>(knob_radius),
                DxLib::GetColor(187, 187, 187),
                TRUE
            );
            DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

            // Knob circle
            DxLib::DrawCircle(
                static_cast<int>(knob_x),
                static_cast<int>(knob_y),
                static_cast<int>(knob_radius),
                DxLib::GetColor(255, 255, 255),
                TRUE
            );

#elif defined(PAXS_USING_SFML)
            // Left circle
            sf::CircleShape left_circle(radius);
            left_circle.setOrigin(sf::Vector2f(radius, radius));
            left_circle.setPosition(sf::Vector2f(x_ + radius, y_ + radius));
            left_circle.setFillColor(bg_color);
            paxg::Window::window().draw(left_circle);

            // Center rectangle
            sf::RectangleShape center_rect(sf::Vector2f(width_ - height_, height_));
            center_rect.setPosition(sf::Vector2f(x_ + radius, y_));
            center_rect.setFillColor(bg_color);
            paxg::Window::window().draw(center_rect);

            // Right circle
            sf::CircleShape right_circle(radius);
            right_circle.setOrigin(sf::Vector2f(radius, radius));
            right_circle.setPosition(sf::Vector2f(x_ + width_ - radius, y_ + radius));
            right_circle.setFillColor(bg_color);
            paxg::Window::window().draw(right_circle);

            // Knob
            const float knob_radius = height_ * 0.4f;
            const float knob_x = x_ + radius + (width_ - height_) * knob_position_;
            const float knob_y = y_ + radius;

            // Shadow
            sf::CircleShape shadow(knob_radius);
            shadow.setOrigin(sf::Vector2f(knob_radius, knob_radius));
            shadow.setPosition(sf::Vector2f(knob_x + 2, knob_y + 2));
            shadow.setFillColor(sf::Color(0, 0, 0, 51));
            paxg::Window::window().draw(shadow);

            // Knob circle
            sf::CircleShape knob(knob_radius);
            knob.setOrigin(sf::Vector2f(knob_radius, knob_radius));
            knob.setPosition(sf::Vector2f(knob_x, knob_y));
            knob.setFillColor(color_knob_);
            paxg::Window::window().draw(knob);
#else
            (void)radius;
            (void)bg_color;
#endif
        }

        /// @brief Get position
        paxs::Vector2<float> getPosition() const {
            return paxs::Vector2<float>{x_, y_};
        }

        /// @brief Get size
        paxs::Vector2<float> getSize() const {
            return paxs::Vector2<float>{width_, height_};
        }

        /// @brief Get bounding rectangle
        paxs::Rect<float> getRect() const {
            return paxs::Rect<float>{x_, y_, width_, height_};
        }
    };

} // namespace paxg

#endif // !PAX_GRAPHICA_TOGGLE_BUTTON_HPP
