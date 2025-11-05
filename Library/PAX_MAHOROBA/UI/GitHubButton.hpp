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

namespace paxs {

    /// @brief GitHubリポジトリへのリンクボタン
    /// @brief GitHub repository link button
    class GitHubButton : public IWidget {
    public:
        /// @brief デフォルトコンストラクタ
        GitHubButton() {
            // GitHubアイコンのテクスチャを読み込んで設定
            paxs::KeyValueTSV<std::string> icon_paths;
            icon_paths.input(paxs::AppConfig::getInstance()->getRootPath() + "Data/MenuIcon/MenuIcons.tsv",
                [&](const std::string& value_) { return value_; });
            std::string github_texture_path = icon_paths.get().at(MurMur3::calcHash("texture_github"));
            github_texture = paxg::Texture{ github_texture_path };
        }

        /// @brief コンストラクタ
        /// @param language_selector 言語セレクター（位置計算に使用）
        GitHubButton(const paxs::Pulldown* language_selector)
            : language_selector_(language_selector) {}

        /// @brief 言語セレクターを設定
        /// @param language_selector 言語セレクター（位置計算に使用）
        void setLanguageSelector(const paxs::Pulldown* language_selector) {
            language_selector_ = language_selector;
        }

        const char* getName() const override {
            return "GitHubButton";
        }

        RenderLayer getLayer() const override {
            return RenderLayer::Header;
        }

        void setEnabled(bool enabled) override {
            enabled_ = enabled;
        }

        bool isEnabled() const override {
            return enabled_;
        }

        void setVisible(bool visible) override {
            visible_ = visible;
        }

        bool isVisible() const override {
            return visible_;
        }

        paxg::Rect getRect() const override {
            if (!language_selector_) return paxg::Rect{0, 0, 0, 0};

            const int github_x = paxg::Window::width() - static_cast<int>(language_selector_->getRect().w()) - 32;
            const int github_y = static_cast<int>((language_selector_->getRect().h() - ICON_SIZE) / 2);
            return paxg::Rect{
                static_cast<float>(github_x),
                static_cast<float>(github_y),
                static_cast<float>(ICON_SIZE),
                static_cast<float>(ICON_SIZE)
            };
        }

        void setPos(const paxg::Vec2i& /*pos*/) override {
            // 位置は language_selector の位置に基づいて自動計算されるため、設定は無視
        }

        bool isHit(int x, int y) const override {
            if (!isVisible() || !isEnabled()) return false;

            const paxg::Rect rect = getRect();
            return (x >= rect.x() && x < rect.x() + rect.w() &&
                y >= rect.y() && y < rect.y() + rect.h());
        }

        EventHandlingResult handleEvent(const MouseEvent& event) override {
            if (!isVisible() || !isEnabled()) {
                return EventHandlingResult::NotHandled();
            }

            if (!isHit(event.x, event.y)) {
                return EventHandlingResult::NotHandled();
            }

            // クリック時にGitHubリポジトリを開く
            if (event.left_button_state == MouseButtonState::Pressed) {
                paxg::System::launchBrowser("https://github.com/AsPJT/PAX_SAPIENTICA");
                return EventHandlingResult::Handled();
            }

            // ボタン上でのマウスイベントは処理済みとする
            if (event.left_button_state == MouseButtonState::Held ||
                event.left_button_state == MouseButtonState::Released) {
                return EventHandlingResult::Handled();
            }

            return EventHandlingResult::NotHandled();
        }

        void render() const override {
            if (!isVisible() || !github_texture) return;

            const paxg::Rect rect = getRect();
            github_texture.resizedDraw(ICON_SIZE, paxg::Vec2i{
                static_cast<int>(rect.x()),
                static_cast<int>(rect.y())
            });
        }

    private:
        static constexpr int ICON_SIZE = 24;

        const paxs::Pulldown* language_selector_ = nullptr;
        paxg::Texture github_texture;
        bool visible_ = true;
        bool enabled_ = true;
    };
} // namespace paxs

#endif // !PAX_MAHOROBA_UI_GITHUB_BUTTON_HPP
