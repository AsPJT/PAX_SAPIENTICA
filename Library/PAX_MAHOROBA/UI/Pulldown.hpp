/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_PULLDOWN_HPP
#define PAX_MAHOROBA_PULLDOWN_HPP

#include <algorithm>
#include <functional>
#include <string>
#include <vector>
#include <span>

#include <PAX_GRAPHICA/Rect.hpp>
#include <PAX_GRAPHICA/Triangle.hpp>

#include <PAX_MAHOROBA/Rendering/FontSystem.hpp>
#include <PAX_MAHOROBA/Rendering/IWidget.hpp>

#include <PAX_SAPIENTICA/Core/Type/UnorderedMap.hpp>
namespace paxs {

    /// @brief プルダウンの表示タイプ
    /// @brief Display type for pulldown
    enum class PulldownDisplayType : std::size_t {
        /// @brief 選択された値をヘッダーに表示（通常のプルダウン）
        SelectedValue = 0,

        /// @brief 固定のヘッダー名を表示（メニューバー用）
        FixedHeader = 1
    };

    /// @brief プルダウンメニューコンポーネント
    /// @brief Pulldown menu component
    /// @details 2つのモードで動作:
    ///          - SelectedValue: 選択した値がヘッダーに表示される（言語選択など）
    ///          - FixedHeader: 固定のヘッダー名が表示される（メニューバーなど）
    class Pulldown : public IWidget {
    private:
        std::span<const std::uint_least32_t> items_key; // 項目の Key 一覧

        std::size_t language_index = 0; // 言語の要素番号
        std::uint_least32_t old_language_key = 0; // 選択されている言語の Key
        std::vector<bool> is_items; // 項目が TRUE か FALSE になっているか格納
        paxs::UnorderedMap<std::uint_least32_t, std::size_t> item_index_key{}; // 項目の Key を格納

        paxg::Rect rect;
        PulldownDisplayType display_type{}; // 表示タイプ (SelectedValue or FixedHeader)
        bool is_one_font = false;

        const LanguageDomain language_domain{}; // 言語ドメイン

        size_t index = 0;
        paxg::Vec2i padding{ default_padding_x, default_padding_y };
        float all_rect_x{}; // 全ての項目の文字幅
        bool is_open = false;

        bool visible_ = true;

        // コールバック関数
        std::function<void(std::size_t index, bool is_selected)> on_selection_changed_;

        // プラットフォーム固有の表示調整定数
        static constexpr float android_width_scale = 2.5f;
        static constexpr float android_rect_width_scale = 2.0f;
        static constexpr float android_height_scale = 1.4f;
        static constexpr float sfml_height_scale = 1.2f;

        // UI要素のサイズ定数
        static constexpr float arrow_radius = 8.0f;
        static constexpr float arrow_rotation_pi = 3.1416f;  // π radians (down)
        static constexpr int shadow_offset_x = 1;
        static constexpr int shadow_offset_y = 1;
        static constexpr int shadow_blur_radius = 4;
        static constexpr int shadow_spread = 1;
        static constexpr int default_padding_x = 6;
        static constexpr int default_padding_y = 2;
        static constexpr int down_button_size = 20;

    public:
        Pulldown() = default;

