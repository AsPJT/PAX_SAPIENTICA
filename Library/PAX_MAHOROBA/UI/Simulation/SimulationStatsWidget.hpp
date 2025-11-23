/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_UI_SIMULATION_STATS_WIDGET_HPP
#define PAX_MAHOROBA_UI_SIMULATION_STATS_WIDGET_HPP

#include <string>

#include <PAX_GRAPHICA/Color.hpp>
#include <PAX_GRAPHICA/Font.hpp>
#include <PAX_GRAPHICA/Rect.hpp>
#include <PAX_GRAPHICA/Vec2.hpp>

#include <PAX_MAHOROBA/Rendering/FontSystem.hpp>
#include <PAX_MAHOROBA/Rendering/InteractiveUIComponent.hpp>

#include <PAX_SAPIENTICA/Simulation/Manager/SimulationManager.hpp>
#include <PAX_SAPIENTICA/Utility/MurMur3.hpp>

namespace paxs {

    /// @brief シミュレーション統計情報表示ウィジェット
    /// @brief Simulation statistics display widget
    /// @details 人口数、集落数、渡来数を表示
    class SimulationStatsWidget : public InteractiveUIComponent {
    private:
        // SimulationStats domain hash
        static constexpr std::uint_least32_t simulation_stats_domain_hash = MurMur3::calcHash("SimulationStats");
        static constexpr std::uint_least32_t label_population_key = MurMur3::calcHash("label_population");
        static constexpr std::uint_least32_t label_settlements_key = MurMur3::calcHash("label_settlements");
        static constexpr std::uint_least32_t label_migration_key = MurMur3::calcHash("label_migration");

        const SimulationManager& simulation_manager_;
        paxs::Vector2<int> pos_ {0, 0};

        static constexpr int line_height_ = 25;
        static constexpr int label_width_ = 140;

    public:
        SimulationStatsWidget(const SimulationManager& simulation_manager)
            : simulation_manager_(simulation_manager) {}

        void render() const override {
            paxg::Font* font = Fonts().getFont(
                static_cast<std::uint_least8_t>(paxg::FontConfig::KOYOMI_FONT_SIZE),
                static_cast<std::uint_least8_t>(paxg::FontConfig::KOYOMI_FONT_BUFFER_THICKNESS)
            );
            if (font == nullptr) return;

            int current_line = 0;

            // 人口数
            const std::string* population_label = Fonts().getLocalesText(simulation_stats_domain_hash, label_population_key);
            if (population_label != nullptr) {
                font->draw(
                    *population_label + ": ",
                    paxg::Vec2i(pos_.x, pos_.y + (line_height_ * current_line)),
                    paxg::Color(0, 0, 0)
                );
            }
            font->draw(
                std::to_string(simulation_manager_.getPopulation()),
                paxg::Vec2i(pos_.x + label_width_, pos_.y + (line_height_ * current_line++)),
                paxg::Color(0, 0, 0)
            );

            // 集落数
            const std::string* settlements_label = Fonts().getLocalesText(simulation_stats_domain_hash, label_settlements_key);
            if (settlements_label != nullptr) {
                font->draw(
                    *settlements_label + ": ",
                    paxg::Vec2i(pos_.x, pos_.y + (line_height_ * current_line)),
                    paxg::Color(0, 0, 0)
                );
            }
            font->draw(
                std::to_string(simulation_manager_.getSettlementCount()),
                paxg::Vec2i(pos_.x + label_width_, pos_.y + (line_height_ * current_line++)),
                paxg::Color(0, 0, 0)
            );

            // 渡来数
            const std::string* migration_label = Fonts().getLocalesText(simulation_stats_domain_hash, label_migration_key);
            if (migration_label != nullptr) {
                font->draw(
                    *migration_label + ": ",
                    paxg::Vec2i(pos_.x, pos_.y + (line_height_ * current_line)),
                    paxg::Color(0, 0, 0)
                );
            }
            font->draw(
                std::to_string(simulation_manager_.getMigrationCount()),
                paxg::Vec2i(pos_.x + label_width_, pos_.y + (line_height_ * current_line++)),
                paxg::Color(0, 0, 0)
            );
        }

        EventHandlingResult handleEvent(const MouseEvent& /*event*/) override {
            return EventHandlingResult::NotHandled();
        }

        Rect<int> getRect() const override {
            return { pos_.x, pos_.y,
                label_width_ + 100,  // ラベル幅 + 数値表示幅
                line_height_ * 3      // 3行分（人口、集落、渡来）
            };
        }
        bool isVisible() const override { return true; }
        void setPos(const Vector2<int>& pos) override { pos_ = pos; }
        const char* getName() const override { return "SimulationStatsWidget"; }
        RenderLayer getLayer() const override { return RenderLayer::UIContent; }
    };

}

#endif // !PAX_MAHOROBA_UI_SIMULATION_STATS_WIDGET_HPP
