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
#include <limits>
#include <string>
#include <vector>
#include <span>

#include <PAX_GRAPHICA/Rect.hpp>
#include <PAX_GRAPHICA/Triangle.hpp>

#include <PAX_MAHOROBA/Rendering/IWidget.hpp>
#include <PAX_MAHOROBA/Rendering/FontSystem.hpp>
#include <PAX_MAHOROBA/Core/Init.hpp>

#include <PAX_SAPIENTICA/UnorderedMap.hpp>
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
    public:
        Pulldown() = default;

        // X を指定したサイズに変更
        void setRectX(const std::size_t x = 0) {
            rect.setX(static_cast<float>(x));
        }
        // 言語変更による更新処理
        void updateLanguage() {
            const std::uint_least32_t select_key = Fonts().getSelectedLanguage().cgetKey();

            paxg::Font* one_font = Fonts().getFont(select_key, font_size, font_buffer_thickness_size);
            if (one_font == nullptr) {
                rect.setH(static_cast<float>(font_size) * 2.f);
            }
            else {
                const float height = static_cast<float>(((*one_font).height()) + padding.y() * 2);
                rect.setH(height);
            }

            rect.setW(0);
            all_rect_x = 0;

            // 直接文字列リストが設定されている場合はそれを使用
            const bool use_direct_text = !display_text_list.empty();
            const std::size_t item_count = use_direct_text ? display_text_list.size() : items_key.size();

            for (std::size_t i = 0; i < item_count; ++i) {
                const std::string* str = nullptr;
                std::string direct_str;

                if (use_direct_text) {
                    // 直接文字列を使用
                    direct_str = display_text_list[i];
                    str = &direct_str;
                } else {
                    // 言語辞書から取得
                    str = Fonts().getText(items_key[i], language_domain);
                }

                if (str == nullptr || str->size() == 0) continue;

                const std::uint_least32_t font_key = ((is_one_font && !use_direct_text) ? items_key[i] : select_key);
                paxg::Font* item_font = Fonts().getFont(font_key, font_size, font_buffer_thickness_size);
                if (item_font == nullptr) continue;

                // 最大の文字数からプルダウンの各項目の幅を定義
                all_rect_x =
                (
                    static_cast<float>((std::max)(static_cast<int>(all_rect_x), static_cast<int>((*item_font).width(*str))))
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
            all_rect_x *= 2.5f;
            rect.setW(rect.w() * 2.0f);
            rect.setH(rect.h() * 1.4f);
#endif
#elif defined(PAXS_USING_SFML)
            rect.setH(rect.h() * 1.2f);
#endif
        }
        /// @brief コンストラクタ
        /// @param items_key_ 項目のキー一覧
        /// @param font_size_ フォントサイズ
        /// @param font_buffer_thickness_size_ フォントの太さ
        /// @param pos_ 表示位置
        /// @param display_type_ 表示タイプ（SelectedValue or FixedHeader）
        /// @param is_one_font_ 単一フォントを使用するか
        Pulldown(
            std::uint_least8_t font_size_,
            std::uint_least8_t font_buffer_thickness_size_,
            const LanguageDomain language_domain_,
            const paxg::Vec2i& pos_ = { 0,0 },
            PulldownDisplayType display_type_ = PulldownDisplayType::SelectedValue,
            const bool is_one_font_ = false)
            :
            font_size(font_size_)
            , font_buffer_thickness_size(font_buffer_thickness_size_)
            , language_domain(language_domain_)
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

        // からか判定
        bool isEmpty() const {
            return items_key.empty();
        }

        // 入力処理
        EventHandlingResult handleEvent(const MouseEvent& event) override {
            if (isEmpty()) return EventHandlingResult::NotHandled();
            if (!visible_ || !enabled_) return EventHandlingResult::NotHandled(); // 非表示または無効の場合は処理をしない

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
                float rx = rect.x();
                float ry = rect.y();
                float rw = rect.w();
                float rh = rect.h();

                // ヘッダー部分
                if (event.x >= rx && event.x < rx + rw && event.y >= ry && event.y < ry + rh) {
                    return EventHandlingResult::Handled();
                }

                // ドロップダウン項目（開いている場合）
                if (is_open) {
                    paxg::Vec2i pos = paxg::Vec2i(
                        static_cast<int>(rect.pos().x()),
                        static_cast<int>(rect.pos().y() + rect.h()));
                    for (std::size_t i = 0; i < items_key.size(); ++i) {
                        const paxg::Rect rect_tmp{ pos, rect.w(), rect.h() };
                        float rtx = rect_tmp.x();
                        float rty = rect_tmp.y();
                        float rtw = rect_tmp.w();
                        float rth = rect_tmp.h();
                        if (event.x >= rtx && event.x < rtx + rtw && event.y >= rty && event.y < rty + rth) {
                            return EventHandlingResult::Handled();
                        }
                        pos.setY(static_cast<int>(pos.y() + rect.h()));
                    }
                }
            }

            // Releasedイベント：実際の処理を行う
            if (event.left_button_state == MouseButtonState::Released) {
                float rx = rect.x();
                float ry = rect.y();
                float rw = rect.w();
                float rh = rect.h();

                // ヘッダー部分をクリック：開閉をトグル
                if (event.x >= rx && event.x < rx + rw && event.y >= ry && event.y < ry + rh) {
                    is_open = (not is_open);
                    return EventHandlingResult::Handled();
                }

                // ドロップダウン項目をクリック（開いている場合）
                if (is_open) {
                    paxg::Vec2i pos = paxg::Vec2i(
                        static_cast<int>(rect.pos().x()),
                        static_cast<int>(rect.pos().y() + rect.h()));
                    for (std::size_t i = 0; i < items_key.size(); ++i) {
                        const paxg::Rect rect_tmp{ pos, rect.w(), rect.h() };
                        float rtx = rect_tmp.x();
                        float rty = rect_tmp.y();
                        float rtw = rect_tmp.w();
                        float rth = rect_tmp.h();
                        if (event.x >= rtx && event.x < rtx + rtw && event.y >= rty && event.y < rty + rth) {
                            // もし選択肢が左クリックされていたら
                            if (i < is_items.size()) {
                                // 項目をオンオフさせる
                                index = i;
                                is_items[i] = !(is_items[i]);
                                is_open = false;
                                return EventHandlingResult::Handled();
                            }
                        }
                        pos.setY(static_cast<int>(pos.y() + rect.h()));
                    }
                }
            }
            return EventHandlingResult::NotHandled();
        }
        // 描画
        void render() const override {
            if (!visible_ || isEmpty()) return;
            const std::size_t item_count = items_key.size();
            if (item_count == 0) return;

            const std::size_t item_index = index;
            rect.draw(paxg::Color{ 243, 243, 243 }); // プルダウンの背景を描画
            // プルダウンのふちを描画
            rect.drawFrame(1, 0, is_open ? paxg::Color{ 255, 165, 0 } : paxg::Color{ 128, 128, 128 });

            // 三角形を描画（下向き▼）
            constexpr float radius = 8.0f;
            static constexpr paxg::TriangleShape down_arrow_shape(radius, 3.1416f); // π radians = down
            const float center_x = static_cast<float>(rect.x() + rect.w() - down_button_size / 2.0 - padding.x());
            const float center_y = static_cast<float>(rect.y() + rect.h() / 2.0);
            paxg::Triangle triangle(center_x, center_y, down_arrow_shape);
            triangle.draw(paxg::Color{ 0, 0, 0 });

            paxg::Vec2i pos = rect.pos();

            const std::uint_least32_t select_key = (is_one_font ? items_key[item_index] : Fonts().getSelectedLanguage().cgetKey());

            // 種別によって描画処理を変える
            if (display_type == PulldownDisplayType::SelectedValue) {
                const std::string* str = nullptr;
                std::string direct_str;

                str = Fonts().getText(items_key[index], language_domain);

                if (str == nullptr || str->size() == 0) return;

                paxg::Font* one_font = Fonts().getFont(select_key, font_size, font_buffer_thickness_size);
                if (one_font == nullptr) return;
                // 文字を描画
                (*one_font).draw(
                    *str,
                    paxg::Vec2i(pos.x() + padding.x(), pos.y() + padding.y()), paxg::Color{ 0, 0, 0 });
            }
            else {
                const std::string* str0 = nullptr;
                std::string direct_str;

                str0 = Fonts().getText(items_key.front(), language_domain);

                if (str0 == nullptr || str0->size() == 0) return;

                paxg::Font* one_font = Fonts().getFont(select_key, font_size, font_buffer_thickness_size);
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
            back_rect.drawShadow({ 1, 1 }, 4, 1).draw();
            // FixedHeader モードの場合は最初の項目（ヘッダー名）をスキップ
            const std::size_t start_index = (display_type == PulldownDisplayType::FixedHeader) ? 1 : 0;
            for (std::size_t i = start_index; i < item_count; ++i) {
                const std::string* i_str = nullptr;
                std::string direct_str;

                i_str = Fonts().getText(items_key[i], language_domain);

                if (i_str == nullptr || i_str->size() == 0) continue;

                const paxg::Rect rect_tmp{ pos, all_rect_x, rect.h() };
                if (rect_tmp.mouseOver()) { // マウスカーソルが四角形の上にある場合
                    // 四角形の色を変える
                    rect_tmp.draw(paxg::Color{ 135, 206, 235 });
                }
                const std::uint_least32_t select_font_key = ((is_one_font) ? items_key[i] : Fonts().getSelectedLanguage().cgetKey());

                paxg::Font* one_font = Fonts().getFont(select_font_key, font_size, font_buffer_thickness_size);
                if (one_font == nullptr) continue;
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
        bool getIsItems(const std::size_t i) const {
            if (is_items.size() == 0) return true; // データがない場合
            if (i < is_items.size()) return is_items[i];
            return is_items.front();
        }
        // 引数の Key の項目が TRUE か FALSE になっているか調べる
        bool getIsItemsKey(const std::uint_least32_t key) const {
            if (is_items.size() == 0) return true; // データがない場合
            if (item_index_key.find(key) == item_index_key.end()) return true; // 引数の Key が存在しない場合
            return getIsItems(item_index_key.at(key));
        }

        std::uint_least32_t getKey() const { return items_key[index]; }

        // プルダウンの開閉状態を管理
        bool isOpen() const { return is_open; }
        void close() { is_open = false; }

        // IWidget インターフェースの実装
        void setPos(const paxg::Vec2i& pos) override { rect.setPos(pos); }
        paxg::Rect getRect() const override { return rect; }

        void setVisible(bool visible) override { visible_ = visible; }
        bool isVisible() const override { return visible_; }

        void setEnabled(bool enabled) override { enabled_ = enabled; }
        bool isEnabled() const override { return enabled_; }

        const char* getName() const override { return "Pulldown"; }

        /// @brief レンダリングレイヤーを取得
        /// @brief Get rendering layer
        /// @note プルダウンは常に最前面（UIOverlay）で描画・入力処理される
        RenderLayer getLayer() const override {
            return RenderLayer::UIOverlay;
        }

        /// @brief ヒットテスト（プルダウンが開いている場合は全項目を含む）
        /// @brief Hit test (includes all items when pulldown is open)
        /// @param x X座標 / X coordinate
        /// @param y Y座標 / Y coordinate
        /// @return 範囲内ならtrue / true if within bounds
        bool isHit(int x, int y) const override {
            if (!isVisible() || !isEnabled()) return false;

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

    private:
        std::span<const std::uint_least32_t> items_key{}; // 項目の Key 一覧
        std::vector<std::string> display_text_list; // 表示する文字列リスト（言語辞書を経由しない場合）

        std::size_t language_index = 0; // 言語の要素番号
        std::uint_least32_t old_language_key = 0; // 選択されている言語の Key
        std::uint_least8_t font_size = 16;
        std::uint_least8_t font_buffer_thickness_size = 16;
        std::vector<bool> is_items{}; // 項目が TRUE か FALSE になっているか格納
        paxs::UnorderedMap<std::uint_least32_t, std::size_t> item_index_key{}; // 項目の Key を格納

        paxg::Rect rect{};
        PulldownDisplayType display_type{}; // 表示タイプ (SelectedValue or FixedHeader)
        bool is_one_font = false;

        const LanguageDomain language_domain{}; // 言語ドメイン

        size_t index = 0;
        paxg::Vec2i padding{ 6, 2 };
        float all_rect_x{}; // 全ての項目の文字幅
        static constexpr int down_button_size = 20;
        bool is_open = false;

        // IWidget インターフェース用の状態
        bool visible_ = true;
        bool enabled_ = true;
    };
}

#endif // !PAX_MAHOROBA_PULLDOWN_HPP