        // X を指定したサイズに変更
        void setRectX(const std::size_t x = 0) {
            rect.setX(static_cast<float>(x));
        }
        // 言語変更による更新処理
        void updateLanguage() {
            paxg::Font* one_font = Fonts().getFont(FontProfiles::PULLDOWN);
            if (one_font == nullptr) {
                rect.setH(static_cast<float>(paxg::FontConfig::PULLDOWN_FONT_SIZE) * 2.f);
            }
            else {
                const float height = static_cast<float>(((*one_font).height()) + (padding.y() * 2));
                rect.setH(height);
            }

            rect.setW(0);
            all_rect_x = 0;

            const std::size_t item_count = items_key.size();

            for (std::size_t i = 0; i < item_count; ++i) {
                const std::string* str = nullptr;

                // 言語辞書から取得
                str = Fonts().getText(items_key[i], language_domain);

                if (str == nullptr || str->size() == 0) {
                    PAXS_WARNING("Pulldown: Missing text for key " + std::to_string(items_key[i]));
                    continue;
                }

                const std::uint_least32_t font_key = (is_one_font ? items_key[i] : Fonts().getSelectedLanguage().cgetKey());
                paxg::Font* item_font = Fonts().getFont(font_key, FontProfiles::PULLDOWN);
                if (item_font == nullptr) {
                    PAXS_WARNING("Pulldown: Missing font for item.");
                    continue;
                }

                // 最大の文字数からプルダウンの各項目の幅を定義
                all_rect_x =
                (
                    static_cast<float>((std::max)(static_cast<int>(all_rect_x), (*item_font).width(*str)))
                );
                // フォントが１つの場合は１行目も項目と同じ幅にする
                if (is_one_font) {
                    rect.setW(all_rect_x);
                }
                else {
                    if (i == 0) {
                        rect.setW(static_cast<float>((*item_font).width(*str)));
                    }
                }
            }
            // プルダウンの幅を設定
            rect.setW(rect.w() + (padding.x() * 2 + down_button_size));
            all_rect_x += (padding.x() * 2 + down_button_size);

#ifdef PAXS_USING_DXLIB
#ifdef __ANDROID__
            all_rect_x *= android_width_scale;
            rect.setW(rect.w() * android_rect_width_scale);
            rect.setH(rect.h() * android_height_scale);
#endif
#elif defined(PAXS_USING_SFML)
            rect.setH(rect.h() * sfml_height_scale);
#endif
        }
        /// @brief コンストラクタ
        /// @param items_key_ 項目のキー一覧
        /// @param pos_ 表示位置
        /// @param display_type_ 表示タイプ（SelectedValue or FixedHeader）
        /// @param is_one_font_ 単一フォントを使用するか
        Pulldown(
            const LanguageDomain language_domain_,
            const paxg::Vec2i& pos_ = { 0,0 },
            PulldownDisplayType display_type_ = PulldownDisplayType::SelectedValue,
            const bool is_one_font_ = false)
            : language_domain(language_domain_)
            , rect{ static_cast<float>(pos_.x()), static_cast<float>(pos_.y()),0, 0 }
            , display_type(display_type_)
            , is_one_font(is_one_font_) {
        }

        void setItemsKey(const std::span<const std::uint_least32_t> items_key_) {
            items_key = items_key_;

            updateLanguage();

            // Key List を作成
            for (std::size_t i = 0; i < items_key.size(); ++i) {
                item_index_key.emplace(items_key[i], i);
                is_items.emplace_back(true);
            }
        }

        /// @brief 選択変更時のコールバックを設定
        void setOnSelectionChanged(std::function<void(std::size_t, bool)> callback) {
            on_selection_changed_ = std::move(callback);
        }

