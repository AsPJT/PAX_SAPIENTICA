/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_MENU_ITEM_HPP
#define PAX_MAHOROBA_MENU_ITEM_HPP

#include <functional>
#include <span>
#include <string>
#include <vector>

#include <PAX_GRAPHICA/Rect.hpp>
#include <PAX_GRAPHICA/Triangle.hpp>

#include <PAX_MAHOROBA/Rendering/IWidget.hpp>
#include <PAX_MAHOROBA/Rendering/FontSystem.hpp>

#include <PAX_SAPIENTICA/Core/Type/UnorderedMap.hpp>
#include <PAX_SAPIENTICA/Utility/MurMur3.hpp>

namespace paxs {

    /// @brief メニューバー用のドロップダウン項目
    class DropDownMenu : public IWidget {
    private:
        // UI要素のサイズ定数
        static constexpr int default_padding_x = 6;
        static constexpr int default_padding_y = 2;
        static constexpr int down_button_size = 20;
        static constexpr int checkmark_width = 20;
        static constexpr int checkmark_x_offset = 5;

        // Locales ドメインキー定数
        static constexpr std::uint_least32_t menubar_domain_key = MurMur3::calcHash("MenuBar");

        // プラットフォーム固有の表示調整定数
        static constexpr float android_width_scale = 2.5f;
        static constexpr float android_rect_width_scale = 2.0f;
        static constexpr float android_height_scale = 1.4f;
        static constexpr float sfml_height_scale = 1.2f;

        // 描画定数
        static constexpr float arrow_radius = 8.0f;
        static constexpr float arrow_rotation_pi = 3.1416f;  // π radians (down)
        static constexpr int shadow_offset_x = 1;
        static constexpr int shadow_offset_y = 1;
        static constexpr int shadow_blur_radius = 4;
        static constexpr int shadow_spread = 1;

        // 項目
        std::span<const std::uint_least32_t> items_key{};

        // フォント設定
        std::uint_least8_t font_size = 16;
        std::uint_least8_t font_buffer_thickness_size = 16;

        // 項目の状態管理
        std::vector<bool> is_items;
        paxs::UnorderedMap<std::uint_least32_t, std::size_t> item_index_key{};

        // レイアウト
        paxg::Rect rect;
        paxg::Vec2i padding{ default_padding_x, default_padding_y };
        float all_rect_width{}; // 全ての項目の文字幅

        // 状態
        bool visible_ = false;  // ドロップダウンの表示状態（MenuBarが制御）

        std::uint_least32_t old_language_key = 0;

    public:
        DropDownMenu() = default;

        /// @brief コンストラクタ
        /// @param items_key_ 項目のキー一覧（最初の項目がヘッダー名）
        /// @param font_size_ フォントサイズ
        /// @param font_buffer_thickness_size_ フォントの太さ
        /// @param pos_ 表示位置
        DropDownMenu(
            const std::span<const std::uint_least32_t> items_key_,
            std::uint_least8_t font_size_,
            std::uint_least8_t font_buffer_thickness_size_,
            const paxg::Vec2i& pos_ = { 0, 0 })
        {
            items_key = items_key_;
            font_size = font_size_;
            font_buffer_thickness_size = font_buffer_thickness_size_;
            rect = paxg::Rect{ static_cast<float>(pos_.x()), static_cast<float>(pos_.y()), 0, 0 };

            updateLanguage();

            // Key List を作成
            for (std::size_t i = 0; i < items_key_.size(); ++i) {
                item_index_key.emplace(items_key_[i], i);
            }

            // 項目の状態を初期化（全てtrue）
            is_items.resize(items_key_.size(), true);
        }

        /// @brief X 座標を設定
        void setRectX(const float x = 0) {
            rect.setX(x);
        }

        /// @brief 言語変更による更新処理
        void updateLanguage() {
            paxg::Font* one_font = Fonts().getFont(font_size, font_buffer_thickness_size);
            if (one_font == nullptr) {
                rect.setH(static_cast<float>(font_size) * 2.f);
            }
            else {
                const float height = static_cast<float>(((*one_font).height()) + padding.y() * 2);
                rect.setH(height);
            }

            rect.setW(0);
            all_rect_width = 0;
            for (std::size_t i = 0; i < items_key.size(); ++i) {
                const std::string* str = Fonts().getLocalesText(menubar_domain_key, items_key[i]);
                if (str == nullptr || str->size() == 0) continue;

                paxg::Font* item_font = Fonts().getFont(font_size, font_buffer_thickness_size);
                if (item_font == nullptr) continue;

                all_rect_width = (std::max)(all_rect_width, static_cast<float>((*item_font).width(*str)));

                // 最初の項目（ヘッダー）の幅を使用
                if (i == 0) {
                    rect.setW(static_cast<float>((*item_font).width(*str)));
                }
            }

            rect.setW(rect.w() + (padding.x() * 2 + down_button_size));
            all_rect_width += (padding.x() * 2 + down_button_size);

            // チェックマークの幅を追加
            all_rect_width += checkmark_width;

#if defined(PAXS_USING_DXLIB) && (__ANDROID__)
            all_rect_x *= android_width_scale;
            rect.setW(rect.w() * android_rect_width_scale);
            rect.setH(rect.h() * android_height_scale);
#elif defined(PAXS_USING_SFML)
            rect.setH(rect.h() * sfml_height_scale);
#endif
        }

