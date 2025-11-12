/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_FONT_HPP
#define PAX_GRAPHICA_FONT_HPP

#include <string>

#if defined(PAXS_USING_SIV3D)
#include <Siv3D.hpp>
#elif defined(PAXS_USING_DXLIB)
#include <DxLib.h>
#elif defined(PAXS_USING_SFML)
#include <SFML/Graphics.hpp>
#include <SFML/System/Utf.hpp>
#endif

#include <PAX_GRAPHICA/Color.hpp>
#include <PAX_GRAPHICA/Vec2.hpp>
#include <PAX_GRAPHICA/Window.hpp>

#include <PAX_SAPIENTICA/System/AppConfig.hpp>
#include <PAX_SAPIENTICA/Utility/Logger.hpp>

namespace paxg{
    struct Font {
        constexpr Font() = default;
#if defined(PAXS_USING_SIV3D)
        s3d::Font font{};
        Font(const int size_, const std::string& path, const int buffer_thickness) {
            font = (path.size() == 0) ?
                s3d::Font(s3d::FontMethod::SDF, size_) :
                s3d::Font(s3d::FontMethod::SDF, size_, s3d::Unicode::FromUTF8(paxs::AppConfig::getInstance()->getRootPath() + path));
            font.setBufferThickness(buffer_thickness);
        }
        bool is_outline = false;
        s3d::TextStyle outline{};
        void setOutline(const double inner, const double outer, const paxg::Color& color) {
            is_outline = true;
            outline = s3d::TextStyle::Outline(inner, outer, color.color);
        }

        void drawBottomLeft(const std::string& str, const paxg::Vec2i& pos, const paxg::Color& color) const {
            if (is_outline) {
                font(s3d::Unicode::FromUTF8(str)).draw(
                    outline,
                    s3d::Arg::bottomLeft = s3d::Vec2(pos.x(), pos.y()),
                    color.color);
            }
            else {
                font(s3d::Unicode::FromUTF8(str)).draw(
                    s3d::Arg::bottomLeft = s3d::Vec2(pos.x(), pos.y()),
                    color.color);
            }
        }
        void drawTopRight(const std::string& str, const paxg::Vec2i& pos, const paxg::Color& color) const {
            if (is_outline) {
                font(s3d::Unicode::FromUTF8(str)).draw(
                    outline,
                    s3d::Arg::topRight = s3d::Vec2(pos.x(), pos.y()),
                    color.color);
            }
            else {
                font(s3d::Unicode::FromUTF8(str)).draw(
                    s3d::Arg::topRight = s3d::Vec2(pos.x(), pos.y()),
                    color.color);
            }
        }
        void draw(const std::string& str, const paxg::Vec2i& pos, const paxg::Color& color) const {
            if (is_outline) {
                font(s3d::Unicode::FromUTF8(str)).draw(
                    outline,
                    s3d::Vec2(pos.x(), pos.y()),
                    color.color);
            }
            else {
                font(s3d::Unicode::FromUTF8(str)).draw(
                    s3d::Vec2(pos.x(), pos.y()),
                    color.color);
            }
        }
        void drawBottomCenter(const std::string& str, const paxg::Vec2i& pos, const paxg::Color& color) const {
            if (is_outline) {
                font(s3d::Unicode::FromUTF8(str)).draw(
                    outline,
                    s3d::Arg::bottomCenter = s3d::Vec2(pos.x(), pos.y()),
                    color.color);
            }
            else {
                font(s3d::Unicode::FromUTF8(str)).draw(
                    s3d::Arg::bottomCenter = s3d::Vec2(pos.x(), pos.y()),
                    color.color);
            }
        }
        void drawTopCenter(const std::string& str, const paxg::Vec2i& pos, const paxg::Color& color) const {
            if (is_outline) {
                font(s3d::Unicode::FromUTF8(str)).draw(
                    outline,
                    s3d::Arg::topCenter = s3d::Vec2(pos.x(), pos.y()),
                    color.color);
            }
            else {
                font(s3d::Unicode::FromUTF8(str)).draw(
                    s3d::Arg::topCenter = s3d::Vec2(pos.x(), pos.y()),
                    color.color);
            }
        }
        void drawAt(const std::string& str, const paxg::Vec2i& pos, const paxg::Color& color) const {
            if (is_outline) {
                font(s3d::Unicode::FromUTF8(str)).drawAt(
                    outline,
                    s3d::Vec2(pos.x(), pos.y()),
                    color.color);
            }
            else {
                font(s3d::Unicode::FromUTF8(str)).drawAt(
                    s3d::Vec2(pos.x(), pos.y()),
                    color.color);
            }
        }

