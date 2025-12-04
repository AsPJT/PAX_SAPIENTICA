/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_SIV3D_FONT_IMPL_HPP
#define PAX_GRAPHICA_SIV3D_FONT_IMPL_HPP

#ifdef PAXS_USING_SIV3D
#include <string>

#include <Siv3D.hpp>

#include <PAX_GRAPHICA/Color.hpp>
#include <PAX_GRAPHICA/Interface/FontImpl.hpp>
#include <PAX_SAPIENTICA/Core/Type/Vector2.hpp>
#include <PAX_SAPIENTICA/System/AppConfig.hpp>

namespace paxg {

    /// @brief Siv3D font implementation
    /// @brief Siv3Dフォント実装
    class Siv3DFontImpl : public FontImpl {
    private:
        s3d::Font font{};
        bool is_outline = false;
        s3d::TextStyle outline{};

    public:
        Siv3DFontImpl(int size, const std::string& path, int buffer_thickness) {
            font = (path.empty()) ?
                s3d::Font(s3d::FontMethod::SDF, size) :
                s3d::Font(s3d::FontMethod::SDF, size, s3d::Unicode::FromUTF8(paxs::AppConfig::getInstance().getRootPath() + path));
            font.setBufferThickness(buffer_thickness);
        }

        void setOutline(double inner, double outer, const Color& color) override {
            is_outline = true;
            outline = s3d::TextStyle::Outline(inner, outer, paxg::ColorF(color));
        }

        void drawBottomLeft(const std::string& str, const paxs::Vector2<int>& pos, const Color& color) const override {
            if (is_outline) {
                font(s3d::Unicode::FromUTF8(str)).draw(
                    outline,
                    s3d::Arg::bottomLeft = s3d::Vec2(pos.x, pos.y),
                    paxg::ColorF(color));
            }
            else {
                font(s3d::Unicode::FromUTF8(str)).draw(
                    s3d::Arg::bottomLeft = s3d::Vec2(pos.x, pos.y),
                    paxg::ColorF(color));
            }
        }

        void drawTopRight(const std::string& str, const paxs::Vector2<int>& pos, const Color& color) const override {
            if (is_outline) {
                font(s3d::Unicode::FromUTF8(str)).draw(
                    outline,
                    s3d::Arg::topRight = s3d::Vec2(pos.x, pos.y),
                    paxg::ColorF(color));
            }
            else {
                font(s3d::Unicode::FromUTF8(str)).draw(
                    s3d::Arg::topRight = s3d::Vec2(pos.x, pos.y),
                    paxg::ColorF(color));
            }
        }

        void drawBottomRight(const std::string& str, const paxs::Vector2<int>& pos, const Color& color) const override {
            if (is_outline) {
                font(s3d::Unicode::FromUTF8(str)).draw(
                    outline,
                    s3d::Arg::bottomRight = s3d::Vec2(pos.x, pos.y),
                    paxg::ColorF(color));
            }
            else {
                font(s3d::Unicode::FromUTF8(str)).draw(
                    s3d::Arg::bottomRight = s3d::Vec2(pos.x, pos.y),
                    paxg::ColorF(color));
            }
        }

        void draw(const std::string& str, const paxs::Vector2<int>& pos, const Color& color) const override {
            if (is_outline) {
                font(s3d::Unicode::FromUTF8(str)).draw(
                    outline,
                    s3d::Vec2(pos.x, pos.y),
                    paxg::ColorF(color));
            }
            else {
                font(s3d::Unicode::FromUTF8(str)).draw(
                    s3d::Vec2(pos.x, pos.y),
                    paxg::ColorF(color));
            }
        }

        void drawBottomCenter(const std::string& str, const paxs::Vector2<int>& pos, const Color& color) const override {
            if (is_outline) {
                font(s3d::Unicode::FromUTF8(str)).draw(
                    outline,
                    s3d::Arg::bottomCenter = s3d::Vec2(pos.x, pos.y),
                    paxg::ColorF(color));
            }
            else {
                font(s3d::Unicode::FromUTF8(str)).draw(
                    s3d::Arg::bottomCenter = s3d::Vec2(pos.x, pos.y),
                    paxg::ColorF(color));
            }
        }

        void drawTopCenter(const std::string& str, const paxs::Vector2<int>& pos, const Color& color) const override {
            if (is_outline) {
                font(s3d::Unicode::FromUTF8(str)).draw(
                    outline,
                    s3d::Arg::topCenter = s3d::Vec2(pos.x, pos.y),
                    paxg::ColorF(color));
            }
            else {
                font(s3d::Unicode::FromUTF8(str)).draw(
                    s3d::Arg::topCenter = s3d::Vec2(pos.x, pos.y),
                    paxg::ColorF(color));
            }
        }

        void drawAt(const std::string& str, const paxs::Vector2<int>& pos, const Color& color) const override {
            if (is_outline) {
                font(s3d::Unicode::FromUTF8(str)).drawAt(
                    outline,
                    s3d::Vec2(pos.x, pos.y),
                    paxg::ColorF(color));
            }
            else {
                font(s3d::Unicode::FromUTF8(str)).drawAt(
                    s3d::Vec2(pos.x, pos.y),
                    paxg::ColorF(color));
            }
        }

        int height() const override {
            return font.height();
        }

        int width(const std::string& str) override {
            return static_cast<int>(font(s3d::Unicode::FromUTF8(str)).region().w);
        }
    };

} // namespace paxg

#endif // PAXS_USING_SIV3D

#endif // !PAX_GRAPHICA_SIV3D_FONT_IMPL_HPP
