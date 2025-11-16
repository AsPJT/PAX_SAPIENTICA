/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_CALENDAR_WIDGET_HPP
#define PAX_MAHOROBA_CALENDAR_WIDGET_HPP

#include <PAX_GRAPHICA/Color.hpp>
#include <PAX_GRAPHICA/Font.hpp>
#include <PAX_GRAPHICA/Vec2.hpp>

#include <PAX_MAHOROBA/Rendering/FontSystem.hpp>
#include <PAX_MAHOROBA/Rendering/IRenderable.hpp>
#include <PAX_MAHOROBA/UI/UILayout.hpp>

#include <PAX_SAPIENTICA/Calendar/Date.hpp>
#include <PAX_SAPIENTICA/Calendar/Koyomi.hpp>
#include <PAX_SAPIENTICA/Key/CalendarKeys.hpp>
#include <PAX_SAPIENTICA/Utility/Logger.hpp>
#include <PAX_SAPIENTICA/Utility/MurMur3.hpp>

namespace paxs {

    /// @brief カレンダーコンテンツの描画クラス
    class CalendarContent : public IRenderable {
    private:
        // 描画に必要な参照（render呼び出し時に設定される）
        const paxs::Koyomi& koyomi_;
        const paxs::UILayout& ui_layout_;

        // カレンダーを描画（言語に応じて自動選択）
        void renderInternal() const {
            const std::uint_least32_t current_language = Fonts().getSelectedLanguage().cgetKey();

            // 日本語・中国語・台湾語の場合はアジア式カレンダー
            if (current_language == MurMur3::calcHash("ja-JP")
                || current_language == MurMur3::calcHash("zh-TW")
                || current_language == MurMur3::calcHash("zh-CN")) {
                renderAsianCalendar();
            }
            else {
                // その他の言語は西洋式カレンダー
                renderWesternCalendar();
            }
        }

