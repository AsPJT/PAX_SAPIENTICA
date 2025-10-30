/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023-2024 As Project
	[Production]	2023-2024 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_CALENDAR_RENDERER_HPP
#define PAX_MAHOROBA_CALENDAR_RENDERER_HPP

#include <PAX_GRAPHICA/Color.hpp>
#include <PAX_GRAPHICA/Font.hpp>
#include <PAX_GRAPHICA/Vec2.hpp>

#include <PAX_MAHOROBA/Calendar.hpp>
#include <PAX_MAHOROBA/CalendarUILayout.hpp>
#include <PAX_MAHOROBA/LanguageFonts.hpp>

#include <PAX_SAPIENTICA/Calendar/Date.hpp>
#include <PAX_SAPIENTICA/Language.hpp>
#include <PAX_SAPIENTICA/MurMur3.hpp>

namespace paxs {

    // カレンダー表示を担当するクラス
    class CalendarRenderer {
    public:
        // 日本語・中国語のカレンダーを描画
        static void renderAsianCalendar(
            const paxs::KoyomiSiv3D& koyomi_siv,
            const paxs::CalendarUILayout& ui_layout,
            int koyomi_font_size,
            int koyomi_font_buffer_thickness_size,
            const SelectLanguage& select_language,
            const paxs::Language& language_text,
            paxs::LanguageFonts& language_fonts
        ) {
            for (std::size_t i = 0; i < koyomi_siv.date_list.size(); ++i) {
                cal::DateOutputType output_type = cal::DateOutputType::name_and_value;
                std::visit([&](const auto& x) { output_type = x.getDateOutputType(); }, koyomi_siv.date_list[i].date);

                int date_year = 0;
                int date_month = 0;
                int date_day = 0;
                bool date_lm = false;

                // 暦の読み方を返す
                const std::string* const text_str = language_text.getStringPtr(
                    koyomi_siv.date_list[i].calendar_name_key,
                    select_language.cgetKey()
                );
                if (text_str == nullptr) continue;

                // 暦描画フォントを指定
                paxg::Font* one_font = language_fonts.getAndAdd(select_language.cgetKey(), static_cast<std::uint_least8_t>(koyomi_font_size), static_cast<std::uint_least8_t>(koyomi_font_buffer_thickness_size));
                if (one_font == nullptr) continue;

                switch (output_type) {
                case paxs::cal::DateOutputType::name_and_ymd:
                    std::visit([&](const auto& x) {
                        date_year = int(x.cgetYear());
                        date_month = int(x.cgetMonth());
                        date_day = int(x.cgetDay());
                        date_lm = x.isLeapMonth();
                        }, koyomi_siv.date_list[i].date);
                    if (date_day <= 0 || date_month <= 0) break;

                    (*one_font).drawTopRight(*text_str,
                        paxg::Vec2i(static_cast<int>(ui_layout.koyomi_font_x), static_cast<int>(ui_layout.koyomi_font_y + i * (koyomi_font_size * 4 / 3))), paxg::Color(0, 0, 0));
                    (*one_font).drawTopRight(reinterpret_cast<const char*>(u8"年"), paxg::Vec2i(static_cast<int>(int(120 * koyomi_font_size / 30.0) + ui_layout.koyomi_font_x), static_cast<int>(ui_layout.koyomi_font_y + i * (koyomi_font_size * 4 / 3))), paxg::Color(0, 0, 0));
                    (*one_font).drawTopRight(reinterpret_cast<const char*>(u8"月"), paxg::Vec2i(static_cast<int>(int(220 * koyomi_font_size / 30.0) + ui_layout.koyomi_font_x), static_cast<int>(ui_layout.koyomi_font_y + i * (koyomi_font_size * 4 / 3))), paxg::Color(0, 0, 0));
                    (*one_font).drawTopRight(reinterpret_cast<const char*>(u8"日"), paxg::Vec2i(static_cast<int>(int(300 * koyomi_font_size / 30.0) + ui_layout.koyomi_font_x), static_cast<int>(ui_layout.koyomi_font_y + i * (koyomi_font_size * 4 / 3))), paxg::Color(0, 0, 0));

                    (*one_font).drawTopRight(std::to_string(date_year), paxg::Vec2i(static_cast<int>(int(85 * koyomi_font_size / 30.0) + ui_layout.koyomi_font_x), static_cast<int>(ui_layout.koyomi_font_y + i * (koyomi_font_size * 4 / 3))), paxg::Color(0, 0, 0));
                    (*one_font).drawTopRight(std::to_string(date_month), paxg::Vec2i(static_cast<int>(int(190 * koyomi_font_size / 30.0) + ui_layout.koyomi_font_x), static_cast<int>(ui_layout.koyomi_font_y + i * (koyomi_font_size * 4 / 3))), paxg::Color(0, 0, 0));
                    (*one_font).drawTopRight(std::to_string(date_day), paxg::Vec2i(static_cast<int>(int(270 * koyomi_font_size / 30.0) + ui_layout.koyomi_font_x), static_cast<int>(ui_layout.koyomi_font_y + i * (koyomi_font_size * 4 / 3))), paxg::Color(0, 0, 0));
                    if (date_lm) {
                        (*one_font).drawTopRight(reinterpret_cast<const char*>(u8"閏"), paxg::Vec2i(static_cast<int>((
                            (date_month < 10) ? int(167 * koyomi_font_size / 30.0) + ui_layout.koyomi_font_x : int(152 * koyomi_font_size / 30.0) + ui_layout.koyomi_font_x
                            )), static_cast<int>(ui_layout.koyomi_font_y + i * (koyomi_font_size * 4 / 3))), paxg::Color(0, 0, 0));
                    }
                    break;
                case paxs::cal::DateOutputType::name_and_value:
                    (*one_font).drawTopRight(*text_str,
                        paxg::Vec2i(static_cast<int>(ui_layout.koyomi_font_x), static_cast<int>(ui_layout.koyomi_font_y + i * (koyomi_font_size * 4 / 3))), paxg::Color(0, 0, 0));
                    std::visit([&](const auto& x) {
                        date_day = int(x.cgetDay());
                        }, koyomi_siv.date_list[i].date);
                    (*one_font).drawTopRight(std::to_string(date_day), paxg::Vec2i(static_cast<int>(int(300 * koyomi_font_size / 30.0) + ui_layout.koyomi_font_x), static_cast<int>(ui_layout.koyomi_font_y + i * (koyomi_font_size * 4 / 3))), paxg::Color(0, 0, 0));
                    break;
                default:
                    break;
                }
            }
        }

