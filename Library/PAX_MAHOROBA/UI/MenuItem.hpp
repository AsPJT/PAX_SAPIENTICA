/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023-2024 As Project
	[Production]	2023-2024 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_MENU_ITEM_HPP
#define PAX_MAHOROBA_MENU_ITEM_HPP

#include <span>
#include <string>
#include <vector>

#include <PAX_GRAPHICA/Rect.hpp>
#include <PAX_GRAPHICA/Triangle.hpp>

#include <PAX_MAHOROBA/UI/IUIWidget.hpp>
#include <PAX_MAHOROBA/Rendering/LanguageFonts.hpp>

#include <PAX_SAPIENTICA/Language.hpp>
#include <PAX_SAPIENTICA/InputStateManager.hpp>
#include <PAX_SAPIENTICA/UnorderedMap.hpp>

namespace paxs {

	/// @brief メニューバー用のドロップダウン項目
	/// @brief Dropdown item for menu bar
	/// @details 固定のヘッダー名を表示し、項目選択時に状態を切り替える
	///          例：「ファイル」「編集」「表示」などのメニュー
	class MenuItem : public IUIWidget {
	private:
		// 言語関連
		const SelectLanguage* select_language_ptr = nullptr;
		const Language* language_ptr = nullptr;
		std::span<const std::uint_least32_t> items_key{};
		LanguageFonts* font = nullptr;

		// フォント設定
		std::uint_least8_t font_size = 16;
		std::uint_least8_t font_buffer_thickness_size = 16;

		// 項目の状態管理
		std::vector<bool> is_items{};
		paxs::UnorderedMap<std::uint_least32_t, std::size_t> item_index_key{};

		// レイアウト
		paxg::Rect rect{};
		paxg::Vec2i padding{ 6, 2 };
		float all_rect_x{}; // 全ての項目の文字幅
		int down_button_size = 20;

		// 状態
		bool is_open = false;
		bool visible_ = true;
		bool enabled_ = true;

		std::uint_least32_t old_language_key = 0;

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

			updateLanguage();

			// Key List を作成
			for (std::size_t i = 0; i < items_key_.size(); ++i) {
				item_index_key.emplace(items_key_[i], i);
			}

			// 項目の状態を初期化（全てtrue）
			is_items.resize(items_key_.size(), true);
		}

		/// @brief X 座標を設定
		void setRectX(const std::size_t x = 0) {
			rect.setX(static_cast<float>(x));
		}

		/// @brief 言語変更による更新処理
		void updateLanguage() {
			if (language_ptr == nullptr) return;
			if (select_language_ptr == nullptr) return;

			if (font == nullptr) {
				rect.setH(static_cast<float>(font_size) * 2.f);
			}
			else {
				const std::uint_least32_t select_key = (*select_language_ptr).cgetKey();
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

				const std::uint_least32_t select_key = (*select_language_ptr).cgetKey();
				paxg::Font* one_font = (*font).getAndAdd(select_key, font_size, font_buffer_thickness_size);
				if (one_font == nullptr) continue;

				all_rect_x = (std::max)(all_rect_x, static_cast<float>((*one_font).width(*str)));

				// 最初の項目（ヘッダー）の幅を使用
				if (i == 0) {
					rect.setW(static_cast<float>((*one_font).width(*str)));
				}
			}

			rect.setW(rect.w() + (padding.x() * 2 + down_button_size));
			all_rect_x += (padding.x() * 2 + down_button_size);

			// チェックマークの幅を追加
			constexpr int checkmark_width = 20;
			all_rect_x += checkmark_width;

#ifdef PAXS_USING_DXLIB
#ifdef __ANDROID__
			all_rect_x *= 2.5f;
			rect.setW(rect.w() * 2.0f);
			rect.setH(rect.h() * 1.4f);
#else
			all_rect_x *= 1.6f;
			rect.setW(rect.w() * 1.6f);
			rect.setH(rect.h() * 1.2f);
#endif
#elif defined(PAXS_USING_SFML)
			rect.setH(rect.h() * 1.2f);
#endif
		}

		/// @brief 更新処理
		void update(paxs::InputStateManager& input_state_manager) override {
			if (!visible_ || !enabled_) return;

			const std::uint_least32_t new_language_key = (*select_language_ptr).cgetKey();
			if (old_language_key != new_language_key) {
				old_language_key = new_language_key;
				updateLanguage();
			}

			// ヘッダーのクリック判定
			if (input_state_manager.get(rect.leftClicked())) {
				is_open = !is_open;
			}

			// ドロップダウンリストのクリック判定
			if (is_open) {
				paxg::Vec2i pos = rect.pos();
				pos.setY(static_cast<int>(pos.y() + rect.h()));

				// 最初の項目（ヘッダー）をスキップ
				for (std::size_t i = 1; i < items_key.size(); ++i) {
					const paxg::Rect rect_tmp{ pos, all_rect_x, rect.h() };
					if (input_state_manager.get(rect_tmp.leftClicked())) {
						// 項目の状態を切り替え
						if (i < is_items.size()) {
							is_items[i] = !is_items[i];
						}
						is_open = false;
						break;
					}
					pos.setY(static_cast<int>(pos.y() + rect.h()));
				}
			}
		}