        // 日本語・中国語のカレンダーを描画
        void renderAsianCalendar() const {
            for (std::size_t i = 0; i < koyomi_.date_list.size(); ++i) {
                cal::DateOutputType output_type = cal::DateOutputType::name_and_value;
                std::visit([&](const auto& x) { output_type = x.getDateOutputType(); }, koyomi_.date_list[i].date);

                int date_year = 0;
                int date_month = 0;
                int date_day = 0;
                bool date_lm = false;

                // 暦の読み方を返す
                const std::string* const text_str = Fonts().getText(
                    koyomi_.date_list[i].calendar_name_key,
                    LanguageDomain::UI
                );
                if (text_str == nullptr) {
                    // 暦が存在しない時代のため、スキップ
                    continue;
                }

                // 暦描画フォントを指定
                paxg::Font* one_font = Fonts().getFont(
                    Fonts().getSelectedLanguage().cgetKey(),
                    static_cast<std::uint_least8_t>(paxg::FontConfig::KOYOMI_FONT_SIZE),
                    static_cast<std::uint_least8_t>(paxg::FontConfig::KOYOMI_FONT_BUFFER_THICKNESS)
                );
                if (one_font == nullptr) {
                    PAXS_WARNING("CalendarContent: Missing font for calendar rendering.");
                    continue;
                }

                switch (output_type) {
                case paxs::cal::DateOutputType::name_and_ymd:
                    std::visit([&](const auto& x) {
                        date_year = int(x.cgetYear());
                        date_month = int(x.cgetMonth());
                        date_day = int(x.cgetDay());
                        date_lm = x.isLeapMonth();
                        }, koyomi_.date_list[i].date);
                    if (date_day <= 0 || date_month <= 0) {
                        break;
                    }

                    (*one_font).drawTopRight(*text_str,
                        paxg::Vec2i(static_cast<int>(ui_layout_.koyomi_font_x), static_cast<int>(ui_layout_.koyomi_font_y + i * (paxg::FontConfig::KOYOMI_FONT_SIZE * 4 / 3))), paxg::Color(0, 0, 0));
                    (*one_font).drawTopRight(reinterpret_cast<const char*>(u8"年"), paxg::Vec2i(static_cast<int>(int(120 * paxg::FontConfig::KOYOMI_FONT_SIZE / 30.0) + ui_layout_.koyomi_font_x), static_cast<int>(ui_layout_.koyomi_font_y + i * (paxg::FontConfig::KOYOMI_FONT_SIZE * 4 / 3))), paxg::Color(0, 0, 0));
                    (*one_font).drawTopRight(reinterpret_cast<const char*>(u8"月"), paxg::Vec2i(static_cast<int>(int(220 * paxg::FontConfig::KOYOMI_FONT_SIZE / 30.0) + ui_layout_.koyomi_font_x), static_cast<int>(ui_layout_.koyomi_font_y + i * (paxg::FontConfig::KOYOMI_FONT_SIZE * 4 / 3))), paxg::Color(0, 0, 0));
                    (*one_font).drawTopRight(reinterpret_cast<const char*>(u8"日"), paxg::Vec2i(static_cast<int>(int(300 * paxg::FontConfig::KOYOMI_FONT_SIZE / 30.0) + ui_layout_.koyomi_font_x), static_cast<int>(ui_layout_.koyomi_font_y + i * (paxg::FontConfig::KOYOMI_FONT_SIZE * 4 / 3))), paxg::Color(0, 0, 0));

                    (*one_font).drawTopRight(std::to_string(date_year), paxg::Vec2i(static_cast<int>(int(85 * paxg::FontConfig::KOYOMI_FONT_SIZE / 30.0) + ui_layout_.koyomi_font_x), static_cast<int>(ui_layout_.koyomi_font_y + i * (paxg::FontConfig::KOYOMI_FONT_SIZE * 4 / 3))), paxg::Color(0, 0, 0));
                    (*one_font).drawTopRight(std::to_string(date_month), paxg::Vec2i(static_cast<int>(int(190 * paxg::FontConfig::KOYOMI_FONT_SIZE / 30.0) + ui_layout_.koyomi_font_x), static_cast<int>(ui_layout_.koyomi_font_y + i * (paxg::FontConfig::KOYOMI_FONT_SIZE * 4 / 3))), paxg::Color(0, 0, 0));
                    (*one_font).drawTopRight(std::to_string(date_day), paxg::Vec2i(static_cast<int>(int(270 * paxg::FontConfig::KOYOMI_FONT_SIZE / 30.0) + ui_layout_.koyomi_font_x), static_cast<int>(ui_layout_.koyomi_font_y + i * (paxg::FontConfig::KOYOMI_FONT_SIZE * 4 / 3))), paxg::Color(0, 0, 0));
                    if (date_lm) {
                        (*one_font).drawTopRight(reinterpret_cast<const char*>(u8"閏"), paxg::Vec2i(static_cast<int>((
                            (date_month < 10) ? int(167 * paxg::FontConfig::KOYOMI_FONT_SIZE / 30.0) + ui_layout_.koyomi_font_x : int(152 * paxg::FontConfig::KOYOMI_FONT_SIZE / 30.0) + ui_layout_.koyomi_font_x
                            )), static_cast<int>(ui_layout_.koyomi_font_y + i * (paxg::FontConfig::KOYOMI_FONT_SIZE * 4 / 3))), paxg::Color(0, 0, 0));
                    }
                    break;
                case paxs::cal::DateOutputType::name_and_value:
                    (*one_font).drawTopRight(*text_str,
                        paxg::Vec2i(static_cast<int>(ui_layout_.koyomi_font_x), static_cast<int>(ui_layout_.koyomi_font_y + i * (paxg::FontConfig::KOYOMI_FONT_SIZE * 4 / 3))), paxg::Color(0, 0, 0));
                    std::visit([&](const auto& x) {
                        date_day = int(x.cgetDay());
                        }, koyomi_.date_list[i].date);
                    (*one_font).drawTopRight(std::to_string(date_day), paxg::Vec2i(static_cast<int>(int(300 * paxg::FontConfig::KOYOMI_FONT_SIZE / 30.0) + ui_layout_.koyomi_font_x), static_cast<int>(ui_layout_.koyomi_font_y + i * (paxg::FontConfig::KOYOMI_FONT_SIZE * 4 / 3))), paxg::Color(0, 0, 0));
                    break;
                default:
                    break;
                }
            }
        }

