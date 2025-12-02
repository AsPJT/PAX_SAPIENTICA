/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_UI_MENU_BAR_TOGGLE_BUTTON_HPP
#define PAX_MAHOROBA_UI_MENU_BAR_TOGGLE_BUTTON_HPP

#include <PAX_GRAPHICA/ToggleButton.hpp>
#include <PAX_MAHOROBA/Rendering/InteractiveUIComponent.hpp>
#include <PAX_SAPIENTICA/Key/MenuBarKeys.hpp>
#include <PAX_SAPIENTICA/System/ApplicationEvents.hpp>
#include <PAX_SAPIENTICA/System/EventBus.hpp>
#include <PAX_SAPIENTICA/Utility/MurMur3.hpp>

namespace paxs {

    /// @brief メニューバーのTerritory表示切替トグルボタン
    /// @brief Toggle button for switching Territory display in menu bar
    class MenuBarToggleButton : public InteractiveUIComponent {
    private:
        paxg::ToggleButton toggle_button_;
        static constexpr int BUTTON_HEIGHT = 24;
        static constexpr int BUTTON_WIDTH = BUTTON_HEIGHT * 2;  // 幅は高さの2倍

    public:
        MenuBarToggleButton() = default;

        /// @brief Initialize toggle button position
        /// @brief トグルボタンの位置を初期化
        /// @param github_button_x GitHubボタンのX座標
        /// @param menu_bar_height メニューバーの高さ
        void init(int github_button_x, int menu_bar_height) {
            const int margin = 8;
            const int x = github_button_x - BUTTON_WIDTH - margin;
            const int y = (menu_bar_height - BUTTON_HEIGHT) / 2;

            toggle_button_.setPosition(static_cast<float>(x), static_cast<float>(y));
            toggle_button_.setSize(static_cast<float>(BUTTON_WIDTH), static_cast<float>(BUTTON_HEIGHT));

            // デフォルトはON（Territory表示）
            toggle_button_.setOn(true);
        }

        void render() const override {
            if (!isVisible()) return;
            toggle_button_.draw();
        }

        /// @brief Update animation state
        /// @brief アニメーション状態を更新
        void update() {
            toggle_button_.update();
        }

        Rect<int> getRect() const override {
            return toggle_button_.getRect().toType<int>();
        }

        const char* getName() const override { return "MenuBarToggleButton"; }
        bool isVisible() const override { return true; }
        void setPos(const Vector2<int>& pos) override {
            toggle_button_.setPosition(static_cast<float>(pos.x), static_cast<float>(pos.y));
        }

        EventHandlingResult handleEvent(const MouseEvent& event) override {
            if (!isVisible()) {
                return EventHandlingResult::Handled();
            }

            // クリックを押した瞬間（Pressed）のみトグルを切り替える
            if (event.left_button_state == MouseButtonState::Pressed) {

                toggle_button_.toggle();

                // TODO: 処理を削除
                // Publish feature visibility change event for Territory (map_line1)
                paxs::EventBus::getInstance().publish(
                    FeatureVisibilityChangeCommandEvent(
                        static_cast<std::uint_least32_t>(paxs::MapLayersMenu::line1),
                        toggle_button_.isOn()
                    )
                );

                return EventHandlingResult::Handled();
            }

            return EventHandlingResult::Handled();
        }

        RenderLayer getLayer() const override {
            return RenderLayer::MenuBar;
        }
    };

} // namespace paxs

#endif // !PAX_MAHOROBA_UI_MENU_BAR_TOGGLE_BUTTON_HPP
