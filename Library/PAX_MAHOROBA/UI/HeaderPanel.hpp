/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_UI_HEADER_PANEL_HPP
#define PAX_MAHOROBA_UI_HEADER_PANEL_HPP

#include <PAX_GRAPHICA/Rect.hpp>
#include <PAX_GRAPHICA/RenderTexture.hpp>
#include <PAX_GRAPHICA/System.hpp>
#include <PAX_GRAPHICA/Texture.hpp>
#include <PAX_GRAPHICA/Vec2.hpp>
#include <PAX_GRAPHICA/Window.hpp>

#include <PAX_MAHOROBA/Rendering/IWidget.hpp>
#include <PAX_MAHOROBA/UI/MenuBar.hpp>
#include <PAX_MAHOROBA/UI/Pulldown.hpp>
#include <PAX_MAHOROBA/Rendering/LanguageFonts.hpp>

#include <PAX_SAPIENTICA/FontConfig.hpp>
#include <PAX_SAPIENTICA/InputStateManager.hpp>
#include <PAX_SAPIENTICA/Key/LanguageKeys.hpp>
#include <PAX_SAPIENTICA/Key/MenuBarKeys.hpp>
#include <PAX_SAPIENTICA/Language.hpp>
#include <PAX_SAPIENTICA/MurMur3.hpp>

namespace paxs {

    /// @brief ヘッダーパネル - アプリ上部のUI（メニューバー + 言語選択）を管理
    /// @brief Header Panel - Manages top UI elements (MenuBar + Language Selector)
    class HeaderPanel : public IWidget {
    public:
        /// @brief 初期化
        /// @param select_language 選択言語
        /// @param language_text 言語テキスト
        /// @param language_fonts フォント管理
        void init(
            const SelectLanguage* select_language,
            const paxs::Language* language_text,
            paxs::LanguageFonts& language_fonts
        ) {
            select_language_ = select_language;
            language_text_ = language_text;

            // 言語選択プルダウンを初期化
            language_selector_ = paxs::Pulldown(
                select_language,
                language_text,
                paxs::LanguageKeys::ALL_LANGUAGE_HASHES,
                language_fonts,
                static_cast<std::uint_least8_t>(FontConfig::PULLDOWN_FONT_SIZE),
                static_cast<std::uint_least8_t>(FontConfig::PULLDOWN_FONT_BUFFER_THICKNESS),
                paxg::Vec2i{ 3000, 0 },
                paxs::PulldownDisplayType::SelectedValue,
                true
            );

            // メニューバーにメニュー項目を追加
            menu_bar_.add(select_language, language_text, paxs::MenuBarKeys::VIEW_MENU_HASHES, language_fonts, static_cast<std::uint_least8_t>(FontConfig::PULLDOWN_FONT_SIZE), static_cast<std::uint_least8_t>(FontConfig::PULLDOWN_FONT_BUFFER_THICKNESS), MurMur3::calcHash("view"));
            menu_bar_.add(select_language, language_text, paxs::MenuBarKeys::FEATURE_MENU_HASHES, language_fonts, static_cast<std::uint_least8_t>(FontConfig::PULLDOWN_FONT_SIZE), static_cast<std::uint_least8_t>(FontConfig::PULLDOWN_FONT_BUFFER_THICKNESS), MurMur3::calcHash("place_names"));
            menu_bar_.add(select_language, language_text, paxs::MenuBarKeys::MAP_MENU_HASHES, language_fonts, static_cast<std::uint_least8_t>(FontConfig::PULLDOWN_FONT_SIZE), static_cast<std::uint_least8_t>(FontConfig::PULLDOWN_FONT_BUFFER_THICKNESS), MurMur3::calcHash("map"));

            calculateLayout();
        }


        /// @brief GitHubアイコンのテクスチャを設定
        /// @brief Set GitHub icon texture
        void setGitHubTexture(const paxg::Texture* texture) {
            github_texture_ = texture;
        }