        // 入力処理
        EventHandlingResult handleEvent(const MouseEvent& event) override {
            if (!visible_ || items_key.empty()) {
                return EventHandlingResult::NotHandled(); // 非表示または無効の場合は処理をしない
            }

            // 言語が変わっていたら更新処理
            const std::uint_least32_t current_language_key = Fonts().getSelectedLanguage().cgetKey();
            if (old_language_key != current_language_key) {
                language_index = Fonts().getSelectedLanguage().cget();
                old_language_key = current_language_key;
                updateLanguage();
            }

            // Pressed/Heldイベント：プルダウン範囲内ならイベントを消費（ドラッグ防止）
            if (event.left_button_state == MouseButtonState::Pressed ||
                event.left_button_state == MouseButtonState::Held) {
                int rect_x = static_cast<int>(rect.x());
                int rect_y = static_cast<int>(rect.y());
                int rect_w = static_cast<int>(rect.w());
                int rect_h = static_cast<int>(rect.h());

                // ヘッダー部分
                if (event.x >= rect_x && event.x < rect_x + rect_w && event.y >= rect_y && event.y < rect_y + rect_h) {
                    return EventHandlingResult::Handled();
                }

                // ドロップダウン項目（開いている場合）
                if (is_open) {
                    paxg::Vec2i pos = paxg::Vec2i(
                        rect_x,
                        rect_y + rect_h);
                    for (std::size_t i = 0; i < items_key.size(); ++i) {
                        const paxg::Rect rect_tmp{ pos, rect.w(), rect.h() };
                        int rtx = static_cast<int>(rect_tmp.x());
                        int rty = static_cast<int>(rect_tmp.y());
                        int rtw = static_cast<int>(rect_tmp.w());
                        int rth = static_cast<int>(rect_tmp.h());
                        if (event.x >= rtx && event.x < rtx + rtw && event.y >= rty && event.y < rty + rth) {
                            return EventHandlingResult::Handled();
                        }
                        pos.setY(pos.y() + rect_h);
                    }
                }
            }

            // Releasedイベント：実際の処理を行う
            if (event.left_button_state == MouseButtonState::Released) {
                int rect_x = static_cast<int>(rect.x());
                int rect_y = static_cast<int>(rect.y());
                int rect_w = static_cast<int>(rect.w());
                int rect_h = static_cast<int>(rect.h());

                // ヘッダー部分をクリック：開閉をトグル
                if (event.x >= rect_x && event.x < rect_x + rect_w && event.y >= rect_y && event.y < rect_y + rect_h) {
                    is_open = (not is_open);
                    return EventHandlingResult::Handled();
                }

                // ドロップダウン項目をクリック（開いている場合）
                if (is_open) {
                    paxg::Vec2i pos = paxg::Vec2i(
                        rect_x,
                        rect_y + rect_h);
                    for (std::size_t i = 0; i < items_key.size(); ++i) {
                        const paxg::Rect rect_tmp{ pos, rect.w(), rect.h() };
                        int rtx = static_cast<int>(rect_tmp.x());
                        int rty = static_cast<int>(rect_tmp.y());
                        int rtw = static_cast<int>(rect_tmp.w());
                        int rth = static_cast<int>(rect_tmp.h());
                        if (event.x >= rtx && event.x < rtx + rtw && event.y >= rty && event.y < rty + rth) {
                            // もし選択肢が左クリックされていたら
                            if (i < is_items.size()) {
                                // 項目をオンオフさせる
                                const std::size_t old_index = index;
                                const bool old_value = is_items[i];

                                index = i;
                                is_items[i] = !(is_items[i]);
                                is_open = false;

                                // ★コールバック呼び出し：選択が変更された場合のみ
                                if (on_selection_changed_ && (old_index != index || old_value != is_items[i])) {
                                    on_selection_changed_(index, is_items[i]);
                                }

                                return EventHandlingResult::Handled();
                            }
                        }
                        pos.setY(pos.y() + rect_h);
                    }
                }
            }
            return EventHandlingResult::NotHandled();
        }
        // 描画
        void render() const override {
            if (!visible_ || items_key.empty()) {
                return;
            }
            const std::size_t item_count = items_key.size();
            if (item_count == 0) {
                return;
            }

            const std::size_t item_index = index;
            rect.draw(paxg::Color{ 243, 243, 243 }); // プルダウンの背景を描画
            // プルダウンのふちを描画
            rect.drawFrame(1, 0, is_open ? paxg::Color{ 255, 165, 0 } : paxg::Color{ 128, 128, 128 });

            // 三角形を描画（下向き▼）
            static constexpr paxg::TriangleShape down_arrow_shape(arrow_radius, arrow_rotation_pi);
            const float center_x = static_cast<float>(rect.x() + rect.w() - down_button_size / 2.0 - padding.x());
            const float center_y = static_cast<float>(rect.y() + rect.h() / 2.0);
            paxg::Triangle triangle(center_x, center_y, down_arrow_shape);
            triangle.draw(paxg::Color{ 0, 0, 0 });

            paxg::Vec2i pos = rect.pos();

            const std::uint_least32_t select_key = (is_one_font ? items_key[item_index] : Fonts().getSelectedLanguage().cgetKey());

            // 種別によって描画処理を変える
            if (display_type == PulldownDisplayType::SelectedValue) {
                const std::string* str = nullptr;

                str = Fonts().getText(items_key[index], language_domain);

                if (str == nullptr || str->size() == 0) return;

                paxg::Font* one_font = Fonts().getFont(select_key, FontProfiles::PULLDOWN);
                if (one_font == nullptr) {
                    PAXS_WARNING("Pulldown::render: Font not found.");
                    return;
                }
                // 文字を描画
                (*one_font).draw(
                    *str,
                    paxg::Vec2i(pos.x() + padding.x(), pos.y() + padding.y()), paxg::Color{ 0, 0, 0 });
            }
            else {
                const std::string* str0 = nullptr;

                str0 = Fonts().getText(items_key.front(), language_domain);

                if (str0 == nullptr || str0->size() == 0) return;

                paxg::Font* one_font = Fonts().getFont(select_key, FontProfiles::PULLDOWN);
                if (one_font == nullptr) return;
                // 文字を描画
                (*one_font).draw(
                    *str0,
                    paxg::Vec2i(pos.x() + padding.x(), pos.y() + padding.y()), paxg::Color{ 0, 0, 0 });
            }

            pos.setY(static_cast<int>(pos.y() + rect.h()));
            if (!is_open) return;

            // 四角形を描画
            const paxg::Rect back_rect{ pos, all_rect_x, (rect.h() * item_count) };
            // 影を描画
            back_rect.drawShadow({ shadow_offset_x, shadow_offset_y }, shadow_blur_radius, shadow_spread).draw();
            // FixedHeader モードの場合は最初の項目（ヘッダー名）をスキップ
            const std::size_t start_index = (display_type == PulldownDisplayType::FixedHeader) ? 1 : 0;
            for (std::size_t i = start_index; i < item_count; ++i) {
                const std::string* i_str = nullptr;

                i_str = Fonts().getText(items_key[i], language_domain);

                if (i_str == nullptr || i_str->size() == 0) continue;

                const paxg::Rect rect_tmp{ pos, all_rect_x, rect.h() };
                if (rect_tmp.mouseOver()) { // マウスカーソルが四角形の上にある場合
                    // 四角形の色を変える
                    rect_tmp.draw(paxg::Color{ 135, 206, 235 });
                }
                const std::uint_least32_t select_font_key = ((is_one_font) ? items_key[i] : Fonts().getSelectedLanguage().cgetKey());

                paxg::Font* one_font = Fonts().getFont(select_font_key, FontProfiles::PULLDOWN);
                if (one_font == nullptr) {
                    continue;
                }
                // 文字を描画
                (*one_font).draw(
                    *i_str,
                    paxg::Vec2i(pos.x() + padding.x(), pos.y() + padding.y()), paxg::Color{ 0, 0, 0 });
                pos.setY(static_cast<int>(pos.y() + rect.h()));
            }
            // ふちを描画
            back_rect.drawFrame(1, 0, paxg::Color{ 128, 128, 128 });
        }

