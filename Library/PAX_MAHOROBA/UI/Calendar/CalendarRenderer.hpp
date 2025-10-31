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

#include <PAX_MAHOROBA/UI/Calendar/CalendarUILayout.hpp>
#include <PAX_MAHOROBA/UI/IUIWidget.hpp>
#include <PAX_MAHOROBA/Simulation/IViewerComponent.hpp>
#include <PAX_MAHOROBA/Rendering/LanguageFonts.hpp>

#include <PAX_SAPIENTICA/Calendar/Date.hpp>
#include <PAX_SAPIENTICA/Calendar/Koyomi.hpp>
#include <PAX_SAPIENTICA/Language.hpp>
#include <PAX_SAPIENTICA/MurMur3.hpp>
#include <PAX_SAPIENTICA/TouchStateManager.hpp>

namespace paxs {

    // カレンダー表示を担当するクラス
    class CalendarRenderer : public IViewerComponent, public IUIWidget {
    public:
        // 初期化（LanguageFontsへの参照を設定）
        void init(paxs::LanguageFonts& fonts) {
            language_fonts_ = &fonts;
        }

        // IViewerComponent インターフェースの実装
        const char* getName() const override {
            return "CalendarRenderer";
        }

        bool isAvailable() const override {
            return true; // カレンダー表示は常に利用可能
        }

        // setEnabled/isEnabledは下部で実装済み（両インターフェースで共有）

        // カレンダーを描画（言語に応じて自動選択）
        void render(
            const paxs::Koyomi& koyomi,
            const paxs::CalendarUILayout& ui_layout,
            int koyomi_font_size,
            int koyomi_font_buffer_thickness_size,
            const SelectLanguage& select_language,
            const paxs::Language& language_text,
            bool is_simulator_active
        ) {
            // 日本語・中国語・台湾語の場合はアジア式カレンダー
            if (select_language.cgetKey() == MurMur3::calcHash("ja-JP")
                || select_language.cgetKey() == MurMur3::calcHash("zh-TW")
                || select_language.cgetKey() == MurMur3::calcHash("zh-CN")) {
                renderAsianCalendar(koyomi, ui_layout, koyomi_font_size, koyomi_font_buffer_thickness_size, select_language, language_text);
            }
            else {
                // その他の言語は西洋式カレンダー
                renderWesternCalendar(koyomi, ui_layout, koyomi_font_size, koyomi_font_buffer_thickness_size, select_language, language_text, is_simulator_active);
            }
        }

    private:
        paxs::LanguageFonts* language_fonts_ = nullptr;

