/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_GRAPHICA_DXLIB_FONT_IMPL_HPP
#define PAX_GRAPHICA_DXLIB_FONT_IMPL_HPP

#ifdef PAXS_USING_DXLIB
#include <string>

#include <DxLib.h>

#include <PAX_GRAPHICA/Color.hpp>
#include <PAX_GRAPHICA/Interface/FontImpl.hpp>

#include <PAX_SAPIENTICA/Core/Platform.hpp>
#include <PAX_SAPIENTICA/Core/Type/Vector2.hpp>
#include <PAX_SAPIENTICA/System/AppConfig.hpp>
#include <PAX_SAPIENTICA/Utility/Logger.hpp>

namespace paxg {

    /// @brief DxLib font implementation
    /// @brief DxLibフォント実装
    class DxLibFontImpl : public FontImpl {
    private:
        int font{ -1 };
        int h{ 0 };

        void drawAlign(int align, paxs::Vector2<int> pos, std::string str_, const Color& color_, unsigned int edge_color = 0) const {
            // align が 0左寄り 1中央寄り 2右寄り
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
                char_count += str_len; // この行の文字数を足す
                // x座標を調整して一行分描画する
                DxLib::DrawNStringToHandle(pos.x + ((align * (sizex - str_width)) / 2), pos.y,
                    str_.c_str(), str_len, DxLib::GetColor(color_.r, color_.g, color_.b), font, edge_color, FALSE);
                char_count += 1; // 改行文字の分
                str_ = std::string(next + 1); // 次の行の先頭にする(+1は改行文字の分)
                pos.y += line_space; // y座標をずらす
            }
        }

    public:
        DxLibFontImpl(int size, const std::string& path, int buffer_thickness) {
            // 文字コードをUTF-8に統一
            DxLib::SetUseCharCodeFormat(DX_CHARCODEFORMAT_UTF8);

            if (path.empty()) {
                // デフォルトフォントを使用
                font = DxLib::CreateFontToHandle(NULL, size, -1,
                    (buffer_thickness <= 0) ? DX_FONTTYPE_NORMAL :
#ifdef PAXS_PLATFORM_ANDROID
                    DX_FONTTYPE_EDGE
#else
                    DX_FONTTYPE_ANTIALIASING_8X8
#endif
                );
            } else {
                // 外部フォントファイルを使用
                const std::string full_path = paxs::AppConfig::getInstance().getRootPath() + path;
                int font_data_handle = DxLib::LoadFontDataToHandle(full_path.c_str(), buffer_thickness);

                if (font_data_handle == -1) {
                    PAXS_WARNING("Failed to load font data: " + full_path);
                    // フォールバック: デフォルトフォント
                    font = DxLib::CreateFontToHandle(NULL, size, -1,
                        (buffer_thickness <= 0) ? DX_FONTTYPE_NORMAL : DX_FONTTYPE_ANTIALIASING_8X8);
                } else {
                    font = DxLib::CreateFontToHandle(NULL, size, -1,
                        (buffer_thickness <= 0) ? DX_FONTTYPE_NORMAL : DX_FONTTYPE_ANTIALIASING_8X8,
                        -1, -1, FALSE, font_data_handle);
                }
            }

            // フォント間隔を0に明示（他のライブラリと同じ挙動にする）
            if (font != -1) {
                DxLib::SetFontSpaceToHandle(font, 0);
            }

            h = size;
        }

        void setOutline(double /*inner*/, double /*outer*/, const Color& /*color*/) override {
            // DxLib doesn't support outline in this implementation
        }

        void drawBottomLeft(const std::string& str, const paxs::Vector2<int>& pos, const Color& color) const override {
            if (font == -1) DxLib::DrawFormatString(pos.x, pos.y - 10, DxLib::GetColor(color.r, color.g, color.b), str.c_str());
            else DxLib::DrawStringToHandle(pos.x, pos.y - 10, str.c_str(), DxLib::GetColor(color.r, color.g, color.b), font, 0xffffffff);
        }