        // Pulldown 固有のメソッド
        size_t getIndex() const { return index; }
        // 引数の添え字番号の項目が TRUE か FALSE になっているか調べる
        bool getIsItems(const std::size_t index) const {
            if (is_items.size() == 0) {
                PAXS_WARNING("Pulldown::getIsItems: No items available.");
                return true; // データがない場合
            }
            if (index < is_items.size()) {
                return is_items[index];
            }
            return is_items.front();
        }
        // 引数の Key の項目が TRUE か FALSE になっているか調べる
        bool getIsItemsKey(const std::uint_least32_t key) const {
            if (is_items.size() == 0) {
                PAXS_WARNING("Pulldown::getIsItemsKey: No items available.");
                return true; // データがない場合
            }
            if (item_index_key.find(key) == item_index_key.end()) return true; // 引数の Key が存在しない場合
            return getIsItems(item_index_key.at(key));
        }

        std::uint_least32_t getKey() const { return items_key[index]; }

        // プルダウンの開閉状態を管理
        bool isOpen() const { return is_open; }
        void close() { is_open = false; }

        bool isHit(int x, int y) const override {
            if (!isVisible()) {
                return false;
            }

            // ヘッダー部分のヒットテスト
            const paxg::Rect header_rect = getRect();
            if (x >= header_rect.x() && x < header_rect.x() + header_rect.w() &&
                y >= header_rect.y() && y < header_rect.y() + header_rect.h()) {
                return true;
            }

            // プルダウンが開いている場合は、ドロップダウン項目のヒットテストも行う
            if (is_open) {
                // ドロップダウン項目の幅は all_rect_x（全項目中の最大幅）
                const float item_width = all_rect_x;
                paxg::Vec2i pos = paxg::Vec2i(
                    static_cast<int>(header_rect.x()),
                    static_cast<int>(header_rect.y() + header_rect.h()));

                for (std::size_t i = 0; i < items_key.size(); ++i) {
                    const paxg::Rect item_rect{ pos, item_width, header_rect.h() };
                    if (x >= item_rect.x() && x < item_rect.x() + item_rect.w() &&
                        y >= item_rect.y() && y < item_rect.y() + item_rect.h()) {
                        return true;
                    }
                    pos.setY(static_cast<int>(pos.y() + header_rect.h()));
                }
            }

            return false;
        }

        void setPos(const paxg::Vec2i& pos) override { rect.setPos(pos); }
        paxg::Rect getRect() const override { return rect; }
        void setVisible(bool visible) { visible_ = visible; }
        bool isVisible() const override { return visible_; }
        const char* getName() const override { return "Pulldown"; }
        RenderLayer getLayer() const override { return RenderLayer::UIOverlay; }
    };
}

#endif // !PAX_MAHOROBA_PULLDOWN_HPP
