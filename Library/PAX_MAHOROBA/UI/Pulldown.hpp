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
#include <PAX_MAHOROBA/Rendering/LanguageFonts.hpp>
#include <PAX_MAHOROBA/Core/Init.hpp>

#include <PAX_SAPIENTICA/Language.hpp>
#include <PAX_SAPIENTICA/InputStateManager.hpp>
#include <PAX_SAPIENTICA/UnorderedMap.hpp>
namespace paxs {

    /// @brief プルダウンの表示タイプ
    /// @brief Display type for pulldown
    enum class PulldownDisplayType : std::size_t {
        /// @brief 選択された値をヘッダーに表示（通常のプルダウン）
        /// @brief Display selected value in header (normal pulldown)
        /// @details 例: 言語選択で「日本語」「English」などが表示される
        SelectedValue = 0,

        /// @brief 固定のヘッダー名を表示（メニューバー用）
        /// @brief Display fixed header name (for menu bar)
        /// @details 例: 「ファイル」「編集」「表示」などのメニュー名が表示される
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
            if (language_ptr == nullptr) return; // 言語がない場合は処理をしない
            if (select_language_ptr == nullptr) return; // 選択されている言語がない場合は処理をしない

            if (font == nullptr) {
                rect.setH(static_cast<float>(font_size) * 2.f);
            }
            else {
                const std::uint_least32_t select_key = ((is_one_font) ? items_key[index] : (*select_language_ptr).cgetKey());
                paxg::Font* one_font = (*font).getAndAdd(select_key, font_size, font_buffer_thickness_size);
                if (one_font == nullptr) {
                    rect.setH(static_cast<float>(font_size) * 2.f);
                }
                else {
                    const float height = static_cast<float>(((*one_font).height()) + padding.y() * 2);
                    rect.setH(height);
                }
            }