		/// @brief 描画処理
		void draw() override {
			if (isEmpty() || !visible_) return;
			if (language_ptr == nullptr || select_language_ptr == nullptr) return;
			if (items_key.size() == 0) return;

			// ヘッダーの背景と枠を描画
			rect.draw(paxg::Color{ 243, 243, 243 });
			rect.drawFrame(1, 0, is_open ? paxg::Color{ 255, 165, 0 } : paxg::Color{ 128, 128, 128 });

			// 下向き三角形を描画
			constexpr float radius = 8.0f;
			static constexpr paxg::TriangleShape down_arrow_shape(radius, 3.1416f);
			const float center_x = static_cast<float>(rect.x() + rect.w() - down_button_size / 2.0 - padding.x());
			const float center_y = static_cast<float>(rect.y() + rect.h() / 2.0);
			paxg::Triangle triangle(center_x, center_y, down_arrow_shape);
			triangle.draw(paxg::Color{ 0, 0, 0 });

			// ヘッダー部分のテキストを描画
			drawHeader();

			// ドロップダウンリストを描画
			if (is_open) {
				drawDropdownList();
			}
		}

		/// @brief プルダウンが空かどうか
		bool isEmpty() const { return items_key.size() == 0; }

		/// @brief プルダウンの開閉状態を取得
		bool isOpen() const { return is_open; }

		/// @brief プルダウンを閉じる
		void close() { is_open = false; }

		/// @brief 項目の状態を設定（インデックス指定）
		void setIsItems(const std::size_t i, const bool new_value) {
			if (is_items.size() == 0) return;
			const std::size_t actual_index = i + 1; // ヘッダー分をオフセット
			if (actual_index < is_items.size()) is_items[actual_index] = new_value;
		}

		/// @brief 項目の状態を設定（キー指定）
		void setIsItems(const std::uint_least32_t key, const bool is_item) {
			if (item_index_key.find(key) == item_index_key.end()) return;
			const std::size_t i = item_index_key.at(key);
			if (i < is_items.size()) {
				is_items[i] = is_item;
			}
		}

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

		/// @brief 引数の Key の項目が TRUE か FALSE になっているか調べる
		bool getIsItemsKey(const std::uint_least32_t key) const {
			if (is_items.size() == 0) return true; // データがない場合
			if (item_index_key.find(key) == item_index_key.end()) return true; // 引数の Key が存在しない場合
			return getIsItems(item_index_key.at(key));
		}

		// IUIWidget インターフェースの実装
		void setPos(const paxg::Vec2i& pos) override { rect.setPos(pos); }
		paxg::Rect getRect() const override { return rect; }
		void setVisible(bool visible) override { visible_ = visible; }
		bool isVisible() const override { return visible_; }
		void setEnabled(bool enabled) override { enabled_ = enabled; }
		bool isEnabled() const override { return enabled_; }
		const char* getName() const override { return "MenuItem"; }
		bool isAvailable() const override { return true; }

	private:
		/// @brief ヘッダー部分のテキストを描画
		/// @details 最初の項目（メニュー名）を常に表示
		void drawHeader() {
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

		/// @brief ドロップダウンリストを描画（チェックマーク付き）
		void drawDropdownList() {
			paxg::Vec2i pos = rect.pos();
			pos.setY(static_cast<int>(pos.y() + rect.h()));

			// 最初の項目（ヘッダー）をスキップ
			const std::size_t display_item_count = items_key.size() - 1;
			const paxg::Rect back_rect{ pos, all_rect_x, static_cast<float>(rect.h() * display_item_count) };
			back_rect.drawShadow({ 1, 1 }, 4, 1).draw();

			constexpr int checkmark_width = 20; // チェックマークの幅

			for (std::size_t i = 1; i < items_key.size(); ++i) {
				const std::string* i_str = (*language_ptr).getStringPtr(items_key[i], (*select_language_ptr).cgetKey());
				if (i_str == nullptr || i_str->size() == 0) continue;

				const paxg::Rect rect_tmp{ pos, all_rect_x, rect.h() };
				if (rect_tmp.mouseOver()) {
					rect_tmp.draw(paxg::Color{ 135, 206, 235 });
				}

				// チェックマークを描画（ONの場合のみ）
				if (i < is_items.size() && is_items[i]) {
					const int check_x = pos.x() + 5;

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
				const std::uint_least32_t select_font_key = (*select_language_ptr).cgetKey();
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
