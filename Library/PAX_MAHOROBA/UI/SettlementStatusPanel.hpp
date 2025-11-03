/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_UI_SETTLEMENT_STATUS_PANEL_HPP
#define PAX_MAHOROBA_UI_SETTLEMENT_STATUS_PANEL_HPP

#include <cstddef>
#include <string>

#include <PAX_GRAPHICA/Font.hpp>
#include <PAX_GRAPHICA/Rect.hpp>
#include <PAX_GRAPHICA/RoundRect.hpp>
#include <PAX_GRAPHICA/Vec2.hpp>

#include <PAX_MAHOROBA/Rendering/IWidget.hpp>
#include <PAX_MAHOROBA/Rendering/LanguageFonts.hpp>
#include <PAX_MAHOROBA/UI/PanelBackground.hpp>

#include <PAX_SAPIENTICA/Language.hpp>

namespace paxs {

    /// @brief Settlement 表示モードステータスパネル
    /// @brief Settlement display mode status panel
    ///
    /// Settlement の表示モード（人口、農耕文化、mtDNA等）を表示するUIパネル。
    /// UI panel that displays the current Settlement display mode (population, farming, mtDNA, etc.).
    class SettlementStatusPanel : public IWidget {
    public:
        SettlementStatusPanel() = default;

        /// @brief 初期化（LanguageFontsへの参照を設定）
        /// @brief Initialize (set reference to LanguageFonts)
        void init(paxs::LanguageFonts& fonts, const SelectLanguage& select_language) {
            language_fonts_ = &fonts;
            select_language_ = &select_language;
        }

        // IRenderable の実装
        void render() const override {
            if (!visible_ || language_fonts_ == nullptr || select_language_ == nullptr) return;

            constexpr int start_x = 40;  // 背景端の左上の X 座標
            constexpr int start_y = 80;  // 背景端の左上の Y 座標
            constexpr int font_space = 20;  // 文字端から背景端までの幅

            const std::string text = getStatusText(select_draw_);

            // フォントを取得
            paxg::Font* font = language_fonts_->getAndAdd(
                select_language_->cgetKey(),
                static_cast<std::uint_least8_t>(paxg::FontConfig::KOYOMI_FONT_SIZE),
                static_cast<std::uint_least8_t>(paxg::FontConfig::KOYOMI_FONT_BUFFER_THICKNESS)
            );
            if (font == nullptr) return;

            // テキストを描画
            font->draw(text, paxg::Vec2i{ start_x + font_space, start_y + font_space },
                      paxg::Color{ 0, 0, 0 });
        }

        RenderLayer getLayer() const override {
            return RenderLayer::UIContent;
        }

        bool isVisible() const override {
            return visible_;
        }

        void setVisible(bool visible) override {
            visible_ = visible;
        }

        // IInputHandler の実装
        bool handleInput(const InputEvent& event) override {
            // このウィジェットは入力を処理しない（表示のみ）
            (void)event;
            return false;
        }

        bool isEnabled() const override {
            return enabled_;
        }

        // IWidget の実装
        paxg::Rect getRect() const override {
            constexpr int start_x = 40;
            constexpr int start_y = 80;
            return paxg::Rect{ start_x, start_y, 300, 60 };  // 概算サイズ
        }

        void setPos(const paxg::Vec2i& pos) override {
            pos_ = pos;
        }

        void setEnabled(bool enabled) override {
            enabled_ = enabled;
        }

        const char* getName() const override {
            return "SettlementStatusPanel";
        }

        bool isAvailable() const override {
            return true;
        }

        /// @brief 表示モードを設定
        /// @brief Set display mode
        /// @param select_draw 表示モード (1-6)
        void setSelectDraw(std::size_t select_draw) {
            select_draw_ = select_draw;
        }

    private:
        std::size_t select_draw_ = 1;  // 表示モード (1-6)
        bool visible_ = false;  // シミュレーション初期化後に表示 / Show after simulation init
        bool enabled_ = true;
        paxg::Vec2i pos_{ 0, 0 };

        // フォント管理
        paxs::LanguageFonts* language_fonts_ = nullptr;
        const SelectLanguage* select_language_ = nullptr;

        /// @brief 表示モードに応じたテキストを取得
        /// @brief Get text according to display mode
        /// @param select_draw 表示モード (1-6)
        /// @return 表示するテキスト / Text to display
        std::string getStatusText(std::size_t select_draw) const {
            switch (select_draw) {
                case 1:
                    return reinterpret_cast<const char*>(u8"1. 人口 Population");
                case 2:
                    return reinterpret_cast<const char*>(u8"2. 農耕文化 Farming");
                case 3:
                    return reinterpret_cast<const char*>(u8"3. mtDNA haplogroup");
                case 4:
                    return reinterpret_cast<const char*>(u8"4. SNP / Genome");
                case 5:
                    return reinterpret_cast<const char*>(u8"5. 言語 Language");
                case 6:
                    return reinterpret_cast<const char*>(u8"6. 青銅 Bronze");
                default:
                    return "";
            }
        }
    };

} // namespace paxs

#endif // !PAX_MAHOROBA_UI_SETTLEMENT_STATUS_PANEL_HPP
