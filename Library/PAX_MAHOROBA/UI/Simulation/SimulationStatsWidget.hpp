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
#include <PAX_MAHOROBA/Rendering/IWidget.hpp>

#include <PAX_SAPIENTICA/Simulation/Manager/SimulationManager.hpp>
#include <PAX_SAPIENTICA/Utility/MurMur3.hpp>

namespace paxs {

    /// @brief シミュレーション統計情報表示ウィジェット
    /// @brief Simulation statistics display widget
    /// @details 人口数と集落数を表示
    class SimulationStatsWidget : public IWidget {
    private:
        const SimulationManager& simulation_manager_;
        paxg::Vec2i pos_{0, 0};

        static constexpr int line_height_ = 25;
        static constexpr int label_width_ = 130;

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
            font->draw(
                (Fonts().getSelectedLanguage().cgetKey() == MurMur3::calcHash("ja-JP")) ?
                    reinterpret_cast<const char*>(u8"人口: ") : "Population: ",
                paxg::Vec2i(pos_.x(), pos_.y() + line_height_ * current_line),
                paxg::Color(0, 0, 0)
            );
            font->draw(
                std::to_string(simulation_manager_.getPopulation()),
                paxg::Vec2i(pos_.x() + label_width_, pos_.y() + line_height_ * current_line++),
                paxg::Color(0, 0, 0)
            );

            // 集落数
            font->draw(
                (Fonts().getSelectedLanguage().cgetKey() == MurMur3::calcHash("ja-JP")) ?
                    reinterpret_cast<const char*>(u8"集落: ") : "Settlements: ",
                paxg::Vec2i(pos_.x(), pos_.y() + line_height_ * current_line),
                paxg::Color(0, 0, 0)
            );
            font->draw(
                std::to_string(simulation_manager_.getSettlementCount()),
                paxg::Vec2i(pos_.x() + label_width_, pos_.y() + line_height_ * current_line++),
                paxg::Color(0, 0, 0)
            );
        }

        EventHandlingResult handleEvent(const MouseEvent& /*event*/) override {
            return EventHandlingResult::NotHandled();
        }

        paxg::Rect getRect() const override {
            return paxg::Rect{
                static_cast<float>(pos_.x()),
                static_cast<float>(pos_.y()),
                static_cast<float>(label_width_ + 100),  // ラベル幅 + 数値表示幅
                static_cast<float>(line_height_ * 2)      // 2行分
            };
        }

        bool isVisible() const override { return true; }
        void setPos(const paxg::Vec2i& pos) override { pos_ = pos; }
        const char* getName() const override { return "SimulationStatsWidget"; }
        RenderLayer getLayer() const override { return RenderLayer::UIContent; }
    };

}

#endif // !PAX_MAHOROBA_UI_SIMULATION_STATS_WIDGET_HPP
