/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com		https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_UI_GITHUB_BUTTON_HPP
#define PAX_MAHOROBA_UI_GITHUB_BUTTON_HPP

#include <PAX_GRAPHICA/Rect.hpp>
#include <PAX_GRAPHICA/System.hpp>
#include <PAX_GRAPHICA/Texture.hpp>
#include <PAX_GRAPHICA/Vec2.hpp>
#include <PAX_GRAPHICA/Window.hpp>

#include <PAX_MAHOROBA/Rendering/IWidget.hpp>
#include <PAX_MAHOROBA/UI/Pulldown.hpp>
#include <PAX_MAHOROBA/UI/Widget/IconButton.hpp>

namespace paxs {

    /// @brief GitHubリポジトリへのリンクボタン
    class GitHubButton : public IconButton {
    private:
        bool enabled_ = true;
        static constexpr int ICON_SIZE = 24;
    public:
        GitHubButton() : IconButton("GitHubButton", MurMur3::calcHash("texture_github")) {}
        RenderLayer getLayer() const override {
            return RenderLayer::MenuBar;
        }
        EventHandlingResult handleEvent(const MouseEvent& event) override {
            if (!isVisible() || !isEnabled()) {
                return EventHandlingResult::NotHandled();
            }

            // クリック時にGitHubリポジトリを開く
            if (event.left_button_state == MouseButtonState::Pressed) {
                paxg::System::launchBrowser("https://github.com/AsPJT/PAX_SAPIENTICA");
            }
            return EventHandlingResult::Handled();
        }
        void init(const paxs::Pulldown& language_selector) {
            const int github_x = paxg::Window::width() - static_cast<int>(language_selector.getRect().w()) - 32;
            const int github_y = static_cast<int>((language_selector.getRect().h() - ICON_SIZE) / 2);
            setPos(paxg::Vec2i{ github_x, github_y });
            setSize(paxg::Vec2i{ ICON_SIZE, ICON_SIZE });
        }
    };
} // namespace paxs

#endif // !PAX_MAHOROBA_UI_GITHUB_BUTTON_HPP