        // 英語のカレンダーを描画
        void renderWesternCalendar() const {
            for (std::size_t i = 0; i < koyomi_.date_list.size(); ++i) {
                cal::DateOutputType output_type = cal::DateOutputType::name_and_value;
                std::visit([&](const auto& x) { output_type = x.getDateOutputType(); }, koyomi_.date_list[i].date);

                int date_year = 0;
                int date_month = 0;
                int date_day = 0;
                bool date_lm = false;
                const int en_cal_name_pos_x = 85;

                // 暦描画フォントを指定
                paxg::Font* one_font = Fonts().getFont(
                    Fonts().getSelectedLanguage().cgetKey(),
                    static_cast<std::uint_least8_t>(paxg::FontConfig::KOYOMI_FONT_SIZE),
                    static_cast<std::uint_least8_t>(paxg::FontConfig::KOYOMI_FONT_BUFFER_THICKNESS)
                );
                if (one_font == nullptr) {
                    PAXS_WARNING("CalendarContent: Missing font for calendar rendering.");
                    continue;
                }

                // 暦の読み方を返す
                const std::string* const text_str = Fonts().getText(
                    koyomi_.date_list[i].calendar_name_key,
                    LanguageDomain::UI
                );
                if (text_str == nullptr) {
                    // 暦が存在しない時代のため、スキップ
                    continue;
                }

                switch (output_type) {
                case paxs::cal::DateOutputType::name_and_ymd:
                    std::visit([&](const auto& x) {
                        date_year = int(x.cgetYear());
                        date_month = int(x.cgetMonth());
                        date_day = int(x.cgetDay());
                        date_lm = x.isLeapMonth();
                        }, koyomi_.date_list[i].date);
                    if (date_day <= 0 || date_month <= 0) break;

                    if (text_str != nullptr) (*one_font).drawTopRight(*text_str,
                        paxg::Vec2i(static_cast<int>(ui_layout_.koyomi_font_en_x), static_cast<int>(ui_layout_.koyomi_font_en_y + i * (paxg::FontConfig::KOYOMI_FONT_SIZE * 4 / 3))), paxg::Color(0, 0, 0));

                    (*one_font).drawTopRight(",", paxg::Vec2i(static_cast<int>(int(95 * paxg::FontConfig::KOYOMI_FONT_SIZE / 30.0) + ui_layout_.koyomi_font_en_x), static_cast<int>(ui_layout_.koyomi_font_en_y + i * (paxg::FontConfig::KOYOMI_FONT_SIZE * 4 / 3))), paxg::Color(0, 0, 0));
                    (*one_font).drawTopRight(",", paxg::Vec2i(static_cast<int>(int(235 * paxg::FontConfig::KOYOMI_FONT_SIZE / 30.0) + ui_layout_.koyomi_font_en_x), static_cast<int>(ui_layout_.koyomi_font_en_y + i * (paxg::FontConfig::KOYOMI_FONT_SIZE * 4 / 3))), paxg::Color(0, 0, 0));
                    (*one_font).drawTopRight("th", paxg::Vec2i(static_cast<int>(int(315 * paxg::FontConfig::KOYOMI_FONT_SIZE / 30.0) + ui_layout_.koyomi_font_en_x), static_cast<int>(ui_layout_.koyomi_font_en_y + i * (paxg::FontConfig::KOYOMI_FONT_SIZE * 4 / 3))), paxg::Color(0, 0, 0));

                    (*one_font).drawTopRight(std::to_string(date_year), paxg::Vec2i(static_cast<int>(int(en_cal_name_pos_x * paxg::FontConfig::KOYOMI_FONT_SIZE / 30.0) + ui_layout_.koyomi_font_en_x), static_cast<int>(ui_layout_.koyomi_font_en_y + i * (paxg::FontConfig::KOYOMI_FONT_SIZE * 4 / 3))), paxg::Color(0, 0, 0));
                    (*one_font).drawTopRight(std::string(koyomi_.month_name[date_month]), paxg::Vec2i(static_cast<int>(int(220 * paxg::FontConfig::KOYOMI_FONT_SIZE / 30.0) + ui_layout_.koyomi_font_en_x), static_cast<int>(ui_layout_.koyomi_font_en_y + i * (paxg::FontConfig::KOYOMI_FONT_SIZE * 4 / 3))), paxg::Color(0, 0, 0));
                    (*one_font).drawTopRight(std::to_string(date_day), paxg::Vec2i(static_cast<int>(int(280 * paxg::FontConfig::KOYOMI_FONT_SIZE / 30.0) + ui_layout_.koyomi_font_en_x), static_cast<int>(ui_layout_.koyomi_font_en_y + i * (paxg::FontConfig::KOYOMI_FONT_SIZE * 4 / 3))), paxg::Color(0, 0, 0));

                    if (date_lm) {
                        (*one_font).drawTopRight("int.", paxg::Vec2i(static_cast<int>((
                            int(152 * paxg::FontConfig::KOYOMI_FONT_SIZE / 30.0) + ui_layout_.koyomi_font_en_x
                            )), static_cast<int>(ui_layout_.koyomi_font_en_y + i * (paxg::FontConfig::KOYOMI_FONT_SIZE * 4 / 3))), paxg::Color(0, 0, 0));
                    }
                    break;
                case paxs::cal::DateOutputType::name_and_value:
                {
                    if (text_str != nullptr) (*one_font).drawTopRight(*text_str,
                        paxg::Vec2i(static_cast<int>(ui_layout_.koyomi_font_en_x), static_cast<int>(ui_layout_.koyomi_font_en_y + i * (paxg::FontConfig::KOYOMI_FONT_SIZE * 4 / 3))), paxg::Color(0, 0, 0));
                }
                std::visit([&](const auto& x) {
                    date_day = int(x.cgetDay());
                    }, koyomi_.date_list[i].date);
                (*one_font).drawTopRight(std::to_string(date_day), paxg::Vec2i(static_cast<int>(int(315 * paxg::FontConfig::KOYOMI_FONT_SIZE / 30.0) + ui_layout_.koyomi_font_en_x), static_cast<int>(ui_layout_.koyomi_font_en_y + i * (paxg::FontConfig::KOYOMI_FONT_SIZE * 4 / 3))), paxg::Color(0, 0, 0));
                break;
                default:
                    break;
                }
            }
        }

    public:
        CalendarContent(const paxs::UILayout& ui_layout, const paxs::Koyomi& koyomi)
        : ui_layout_(ui_layout), koyomi_(koyomi) {}

        void render() const override {
            renderInternal();
        }

        bool isVisible() const override { return true; }
        RenderLayer getLayer() const override { return RenderLayer::UIContent; }
    };

}

#endif // !PAX_MAHOROBA_CALENDAR_WIDGET_HPP
