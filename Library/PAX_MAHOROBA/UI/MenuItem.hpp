/*##########################################################################################

    PAX SAPIENTICA Library 💀🌿🌏

    [Planning]		2023-2024 As Project
    [Production]	2023-2024 As Project
    [Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
    [License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_MENU_ITEM_HPP
#define PAX_MAHOROBA_MENU_ITEM_HPP

#include <PAX_MAHOROBA/UI/DropdownBase.hpp>

namespace paxs {

    /// @brief メニューバー用のドロップダウン項目
    /// @brief Dropdown item for menu bar
    /// @details 固定のヘッダー名を表示し、項目選択時に状態を切り替える
    ///          例：「ファイル」「編集」「表示」などのメニュー
    class MenuItem : public DropdownBase {
    public:
        MenuItem() = default;

        /// @brief コンストラクタ
        /// @param select_language_ptr_ 選択されている言語
        /// @param language_ptr_ 言語データ
        /// @param items_key_ 項目のキー一覧（最初の項目がヘッダー名）
        /// @param font_ フォント
        /// @param font_size_ フォントサイズ
        /// @param font_buffer_thickness_size_ フォントの太さ
        /// @param pos_ 表示位置
        MenuItem(
            const SelectLanguage* select_language_ptr_,
            const Language* language_ptr_,
            const std::span<const std::uint_least32_t> items_key_,
            LanguageFonts& font_,
            std::uint_least8_t font_size_,
            std::uint_least8_t font_buffer_thickness_size_,
            const paxg::Vec2i& pos_ = { 0, 0 })
        {
            select_language_ptr = select_language_ptr_;
            language_ptr = language_ptr_;
            items_key = items_key_;
            font = &font_;
            font_size = font_size_;
            font_buffer_thickness_size = font_buffer_thickness_size_;
            rect = paxg::Rect{ static_cast<float>(pos_.x()), static_cast<float>(pos_.y()), 0, 0 };
            is_one_font = false;

            updateLanguage();

            // Key List を作成
            for (std::size_t i = 0; i < items_key_.size(); ++i) {
                item_index_key.emplace(items_key_[i], i);
            }

            // 項目の状態を初期化（全てtrue）
            is_items.resize(items_key_.size(), true);
        }

    protected:
        /// @brief ヘッダー部分のテキストを描画
        /// @details 最初の項目（メニュー名）を常に表示
        void drawHeader() override {
            if (items_key.size() == 0) return;

            const std::string* str = (*language_ptr).getStringPtr(items_key.front(), (*select_language_ptr).cgetKey());
            if (str == nullptr || str->size() == 0) return;

            const std::uint_least32_t select_key = (*select_language_ptr).cgetKey();
            paxg::Font* one_font = (*font).getAndAdd(select_key, font_size, font_buffer_thickness_size);
            if (one_font == nullptr) return;

            (*one_font).draw(
                *str,
                paxg::Vec2i(rect.x() + padding.x(), rect.y() + padding.y()),
                paxg::Color{ 0, 0, 0 });
        }

        /// @brief 項目が選択された時の処理
        /// @details 項目の状態を切り替える（ON/OFF）
        void onItemSelected(std::size_t index) override {
            if (index < is_items.size()) {
                is_items[index] = !is_items[index];
            }
        }

        /// @brief ドロップダウンリストの開始インデックスを取得
        /// @details 最初の項目はヘッダー名なのでスキップ（1から開始）
        std::size_t getDropdownStartIndex() const override {
            return 1; // 最初の項目（ヘッダー名）をスキップ
        }

        /// @brief ヘッダーのフォントキーを取得
        std::uint_least32_t getHeaderFontKey() const override {
            return (*select_language_ptr).cgetKey();
        }

    public:
        /// @brief 項目の状態を取得（インデックス指定）
        /// @details MenuItemでは最初の項目がヘッダー名なので、+1してアクセス
        bool getIsItems(const std::size_t i) const {
            if (is_items.size() == 0) return true;
            const std::size_t actual_index = i + 1; // ヘッダー分をオフセット
            if (actual_index < is_items.size()) return is_items[actual_index];
            return is_items.front();
        }

        /// @brief 項目の状態を取得（キー指定）
        bool getIsItems(const std::uint_least32_t key) const {
            if (item_index_key.find(key) == item_index_key.end()) return true;
            const std::size_t index = item_index_key.at(key);
            // キーで取得する場合、インデックスは既に正しい位置を指しているので
            // ヘッダー（インデックス0）を参照している場合のみスキップ
            if (index == 0) return true; // ヘッダー自体は常にtrue
            if (index < is_items.size()) return is_items[index];
            return is_items.front();
        }

        /// @brief 項目の状態を取得（キー指定、getIsItemsKeyエイリアス）
        bool getIsItemsKey(const std::uint_least32_t key) const {
            return getIsItems(key);
        }

    protected:
        /// @brief ドロップダウンリストを描画（チェックマーク付き）
        void drawDropdownList() override {
            paxg::Vec2i pos = rect.pos();
            pos.setY(static_cast<int>(pos.y() + rect.h()));

            const std::size_t start_index = getDropdownStartIndex();
            const std::size_t display_item_count = items_key.size() - start_index;
            const paxg::Rect back_rect{ pos, all_rect_x, static_cast<float>(rect.h() * display_item_count) };
            back_rect.drawShadow({ 1, 1 }, 4, 1).draw();

            constexpr int checkmark_width = 20; // チェックマークの幅

            for (std::size_t i = start_index; i < items_key.size(); ++i) {
                const std::string* i_str = (*language_ptr).getStringPtr(items_key[i], (*select_language_ptr).cgetKey());
                if (i_str == nullptr || i_str->size() == 0) continue;

                const paxg::Rect rect_tmp{ pos, all_rect_x, rect.h() };
                if (rect_tmp.mouseOver()) {
                    rect_tmp.draw(paxg::Color{ 135, 206, 235 });
                }

                // チェックマークを描画（ONの場合のみ）
                if (i < is_items.size() && is_items[i]) {
                    const int check_x = pos.x() + 5;
                    const int check_y = static_cast<int>(pos.y() + rect.h() / 2.0f);

                    // シンプルなチェックマーク "✓" を描画
                    const std::uint_least32_t select_font_key = (*select_language_ptr).cgetKey();
                    paxg::Font* check_font = (*font).getAndAdd(select_font_key, font_size, font_buffer_thickness_size);
                    if (check_font != nullptr) {
                        (*check_font).draw(
                            reinterpret_cast<const char*>(u8"✓"),
                            paxg::Vec2i(check_x, pos.y() + padding.y()),
                            paxg::Color{ 0, 0, 0 });
                    }
                }

                // テキストを描画（チェックマークの分だけ右にずらす）
                const std::uint_least32_t select_font_key = getItemFontKey(i);
                paxg::Font* one_font = (*font).getAndAdd(select_font_key, font_size, font_buffer_thickness_size);
                if (one_font == nullptr) continue;

                (*one_font).draw(
                    *i_str,
                    paxg::Vec2i(pos.x() + padding.x() + checkmark_width, pos.y() + padding.y()),
                    paxg::Color{ 0, 0, 0 });
                pos.setY(static_cast<int>(pos.y() + rect.h()));
            }

            back_rect.drawFrame(1, 0, paxg::Color{ 128, 128, 128 });
        }
    };

} // namespace paxs

#endif // !PAX_MAHOROBA_MENU_ITEM_HPP