        /// @brief 入力処理（
        EventHandlingResult handleEvent(const MouseEvent& event) override {
            paxg::Vec2i pos = rect.pos();
            pos.setY((int)(pos.y() + rect.h()));

            for (std::size_t i = 1; i < items_key.size(); ++i) {
                const paxg::Rect item_rect{ pos, all_rect_width, rect.h() };
                if (item_rect.contains(static_cast<float>(event.pos.x), static_cast<float>(event.pos.y))) {
                    // もともとやってたトグル処理
                    if (i < is_items.size()) {
                        const bool old_value = is_items[i];
                        is_items[i] = !is_items[i];

                        // ★コールバック呼び出し：チェック状態が変更された
                        if (on_item_toggled_ && old_value != is_items[i]) {
                            on_item_toggled_(i, is_items[i]);
                        }
                    }
                    // 押されたら閉じる
                    visible_ = false;
                    return EventHandlingResult::Handled();
                }
                pos.setY((int)(pos.y() + rect.h()));
            }
            return EventHandlingResult::NotHandled();
        }

        /// @brief 描画処理
        void render() const override {
            if (items_key.size() == 0) {
                return;
            }

            // ヘッダーの背景と枠を描画（常に表示）
            rect.draw(paxg::Color{ 243, 243, 243 });
            rect.drawFrame(1, 0, visible_ ? paxg::Color{ 255, 165, 0 } : paxg::Color{ 128, 128, 128 });

            // 下向き三角形を描画
            static constexpr paxg::TriangleShape down_arrow_shape(arrow_radius, arrow_rotation_pi);
            const float center_x = static_cast<float>(rect.x() + rect.w() - down_button_size / 2.0 - padding.x());
            const float center_y = static_cast<float>(rect.y() + rect.h() / 2.0);
            paxg::Triangle triangle(center_x, center_y, down_arrow_shape);
            triangle.draw(paxg::Color{ 0, 0, 0 });

            // ヘッダー部分のテキストを描画
            drawHeader();

            // ドロップダウンリストを描画（visible_がtrueの時のみ）
            if (visible_) {
                drawDropdownList();
            }
        }

        /// @brief プルダウンを閉じる
        void close() { visible_ = false; }

        /// @brief 項目トグル時のコールバックを設定
        /// @brief Set callback for item toggle
        /// @param callback コールバック関数（引数: index, is_checked）
        void setOnItemToggled(std::function<void(std::size_t, bool)> callback) {
            on_item_toggled_ = std::move(callback);
        }

        /// @brief 項目の状態を設定（インデックス指定）
        void setIsItems(const std::size_t index, const bool new_value) {
            if (is_items.size() == 0) {
                PAXS_WARNING("DropDownMenu: No items to set state for.");
                return;
            }
            const std::size_t actual_index = index + 1; // ヘッダー分をオフセット
            if (actual_index < is_items.size()) {
                is_items[actual_index] = new_value;
            }
        }

        /// @brief 項目の状態を設定（キー指定）
        void setIsItems(const std::uint_least32_t key, const bool is_item) {
            if (item_index_key.find(key) == item_index_key.end()) {
                PAXS_WARNING("DropDownMenu: Key not found in item_index_key.");
                return;
            }
            const std::size_t index = item_index_key.at(key);
            if (index < is_items.size()) {
                is_items[index] = is_item;
            }
        }

        /// @brief 項目の状態を取得（インデックス指定）
        /// @details MenuItemでは最初の項目がヘッダー名なので、+1してアクセス
        bool getIsItems(const std::size_t index) const {
            if (is_items.size() == 0) {
                PAXS_WARNING("DropDownMenu: No items to get state from.");
                return true;
            }
            const std::size_t actual_index = index + 1; // ヘッダー分をオフセット
            if (actual_index < is_items.size()) {
                return is_items[actual_index];
            }
            return is_items.front();
        }

        /// @brief 項目の状態を取得（キー指定）
        bool getIsItems(const std::uint_least32_t key) const {
            if (!item_index_key.contains(key)) {
                PAXS_WARNING("DropDownMenu: Key not found in item_index_key.");
                return true;
            }
            const std::size_t index = item_index_key.at(key);
            // キーで取得する場合、インデックスは既に正しい位置を指しているので
            // ヘッダー（インデックス0）を参照している場合のみスキップ
            if (index == 0) {
                return true; // ヘッダー自体は常にtrue
            }
            if (index < is_items.size()) {
                return is_items[index];
            }
            return is_items.front();
        }

        /// @brief 引数の Key の項目が TRUE か FALSE になっているか調べる
        bool getIsItemsKey(const std::uint_least32_t key) const {
            if (is_items.size() == 0) {
                PAXS_WARNING("DropDownMenu: No items to check for key.");
                return true; // データがない場合
            }
            if (!item_index_key.contains(key)) {
                PAXS_WARNING("DropDownMenu: Key not found in item_index_key.");
                return true; // 引数の Key が存在しない場合
            }
            return getIsItems(item_index_key.at(key));
        }

