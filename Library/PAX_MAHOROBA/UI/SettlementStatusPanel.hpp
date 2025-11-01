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
#include <PAX_GRAPHICA/RenderTexture.hpp>
#include <PAX_GRAPHICA/RoundRect.hpp>
#include <PAX_GRAPHICA/Vec2.hpp>

#include <PAX_MAHOROBA/UI/IUIWidget.hpp>
#include <PAX_MAHOROBA/UI/PanelBackground.hpp>

namespace paxs {

    /// @brief Settlement 表示モードステータスパネル
    /// @brief Settlement display mode status panel
    ///
    /// Settlement の表示モード（人口、農耕文化、mtDNA等）を表示するUIパネル。
    /// UI panel that displays the current Settlement display mode (population, farming, mtDNA, etc.).
    class SettlementStatusPanel : public IUIWidget {
    public:
        SettlementStatusPanel() = default;

        /// @brief 影用のテクスチャを設定
        /// @brief Set textures for shadow rendering
        void setShadowTextures(paxg::RenderTexture& shadow_tex, paxg::RenderTexture& internal_tex) {
            background_.setShadowTextures(shadow_tex, internal_tex);
        }

        // IRenderable の実装
        void render() override {
            if (!visible_) return;

            constexpr int start_x = 40;  // 背景端の左上の X 座標
            constexpr int start_y = 80;  // 背景端の左上の Y 座標
            constexpr int font_space = 20;  // 文字端から背景端までの幅

            std::string text = getStatusText(select_draw_);

            const int text_width = select_font_.width(text);
            const int text_height = select_font_.height();

            // 背景を描画
            background_.draw(
                start_x, start_y,
                text_width + font_space * 2,
                text_height + font_space * 2,
                10,  // corner_radius
                paxg::Color{255, 255, 255}  // bg_color
            );

            // テキストを描画
            select_font_.draw(text, paxg::Vec2i{ start_x + font_space, start_y + font_space },
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

        // IUIWidget の実装
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
        paxg::Font select_font_{ 30, "", 3 };  // 選択肢を表示するフォント
        PanelBackground background_;  // 背景と影の描画

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
