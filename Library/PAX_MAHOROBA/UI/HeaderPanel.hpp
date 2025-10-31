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
#include <PAX_GRAPHICA/Vec2.hpp>
#include <PAX_GRAPHICA/Window.hpp>

#include <PAX_MAHOROBA/UI/IUIWidget.hpp>
#include <PAX_MAHOROBA/UI/MenuBar.hpp>
#include <PAX_MAHOROBA/UI/Pulldown.hpp>
#include <PAX_MAHOROBA/Rendering/LanguageFonts.hpp>
#include <PAX_MAHOROBA/Rendering/ShadowRenderer.hpp>

#include <PAX_SAPIENTICA/InputStateManager.hpp>
#include <PAX_SAPIENTICA/Key/LanguageKeys.hpp>
#include <PAX_SAPIENTICA/Key/MenuBarKeys.hpp>
#include <PAX_SAPIENTICA/Language.hpp>
#include <PAX_SAPIENTICA/MurMur3.hpp>

namespace paxs {

    /// @brief ヘッダーパネル - アプリ上部のUI（メニューバー + 言語選択）を管理
    /// @brief Header Panel - Manages top UI elements (MenuBar + Language Selector)
    class HeaderPanel : public IUIWidget {
    public:
        /// @brief 初期化
        /// @param select_language 選択言語
        /// @param language_text 言語テキスト
        /// @param language_fonts フォント管理
        /// @param font_size フォントサイズ
        /// @param font_thickness フォント太さ
        void init(
            const SelectLanguage* select_language,
            const paxs::Language* language_text,
            paxs::LanguageFonts& language_fonts,
            std::uint_least8_t font_size,
            std::uint_least8_t font_thickness
        ) {
            select_language_ = select_language;
            language_text_ = language_text;
            font_size_ = font_size;
            font_thickness_ = font_thickness;

            // 言語選択プルダウンを初期化
            language_selector_ = paxs::Pulldown(
                select_language,
                language_text,
                paxs::LanguageKeys::ALL_LANGUAGE_HASHES,
                language_fonts,
                font_size,
                font_thickness,
                paxg::Vec2i{ 3000, 0 },
                paxs::PulldownDisplayType::SelectedValue,
                true
            );

            // メニューバーにメニュー項目を追加
            menu_bar_.add(select_language, language_text, paxs::MenuBarKeys::VIEW_MENU_HASHES, language_fonts, font_size, font_thickness, MurMur3::calcHash("view"));
            menu_bar_.add(select_language, language_text, paxs::MenuBarKeys::FEATURE_MENU_HASHES, language_fonts, font_size, font_thickness, MurMur3::calcHash("place_names"));
            menu_bar_.add(select_language, language_text, paxs::MenuBarKeys::MAP_MENU_HASHES, language_fonts, font_size, font_thickness, MurMur3::calcHash("map"));

            calculateLayout();
        }

        /// @brief 影用のテクスチャを設定
        /// @brief Set textures for shadow rendering
        void setShadowTextures(paxg::RenderTexture& shadow_tex, paxg::RenderTexture& internal_tex);

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

        // IUIWidget インターフェースの実装
        void update(paxs::InputStateManager& input_state_manager) override {
            if (!visible_ || !enabled_) return;

            calculateLayout();  // 毎フレーム位置を更新

            // メニューバーと言語選択を更新
            menu_bar_.update(input_state_manager);
            language_selector_.update(input_state_manager);
        }

        void draw() override {
            if (!visible_) return;

            // 背景パネルを描画（影付き）
            drawBackground();

            // メニューバーと言語選択を描画
            menu_bar_.draw();
            language_selector_.draw();
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
        std::uint_least8_t font_size_ = 24;
        std::uint_least8_t font_thickness_ = 3;

        // 影描画用テクスチャ（外部から注入）
        paxg::RenderTexture* shadow_texture_ = nullptr;
        paxg::RenderTexture* internal_texture_ = nullptr;

        /// @brief 背景パネルを描画
        /// @brief Draw background panel
        void drawBackground() {
#ifdef PAXS_USING_SIV3D
            // Siv3D: Use high-quality shadow renderer with Gaussian blur
            if (shadow_texture_ && internal_texture_) {
                paxs::ShadowRenderer::renderShadowWithPanels(
                    *shadow_texture_,
                    *internal_texture_,
                    [this]() {
                        // 影の形状を描画
                        paxg::Rect{ 0, 0, static_cast<float>(paxg::Window::width()), static_cast<float>(getHeight()) }.draw();
                    },
                    [this]() {
                        // パネル本体を描画
                        paxg::Rect{ 0, 0, static_cast<float>(paxg::Window::width()), static_cast<float>(getHeight()) }
                            .draw(paxg::Color{243, 243, 243});
                    }
                );
            }
#else
            // SFML/DxLib: Use simple shadow with drawShadow method
            paxg::Rect{ 0, 0, static_cast<float>(paxg::Window::width()), static_cast<float>(getHeight()) }
                .drawShadow({1, 1}, 4, 1).draw(paxg::Color{243, 243, 243});
#endif
        }
    };

    // Inline implementations
    inline void HeaderPanel::setShadowTextures(paxg::RenderTexture& shadow_tex, paxg::RenderTexture& internal_tex) {
        shadow_texture_ = &shadow_tex;
        internal_texture_ = &internal_tex;
    }

} // namespace paxs

#endif // !PAX_MAHOROBA_UI_HEADER_PANEL_HPP