        int height() const {
            return font.height();
        }
        int width(const std::string& str_) {
            return static_cast<int>(font(s3d::Unicode::FromUTF8(str_)).region().w);
        }

#elif defined(PAXS_USING_DXLIB)
        int font{ -1 }; int h{ 0 };
        Font(const int size_, const std::string& path, const int buffer_thickness) {
            // 文字コードをUTF-8に統一
            DxLib::SetUseCharCodeFormat(DX_CHARCODEFORMAT_UTF8);

            if (path.size() == 0) {
                // デフォルトフォントを使用
                font = DxLib::CreateFontToHandle(NULL, size_, -1,
                    (buffer_thickness <= 0) ? DX_FONTTYPE_NORMAL :
#ifdef __ANDROID__
                    DX_FONTTYPE_EDGE
#else
                    DX_FONTTYPE_ANTIALIASING_8X8
#endif
                );
            } else {
                // 外部フォントファイルを使用
                const std::string full_path = paxs::AppConfig::getInstance()->getRootPath() + path;
                int font_data_handle = DxLib::LoadFontDataToHandle(full_path.c_str(), buffer_thickness);

                if (font_data_handle == -1) {
                    PAXS_WARNING("Failed to load font data: " + full_path);
                    // フォールバック: デフォルトフォント
                    font = DxLib::CreateFontToHandle(NULL, size_, -1,
                        (buffer_thickness <= 0) ? DX_FONTTYPE_NORMAL : DX_FONTTYPE_ANTIALIASING_8X8);
                } else {
                    font = DxLib::CreateFontToHandle(NULL, size_, -1,
                        (buffer_thickness <= 0) ? DX_FONTTYPE_NORMAL : DX_FONTTYPE_ANTIALIASING_8X8,
                        -1, -1, FALSE, font_data_handle);
                }
            }

            // フォント間隔を0に明示（他のライブラリと同じ挙動にする）
            if (font != -1) {
                DxLib::SetFontSpaceToHandle(font, 0);
            }

            h = size_;
        }
        void setOutline(const double inner, const double outer, const paxg::Color& color) const {

        }

        void drawAlign(int align, paxg::Vec2i pos, std::string str_, const paxg::Color& color_, unsigned int edge_color = 0) const {
            // std::size_t string_length;
            // align　が　0左寄り　1中央寄り　2右寄り
            if (align < 0) align = 0;
            if (align > 2) align = 2;
            std::size_t str_len = str_.size(); // 全体の文字数を得る
            int sizex; int sizey; int line_count; // 描画した時のサイズと行数を調べる
            DxLib::GetDrawStringSizeToHandle(&sizex, &sizey, &line_count, str_.c_str(), static_cast<int>(str_len), font, FALSE);
            const char* last = &(str_[0]) + str_len; // 終端を得る
            const char* next; // 次の改行ポイント
            int line_space = DxLib::GetFontLineSpaceToHandle(font); // 一行の縦幅を得る
            int str_width; // 一行の横幅
            std::size_t char_count = 0; // 描画した文字数をカウント
            for (int i = 0; i < line_count; i++) { // 行数分、繰り返す
                next = DxLib::strstrDx(str_.c_str(), "\n"); // 次の改行ポイントを探す
                if (next == NULL) next = last; // 改行が見つからない場合は最終行ということなので終端を代入
                str_len = next - &(str_[0]); // この行の文字数を得る
                str_width = DxLib::GetDrawNStringWidthToHandle(str_.c_str(), str_len, font, FALSE); // この行の横幅を得る
                // if (char_count < string_length) { // 描画した文字数がまだ指定範囲を超えていない場合
                char_count += str_len; // この行の文字数を足す
                // if (char_count > string_length) { // この行の中に指定した終端がある場合
                //     str_len -= (char_count - string_length); // 差分を引いて文字数を調整
                // }
                // x座標を調整して一行分描画する
                DxLib::DrawNStringToHandle(pos.x() + ((align * (sizex - str_width)) / 2), pos.y(),
                    str_.c_str(), str_len, DxLib::GetColor(color_.r, color_.g, color_.b), font, edge_color, FALSE);
                // }
                char_count += 1; // 改行文字の分
                str_ = std::string(next + 1); // 次の行の先頭にする(+1は改行文字の分)
                pos.setY(pos.y() + line_space); // y座標をずらす
            }
        }

