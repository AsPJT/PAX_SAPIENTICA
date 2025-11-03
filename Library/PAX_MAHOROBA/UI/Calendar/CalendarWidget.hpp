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

#include <PAX_MAHOROBA/UI/UILayout.hpp>
#include <PAX_MAHOROBA/Rendering/IWidget.hpp>
#include <PAX_MAHOROBA/Rendering/LanguageFonts.hpp>

#include <PAX_SAPIENTICA/Calendar/Date.hpp>
#include <PAX_SAPIENTICA/Calendar/Koyomi.hpp>
#include <PAX_SAPIENTICA/Language.hpp>
#include <PAX_SAPIENTICA/MurMur3.hpp>

namespace paxs {

    /// @brief カレンダー表示ウィジェット
    /// @brief Calendar display widget
    ///
    /// カレンダー情報の描画を担当します。
    /// Handles rendering of calendar information.
    class CalendarWidget : public IWidget {
    public:
        // 初期化（LanguageFontsへの参照を設定）
        void init(paxs::LanguageFonts& fonts) {
            language_fonts_ = &fonts;
        }

        // IWidget インターフェースの実装（コンポーネント情報）
        const char* getName() const override {
            return "CalendarWidget";
        }

        /// @brief レンダリングレイヤーを取得
        /// @brief Get rendering layer
        RenderLayer getLayer() const override {
            return RenderLayer::UIContent;
        }

        bool isAvailable() const override {
            return true; // カレンダー表示は常に利用可能
        }

    private:
        paxs::LanguageFonts* language_fonts_ = nullptr;

        // カレンダーを描画（言語に応じて自動選択）
        void renderInternal() const {
            if (!koyomi_ || !ui_layout_ || !select_language_ || !language_text_) return;

            // 日本語・中国語・台湾語の場合はアジア式カレンダー
            if (select_language_->cgetKey() == MurMur3::calcHash("ja-JP")
                || select_language_->cgetKey() == MurMur3::calcHash("zh-TW")
                || select_language_->cgetKey() == MurMur3::calcHash("zh-CN")) {
                renderAsianCalendar();
            }
            else {
                // その他の言語は西洋式カレンダー
                renderWesternCalendar();
            }
        }

