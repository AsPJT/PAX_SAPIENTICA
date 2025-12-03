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
#include <PAX_MAHOROBA/Rendering/UIComponent.hpp>
#include <PAX_MAHOROBA/UI/UILayout.hpp>

#include <PAX_SAPIENTICA/Calendar/Date.hpp>
#include <PAX_SAPIENTICA/Calendar/Koyomi.hpp>
#include <PAX_SAPIENTICA/Key/CalendarKeys.hpp>
#include <PAX_SAPIENTICA/Utility/Logger.hpp>
#include <PAX_SAPIENTICA/Utility/MurMur3.hpp>

namespace paxs {

    /// @brief カレンダーコンテンツの描画クラス
    class CalendarContent : public UIComponent {
    private:
        // Locales ドメインキー定数
        static constexpr std::uint_least32_t calendar_type_domain_key = MurMur3::calcHash("CalendarType");
        static constexpr std::uint_least32_t calendar_gengo_domain_key = MurMur3::calcHash("Gengo");
        static constexpr std::uint_least32_t calendar_chinese_domain_key = MurMur3::calcHash("Chinese");
        // static constexpr std::uint_least32_t calendar_sueki_domain_key = MurMur3::calcHash("Sueki"); // 将来の実装用

        // Calendar UI domain and keys
        static constexpr std::uint_least32_t calendar_ui_domain_key = MurMur3::calcHash("CalendarUnits");
        static constexpr std::uint_least32_t unit_year_key = MurMur3::calcHash("unit_year");
        static constexpr std::uint_least32_t unit_month_key = MurMur3::calcHash("unit_month");
        static constexpr std::uint_least32_t unit_day_key = MurMur3::calcHash("unit_day");
        static constexpr std::uint_least32_t unit_leap_key = MurMur3::calcHash("unit_leap");

        // 描画に必要な参照（render呼び出し時に設定される）
        const paxs::Koyomi& koyomi_;
        const paxs::UILayout& ui_layout_;