        void drawBottomLeft(const std::string& str, const paxg::Vec2i& pos, const paxg::Color& color) const {
            if (font == -1) DxLib::DrawFormatString(pos.x(), pos.y() - 10, DxLib::GetColor(color.r, color.g, color.b), str.c_str());
            else DxLib::DrawStringToHandle(pos.x(), pos.y() - 10, str.c_str(), DxLib::GetColor(color.r, color.g, color.b), font, 0xffffffff);
        }
        void drawTopRight(const std::string& str, const paxg::Vec2i& pos, const paxg::Color& color) const {
            if (font == -1) DxLib::DrawFormatString(pos.x(), pos.y() + 10, DxLib::GetColor(color.r, color.g, color.b), str.c_str());
            else {
                int size_x = 0, size_y = 0, line_count = 0; // 描画した時のサイズと行数を調べる
                DxLib::GetDrawStringSizeToHandle(&size_x, &size_y, &line_count, str.c_str(), static_cast<int>(str.size()), font, FALSE);
                DxLib::DrawStringToHandle(pos.x() - size_x, pos.y() + size_y / 2, str.c_str(), DxLib::GetColor(color.r, color.g, color.b), font, 0xffffffff);
            }
        }
        void draw(const std::string& str, const paxg::Vec2i& pos, const paxg::Color& color) const {
            if (font == -1) DxLib::DrawFormatString(pos.x(), pos.y(), DxLib::GetColor(color.r, color.g, color.b), str.c_str());
            else DxLib::DrawStringToHandle(pos.x(), pos.y(), str.c_str(), DxLib::GetColor(color.r, color.g, color.b), font, 0xffffffff);
        }
        void drawBottomCenter(const std::string& str, const paxg::Vec2i& pos, const paxg::Color& color) const {
            if (font == -1) DxLib::DrawFormatString(pos.x(), pos.y() - 10, DxLib::GetColor(color.r, color.g, color.b), str.c_str());
            else {
                int size_x = 0, size_y = 0, line_count = 0; // 描画した時のサイズと行数を調べる
                DxLib::GetDrawStringSizeToHandle(&size_x, &size_y, &line_count, str.c_str(), static_cast<int>(str.size()), font, FALSE);
                DxLib::DrawStringToHandle(pos.x() - size_x / 2, pos.y() - size_y / 2, str.c_str(), DxLib::GetColor(color.r, color.g, color.b), font, 0xffffffff);
            }
        }
        void drawTopCenter(const std::string& str, const paxg::Vec2i& pos, const paxg::Color& color) const {
            if (font == -1) DxLib::DrawFormatString(pos.x(), pos.y() + 10, DxLib::GetColor(color.r, color.g, color.b), str.c_str());
            else {
                int size_x = 0, size_y = 0, line_count = 0; // 描画した時のサイズと行数を調べる
                DxLib::GetDrawStringSizeToHandle(&size_x, &size_y, &line_count, str.c_str(), static_cast<int>(str.size()), font, FALSE);
                DxLib::DrawStringToHandle(pos.x() - size_x / 2, pos.y() + size_y / 2, str.c_str(), DxLib::GetColor(color.r, color.g, color.b), font, 0xffffffff);
            }
        }
        void drawAt(const std::string& str, const paxg::Vec2i& pos, const paxg::Color& color) const {
            if (font == -1) DxLib::DrawFormatString(pos.x(), pos.y(), DxLib::GetColor(color.r, color.g, color.b), str.c_str());
            else {
                int size_x = 0, size_y = 0, line_count = 0; // 描画した時のサイズと行数を調べる
                DxLib::GetDrawStringSizeToHandle(&size_x, &size_y, &line_count, str.c_str(), static_cast<int>(str.size()), font, FALSE);
                DxLib::DrawStringToHandle(pos.x() - size_x / 2, pos.y(), str.c_str(), DxLib::GetColor(color.r, color.g, color.b), font, 0xffffffff);
            }
        }

