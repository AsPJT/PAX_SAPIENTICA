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
#include <PAX_MAHOROBA/Rendering/FontSystem.hpp>
#include <PAX_MAHOROBA/UI/PanelBackground.hpp>

#include <PAX_SAPIENTICA/FeatureVisibilityManager.hpp>

namespace paxs {

    /// @brief Settlement 表示モードステータスパネル
    /// @brief Settlement display mode status panel
    ///
    /// Settlement の表示モード（人口、農耕文化、mtDNA等）を表示するUIパネル。
    /// UI panel that displays the current Settlement display mode (population, farming, mtDNA, etc.).
    class SettlementStatusPanel : public IWidget {
    public:
        SettlementStatusPanel(
            const paxs::FeatureVisibilityManager* visible_manager)
            : visible_manager_ptr(visible_manager) {}

        /// @brief 初期化
        /// @brief Initialize
        void init() {
            // FontSystemを使用するため何もしない
        }

        // IRenderable の実装
        void render() const override {
            if (!visible_) return;

            constexpr int start_x = 40;  // 背景端の左上の X 座標
            constexpr int start_y = 80;  // 背景端の左上の Y 座標
            constexpr int font_space = 20;  // 文字端から背景端までの幅

            const std::string text = getStatusText(select_draw_);

            // フォントを取得
            paxg::Font* font = Fonts().getFont(
                Fonts().getSelectedLanguage().cgetKey(),
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

        /// @brief 表示モードを設定
        /// @brief Set display mode
        /// @param select_draw 表示モード (1-6)
        void setSelectDraw(std::size_t select_draw) {
            select_draw_ = select_draw;
        }

        const char* getName() const override {
            return "SettlementStatusPanel";
        }

        paxg::Rect getRect() const override {
            return paxg::Rect{
                static_cast<float>(pos_.x()),
                static_cast<float>(pos_.y()),
                300.0f,  // width
                60.0f    // height
            };
        }

        void setPos(const paxg::Vec2i& pos) override {
            pos_ = pos;
        }

        bool isEnabled() const override {
            return visible_;
        }

        void setEnabled(bool enabled) override {
            visible_ = enabled;
        }

        EventHandlingResult handleEvent(const MouseEvent& /*event*/) override {
            // このパネルはマウス入力を処理しない
            return EventHandlingResult::NotHandled();
        }

    private:
        std::size_t select_draw_ = 1;  // 表示モード (1-6)
        bool visible_ = false;  // シミュレーション初期化後に表示
        paxg::Vec2i pos_{ 0, 0 };

        // 可視性管理
        const paxs::FeatureVisibilityManager* visible_manager_ptr = nullptr;

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