        // 日本語・中国語のカレンダーを描画
        void renderAsianCalendar() const {
            if (language_fonts_ == nullptr) return;

            for (std::size_t i = 0; i < koyomi_->date_list.size(); ++i) {
                cal::DateOutputType output_type = cal::DateOutputType::name_and_value;
                std::visit([&](const auto& x) { output_type = x.getDateOutputType(); }, koyomi_->date_list[i].date);

                int date_year = 0;
                int date_month = 0;
                int date_day = 0;
                bool date_lm = false;

                // 暦の読み方を返す
                const std::string* const text_str = language_text_->getStringPtr(
                    koyomi_->date_list[i].calendar_name_key,
                    select_language_->cgetKey()
                );
                if (text_str == nullptr) continue;

                // 暦描画フォントを指定
                paxg::Font* one_font = language_fonts_->getAndAdd(select_language_->cgetKey(), static_cast<std::uint_least8_t>(paxg::FontConfig::KOYOMI_FONT_SIZE), static_cast<std::uint_least8_t>(paxg::FontConfig::KOYOMI_FONT_BUFFER_THICKNESS));
                if (one_font == nullptr) continue;

                switch (output_type) {
                case paxs::cal::DateOutputType::name_and_ymd:
                    std::visit([&](const auto& x) {
                        date_year = int(x.cgetYear());
                        date_month = int(x.cgetMonth());
                        date_day = int(x.cgetDay());
                        date_lm = x.isLeapMonth();
                        }, koyomi_->date_list[i].date);
                    if (date_day <= 0 || date_month <= 0) break;

                    (*one_font).drawTopRight(*text_str,
                        paxg::Vec2i(static_cast<int>(ui_layout_->koyomi_font_x), static_cast<int>(ui_layout_->koyomi_font_y + i * (paxg::FontConfig::KOYOMI_FONT_SIZE * 4 / 3))), paxg::Color(0, 0, 0));
                    (*one_font).drawTopRight(reinterpret_cast<const char*>(u8"年"), paxg::Vec2i(static_cast<int>(int(120 * paxg::FontConfig::KOYOMI_FONT_SIZE / 30.0) + ui_layout_->koyomi_font_x), static_cast<int>(ui_layout_->koyomi_font_y + i * (paxg::FontConfig::KOYOMI_FONT_SIZE * 4 / 3))), paxg::Color(0, 0, 0));
                    (*one_font).drawTopRight(reinterpret_cast<const char*>(u8"月"), paxg::Vec2i(static_cast<int>(int(220 * paxg::FontConfig::KOYOMI_FONT_SIZE / 30.0) + ui_layout_->koyomi_font_x), static_cast<int>(ui_layout_->koyomi_font_y + i * (paxg::FontConfig::KOYOMI_FONT_SIZE * 4 / 3))), paxg::Color(0, 0, 0));
                    (*one_font).drawTopRight(reinterpret_cast<const char*>(u8"日"), paxg::Vec2i(static_cast<int>(int(300 * paxg::FontConfig::KOYOMI_FONT_SIZE / 30.0) + ui_layout_->koyomi_font_x), static_cast<int>(ui_layout_->koyomi_font_y + i * (paxg::FontConfig::KOYOMI_FONT_SIZE * 4 / 3))), paxg::Color(0, 0, 0));

                    (*one_font).drawTopRight(std::to_string(date_year), paxg::Vec2i(static_cast<int>(int(85 * paxg::FontConfig::KOYOMI_FONT_SIZE / 30.0) + ui_layout_->koyomi_font_x), static_cast<int>(ui_layout_->koyomi_font_y + i * (paxg::FontConfig::KOYOMI_FONT_SIZE * 4 / 3))), paxg::Color(0, 0, 0));
                    (*one_font).drawTopRight(std::to_string(date_month), paxg::Vec2i(static_cast<int>(int(190 * paxg::FontConfig::KOYOMI_FONT_SIZE / 30.0) + ui_layout_->koyomi_font_x), static_cast<int>(ui_layout_->koyomi_font_y + i * (paxg::FontConfig::KOYOMI_FONT_SIZE * 4 / 3))), paxg::Color(0, 0, 0));
                    (*one_font).drawTopRight(std::to_string(date_day), paxg::Vec2i(static_cast<int>(int(270 * paxg::FontConfig::KOYOMI_FONT_SIZE / 30.0) + ui_layout_->koyomi_font_x), static_cast<int>(ui_layout_->koyomi_font_y + i * (paxg::FontConfig::KOYOMI_FONT_SIZE * 4 / 3))), paxg::Color(0, 0, 0));
                    if (date_lm) {
                        (*one_font).drawTopRight(reinterpret_cast<const char*>(u8"閏"), paxg::Vec2i(static_cast<int>((
                            (date_month < 10) ? int(167 * paxg::FontConfig::KOYOMI_FONT_SIZE / 30.0) + ui_layout_->koyomi_font_x : int(152 * paxg::FontConfig::KOYOMI_FONT_SIZE / 30.0) + ui_layout_->koyomi_font_x
                            )), static_cast<int>(ui_layout_->koyomi_font_y + i * (paxg::FontConfig::KOYOMI_FONT_SIZE * 4 / 3))), paxg::Color(0, 0, 0));
                    }
                    break;
                case paxs::cal::DateOutputType::name_and_value:
                    (*one_font).drawTopRight(*text_str,
                        paxg::Vec2i(static_cast<int>(ui_layout_->koyomi_font_x), static_cast<int>(ui_layout_->koyomi_font_y + i * (paxg::FontConfig::KOYOMI_FONT_SIZE * 4 / 3))), paxg::Color(0, 0, 0));
                    std::visit([&](const auto& x) {
                        date_day = int(x.cgetDay());
                        }, koyomi_->date_list[i].date);
                    (*one_font).drawTopRight(std::to_string(date_day), paxg::Vec2i(static_cast<int>(int(300 * paxg::FontConfig::KOYOMI_FONT_SIZE / 30.0) + ui_layout_->koyomi_font_x), static_cast<int>(ui_layout_->koyomi_font_y + i * (paxg::FontConfig::KOYOMI_FONT_SIZE * 4 / 3))), paxg::Color(0, 0, 0));
                    break;
                default:
                    break;
                }
            }
        }