        bool isHitHeader(int x, int y) const {
            const paxg::Rect header_rect = getRect();
            bool result = header_rect.contains(static_cast<float>(x), static_cast<float>(y));
            return result;
        }

        bool isHit(const paxs::Vector2<int>& mouse_pos) const override {
            if (!visible_) {
                return false;
            }

            // ヘッダーは親(MenuSystem)が判定するのでここでは見ない

            paxg::Vec2i pos = rect.pos();
            pos.setY((int)(pos.y() + rect.h()));

            for (std::size_t i = 1; i < items_key.size(); ++i) {
                const paxg::Rect item_rect{ pos, all_rect_width, rect.h() };
                if (item_rect.contains((float)mouse_pos.x, (float)mouse_pos.y)) {
                    return true;
                }
                pos.setY((int)(pos.y() + rect.h()));
            }
            return false;
        }

        void setPos(const Vector2<int>& pos) override {
            rect.setPos(paxg::Vec2i(pos.x, pos.y));
        }
        Rect<int> getRect() const override {
            return {
                static_cast<int>(rect.x()),
                static_cast<int>(rect.y()),
                static_cast<int>(rect.w()),
                static_cast<int>(rect.h())
            };
        }
        bool isVisible() const override { return visible_; }
        void setVisible(bool visible) { visible_ = visible; }
        const char* getName() const override { return "DropDownMenu"; }
        RenderLayer getLayer() const override { return RenderLayer::MenuBar; }

    private:
        /// @brief ヘッダー部分のテキストを描画
        /// @details 最初の項目（メニュー名）を常に表示
        void drawHeader() const {
            if (items_key.size() == 0) {
                PAXS_WARNING("DropDownMenu: No items to display in header.");
                return;
            }

            const std::string* str = Fonts().getLocalesText(menubar_domain_key, items_key.front());
            if (str == nullptr || str->size() == 0) {
                PAXS_WARNING("DropDownMenu: Missing text for header item.");
                return;
            }

            paxg::Font* one_font = Fonts().getFont(font_size, font_buffer_thickness_size);
            if (one_font == nullptr) {
                PAXS_WARNING("DropDownMenu: Missing font for header item.");
                return;
            }

            (*one_font).draw(
                *str,
                paxg::Vec2i(static_cast<int>(rect.x() + padding.x()), static_cast<int>(rect.y() + padding.y())),
                paxg::Color{ 0, 0, 0 });
        }

        /// @brief ドロップダウンリストを描画（チェックマーク付き）
        void drawDropdownList() const {
            paxg::Vec2i pos = rect.pos();
            pos.setY(static_cast<int>(pos.y() + rect.h()));

            // 最初の項目（ヘッダー）をスキップ
            const std::size_t display_item_count = items_key.size() - 1;
            const paxg::Rect back_rect{ pos, all_rect_width, static_cast<float>(rect.h() * display_item_count) };
            back_rect.drawShadow({ shadow_offset_x, shadow_offset_y }, shadow_blur_radius, shadow_spread).draw();

            for (std::size_t i = 1; i < items_key.size(); ++i) {
                const std::string* i_str = Fonts().getLocalesText(menubar_domain_key, items_key[i]);
                if (i_str == nullptr || i_str->size() == 0) continue;

                const paxg::Rect rect_tmp{ pos, all_rect_width, rect.h() };
                if (rect_tmp.mouseOver()) {
                    rect_tmp.draw(paxg::Color{ 135, 206, 235 });
                }

                // チェックマークを描画（ONの場合のみ）
                if (i < is_items.size() && is_items[i]) {
                    const int check_x = pos.x() + checkmark_x_offset;

                    // シンプルなチェックマーク "✓" を描画
                    paxg::Font* check_font = Fonts().getFont(font_size, font_buffer_thickness_size);
                    if (check_font != nullptr) {
                        (*check_font).draw(
                            reinterpret_cast<const char*>(u8"✓"),
                            paxg::Vec2i(check_x, pos.y() + padding.y()),
                            paxg::Color{ 0, 0, 0 });
                    }
                }

                // テキストを描画（チェックマークの分だけ右にずらす）
                paxg::Font* one_font = Fonts().getFont(font_size, font_buffer_thickness_size);
                if (one_font == nullptr) continue;

                (*one_font).draw(
                    *i_str,
                    paxg::Vec2i(pos.x() + padding.x() + checkmark_width, pos.y() + padding.y()),
                    paxg::Color{ 0, 0, 0 });
                pos.setY(static_cast<int>(pos.y() + rect.h()));
            }

            back_rect.drawFrame(1, 0, paxg::Color{ 128, 128, 128 });
        }

        // コールバック関数
        std::function<void(std::size_t index, bool is_checked)> on_item_toggled_;
    };

} // namespace paxs

#endif // !PAX_MAHOROBA_MENU_ITEM_HPP