        // 日本語・中国語のカレンダーを描画
        void renderAsianCalendar(
            const paxs::Koyomi& koyomi,
            const paxs::CalendarUILayout& ui_layout,
            int koyomi_font_size,
            int koyomi_font_buffer_thickness_size,
            const SelectLanguage& select_language,
            const paxs::Language& language_text
        ) {
            if (language_fonts_ == nullptr) return;

            for (std::size_t i = 0; i < koyomi.date_list.size(); ++i) {
                cal::DateOutputType output_type = cal::DateOutputType::name_and_value;
                std::visit([&](const auto& x) { output_type = x.getDateOutputType(); }, koyomi.date_list[i].date);

                int date_year = 0;
                int date_month = 0;
                int date_day = 0;
                bool date_lm = false;

                // 暦の読み方を返す
                const std::string* const text_str = language_text.getStringPtr(
                    koyomi.date_list[i].calendar_name_key,
                    select_language.cgetKey()
                );
                if (text_str == nullptr) continue;

                // 暦描画フォントを指定
                paxg::Font* one_font = language_fonts_->getAndAdd(select_language.cgetKey(), static_cast<std::uint_least8_t>(koyomi_font_size), static_cast<std::uint_least8_t>(koyomi_font_buffer_thickness_size));
                if (one_font == nullptr) continue;

                switch (output_type) {
                case paxs::cal::DateOutputType::name_and_ymd:
                    std::visit([&](const auto& x) {
                        date_year = int(x.cgetYear());
                        date_month = int(x.cgetMonth());
                        date_day = int(x.cgetDay());
                        date_lm = x.isLeapMonth();
                        }, koyomi.date_list[i].date);
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
                        }, koyomi.date_list[i].date);
                    (*one_font).drawTopRight(std::to_string(date_day), paxg::Vec2i(static_cast<int>(int(300 * koyomi_font_size / 30.0) + ui_layout.koyomi_font_x), static_cast<int>(ui_layout.koyomi_font_y + i * (koyomi_font_size * 4 / 3))), paxg::Color(0, 0, 0));
                    break;
                default:
                    break;
                }
            }
        }

        // 英語のカレンダーを描画
        void renderWesternCalendar(
            const paxs::Koyomi& koyomi,
            const paxs::CalendarUILayout& ui_layout,
            int koyomi_font_size,
            int koyomi_font_buffer_thickness_size,
            const SelectLanguage& select_language,
            const paxs::Language& language_text,
            bool is_simulator_active
        ) {
            if (language_fonts_ == nullptr) return;

            for (std::size_t i = 0; i < koyomi.date_list.size(); ++i) {
                cal::DateOutputType output_type = cal::DateOutputType::name_and_value;
                std::visit([&](const auto& x) { output_type = x.getDateOutputType(); }, koyomi.date_list[i].date);

                int date_year = 0;
                int date_month = 0;
                int date_day = 0;
                bool date_lm = false;
                const int en_cal_name_pos_x = 85;

                // 暦描画フォントを指定
                paxg::Font* one_font = language_fonts_->getAndAdd(select_language.cgetKey(), static_cast<std::uint_least8_t>(koyomi_font_size), static_cast<std::uint_least8_t>(koyomi_font_buffer_thickness_size));
                if (one_font == nullptr) continue;
                // 年描画フォントを指定
                paxg::Font* big_year_font = language_fonts_->getAndAdd(select_language.cgetKey(), static_cast<std::uint_least8_t>(koyomi_font_size * 3), static_cast<std::uint_least8_t>(koyomi_font_buffer_thickness_size));
                if (big_year_font == nullptr) continue;

                // 暦の読み方を返す
                const std::string* const text_str = language_text.getStringPtr(
                    koyomi.date_list[i].calendar_name_key,
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
                        }, koyomi.date_list[i].date);
                    if (date_day <= 0 || date_month <= 0) break;

                    if (text_str != nullptr) (*one_font).drawTopRight(*text_str,
                        paxg::Vec2i(static_cast<int>(ui_layout.koyomi_font_en_x), static_cast<int>(ui_layout.koyomi_font_en_y + i * (koyomi_font_size * 4 / 3))), paxg::Color(0, 0, 0));

                    (*one_font).drawTopRight(",", paxg::Vec2i(static_cast<int>(int(95 * koyomi_font_size / 30.0) + ui_layout.koyomi_font_en_x), static_cast<int>(ui_layout.koyomi_font_en_y + i * (koyomi_font_size * 4 / 3))), paxg::Color(0, 0, 0));
                    (*one_font).drawTopRight(",", paxg::Vec2i(static_cast<int>(int(235 * koyomi_font_size / 30.0) + ui_layout.koyomi_font_en_x), static_cast<int>(ui_layout.koyomi_font_en_y + i * (koyomi_font_size * 4 / 3))), paxg::Color(0, 0, 0));
                    (*one_font).drawTopRight("th", paxg::Vec2i(static_cast<int>(int(315 * koyomi_font_size / 30.0) + ui_layout.koyomi_font_en_x), static_cast<int>(ui_layout.koyomi_font_en_y + i * (koyomi_font_size * 4 / 3))), paxg::Color(0, 0, 0));

                    (*one_font).drawTopRight(std::to_string(date_year), paxg::Vec2i(static_cast<int>(int(en_cal_name_pos_x * koyomi_font_size / 30.0) + ui_layout.koyomi_font_en_x), static_cast<int>(ui_layout.koyomi_font_en_y + i * (koyomi_font_size * 4 / 3))), paxg::Color(0, 0, 0));
                    (*one_font).drawTopRight(std::string(koyomi.month_name[date_month]), paxg::Vec2i(static_cast<int>(int(220 * koyomi_font_size / 30.0) + ui_layout.koyomi_font_en_x), static_cast<int>(ui_layout.koyomi_font_en_y + i * (koyomi_font_size * 4 / 3))), paxg::Color(0, 0, 0));
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
                    }, koyomi.date_list[i].date);
                (*one_font).drawTopRight(std::to_string(date_day), paxg::Vec2i(static_cast<int>(int(315 * koyomi_font_size / 30.0) + ui_layout.koyomi_font_en_x), static_cast<int>(ui_layout.koyomi_font_en_y + i * (koyomi_font_size * 4 / 3))), paxg::Color(0, 0, 0));
                break;
                default:
                    break;
                }
            }
        }

    private:
        // IUIWidget用の状態管理
        bool visible_ = true;
        bool enabled_ = true;
        paxg::Vec2i pos_{0, 0};

        // 描画に必要な参照（render呼び出し時に設定される）
        const paxs::Koyomi* koyomi_ = nullptr;
        const paxs::CalendarUILayout* ui_layout_ = nullptr;
        int koyomi_font_size_ = 22;
        int koyomi_font_buffer_thickness_size_ = 3;
        const SelectLanguage* select_language_ = nullptr;
        const paxs::Language* language_text_ = nullptr;
        bool is_simulator_active_ = false;

    public:
        // IUIWidget インターフェースの実装
        void update(paxs::TouchStateManager& tm) override {
            // CalendarRendererは入力処理を行わないため、空実装
            (void)tm;
        }

        void draw() override {
            if (!visible_ || !koyomi_ || !ui_layout_ || !select_language_ || !language_text_) return;

            // 保存された引数でrender()を呼び出し
            render(*koyomi_, *ui_layout_, koyomi_font_size_,
                   koyomi_font_buffer_thickness_size_, *select_language_,
                   *language_text_, is_simulator_active_);
        }

        paxg::Rect getRect() const override {
            if (!ui_layout_) return paxg::Rect{0, 0, 0, 0};
            return paxg::Rect{
                static_cast<float>(pos_.x()),
                static_cast<float>(ui_layout_->koyomi_font_y),
                static_cast<float>(ui_layout_->koyomi_font_en_x + 400),
                static_cast<float>(koyomi_font_size_ * 4 * 7) // 7行分
            };
        }

        void setPos(const paxg::Vec2i& pos) override {
            pos_ = pos;
        }

        void setVisible(bool visible) override { visible_ = visible; }
        bool isVisible() const override { return visible_; }

        // setEnabled/isEnabledはIViewerComponentとIUIWidgetの両方で共有
        void setEnabled(bool enabled) override { enabled_ = enabled; }
        bool isEnabled() const override { return enabled_; }

        // CalendarRenderer固有の参照設定メソッド
        void setRenderParams(
            const paxs::Koyomi& koyomi,
            const paxs::CalendarUILayout& ui_layout,
            int koyomi_font_size,
            int koyomi_font_buffer_thickness_size,
            const SelectLanguage& select_language,
            const paxs::Language& language_text,
            bool is_simulator_active
        ) {
            koyomi_ = &koyomi;
            ui_layout_ = &ui_layout;
            koyomi_font_size_ = koyomi_font_size;
            koyomi_font_buffer_thickness_size_ = koyomi_font_buffer_thickness_size;
            select_language_ = &select_language;
            language_text_ = &language_text;
            is_simulator_active_ = is_simulator_active;
        }
    };

}

#endif // !PAX_MAHOROBA_CALENDAR_RENDERER_HPP