        /// @brief レイアウトを計算（画面サイズ変更時に呼び出し）
        void calculateLayout() {
            // 言語選択プルダウンを右端に配置
            language_selector_.setPos(paxg::Vec2i{
                static_cast<int>(paxg::Window::width() - language_selector_.getRect().w()),
                0
            });
        }

        /// @brief ヘッダーの高さを取得
        int getHeight() const {
            return language_selector_.getRect().h();
        }

        /// @brief メニューバーの取得（読み取り専用）
        const MenuBar& getMenuBar() const {
            return menu_bar_;
        }

        /// @brief メニューバーの取得（変更可能）- TileManager用
        MenuBar& getMenuBar() {
            return menu_bar_;
        }

        /// @brief 言語選択のインデックスを取得
        int getLanguageIndex() const {
            return language_selector_.getIndex();
        }

        /// @brief 言語選択のキーを取得
        std::uint_least32_t getLanguageKey() const {
            return language_selector_.getKey();
        }

        // IWidget インターフェースの実装
        bool handleInput(const InputEvent& event) override {
            if (!visible_ || !enabled_) return false;
            if (event.input_state_manager == nullptr) return false;

            calculateLayout();  // 毎フレーム位置を更新

            // メニューバーと言語選択を更新
            menu_bar_.handleInput(event);
            language_selector_.handleInput(event);

            // GitHubアイコンのクリック判定（言語セレクターの左、HeaderPanel中央に配置）
            const float github_x = static_cast<float>(paxg::Window::width() - language_selector_.getRect().w() - 32);
            const float github_y = (language_selector_.getRect().h() - 28.0f) / 2.0f;  // 中央配置
            if (event.input_state_manager->get(paxg::Rect(github_x, github_y, 28.0f, 28.0f).leftClicked())) {
                paxg::System::launchBrowser("https://github.com/AsPJT/PAX_SAPIENTICA");
                return true;
            }
            return true;
        }

        void render() override {
            if (!visible_) return;

            // メニューバーと言語選択を描画
            menu_bar_.render();
            language_selector_.render();

            // GitHubアイコンを描画（言語セレクターの左、HeaderPanel中央に配置）
            if (github_texture_) {
                const int github_x = paxg::Window::width() - static_cast<int>(language_selector_.getRect().w()) - 32;
                const int github_y = static_cast<int>((language_selector_.getRect().h() - 24) / 2);  // 中央配置
                github_texture_->resizedDraw(24, paxg::Vec2i{ github_x, github_y });
            }
        }

        paxg::Rect getRect() const override {
            return paxg::Rect{
                0,
                0,
                static_cast<float>(paxg::Window::width()),
                static_cast<float>(getHeight())
            };
        }

        void setPos(const paxg::Vec2i& pos) override {
            (void)pos;  // ヘッダーは常に画面上部なので位置指定は無視
        }

        void setVisible(bool visible) override { visible_ = visible; }
        bool isVisible() const override { return visible_; }

        void setEnabled(bool enabled) override { enabled_ = enabled; }
        bool isEnabled() const override { return enabled_; }

        const char* getName() const override { return "HeaderPanel"; }

        /// @brief レンダリングレイヤーを取得
        /// @brief Get rendering layer
        RenderLayer getLayer() const override {
            return RenderLayer::UIContent;
        }
        bool isAvailable() const override { return true; }

    private:
        // 状態管理
        bool visible_ = true;
        bool enabled_ = true;

        // 子ウィジェット
        paxs::Pulldown language_selector_;
        paxs::MenuBar menu_bar_;

        // 設定値
        const SelectLanguage* select_language_ = nullptr;
        const paxs::Language* language_text_ = nullptr;

        // GitHubアイコンテクスチャ（外部から注入）
        const paxg::Texture* github_texture_ = nullptr;
    };

} // namespace paxs

#endif // !PAX_MAHOROBA_UI_HEADER_PANEL_HPP