        // 英語のカレンダーを描画
        static void renderWesternCalendar(
            const paxs::KoyomiSiv3D& koyomi_siv,
            const paxs::CalendarUILayout& ui_layout,
            int koyomi_font_size,
            int koyomi_font_buffer_thickness_size,
            const SelectLanguage& select_language,
            const paxs::Language& language_text,
            paxs::LanguageFonts& language_fonts,
            bool is_simulator_active
        ) {
            for (std::size_t i = 0; i < koyomi_siv.date_list.size(); ++i) {
                cal::DateOutputType output_type = cal::DateOutputType::name_and_value;
                std::visit([&](const auto& x) { output_type = x.getDateOutputType(); }, koyomi_siv.date_list[i].date);

                int date_year = 0;
                int date_month = 0;
                int date_day = 0;
                bool date_lm = false;
                const int en_cal_name_pos_x = 85;

                // 暦描画フォントを指定
                paxg::Font* one_font = language_fonts.getAndAdd(select_language.cgetKey(), static_cast<std::uint_least8_t>(koyomi_font_size), static_cast<std::uint_least8_t>(koyomi_font_buffer_thickness_size));
                if (one_font == nullptr) continue;
                // 年描画フォントを指定
                paxg::Font* big_year_font = language_fonts.getAndAdd(select_language.cgetKey(), static_cast<std::uint_least8_t>(koyomi_font_size * 3), static_cast<std::uint_least8_t>(koyomi_font_buffer_thickness_size));
                if (big_year_font == nullptr) continue;

                // 暦の読み方を返す
                const std::string* const text_str = language_text.getStringPtr(
                    koyomi_siv.date_list[i].calendar_name_key,
                    select_language.cgetKey()
                );
                if (text_str == nullptr) continue;

                switch (output_type) {
                case paxs::cal::DateOutputType::name_and_ymd:
                    std::visit([&](const auto& x) {
                        date_year = int(x.cgetYear());
                        date_month = int(x.cgetMonth());
                        date_day = int(x.cgetDay());
                        date_lm = x.isLeapMonth();
                        }, koyomi_siv.date_list[i].date);
                    if (date_day <= 0 || date_month <= 0) break;

                    if (text_str != nullptr) (*one_font).drawTopRight(*text_str,
                        paxg::Vec2i(static_cast<int>(ui_layout.koyomi_font_en_x), static_cast<int>(ui_layout.koyomi_font_en_y + i * (koyomi_font_size * 4 / 3))), paxg::Color(0, 0, 0));

                    (*one_font).drawTopRight(",", paxg::Vec2i(static_cast<int>(int(95 * koyomi_font_size / 30.0) + ui_layout.koyomi_font_en_x), static_cast<int>(ui_layout.koyomi_font_en_y + i * (koyomi_font_size * 4 / 3))), paxg::Color(0, 0, 0));
                    (*one_font).drawTopRight(",", paxg::Vec2i(static_cast<int>(int(235 * koyomi_font_size / 30.0) + ui_layout.koyomi_font_en_x), static_cast<int>(ui_layout.koyomi_font_en_y + i * (koyomi_font_size * 4 / 3))), paxg::Color(0, 0, 0));
                    (*one_font).drawTopRight("th", paxg::Vec2i(static_cast<int>(int(315 * koyomi_font_size / 30.0) + ui_layout.koyomi_font_en_x), static_cast<int>(ui_layout.koyomi_font_en_y + i * (koyomi_font_size * 4 / 3))), paxg::Color(0, 0, 0));

                    (*one_font).drawTopRight(std::to_string(date_year), paxg::Vec2i(static_cast<int>(int(en_cal_name_pos_x * koyomi_font_size / 30.0) + ui_layout.koyomi_font_en_x), static_cast<int>(ui_layout.koyomi_font_en_y + i * (koyomi_font_size * 4 / 3))), paxg::Color(0, 0, 0));
                    (*one_font).drawTopRight(std::string(koyomi_siv.month_name[date_month]), paxg::Vec2i(static_cast<int>(int(220 * koyomi_font_size / 30.0) + ui_layout.koyomi_font_en_x), static_cast<int>(ui_layout.koyomi_font_en_y + i * (koyomi_font_size * 4 / 3))), paxg::Color(0, 0, 0));
                    (*one_font).drawTopRight(std::to_string(date_day), paxg::Vec2i(static_cast<int>(int(280 * koyomi_font_size / 30.0) + ui_layout.koyomi_font_en_x), static_cast<int>(ui_layout.koyomi_font_en_y + i * (koyomi_font_size * 4 / 3))), paxg::Color(0, 0, 0));

                    if (!is_simulator_active) {
                        if (i == 1) (*big_year_font).drawTopRight(std::to_string(date_year)/* + " AD"*/, paxg::Vec2i(static_cast<int>(int(en_cal_name_pos_x * koyomi_font_size / 30.0) + ui_layout.koyomi_font_en_x + 100 - 70), static_cast<int>(550 - 30 + ui_layout.koyomi_font_en_y + i * (koyomi_font_size * 4 / 3))), paxg::Color(0, 0, 0));
                    }

                    if (date_lm) {
                        (*one_font).drawTopRight("int.", paxg::Vec2i(static_cast<int>((
                            int(152 * koyomi_font_size / 30.0) + ui_layout.koyomi_font_en_x
                            )), static_cast<int>(ui_layout.koyomi_font_en_y + i * (koyomi_font_size * 4 / 3))), paxg::Color(0, 0, 0));
                    }
                    break;
                case paxs::cal::DateOutputType::name_and_value:
                {
                    if (text_str != nullptr) (*one_font).drawTopRight(*text_str,
                        paxg::Vec2i(static_cast<int>(ui_layout.koyomi_font_en_x), static_cast<int>(ui_layout.koyomi_font_en_y + i * (koyomi_font_size * 4 / 3))), paxg::Color(0, 0, 0));
                }
                std::visit([&](const auto& x) {
                    date_day = int(x.cgetDay());
                    }, koyomi_siv.date_list[i].date);
                (*one_font).drawTopRight(std::to_string(date_day), paxg::Vec2i(static_cast<int>(int(315 * koyomi_font_size / 30.0) + ui_layout.koyomi_font_en_x), static_cast<int>(ui_layout.koyomi_font_en_y + i * (koyomi_font_size * 4 / 3))), paxg::Color(0, 0, 0));
                break;
                default:
                    break;
                }
            }
        }

        // カレンダーを描画（言語に応じて自動選択）
        static void render(
            const paxs::KoyomiSiv3D& koyomi_siv,
            const paxs::CalendarUILayout& ui_layout,
            int koyomi_font_size,
            int koyomi_font_buffer_thickness_size,
            const SelectLanguage& select_language,
            const paxs::Language& language_text,
            paxs::LanguageFonts& language_fonts,
            bool is_simulator_active
        ) {
            // 日本語・中国語・台湾語の場合はアジア式カレンダー
            if (select_language.cgetKey() == MurMur3::calcHash("ja-JP")
                || select_language.cgetKey() == MurMur3::calcHash("zh-TW")
                || select_language.cgetKey() == MurMur3::calcHash("zh-CN")) {
                renderAsianCalendar(koyomi_siv, ui_layout, koyomi_font_size, koyomi_font_buffer_thickness_size, select_language, language_text, language_fonts);
            }
            else {
                // その他の言語は西洋式カレンダー
                renderWesternCalendar(koyomi_siv, ui_layout, koyomi_font_size, koyomi_font_buffer_thickness_size, select_language, language_text, language_fonts, is_simulator_active);
            }
        }
    };

}

#endif // !PAX_MAHOROBA_CALENDAR_RENDERER_HPP