        // 英語のカレンダーを描画
        void renderWesternCalendar() const {
            if (language_fonts_ == nullptr) return;

            for (std::size_t i = 0; i < koyomi_->date_list.size(); ++i) {
                cal::DateOutputType output_type = cal::DateOutputType::name_and_value;
                std::visit([&](const auto& x) { output_type = x.getDateOutputType(); }, koyomi_->date_list[i].date);

                int date_year = 0;
                int date_month = 0;
                int date_day = 0;
                bool date_lm = false;
                const int en_cal_name_pos_x = 85;

                // 暦描画フォントを指定
                paxg::Font* one_font = language_fonts_->getAndAdd(select_language_->cgetKey(), static_cast<std::uint_least8_t>(paxg::FontConfig::KOYOMI_FONT_SIZE), static_cast<std::uint_least8_t>(paxg::FontConfig::KOYOMI_FONT_BUFFER_THICKNESS));
                if (one_font == nullptr) continue;

                // 暦の読み方を返す
                const std::string* const text_str = language_text_->getStringPtr(
                    koyomi_->date_list[i].calendar_name_key,
                    select_language_->cgetKey()
                );
                if (text_str == nullptr) continue;

                switch (output_type) {
                case paxs::cal::DateOutputType::name_and_ymd:
                    std::visit([&](const auto& x) {
                        date_year = int(x.cgetYear());
                        date_month = int(x.cgetMonth());
                        date_day = int(x.cgetDay());
                        date_lm = x.isLeapMonth();
                        }, koyomi_->date_list[i].date);
                    if (date_day <= 0 || date_month <= 0) break;

                    if (text_str != nullptr) (*one_font).drawTopRight(*text_str,
                        paxg::Vec2i(static_cast<int>(ui_layout_->koyomi_font_en_x), static_cast<int>(ui_layout_->koyomi_font_en_y + i * (paxg::FontConfig::KOYOMI_FONT_SIZE * 4 / 3))), paxg::Color(0, 0, 0));

                    (*one_font).drawTopRight(",", paxg::Vec2i(static_cast<int>(int(95 * paxg::FontConfig::KOYOMI_FONT_SIZE / 30.0) + ui_layout_->koyomi_font_en_x), static_cast<int>(ui_layout_->koyomi_font_en_y + i * (paxg::FontConfig::KOYOMI_FONT_SIZE * 4 / 3))), paxg::Color(0, 0, 0));
                    (*one_font).drawTopRight(",", paxg::Vec2i(static_cast<int>(int(235 * paxg::FontConfig::KOYOMI_FONT_SIZE / 30.0) + ui_layout_->koyomi_font_en_x), static_cast<int>(ui_layout_->koyomi_font_en_y + i * (paxg::FontConfig::KOYOMI_FONT_SIZE * 4 / 3))), paxg::Color(0, 0, 0));
                    (*one_font).drawTopRight("th", paxg::Vec2i(static_cast<int>(int(315 * paxg::FontConfig::KOYOMI_FONT_SIZE / 30.0) + ui_layout_->koyomi_font_en_x), static_cast<int>(ui_layout_->koyomi_font_en_y + i * (paxg::FontConfig::KOYOMI_FONT_SIZE * 4 / 3))), paxg::Color(0, 0, 0));

                    (*one_font).drawTopRight(std::to_string(date_year), paxg::Vec2i(static_cast<int>(int(en_cal_name_pos_x * paxg::FontConfig::KOYOMI_FONT_SIZE / 30.0) + ui_layout_->koyomi_font_en_x), static_cast<int>(ui_layout_->koyomi_font_en_y + i * (paxg::FontConfig::KOYOMI_FONT_SIZE * 4 / 3))), paxg::Color(0, 0, 0));
                    (*one_font).drawTopRight(std::string(koyomi_->month_name[date_month]), paxg::Vec2i(static_cast<int>(int(220 * paxg::FontConfig::KOYOMI_FONT_SIZE / 30.0) + ui_layout_->koyomi_font_en_x), static_cast<int>(ui_layout_->koyomi_font_en_y + i * (paxg::FontConfig::KOYOMI_FONT_SIZE * 4 / 3))), paxg::Color(0, 0, 0));
                    (*one_font).drawTopRight(std::to_string(date_day), paxg::Vec2i(static_cast<int>(int(280 * paxg::FontConfig::KOYOMI_FONT_SIZE / 30.0) + ui_layout_->koyomi_font_en_x), static_cast<int>(ui_layout_->koyomi_font_en_y + i * (paxg::FontConfig::KOYOMI_FONT_SIZE * 4 / 3))), paxg::Color(0, 0, 0));

                    if (date_lm) {
                        (*one_font).drawTopRight("int.", paxg::Vec2i(static_cast<int>((
                            int(152 * paxg::FontConfig::KOYOMI_FONT_SIZE / 30.0) + ui_layout_->koyomi_font_en_x
                            )), static_cast<int>(ui_layout_->koyomi_font_en_y + i * (paxg::FontConfig::KOYOMI_FONT_SIZE * 4 / 3))), paxg::Color(0, 0, 0));
                    }
                    break;
                case paxs::cal::DateOutputType::name_and_value:
                {
                    if (text_str != nullptr) (*one_font).drawTopRight(*text_str,
                        paxg::Vec2i(static_cast<int>(ui_layout_->koyomi_font_en_x), static_cast<int>(ui_layout_->koyomi_font_en_y + i * (paxg::FontConfig::KOYOMI_FONT_SIZE * 4 / 3))), paxg::Color(0, 0, 0));
                }
                std::visit([&](const auto& x) {
                    date_day = int(x.cgetDay());
                    }, koyomi_->date_list[i].date);
                (*one_font).drawTopRight(std::to_string(date_day), paxg::Vec2i(static_cast<int>(int(315 * paxg::FontConfig::KOYOMI_FONT_SIZE / 30.0) + ui_layout_->koyomi_font_en_x), static_cast<int>(ui_layout_->koyomi_font_en_y + i * (paxg::FontConfig::KOYOMI_FONT_SIZE * 4 / 3))), paxg::Color(0, 0, 0));
                break;
                default:
                    break;
                }
            }
        }