        // カレンダーを描画（言語に応じて自動選択）
        void renderInternal() const {
            const std::uint_least32_t current_language = Fonts().getSelectedLanguageKey();

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

                // 日付の型に応じてドメインを選択
                std::uint_least32_t domain_key = calendar_type_domain_key;
                if (std::holds_alternative<cal::JapanDate>(koyomi_.date_list[i].date)) {
                    domain_key = calendar_gengo_domain_key;
                }
                else if (std::holds_alternative<cal::ChinaDate>(koyomi_.date_list[i].date)) {
                    domain_key = calendar_chinese_domain_key;
                }

                // 暦の読み方を返す（存在しない時代の暦はnullptrが返る想定なので警告抑制）
                const std::string* const text_str = Fonts().getLocalesText(
                    domain_key,
                    koyomi_.date_list[i].calendar_name_key,
                    true  // suppress_warning = true
                );
                if (text_str == nullptr) {
                    // 暦が存在しない時代のため、スキップ
                    continue;
                }

                // 暦描画フォントを指定
                paxg::Font* one_font = Fonts().getFont(
                    Fonts().getSelectedLanguageKey(),
                    static_cast<std::uint_least8_t>(paxg::FontConfig::KOYOMI_FONT_SIZE),
                    static_cast<std::uint_least8_t>(paxg::FontConfig::KOYOMI_FONT_BUFFER_THICKNESS)
                );
                if (one_font == nullptr) {
                    PAXS_WARNING("CalendarContent: Missing font for calendar rendering.");
                    continue;
                }

                switch (output_type) {
                case paxs::cal::DateOutputType::name_and_ymd:
                {
                    // GregorianDate
                    if (const auto* gregorian_ptr = std::get_if<cal::GregorianDate>(&koyomi_.date_list[i].date)) {
                        date_year = static_cast<int>(gregorian_ptr->getYear());
                        date_month = static_cast<int>(gregorian_ptr->getMonth());
                        date_day = static_cast<int>(gregorian_ptr->getDay());
                        date_lm = paxs::cal::GregorianDate::isLeapMonth();
                    }
                    // JulianDate
                    else if (const auto* julian_ptr = std::get_if<cal::JulianDate>(&koyomi_.date_list[i].date)) {
                        date_year = static_cast<int>(julian_ptr->getYear());
                        date_month = static_cast<int>(julian_ptr->getMonth());
                        date_day = static_cast<int>(julian_ptr->getDay());
                        date_lm = paxs::cal::JulianDate::isLeapMonth();
                    }
                    // IslamicDate
                    else if (const auto* islamic_ptr = std::get_if<cal::IslamicDate>(&koyomi_.date_list[i].date)) {
                        date_year = static_cast<int>(islamic_ptr->getYear());
                        date_month = static_cast<int>(islamic_ptr->getMonth());
                        date_day = static_cast<int>(islamic_ptr->getDay());
                        date_lm = paxs::cal::IslamicDate::isLeapMonth();
                    }
                    // JapanDate
                    else if (const auto* japan_ptr = std::get_if<cal::JapanDate>(&koyomi_.date_list[i].date)) {
                        date_year = static_cast<int>(japan_ptr->getYear());
                        date_month = static_cast<int>(japan_ptr->getMonth());
                        date_day = static_cast<int>(japan_ptr->getDay());
                        date_lm = japan_ptr->isLeapMonth();
                    }
                    // ChinaDate
                    else if (const auto* china_ptr = std::get_if<cal::ChinaDate>(&koyomi_.date_list[i].date)) {
                        date_year = static_cast<int>(china_ptr->getYear());
                        date_month = static_cast<int>(china_ptr->getMonth());
                        date_day = static_cast<int>(china_ptr->getDay());
                        date_lm = china_ptr->isLeapMonth();
                    }

                    if (date_day <= 0 || date_month <= 0) {
                        break;
                    }

                    (*one_font).drawTopRight(*text_str,
                        paxg::Vec2i(static_cast<int>(ui_layout_.koyomi_font_x), static_cast<int>(ui_layout_.koyomi_font_y + i * (paxg::FontConfig::KOYOMI_FONT_SIZE * 4 / 3))), paxg::Color(0, 0, 0));

                    // Get localized unit strings
                    const std::string* unit_year = Fonts().getLocalesText(calendar_ui_domain_key, unit_year_key);
                    const std::string* unit_month = Fonts().getLocalesText(calendar_ui_domain_key, unit_month_key);
                    const std::string* unit_day = Fonts().getLocalesText(calendar_ui_domain_key, unit_day_key);

                    if (unit_year != nullptr) {
                        (*one_font).drawTopRight(*unit_year, paxg::Vec2i(static_cast<int>((120 * paxg::FontConfig::KOYOMI_FONT_SIZE / 30.0) + ui_layout_.koyomi_font_x), static_cast<int>(ui_layout_.koyomi_font_y + i * (paxg::FontConfig::KOYOMI_FONT_SIZE * 4 / 3))), paxg::Color(0, 0, 0));
                    } else {
                        PAXS_WARNING("[CalendarContent] Missing unit_year text");
                    }
                    if (unit_month != nullptr) {
                        (*one_font).drawTopRight(*unit_month, paxg::Vec2i(static_cast<int>((220 * paxg::FontConfig::KOYOMI_FONT_SIZE / 30.0) + ui_layout_.koyomi_font_x), static_cast<int>(ui_layout_.koyomi_font_y + i * (paxg::FontConfig::KOYOMI_FONT_SIZE * 4 / 3))), paxg::Color(0, 0, 0));
                    } else {
                        PAXS_WARNING("[CalendarContent] Missing unit_month text");
                    }
                    if (unit_day != nullptr) {
                        (*one_font).drawTopRight(*unit_day, paxg::Vec2i(static_cast<int>((300 * paxg::FontConfig::KOYOMI_FONT_SIZE / 30.0) + ui_layout_.koyomi_font_x), static_cast<int>(ui_layout_.koyomi_font_y + i * (paxg::FontConfig::KOYOMI_FONT_SIZE * 4 / 3))), paxg::Color(0, 0, 0));
                    } else {
                        PAXS_WARNING("[CalendarContent] Missing unit_day text");
                    }

                    (*one_font).drawTopRight(std::to_string(date_year), paxg::Vec2i(static_cast<int>((85 * paxg::FontConfig::KOYOMI_FONT_SIZE / 30) + ui_layout_.koyomi_font_x), static_cast<int>(ui_layout_.koyomi_font_y + i * (paxg::FontConfig::KOYOMI_FONT_SIZE * 4 / 3))), paxg::Color(0, 0, 0));
                    (*one_font).drawTopRight(std::to_string(date_month), paxg::Vec2i(static_cast<int>((190 * paxg::FontConfig::KOYOMI_FONT_SIZE / 30) + ui_layout_.koyomi_font_x), static_cast<int>(ui_layout_.koyomi_font_y + i * (paxg::FontConfig::KOYOMI_FONT_SIZE * 4 / 3))), paxg::Color(0, 0, 0));
                    (*one_font).drawTopRight(std::to_string(date_day), paxg::Vec2i(static_cast<int>((270 * paxg::FontConfig::KOYOMI_FONT_SIZE / 30) + ui_layout_.koyomi_font_x), static_cast<int>(ui_layout_.koyomi_font_y + i * (paxg::FontConfig::KOYOMI_FONT_SIZE * 4 / 3))), paxg::Color(0, 0, 0));
                    if (date_lm) {
                        const std::string* unit_leap = Fonts().getLocalesText(calendar_ui_domain_key, unit_leap_key);
                        if (unit_leap != nullptr) {
                            (*one_font).drawTopRight(*unit_leap, paxg::Vec2i(static_cast<int>((
                                (date_month < 10) ? int(167 * paxg::FontConfig::KOYOMI_FONT_SIZE / 30.0) + ui_layout_.koyomi_font_x : int(152 * paxg::FontConfig::KOYOMI_FONT_SIZE / 30.0) + ui_layout_.koyomi_font_x
                                )), static_cast<int>(ui_layout_.koyomi_font_y + i * (paxg::FontConfig::KOYOMI_FONT_SIZE * 4 / 3))), paxg::Color(0, 0, 0));
                        } else {
                            PAXS_WARNING("[CalendarContent] Missing unit_leap text for leap month");
                        }
                    }
                    break;
                }
                case paxs::cal::DateOutputType::name_and_value:
                {
                    (*one_font).drawTopRight(*text_str,
                        paxg::Vec2i(static_cast<int>(ui_layout_.koyomi_font_x), static_cast<int>(ui_layout_.koyomi_font_y + i * (paxg::FontConfig::KOYOMI_FONT_SIZE * 4 / 3))), paxg::Color(0, 0, 0));
                    std::visit([&](const auto& x) {
                        date_day = int(x.getDay());
                        }, koyomi_.date_list[i].date);
                    (*one_font).drawTopRight(std::to_string(date_day), paxg::Vec2i(static_cast<int>(int(300 * paxg::FontConfig::KOYOMI_FONT_SIZE / 30.0) + ui_layout_.koyomi_font_x), static_cast<int>(ui_layout_.koyomi_font_y + i * (paxg::FontConfig::KOYOMI_FONT_SIZE * 4 / 3))), paxg::Color(0, 0, 0));
                    break;
                }
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

                // 日付の型に応じてドメインを選択
                std::uint_least32_t domain_key = calendar_type_domain_key;
                if (std::holds_alternative<cal::JapanDate>(koyomi_.date_list[i].date)) {
                    domain_key = calendar_gengo_domain_key;
                }
                else if (std::holds_alternative<cal::ChinaDate>(koyomi_.date_list[i].date)) {
                    domain_key = calendar_chinese_domain_key;
                }

                // 暦描画フォントを指定
                paxg::Font* one_font = Fonts().getFont(
                    Fonts().getSelectedLanguageKey(),
                    static_cast<std::uint_least8_t>(paxg::FontConfig::KOYOMI_FONT_SIZE),
                    static_cast<std::uint_least8_t>(paxg::FontConfig::KOYOMI_FONT_BUFFER_THICKNESS)
                );
                if (one_font == nullptr) {
                    PAXS_WARNING("CalendarContent: Missing font for calendar rendering.");
                    continue;
                }

                // 暦の読み方を返す（存在しない時代の暦はnullptrが返る想定なので警告抑制）
                const std::string* const text_str = Fonts().getLocalesText(
                    domain_key,
                    koyomi_.date_list[i].calendar_name_key,
                    true  // suppress_warning = true
                );
                if (text_str == nullptr) {
                    // 暦が存在しない時代のため、スキップ
                    continue;
                }

                switch (output_type) {
                case paxs::cal::DateOutputType::name_and_ymd:
                    // GregorianDate
                    if (const auto* gregorian_ptr = std::get_if<cal::GregorianDate>(&koyomi_.date_list[i].date)) {
                        date_year = static_cast<int>(gregorian_ptr->getYear());
                        date_month = static_cast<int>(gregorian_ptr->getMonth());
                        date_day = static_cast<int>(gregorian_ptr->getDay());
                        date_lm = paxs::cal::GregorianDate::isLeapMonth();
                    }
                    // JulianDate
                    else if (const auto* julian_ptr = std::get_if<cal::JulianDate>(&koyomi_.date_list[i].date)) {
                        date_year = static_cast<int>(julian_ptr->getYear());
                        date_month = static_cast<int>(julian_ptr->getMonth());
                        date_day = static_cast<int>(julian_ptr->getDay());
                        date_lm = paxs::cal::JulianDate::isLeapMonth();
                    }
                    // IslamicDate
                    else if (const auto* islamic_ptr = std::get_if<cal::IslamicDate>(&koyomi_.date_list[i].date)) {
                        date_year = static_cast<int>(islamic_ptr->getYear());
                        date_month = static_cast<int>(islamic_ptr->getMonth());
                        date_day = static_cast<int>(islamic_ptr->getDay());
                        date_lm = paxs::cal::IslamicDate::isLeapMonth();
                    }
                    // JapanDate
                    else if (const auto* japan_ptr = std::get_if<cal::JapanDate>(&koyomi_.date_list[i].date)) {
                        date_year = static_cast<int>(japan_ptr->getYear());
                        date_month = static_cast<int>(japan_ptr->getMonth());
                        date_day = static_cast<int>(japan_ptr->getDay());
                        date_lm = japan_ptr->isLeapMonth();
                    }
                    // ChinaDate
                    else if (const auto* china_ptr = std::get_if<cal::ChinaDate>(&koyomi_.date_list[i].date)) {
                        date_year = static_cast<int>(china_ptr->getYear());
                        date_month = static_cast<int>(china_ptr->getMonth());
                        date_day = static_cast<int>(china_ptr->getDay());
                        date_lm = china_ptr->isLeapMonth();
                    }

                    if (date_day <= 0 || date_month <= 0) {
                        break;
                    }

                    if (text_str != nullptr) {
                        (*one_font).drawTopRight(*text_str,
                        paxg::Vec2i(static_cast<int>(ui_layout_.koyomi_font_en_x), static_cast<int>(ui_layout_.koyomi_font_en_y + i * (paxg::FontConfig::KOYOMI_FONT_SIZE * 4 / 3))), paxg::Color(0, 0, 0));
                    }

                    (*one_font).drawTopRight(",", paxg::Vec2i(static_cast<int>(int(95 * paxg::FontConfig::KOYOMI_FONT_SIZE / 30.0) + ui_layout_.koyomi_font_en_x), static_cast<int>(ui_layout_.koyomi_font_en_y + i * (paxg::FontConfig::KOYOMI_FONT_SIZE * 4 / 3))), paxg::Color(0, 0, 0));
                    (*one_font).drawTopRight(",", paxg::Vec2i(static_cast<int>(int(235 * paxg::FontConfig::KOYOMI_FONT_SIZE / 30.0) + ui_layout_.koyomi_font_en_x), static_cast<int>(ui_layout_.koyomi_font_en_y + i * (paxg::FontConfig::KOYOMI_FONT_SIZE * 4 / 3))), paxg::Color(0, 0, 0));
                    (*one_font).drawTopRight("th", paxg::Vec2i(static_cast<int>(int(315 * paxg::FontConfig::KOYOMI_FONT_SIZE / 30.0) + ui_layout_.koyomi_font_en_x), static_cast<int>(ui_layout_.koyomi_font_en_y + i * (paxg::FontConfig::KOYOMI_FONT_SIZE * 4 / 3))), paxg::Color(0, 0, 0));

                    (*one_font).drawTopRight(std::to_string(date_year), paxg::Vec2i(static_cast<int>(int(en_cal_name_pos_x * paxg::FontConfig::KOYOMI_FONT_SIZE / 30.0) + ui_layout_.koyomi_font_en_x), static_cast<int>(ui_layout_.koyomi_font_en_y + i * (paxg::FontConfig::KOYOMI_FONT_SIZE * 4 / 3))), paxg::Color(0, 0, 0));
                    (*one_font).drawTopRight(std::string(koyomi_.month_name[date_month]), paxg::Vec2i(static_cast<int>(int(220 * paxg::FontConfig::KOYOMI_FONT_SIZE / 30.0) + ui_layout_.koyomi_font_en_x), static_cast<int>(ui_layout_.koyomi_font_en_y + i * (paxg::FontConfig::KOYOMI_FONT_SIZE * 4 / 3))), paxg::Color(0, 0, 0));
                    (*one_font).drawTopRight(std::to_string(date_day), paxg::Vec2i(static_cast<int>(int(280 * paxg::FontConfig::KOYOMI_FONT_SIZE / 30.0) + ui_layout_.koyomi_font_en_x), static_cast<int>(ui_layout_.koyomi_font_en_y + i * (paxg::FontConfig::KOYOMI_FONT_SIZE * 4 / 3))), paxg::Color(0, 0, 0));

                    if (date_lm) {
                        const std::string* unit_leap = Fonts().getLocalesText(calendar_ui_domain_key, unit_leap_key);
                        if (unit_leap != nullptr) {
                            (*one_font).drawTopRight(*unit_leap, paxg::Vec2i(static_cast<int>((
                                int(152 * paxg::FontConfig::KOYOMI_FONT_SIZE / 30.0) + ui_layout_.koyomi_font_en_x
                                )), static_cast<int>(ui_layout_.koyomi_font_en_y + i * (paxg::FontConfig::KOYOMI_FONT_SIZE * 4 / 3))), paxg::Color(0, 0, 0));
                        }
                    }
                    break;
                case paxs::cal::DateOutputType::name_and_value:
                {
                    if (text_str != nullptr) {
                        (*one_font).drawTopRight(*text_str,
                            paxs::Vector2<int>(
                                static_cast<int>(ui_layout_.koyomi_font_en_x),
                                static_cast<int>(ui_layout_.koyomi_font_en_y + (i * paxg::FontConfig::KOYOMI_FONT_SIZE * 4 / 3))
                            ),
                            paxg::Color(0, 0, 0)
                        );
                    }
                }

                std::visit([&](const auto& x) {
                    date_day = static_cast<int>(x.getDay());
                    }, koyomi_.date_list[i].date);

                (*one_font).drawTopRight(
                    std::to_string(date_day),
                    paxs::Vector2<int>(
                        static_cast<int>((315 * paxg::FontConfig::KOYOMI_FONT_SIZE / 30.0) + ui_layout_.koyomi_font_en_x),
                        static_cast<int>(ui_layout_.koyomi_font_en_y + (i * paxg::FontConfig::KOYOMI_FONT_SIZE * 4 / 3))
                    ),
                    paxg::Color(0, 0, 0));
                break;
                default:
                    break;
                }
            }
        }

    public:
        CalendarContent(const paxs::UILayout& ui_layout, const paxs::Koyomi& koyomi)
        : koyomi_(koyomi), ui_layout_(ui_layout) {}

        void render() const override {
            renderInternal();
        }

        bool isVisible() const override { return true; }
        RenderLayer getLayer() const override { return RenderLayer::UIContent; }
        Rect<int> getRect() const override { return ui_layout_.calendar_panel.getRect(); }
        const char* getName() const override { return "CalendarContent"; }
        void setPos(const Vector2<int>& /*pos*/) override {}
    };

}

#endif // !PAX_MAHOROBA_CALENDAR_WIDGET_HPP