            rect.setW(0);
            all_rect_x = 0;
            for (std::size_t i = 0; i < items_key.size(); ++i) {
                const std::string* str = (*language_ptr).getStringPtr(items_key[i], (*select_language_ptr).cgetKey());
                if (str == nullptr) continue;
                if (str->size() == 0) continue;

                const std::uint_least32_t select_key = ((is_one_font) ? items_key[i] : (*select_language_ptr).cgetKey());

                paxg::Font* one_font = (*font).getAndAdd(select_key, font_size, font_buffer_thickness_size);
                if (one_font == nullptr) continue;

                // 最大の文字数からプルダウンの各項目の幅を定義
                //rect.setW
                all_rect_x =
                (
                    static_cast<float>((std::max)(static_cast<int>(all_rect_x), static_cast<int>((*one_font).width(*str))))
                );
                // フォントが１つの場合は１行目も項目と同じ幅にする
                if (is_one_font) {
                    rect.setW(all_rect_x);
                }
                else {
                    if (i == 0) {
                        rect.setW(static_cast<float>((*one_font).width(*str)));
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
        /// @param select_language_ptr_ 選択されている言語
        /// @param language_ptr_ 言語データ
        /// @param items_key_ 項目のキー一覧
        /// @param font_ フォント
        /// @param font_size_ フォントサイズ
        /// @param font_buffer_thickness_size_ フォントの太さ
        /// @param pos_ 表示位置
        /// @param display_type_ 表示タイプ（SelectedValue or FixedHeader）
        /// @param is_one_font_ 単一フォントを使用するか
        Pulldown(
            const SelectLanguage* select_language_ptr_,
            const Language* language_ptr_,
            const std::span<const std::uint_least32_t> items_key_,
            LanguageFonts& font_,
            std::uint_least8_t font_size_,
            std::uint_least8_t font_buffer_thickness_size_,
            const paxg::Vec2i& pos_ = { 0,0 },
            PulldownDisplayType display_type_ = PulldownDisplayType::SelectedValue,
            const bool is_one_font_ = false)
            :
            select_language_ptr(select_language_ptr_)
            , language_ptr(language_ptr_)
            , items_key(items_key_)
            , font{ &font_ }
            , font_size(font_size_)
            , font_buffer_thickness_size(font_buffer_thickness_size_)
            , rect{ static_cast<float>(pos_.x()), static_cast<float>(pos_.y()),0, 0 }
            , display_type(display_type_)
            , is_one_font(is_one_font_) {

            updateLanguage();

            // Key List を作成
            for (std::size_t i = 0; i < items_key_.size(); ++i) {
                item_index_key.emplace(items_key_[i], i);
                is_items.emplace_back(true);
            }
        }
        // からか判定
        bool isEmpty() const {
            return items_key.empty();
        }

        // 入力処理
        InputHandlingResult handleInput(const InputEvent& event) override {
            if (isEmpty()) return InputHandlingResult::NotHandled();
            if (language_ptr == nullptr) return InputHandlingResult::NotHandled(); // 言語がない場合は処理をしない
            if (select_language_ptr == nullptr) return InputHandlingResult::NotHandled(); // 選択されている言語がない場合は処理をしない
            if (font == nullptr) return InputHandlingResult::NotHandled();
            if (!visible_ || !enabled_) return InputHandlingResult::NotHandled(); // 非表示または無効の場合は処理をしない
            if (event.input_state_manager == nullptr) return InputHandlingResult::NotHandled();
            paxs::InputStateManager& input_state_manager = *event.input_state_manager;

            // 言語が変わっていたら更新処理
            if (old_language_key != (*select_language_ptr).cgetKey()) {
                language_index = (*select_language_ptr).cget();
                old_language_key = (*select_language_ptr).cgetKey();
                updateLanguage();
            }
            // 左クリックで開く
            if (input_state_manager.get(
                rect.leftClicked()
            )) {
                is_open = (not is_open);
                return InputHandlingResult::Handled();
            }
            paxg::Vec2i pos = paxg::Vec2i(
                static_cast<int>(rect.pos().x()),
                static_cast<int>(rect.pos().y() + rect.h()));
            if (is_open) { // 項目が開かれている場合
                // その項目の選択肢
                for (std::size_t i = 0; i < items_key.size(); ++i) {
                    const paxg::Rect rect_tmp{ pos, rect.w(), rect.h() };
                    if (input_state_manager.get(
                        rect_tmp.leftClicked()
                    )) { // もし選択肢が左クリックされていたら
                        if (i < is_items.size()) {
                            // 項目をオンオフさせる
                            index = i;
                            is_items[i] = !(is_items[i]);
                            is_open = false;
                            return InputHandlingResult::Handled();
                        }
                    }
                    pos.setY(static_cast<int>(pos.y() + rect.h()));
                }
            }
            return InputHandlingResult::NotHandled();
        }
        // 描画
        void render() const override {
            if (isEmpty()) {
                return;
            }
            // 言語がない場合は処理をしない
            if (language_ptr == nullptr) {
                return;
            }
            // 選択されている言語がない場合は処理をしない
            if (select_language_ptr == nullptr) {
                return;
            }
            // 項目がない場合は処理をしない
            if (items_key.size() == 0) {
                return;
            }
            if (!visible_) return; // 非表示の場合は描画しない

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

            const std::uint_least32_t select_key = ((is_one_font) ? items_key[item_index] : (*select_language_ptr).cgetKey());

            // 種別によって描画処理を変える
            // SelectedValue: 選択された値を表示（言語選択など）
            // FixedHeader: 固定のヘッダー名を表示（メニューバーなど）
            if (display_type == PulldownDisplayType::SelectedValue) {
                const std::string* str = (*language_ptr).getStringPtr(items_key[index], (*select_language_ptr).cgetKey());
                if (str == nullptr) {
                    return;
                }
                if (str->size() == 0) {
                    return;
                }

                paxg::Font* one_font = (*font).getAndAdd(select_key, font_size, font_buffer_thickness_size);
                if (one_font == nullptr) {
                    return;
                }
                // 文字を描画
                (*one_font).draw(
                    *str,
                    paxg::Vec2i(pos.x() + padding.x(), pos.y() + padding.y()), paxg::Color{ 0, 0, 0 });
            }
            else {
                const std::string* str0 = (*language_ptr).getStringPtr(items_key.front(), (*select_language_ptr).cgetKey());
                if (str0 == nullptr) {
                    return;
                }
                if (str0->size() == 0) {
                    return;
                }

                paxg::Font* one_font = (*font).getAndAdd(select_key, font_size, font_buffer_thickness_size);
                if (one_font == nullptr) {
                    return;
                }
                // 文字を描画
                (*one_font).draw(
                    *str0,
                    paxg::Vec2i(pos.x() + padding.x(), pos.y() + padding.y()), paxg::Color{ 0, 0, 0 });
            }

            pos.setY(static_cast<int>(pos.y() + rect.h()));
            if (!is_open) {
                return;
            }

            // 四角形を描画
            const paxg::Rect back_rect{ pos, all_rect_x, (rect.h() * items_key.size()) };
            // 影を描画
            back_rect.drawShadow({ 1, 1 }, 4, 1).draw();
            // FixedHeader モードの場合は最初の項目（ヘッダー名）をスキップ
            const std::size_t start_index = (display_type == PulldownDisplayType::FixedHeader) ? 1 : 0;
            for (std::size_t i = start_index; i < items_key.size(); ++i) {

                const std::string* i_str = (*language_ptr).getStringPtr(items_key[i], (*select_language_ptr).cgetKey());
                if (i_str == nullptr) continue;
                if (i_str->size() == 0) continue;

                const paxg::Rect rect_tmp{ pos, all_rect_x, rect.h() };
                if (rect_tmp.mouseOver()) { // マウスカーソルが四角形の上にある場合
                    // 四角形の色を変える
                    rect_tmp.draw(paxg::Color{ 135, 206, 235 });
                }
                const std::uint_least32_t select_font_key = ((is_one_font) ? items_key[i] : (*select_language_ptr).cgetKey());

                paxg::Font* one_font = (*font).getAndAdd(select_font_key, font_size, font_buffer_thickness_size);
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
        RenderLayer getLayer() const override {
            return is_open ? RenderLayer::UIOverlay : RenderLayer::UIContent;
        }

        bool isAvailable() const override { return true; }

    private:
        const SelectLanguage* select_language_ptr = nullptr; // 選択されている言語
        const Language* language_ptr = nullptr; // 言語
        std::span<const std::uint_least32_t> items_key{}; // 項目の Key 一覧
        LanguageFonts* font = nullptr;

        std::size_t language_index = 0; // 言語の要素番号
        std::uint_least32_t old_language_key = 0; // 選択されている言語の Key
        std::uint_least8_t font_size = 16;
        std::uint_least8_t font_buffer_thickness_size = 16;
        std::vector<bool> is_items{}; // 項目が TRUE か FALSE になっているか格納
        paxs::UnorderedMap<std::uint_least32_t, std::size_t> item_index_key{}; // 項目の Key を格納

        paxg::Rect rect{};
        PulldownDisplayType display_type{}; // 表示タイプ (SelectedValue or FixedHeader)
        bool is_one_font = false;

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
