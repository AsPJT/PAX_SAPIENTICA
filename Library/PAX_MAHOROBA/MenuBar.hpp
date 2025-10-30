/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_MENU_BAR_HPP
#define PAX_MAHOROBA_MENU_BAR_HPP

#include <PAX_MAHOROBA/IUIWidget.hpp>
#include <PAX_MAHOROBA/Pulldown.hpp>

namespace paxs {
    // メニューバーを管理
    class MenuBar : public IUIWidget {
    public:

        void add(
            const SelectLanguage* select_language_ptr_,
            const Language* language_ptr_,
            const std::span<const std::uint_least32_t> items_key_,
            LanguageFonts& font_menu_bar,
            std::uint_least8_t font_size_,
            std::uint_least8_t font_buffer_thickness_size_,
            const std::uint_least32_t pulldown_key_) {

            if (pdv.size() != 0) {
                start_x += static_cast<std::size_t>(pdv.back().getRect().w());
            }
            pulldown_key.emplace(pulldown_key_, pdv.size());
            pdv.emplace_back(paxs::Pulldown(
                select_language_ptr_,
                language_ptr_,
                items_key_,
                font_menu_bar,
                font_size_,
                font_buffer_thickness_size_,
                paxg::Vec2i{ static_cast<int>(start_x), 0 },
                paxs::PulldownDisplayType::FixedHeader));
        }

        void update(paxs::TouchStateManager& tm_) override {
            start_x = 0;

            // 更新前の開閉状態を記録
            std::vector<bool> was_open;
            was_open.reserve(pdv.size());
            for (const auto& pd : pdv) {
                was_open.push_back(pd.isOpen());
            }

            // 各プルダウンを更新
            for (std::size_t i = 0; i < pdv.size(); ++i) {
                if (visible_ && enabled_) {
                    pdv[i].update(tm_);
                }
                pdv[i].setRectX(start_x);
                start_x += static_cast<std::size_t>(pdv[i].getRect().w());

                // このプルダウンが新しく開かれた場合、他のプルダウンを閉じる
                if (visible_ && enabled_ && !was_open[i] && pdv[i].isOpen()) {
                    for (std::size_t j = 0; j < pdv.size(); ++j) {
                        if (j != i && pdv[j].isOpen()) {
                            pdv[j].close();
                        }
                    }
                }
            }
        }
        void draw() override {
            if (!visible_) return;
            for (auto& pd : pdv) {
                pd.draw();
            }
        }

        paxs::Pulldown* getPulldown(const std::uint_least32_t key) {
            return (pulldown_key.find(key) != pulldown_key.end()) ? &pdv[pulldown_key.at(key)] : nullptr;
        }
        const paxs::Pulldown* cgetPulldown(const std::uint_least32_t key) const {
            return (pulldown_key.find(key) != pulldown_key.end()) ? &pdv[pulldown_key.at(key)] : nullptr;
        }

    private:
        // メニューバーに付属するプルダウンが左から順番に格納されている
        // 例） | ファイル | 編集 | 表示 |
        std::vector<paxs::Pulldown> pdv;

        // 各プルダウンに紐づけられた Key (Hash)
        paxs::UnorderedMap<std::uint_least32_t, std::size_t> pulldown_key{};

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
            if (pdv.empty()) return paxg::Rect{0, 0, 0, 0};
            // メニューバー全体の矩形を返す
            float total_width = static_cast<float>(start_x);
            float height = pdv.front().getRect().h();
            return paxg::Rect{0, 0, total_width, height};
        }

        void setVisible(bool visible) override { visible_ = visible; }
        bool isVisible() const override { return visible_; }

        void setEnabled(bool enabled) override { enabled_ = enabled; }
        bool isEnabled() const override { return enabled_; }
    };
} // namespace paxs

#endif // !PAX_MAHOROBA_MENU_BAR_HPP