        int height() const {
            if (font == -1) return h;
            // DxLibで実際の行高さを取得（他のライブラリと同じ挙動にする）
            return DxLib::GetFontLineSpaceToHandle(font);
        }
        int width(const std::string& str_) {
            if (font == -1) return static_cast<int>(static_cast<double>(str_.size()) * h * 0.5);
            // DxLibで実際の文字列幅を取得（フォントハンドルを使用）
            int w = 0, h_temp = 0;
            DxLib::GetDrawStringSizeToHandle(&w, &h_temp, NULL, str_.c_str(), static_cast<int>(str_.size()), font, FALSE);
            return w;
        }

#elif defined(PAXS_USING_SFML)
        sf::Font font{};
        int size = 0;
        Font(const int size_, const std::string& path, const int /*buffer_thickness*/) {
            size = size_;
            if (path.size() == 0) return;
            if (!font.openFromFile(paxs::AppConfig::getInstance()->getRootPath() + path)) {
                PAXS_WARNING(path + " is missing.");
            }
        }

        void setOutline(const double /*inner*/, const double /*outer*/, const paxg::Color& /*color*/) {

        }

        void drawBottomLeft(const std::string& str, const paxg::Vec2i& pos, const paxg::Color& color) const {
            sf::Text text(font);
            std::wstring wstr;
            sf::Utf<8>::toWide(str.begin(), str.end(), std::back_inserter(wstr));
            text.setString(wstr);
            text.setCharacterSize(size);
            text.setFillColor(color.color);
            text.setOutlineColor(sf::Color::White);
            text.setOutlineThickness(2.0f);
            text.setPosition({ static_cast<float>(pos.x()), static_cast<float>(pos.y() - size / 2) });
            paxg::Window::window().draw(text);
        }

        void drawTopRight(const std::string& str, const paxg::Vec2i& pos, const paxg::Color& color) const {
            sf::Text text(font);
            std::wstring wstr;
            sf::Utf<8>::toWide(str.begin(), str.end(), std::back_inserter(wstr));
            text.setString(wstr);
            text.setCharacterSize(size);
            text.setFillColor(color.color);
            text.setOutlineColor(sf::Color::White);
            text.setOutlineThickness(2.0f);
            text.setPosition({ static_cast<float>(pos.x() - text.getGlobalBounds().size.x), static_cast<float>(pos.y() + size / 2) });
            paxg::Window::window().draw(text);
        }

        void draw(const std::string& str, const paxg::Vec2i& pos, const paxg::Color& color) const {
            sf::Text text(font);
            std::wstring wstr;
            sf::Utf<8>::toWide(str.begin(), str.end(), std::back_inserter(wstr));
            text.setString(wstr);
            text.setCharacterSize(size);
            text.setFillColor(color.color);
            text.setOutlineColor(sf::Color::White);
            text.setOutlineThickness(2.0f);
            text.setPosition({ static_cast<float>(pos.x()), static_cast<float>(pos.y()) });
            paxg::Window::window().draw(text);
        }

        void drawBottomCenter(const std::string& str, const paxg::Vec2i& pos, const paxg::Color& color) const {
            sf::Text text(font);
            std::wstring wstr;
            sf::Utf<8>::toWide(str.begin(), str.end(), std::back_inserter(wstr));
            text.setString(wstr);
            text.setCharacterSize(size);
            text.setFillColor(color.color);
            text.setOutlineColor(sf::Color::White);
            text.setOutlineThickness(2.0f);
            text.setPosition({ static_cast<float>(pos.x() - text.getGlobalBounds().size.x / 2), static_cast<float>(pos.y() - size / 2) });
            paxg::Window::window().draw(text);
        }

