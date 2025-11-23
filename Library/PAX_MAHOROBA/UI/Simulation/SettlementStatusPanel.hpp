/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_UI_SETTLEMENT_STATUS_PANEL_HPP
#define PAX_MAHOROBA_UI_SETTLEMENT_STATUS_PANEL_HPP

#include <PAX_GRAPHICA/Rect.hpp>
#include <PAX_GRAPHICA/Vec2.hpp>

#include <PAX_MAHOROBA/Rendering/FontSystem.hpp>
#include <PAX_MAHOROBA/Rendering/UIComponent.hpp>

#include <PAX_SAPIENTICA/System/FeatureVisibilityManager.hpp>
#include <PAX_SAPIENTICA/Utility/Logger.hpp>
#include <PAX_SAPIENTICA/Utility/MurMur3.hpp>

namespace paxs {

    /// @brief Settlement 表示モードステータスパネル
    /// @brief Settlement display mode status panel
    ///
    /// Settlement の表示モード（人口、農耕文化、mtDNA等）を表示するUIパネル。
    class SettlementStatusPanel : public UIComponent {
    private:
        // SimulationStats domain hash
        static constexpr std::uint_least32_t simulation_stats_domain_hash = MurMur3::calcHash("SimulationStats");
        static constexpr std::uint_least32_t display_mode_population_key = MurMur3::calcHash("display_mode_population");
        static constexpr std::uint_least32_t display_mode_farming_key = MurMur3::calcHash("display_mode_farming");
        static constexpr std::uint_least32_t display_mode_mtdna_key = MurMur3::calcHash("display_mode_mtdna");
        static constexpr std::uint_least32_t display_mode_snp_key = MurMur3::calcHash("display_mode_snp");
        static constexpr std::uint_least32_t display_mode_language_key = MurMur3::calcHash("display_mode_language");
        static constexpr std::uint_least32_t display_mode_bronze_key = MurMur3::calcHash("display_mode_bronze");

    public:
        SettlementStatusPanel(const paxs::FeatureVisibilityManager& visible_manager)
            : visible_manager_(visible_manager) {}

        void render() const override {
            if (!isVisible()) {
                return;
            }

            const std::string text = getStatusText();

            // フォントを取得
            paxg::Font* font = Fonts().getFont(
                Fonts().getSelectedLanguageKey(),
                static_cast<std::uint_least8_t>(paxg::FontConfig::KOYOMI_FONT_SIZE),
                static_cast<std::uint_least8_t>(paxg::FontConfig::KOYOMI_FONT_BUFFER_THICKNESS)
            );
            if (font == nullptr) {
                PAXS_WARNING("SettlementStatusPanel::render: Font not found.");
                return;
            }

            // テキストを描画
            font->draw(text,  start_pos + font_space,
                      paxg::Color{ 0, 0, 0 });
        }

        /// @brief 表示モードを設定
        /// @brief Set display mode
        /// @param select_draw 表示モード (1-6)
        void setSelectDraw(std::size_t select_draw) {
            select_draw_ = select_draw;
        }

        Rect<int> getRect() const override {
            return { pos_.x, pos_.y,
                300, 60  // 幅と高さを固定
            };
        }

        void setVisible(bool visible) { visible_ = visible; }
        bool isVisible() const override { return visible_ && visible_manager_.isVisible(ViewMenu::simulation); }
        void setPos(const Vector2<int>& pos) override { pos_ = pos; }
        const char* getName() const override { return "SettlementStatusPanel"; }
        RenderLayer getLayer() const override { return RenderLayer::UIContent; }

    private:
        static constexpr Vector2<int> start_pos{ 40, 80 };  // 背景端の左上座標
        static constexpr Vector2<int> font_space{ 20, 18 };  // 文字端から背景端までの幅と高さ

        std::size_t select_draw_ = 1;  // 表示モード (1-6)
        bool visible_ = false;  // シミュレーション初期化後に表示
        paxs::Vector2<int> pos_{ start_pos };

        const paxs::FeatureVisibilityManager& visible_manager_;

        /// @brief 表示モードに応じたテキストを取得
        /// @brief Get text according to display mode
        /// @param select_draw 表示モード (1-6)
        /// @return 表示するテキスト / Text to display
        std::string getStatusText() const {
            const std::string* text = nullptr;
            std::uint_least32_t key = 0;

            switch (select_draw_) {
                case 1:
                    key = display_mode_population_key;
                    break;
                case 2:
                    key = display_mode_farming_key;
                    break;
                case 3:
                    key = display_mode_mtdna_key;
                    break;
                case 4:
                    key = display_mode_snp_key;
                    break;
                case 5:
                    key = display_mode_language_key;
                    break;
                case 6:
                    key = display_mode_bronze_key;
                    break;
                default:
                    return "";
            }

            text = Fonts().getLocalesText(simulation_stats_domain_hash, key);
            if (text == nullptr) {
                PAXS_WARNING("[SettlementStatusPanel] Missing text for display mode " + std::to_string(select_draw_));
                return "";
            }
            return *text;
        }
    };

} // namespace paxs

#endif // !PAX_MAHOROBA_UI_SETTLEMENT_STATUS_PANEL_HPP
