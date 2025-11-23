/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_MENU_BAR_HPP
#define PAX_MAHOROBA_MENU_BAR_HPP

#include <vector>

#include <PAX_MAHOROBA/Rendering/InteractiveUIComponent.hpp>
#include <PAX_MAHOROBA/UI/MenuBar/DropDownMenu.hpp>

#include <PAX_SAPIENTICA/Key/MenuBarKeys.hpp>

namespace paxs {
    /// @brief DropDownMenu（固定ヘッダー型）を複数持つ
    class MenuSystem : public InteractiveUIComponent {
    private:
        // メニューバーに付属するメニュー項目が左から順番に格納されている
        std::vector<paxs::DropDownMenu> menu_list;

        paxg::Rect bar_rect_{0,0,0,0};

        // 各メニュー項目に紐づけられた Key 値
        paxs::UnorderedMap<paxs::MenuBarType, std::size_t> menu_list_key;

        int start_x = 0;

    void layout() {
        float x = 0.f;
        float h = 0.f;

        for (paxs::DropDownMenu& menu : menu_list) {
            const float w = static_cast<float>(menu.getRect().width());
            const float item_h = static_cast<float>(menu.getRect().height());

            menu.setRectX(x);

            x += w; // 幅ぶんだけ進む
            h = (std::max)(h, item_h);
        }

        // メニューバー全体のrectも左上(0,0)から幅xだけにする
        bar_rect_ = paxg::Rect{0.f, 0.f, x, h};
    }

    public:
        MenuSystem() = default;

        /// @brief メニュー項目を追加
        /// @param items_key_ 項目のキー一覧（最初の項目がメニュー名）
        /// @param font_size_ フォントサイズ
        /// @param font_buffer_thickness_size_ フォントの太さ
        /// @param menu_type_ メニュー項目の種類
        void add(
            const std::span<const std::uint_least32_t> items_key_,
            std::uint_least8_t font_size_,
            std::uint_least8_t font_buffer_thickness_size_,
            const paxs::MenuBarType menu_type_) {

            if (menu_list.size() != 0) {
                start_x += menu_list.back().getRect().width();
            }
            menu_list_key.emplace(menu_type_, menu_list.size());
            menu_list.emplace_back(paxs::DropDownMenu(
                items_key_,
                font_size_,
                font_buffer_thickness_size_,
                paxg::Vec2i{ start_x, 0 }));

            layout();
        }

        EventHandlingResult handleEvent(const MouseEvent& event) override {
            if (event.left_button_state == MouseButtonState::Held ||
                event.left_button_state == MouseButtonState::Released) {
                return EventHandlingResult::Handled();
            }

            for (std::size_t i = 0; i < menu_list.size(); ++i) {
                if (menu_list[i].isHitHeader(event.pos.x, event.pos.y)) {
                    const bool was_open = menu_list[i].isVisible();
                    // 全部閉じる
                    for (auto& menu : menu_list) {
                        menu.setVisible(false);
                    }
                    // 今クリックしたやつだけトグル
                    menu_list[i].setVisible(!was_open);
                    return EventHandlingResult::Handled();
                }
            }

            // 開いてる子のドロップダウンにイベントを渡す
            for (paxs::DropDownMenu& menu : menu_list) {
                if (menu.isVisible()) {
                    paxs::EventHandlingResult r = menu.handleEvent(event);
                    if (r.handled) return r;
                }
            }

            return EventHandlingResult::NotHandled();
        }

        void render() const override {
            for (auto& item : menu_list) {
                item.render();
            }
        }

        /// @brief メニュー項目を取得
        /// @param type メニュー項目の種類
        /// @return メニュー項目のポインタ（存在しない場合はnullptr）
        paxs::DropDownMenu* getDropDownMenu(const paxs::MenuBarType type) {
            const auto iterator = menu_list_key.find(type);
            return iterator != menu_list_key.end() ? &menu_list[iterator->second] : nullptr;
        }
        const paxs::DropDownMenu* getDropDownMenu(const paxs::MenuBarType type) const {
            const auto iterator = menu_list_key.find(type);
            return iterator != menu_list_key.end() ? &menu_list[iterator->second] : nullptr;
        }

        bool isHit(const paxs::Vector2<int>& pos) const override {
            // ヘッダー列に当たっていたらtrue
            for (auto const& mi : menu_list) {
                if (mi.isHitHeader(pos.x, pos.y)) return true;
            }

            // 開いてる子がヒットしていないかも見る
            for (auto const& mi : menu_list) {
                if (mi.isHit(pos)) {
                    return true;
                }
            }
            return false;
        }

        void updateMenuWidth() {
            for (paxs::DropDownMenu& menu : menu_list) {
                menu.updateLanguage();
            }
            layout();
        }

        bool isVisible() const override { return true; }
        void setPos(const Vector2<int>& /*pos*/) override {}
        Rect<int> getRect() const override {
        const paxg::Rect& r = bar_rect_;
        return {
            static_cast<int>(r.x()),
            static_cast<int>(r.y()),
            static_cast<int>(r.w()),
            static_cast<int>(r.h())
        };
    }
        const char* getName() const override { return "MenuSystem"; }
        RenderLayer getLayer() const override { return RenderLayer::MenuBar; }
    };
} // namespace paxs

#endif // !PAX_MAHOROBA_MENU_BAR_HPP
