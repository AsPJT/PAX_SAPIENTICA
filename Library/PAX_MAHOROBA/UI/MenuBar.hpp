/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_MENU_BAR_HPP
#define PAX_MAHOROBA_MENU_BAR_HPP

#include <PAX_MAHOROBA/UI/IUIWidget.hpp>
#include <PAX_MAHOROBA/UI/MenuItem.hpp>
#include <PAX_MAHOROBA/UI/Pulldown.hpp>

namespace paxs {
    /// @brief メニューバーを管理
    /// @brief Manages menu bar
    /// @details MenuItem（固定ヘッダー型のドロップダウン）を複数保持し、
    ///          メニューバーとしての振る舞いを提供
    class MenuBar : public IUIWidget {
    public:

        /// @brief メニュー項目を追加
        /// @param select_language_ptr_ 選択されている言語
        /// @param language_ptr_ 言語データ
        /// @param items_key_ 項目のキー一覧（最初の項目がメニュー名）
        /// @param font_menu_bar フォント
        /// @param font_size_ フォントサイズ
        /// @param font_buffer_thickness_size_ フォントの太さ
        /// @param menu_key_ メニューのキー（識別用）
        void add(
            const SelectLanguage* select_language_ptr_,
            const Language* language_ptr_,
            const std::span<const std::uint_least32_t> items_key_,
            LanguageFonts& font_menu_bar,
            std::uint_least8_t font_size_,
            std::uint_least8_t font_buffer_thickness_size_,
            const std::uint_least32_t menu_key_) {

            if (menu_items.size() != 0) {
                start_x += static_cast<std::size_t>(menu_items.back().getRect().w());
            }
            menu_item_key.emplace(menu_key_, menu_items.size());
            menu_items.emplace_back(paxs::MenuItem(
                select_language_ptr_,
                language_ptr_,
                items_key_,
                font_menu_bar,
                font_size_,
                font_buffer_thickness_size_,
                paxg::Vec2i{ static_cast<int>(start_x), 0 }));
        }

        void update(paxs::TouchStateManager& tm_) override {
            start_x = 0;

            // 更新前の開閉状態を記録
            std::vector<bool> was_open;
            was_open.reserve(menu_items.size());
            for (const auto& item : menu_items) {
                was_open.push_back(item.isOpen());
            }

            // 各メニュー項目を更新
            for (std::size_t i = 0; i < menu_items.size(); ++i) {
                if (visible_ && enabled_) {
                    menu_items[i].update(tm_);
                }
                menu_items[i].setRectX(start_x);
                start_x += static_cast<std::size_t>(menu_items[i].getRect().w());

                // このメニュー項目が新しく開かれた場合、他のメニュー項目を閉じる
                if (visible_ && enabled_ && !was_open[i] && menu_items[i].isOpen()) {
                    for (std::size_t j = 0; j < menu_items.size(); ++j) {
                        if (j != i && menu_items[j].isOpen()) {
                            menu_items[j].close();
                        }
                    }
                }
            }
        }
        void draw() override {
            if (!visible_) return;
            for (auto& item : menu_items) {
                item.draw();
            }
        }

        /// @brief メニュー項目を取得
        /// @param key メニュー項目のキー
        /// @return メニュー項目のポインタ（存在しない場合はnullptr）
        paxs::MenuItem* getMenuItem(const std::uint_least32_t key) {
            return (menu_item_key.find(key) != menu_item_key.end()) ? &menu_items[menu_item_key.at(key)] : nullptr;
        }
        const paxs::MenuItem* cgetMenuItem(const std::uint_least32_t key) const {
            return (menu_item_key.find(key) != menu_item_key.end()) ? &menu_items[menu_item_key.at(key)] : nullptr;
        }

    private:
        // メニューバーに付属するメニュー項目が左から順番に格納されている
        // 例） | ファイル | 編集 | 表示 |
        std::vector<paxs::MenuItem> menu_items;

        // 各メニュー項目に紐づけられた Key (Hash)
        paxs::UnorderedMap<std::uint_least32_t, std::size_t> menu_item_key{};

        std::size_t start_x = 0;

        // IUIWidget インターフェース用の状態
        bool visible_ = true;
        bool enabled_ = true;

    public:
        // IUIWidget インターフェースの実装
        void setPos(const paxg::Vec2i& pos) override {
            // MenuBarは常に画面上部に配置されるため、positionの変更は実装しない
            // 必要に応じて各Pulldownの位置を調整することは可能
        }

        paxg::Rect getRect() const override {
            if (menu_items.empty()) return paxg::Rect{0, 0, 0, 0};
            // メニューバー全体の矩形を返す
            float total_width = static_cast<float>(start_x);
            float height = menu_items.front().getRect().h();
            return paxg::Rect{0, 0, total_width, height};
        }

        void setVisible(bool visible) override { visible_ = visible; }
        bool isVisible() const override { return visible_; }

        void setEnabled(bool enabled) override { enabled_ = enabled; }
        bool isEnabled() const override { return enabled_; }
    };
} // namespace paxs

#endif // !PAX_MAHOROBA_MENU_BAR_HPP