    private:
        // IWidget用の状態管理
        bool visible_ = true;
        bool enabled_ = true;
        paxg::Vec2i pos_{0, 0};

        // 描画に必要な参照（render呼び出し時に設定される）
        const paxs::Koyomi* koyomi_ = nullptr;
        const paxs::UILayout* ui_layout_ = nullptr;
        const SelectLanguage* select_language_ = nullptr;
        const paxs::Language* language_text_ = nullptr;

    public:
        // IWidget インターフェースの実装
        bool handleInput(const InputEvent& event) override {
            // CalendarWidgetは入力処理を行わない
            (void)event;
            return false;
        }

        void render() const override {
            if (!visible_) return;

            // メンバー変数を使用してrenderInternal()を呼び出し
            renderInternal();
        }

        paxg::Rect getRect() const override {
            if (!ui_layout_) return paxg::Rect{0, 0, 0, 0};
            return paxg::Rect{
                static_cast<float>(pos_.x()),
                static_cast<float>(ui_layout_->koyomi_font_y),
                static_cast<float>(ui_layout_->koyomi_font_en_x + 400),
                static_cast<float>(paxg::FontConfig::KOYOMI_FONT_SIZE * 4 * 7) // 7行分
            };
        }

        void setPos(const paxg::Vec2i& pos) override {
            pos_ = pos;
        }

        void setVisible(bool visible) override { visible_ = visible; }
        bool isVisible() const override { return visible_; }

        // setEnabled/isEnabledは下部で実装済み
        void setEnabled(bool enabled) override { enabled_ = enabled; }
        bool isEnabled() const override { return enabled_; }

        // CalendarWidget固有の参照設定メソッド
        void setRenderParams(
            const paxs::Koyomi& koyomi,
            const paxs::UILayout& ui_layout,
            const SelectLanguage& select_language,
            const paxs::Language& language_text
        ) {
            koyomi_ = &koyomi;
            ui_layout_ = &ui_layout;
            select_language_ = &select_language;
            language_text_ = &language_text;
        }
    };

}

#endif // !PAX_MAHOROBA_CALENDAR_WIDGET_HPP