        void drawTopRight(const std::string& str, const paxs::Vector2<int>& pos, const Color& color) const override {
            if (font == -1) DxLib::DrawFormatString(pos.x, pos.y + 10, DxLib::GetColor(color.r, color.g, color.b), str.c_str());
            else {
                int size_x = 0, size_y = 0, line_count = 0;
                DxLib::GetDrawStringSizeToHandle(&size_x, &size_y, &line_count, str.c_str(), static_cast<int>(str.size()), font, FALSE);
                DxLib::DrawStringToHandle(pos.x - size_x, pos.y + size_y / 2, str.c_str(), DxLib::GetColor(color.r, color.g, color.b), font, 0xffffffff);
            }
        }

        void drawBottomRight(const std::string& str, const paxs::Vector2<int>& pos, const Color& color) const override {
            if (font == -1) DxLib::DrawFormatString(pos.x, pos.y - 10, DxLib::GetColor(color.r, color.g, color.b), str.c_str());
            else {
                int size_x = 0, size_y = 0, line_count = 0;
                DxLib::GetDrawStringSizeToHandle(&size_x, &size_y, &line_count, str.c_str(), static_cast<int>(str.size()), font, FALSE);
                DxLib::DrawStringToHandle(pos.x - size_x, pos.y - size_y, str.c_str(), DxLib::GetColor(color.r, color.g, color.b), font, 0xffffffff);
            }
        }

        void draw(const std::string& str, const paxs::Vector2<int>& pos, const Color& color) const override {
            if (font == -1) DxLib::DrawFormatString(pos.x, pos.y, DxLib::GetColor(color.r, color.g, color.b), str.c_str());
            else DxLib::DrawStringToHandle(pos.x, pos.y, str.c_str(), DxLib::GetColor(color.r, color.g, color.b), font, 0xffffffff);
        }

        void drawBottomCenter(const std::string& str, const paxs::Vector2<int>& pos, const Color& color) const override {
            if (font == -1) DxLib::DrawFormatString(pos.x, pos.y - 10, DxLib::GetColor(color.r, color.g, color.b), str.c_str());
            else {
                int size_x = 0, size_y = 0, line_count = 0;
                DxLib::GetDrawStringSizeToHandle(&size_x, &size_y, &line_count, str.c_str(), static_cast<int>(str.size()), font, FALSE);
                DxLib::DrawStringToHandle(pos.x - size_x / 2, pos.y - size_y / 2, str.c_str(), DxLib::GetColor(color.r, color.g, color.b), font, 0xffffffff);
            }
        }

        void drawTopCenter(const std::string& str, const paxs::Vector2<int>& pos, const Color& color) const override {
            if (font == -1) DxLib::DrawFormatString(pos.x, pos.y + 10, DxLib::GetColor(color.r, color.g, color.b), str.c_str());
            else {
                int size_x = 0, size_y = 0, line_count = 0;
                DxLib::GetDrawStringSizeToHandle(&size_x, &size_y, &line_count, str.c_str(), static_cast<int>(str.size()), font, FALSE);
                DxLib::DrawStringToHandle(pos.x - size_x / 2, pos.y + size_y / 2, str.c_str(), DxLib::GetColor(color.r, color.g, color.b), font, 0xffffffff);
            }
        }

        void drawAt(const std::string& str, const paxs::Vector2<int>& pos, const Color& color) const override {
            if (font == -1) DxLib::DrawFormatString(pos.x, pos.y, DxLib::GetColor(color.r, color.g, color.b), str.c_str());
            else {
                int size_x = 0, size_y = 0, line_count = 0;
                DxLib::GetDrawStringSizeToHandle(&size_x, &size_y, &line_count, str.c_str(), static_cast<int>(str.size()), font, FALSE);
                DxLib::DrawStringToHandle(pos.x - size_x / 2, pos.y - size_y / 2, str.c_str(), DxLib::GetColor(color.r, color.g, color.b), font, 0xffffffff);
            }
        }

        int height() const override {
            if (font == -1) return h;
            return DxLib::GetFontLineSpaceToHandle(font);
        }

        int width(const std::string& str) override {
            if (font == -1) return static_cast<int>(static_cast<double>(str.size()) * h * 0.5);
            int w = 0, h_temp = 0;
            DxLib::GetDrawStringSizeToHandle(&w, &h_temp, NULL, str.c_str(), static_cast<int>(str.size()), font, FALSE);
            return w;
        }
    };

} // namespace paxg

#endif // PAXS_USING_DXLIB

#endif // !PAX_GRAPHICA_DXLIB_FONT_IMPL_HPP
