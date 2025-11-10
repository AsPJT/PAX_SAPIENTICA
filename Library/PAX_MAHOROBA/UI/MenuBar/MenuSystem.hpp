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

#include <PAX_MAHOROBA/Rendering/IWidget.hpp>
#include <PAX_MAHOROBA/UI/MenuBar/DropDownMenu.hpp>

namespace paxs {
    /// @brief DropDownMenu（固定ヘッダー型）を複数持つ
    class MenuSystem : public IWidget {
    public:
        MenuSystem() = default;

        /// @brief メニュー項目を追加
        /// @param items_key_ 項目のキー一覧（最初の項目がメニュー名）
        /// @param font_size_ フォントサイズ
        /// @param font_buffer_thickness_size_ フォントの太さ
        /// @param menu_key_ メニューのキー（識別用）
        void add(
            const std::span<const std::uint_least32_t> items_key_,
            std::uint_least8_t font_size_,
            std::uint_least8_t font_buffer_thickness_size_,
            const std::uint_least32_t menu_key_) {

            if (menu_list.size() != 0) {
                start_x += static_cast<std::size_t>(menu_list.back().getRect().w());
            }
            menu_list_key.emplace(menu_key_, menu_list.size());
            menu_list.emplace_back(paxs::DropDownMenu(
                items_key_,
                font_size_,
                font_buffer_thickness_size_,
                paxg::Vec2i{ static_cast<int>(start_x), 0 }));

            layout();
        }

        EventHandlingResult handleEvent(const MouseEvent& event) override {
            if (event.left_button_state == MouseButtonState::Held ||
                event.left_button_state == MouseButtonState::Released) {
                return EventHandlingResult::Handled();
            }

            for (std::size_t i = 0; i < menu_list.size(); ++i) {
                if (menu_list[i].isHitHeader(event.x, event.y)) {
                    const bool was_open = menu_list[i].isOpen();
                    // 全部閉じる
                    for (auto& mi : menu_list) mi.setVisible(false);
                    // 今クリックしたやつだけトグル
                    menu_list[i].setVisible(!was_open);
                    return EventHandlingResult::Handled();
                }
            }

            // 開いてる子のドロップダウンにイベントを渡す
            for (paxs::DropDownMenu& mi : menu_list) {
                if (mi.isOpen()) {
                    paxs::EventHandlingResult r = mi.handleEvent(event);
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
        /// @param key メニュー項目のキー
        /// @return メニュー項目のポインタ（存在しない場合はnullptr）
        paxs::DropDownMenu* getDropDownMenu(const std::uint_least32_t key) {
            return (menu_list_key.find(key) != menu_list_key.end()) ? &menu_list[menu_list_key.at(key)] : nullptr;
        }
        const paxs::DropDownMenu* cgetDropDownMenu(const std::uint_least32_t key) const {
            return (menu_list_key.find(key) != menu_list_key.end()) ? &menu_list[menu_list_key.at(key)] : nullptr;
        }

    private:
        // メニューバーに付属するメニュー項目が左から順番に格納されている
        std::vector<paxs::DropDownMenu> menu_list;

        paxg::Rect bar_rect_{0,0,0,0};

        // 各メニュー項目に紐づけられた Key (Hash)
        paxs::UnorderedMap<std::uint_least32_t, std::size_t> menu_list_key{};

        std::size_t start_x = 0;

    void layout() {
        float x = 0.f;
        float h = 0.f;

        for (paxs::DropDownMenu& menu : menu_list) {
            const float w = menu.getRect().w();
            const float item_h = menu.getRect().h();

            menu.setRectX(x);

            x += w; // 幅ぶんだけ進む
            h = (std::max)(h, item_h);
        }

        // メニューバー全体のrectも左上(0,0)から幅xだけにする
        bar_rect_ = paxg::Rect{0.f, 0.f, x, h};
    }

    public:
        RenderLayer getLayer() const override {
            return RenderLayer::MenuBar;
        }

        bool isHit(int x, int y) const override {
            // ヘッダー列に当たっていたらtrue
            for (auto const& mi : menu_list) {
                if (mi.isHitHeader(x, y)) return true;
            }

            // 開いてる子がヒットしていないかも見る
            for (auto const& mi : menu_list) {
                if (mi.isHit(x, y)) {
                    return true;
                }
            }
            return false;
        }

        paxg::Rect getRect() const override {
            return bar_rect_;
        }

        void updateMenuWidth() {
            for (paxs::DropDownMenu& menu : menu_list) {
                menu.updateLanguage();
            }
            layout();
        }
        void setPos(const paxg::Vec2i& /*pos*/) override {
            // MenuBarは常に画面上部に配置されるため、positionの変更は実装しない
        }

        void setVisible(bool /*visible*/) override {}
        bool isVisible() const override { return true; }
        void setEnabled(bool /*enabled*/) override {}
        bool isEnabled() const override { return true; }
        const char* getName() const override { return "MenuSystem"; }

    };
} // namespace paxs

#endif // !PAX_MAHOROBA_MENU_BAR_HPP