        void drawTopCenter(const std::string& str, const paxg::Vec2i& pos, const paxg::Color& color) const {
            sf::Text text(font);
            std::wstring wstr;
            sf::Utf<8>::toWide(str.begin(), str.end(), std::back_inserter(wstr));
            text.setString(wstr);
            text.setCharacterSize(size);
            text.setFillColor(color.color);
            text.setOutlineColor(sf::Color::White);
            text.setOutlineThickness(2.0f);
            text.setPosition({ static_cast<float>(pos.x() - text.getGlobalBounds().size.x / 2), static_cast<float>(pos.y() + size / 2) });
            paxg::Window::window().draw(text);
        }

        void drawAt(const std::string& str, const paxg::Vec2i& pos, const paxg::Color& color) const {
            sf::Text text(font);
            std::wstring wstr;
            sf::Utf<8>::toWide(str.begin(), str.end(), std::back_inserter(wstr));
            text.setString(wstr);
            text.setCharacterSize(size);
            text.setFillColor(color.color);
            text.setOutlineColor(sf::Color::White);
            text.setOutlineThickness(2.0f);
            text.setPosition({ static_cast<float>(pos.x() - text.getGlobalBounds().size.x / 2), static_cast<float>(pos.y()) });
            paxg::Window::window().draw(text);
        }

        int height() const {
            return size;
        }

        int width(const std::string& str_) {
            sf::Text text(font);
            std::wstring wstr;
            sf::Utf<8>::toWide(str_.begin(), str_.end(), std::back_inserter(wstr));
            text.setString(wstr);
            text.setCharacterSize(size);

            return static_cast<int>(text.getGlobalBounds().size.x);
            // return str_.size() * size * 0.5;
        }

#else
        Font([[maybe_unused]] const int size_, [[maybe_unused]] const std::string& path, [[maybe_unused]] const int buffer_thickness) {
        }
        void setOutline([[maybe_unused]] const double inner, [[maybe_unused]] const double outer, [[maybe_unused]] const paxg::Color& color) {
        }

        void drawBottomLeft([[maybe_unused]] const std::string& str, [[maybe_unused]] const paxg::Vec2i& pos, [[maybe_unused]] const paxg::Color& color) const {
        }
        void drawTopRight([[maybe_unused]] const std::string& str, [[maybe_unused]] const paxg::Vec2i& pos, [[maybe_unused]] const paxg::Color& color) const {
        }
        void draw([[maybe_unused]] const std::string& str, [[maybe_unused]] const paxg::Vec2i& pos, [[maybe_unused]] const paxg::Color& color) const {
        }
        void drawBottomCenter([[maybe_unused]] const std::string& str, [[maybe_unused]] const paxg::Vec2i& pos, [[maybe_unused]] const paxg::Color& color) const {
        }
        void drawTopCenter([[maybe_unused]] const std::string& str, [[maybe_unused]] const paxg::Vec2i& pos, [[maybe_unused]] const paxg::Color& color) const {
        }
        void drawAt([[maybe_unused]] const std::string& str, [[maybe_unused]] const paxg::Vec2i& pos, [[maybe_unused]] const paxg::Color& color) const {
        }

        int height() const {
            return 0;
        }
        int width([[maybe_unused]] const std::string& str_) {
            return 0;
        }
#endif
    };

    /// @brief フォント設定の定数
    /// @brief Font configuration constants
    struct FontConfig {
        // プルダウンメニューのフォント設定
        static constexpr int PULLDOWN_FONT_SIZE =
#if defined(PAXS_USING_DXLIB) && defined(__ANDROID__)
            40;
#else
            24;
#endif
        static constexpr int PULLDOWN_FONT_BUFFER_THICKNESS = 3;

        // 暦表示のフォント設定
        static constexpr int KOYOMI_FONT_SIZE = 22;
        static constexpr int KOYOMI_FONT_BUFFER_THICKNESS = 3;
    };

}

#endif // !PAX_GRAPHICA_FONT_HPP
