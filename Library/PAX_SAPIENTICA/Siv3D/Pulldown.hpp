/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023 As Project
	[Production]	2023 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_SAPIENTICA_SIV3D_PULLDOWN_HPP
#define PAX_SAPIENTICA_SIV3D_PULLDOWN_HPP

/*##########################################################################################

##########################################################################################*/

#include <Siv3D.hpp> // OpenSiv3D v0.6.6
#include <vector>

namespace paxs {

	enum class PulldownType : int {
		Empty,
		Zero, // 項目も選択 (0)
		One // 1 から選択 (1)
	};

	class Pulldown {
	public:

		Pulldown() = default;

		void setRectX(const std::size_t x = 0) {
			rect.x = s3d::Rect::value_type(x);
		}

		void updateLanguage() {
			item_size = 0;
			rect.w = 0;
			for (std::size_t i = start_index; i < itemsa.size(); ++i) {
				const auto& item_front = itemsa[i].front();
				const auto& item = itemsa[i][language_index + start_language_index];
				if (item_front.size() == 0) break;
				if (item_front[0] == '>') break;
				++item_size;
				rect.w = s3d::Max(rect.w, static_cast<s3d::int32>(font.front()(item).region().w));
			}
			rect.w += (padding.x * 2 + down_button_size);
		}

		Pulldown(
			const std::vector<std::vector<s3d::String>>& items_, // 表示するもの
			const std::size_t start_index_,
			const std::size_t start_language_index_,
			const std::vector<s3d::Font>& font_,
			const s3d::Point& pos_ = { 0,0 },
			PulldownType pdt_ = PulldownType::Zero,
			const bool is_one_font_ = false)
			: 
			start_index{start_index_}
			, start_language_index{ start_language_index_}
			, font{ font_ }
			, itemsa{ items_ }
			, rect{ pos_, 0, (font.front().height() + padding.y * 2)}
			, pdt(pdt_)
			, is_one_font(is_one_font_){
			updateLanguage();
			is_items.resize(item_size);
			for (const auto& is_item : is_items) {
				is_item = true;
			}
		}

		bool isEmpty() const {
			return itemsa.empty();
		}

		void update(std::size_t language_index_) {
			if (isEmpty()) return;
			if (language_index != language_index_) {
				language_index = language_index_;
				updateLanguage();
			}
			if (rect.leftClicked()) {
				is_open = (not is_open);
			}
			s3d::Point pos = rect.pos.movedBy(0, rect.h);
			if (is_open) {
				for (auto i : s3d::step(itemsa.size())) {
					const s3d::Rect rect_tmp{ pos, rect.w, rect.h };
					if (rect_tmp.leftClicked()) {
						if (i < is_items.size()) {
							index = i;
							is_items[i] = !(is_items[i]);
							is_open = false;
							break;
						}
					}
					pos.y += rect.h;
				}
			}
		}

		void draw() const {
			const std::size_t item_index = index + start_index;
			rect.draw(s3d::Color{ 243,243,243 });
			if (isEmpty())return;
			rect.drawFrame(1, 0, is_open ? s3d::Palette::Orange : s3d::Palette::Gray);
			s3d::Point pos = rect.pos;

			const std::size_t select_index = ((is_one_font) ? item_index : language_index);
			switch (pdt) {
			case paxs::PulldownType::Empty:break;
			case paxs::PulldownType::Zero:
				font[select_index](
					(language_index + start_language_index < itemsa[item_index].size()) ? itemsa[item_index][language_index + start_language_index] : itemsa[item_index][start_language_index]
				).draw(pos + padding, s3d::Palette::Black);
				break;
			case paxs::PulldownType::One:
				font[language_index](
					(language_index + start_language_index < itemsa[item_index].size()) ? itemsa[start_index][language_index + start_language_index] : itemsa[item_index][start_language_index]
				).draw(pos + padding, s3d::Palette::Black);
				break;
			default:break;
			}
			s3d::Triangle{ (rect.x + rect.w - down_button_size / 2.0 - padding.x), (rect.y + rect.h / 2.0),
				(down_button_size * 0.5), 3.1416 }.draw(s3d::Palette::Black);
			pos.y += rect.h;
			if (is_open) {
				const s3d::Rect backRect{ pos, rect.w, (rect.h * item_size/*itemsa.size()*/)};
				backRect.drawShadow({ 1, 1 }, 4, 1).draw();
				//for (const auto& item : items) {
				std::size_t i = start_index;
				switch (pdt) {
				case paxs::PulldownType::Empty:break;
				case paxs::PulldownType::Zero:i = start_index;break;
				case paxs::PulldownType::One:i = start_index + 1;break;
				default:break;
				}
				for (; i < itemsa.size(); ++i) {
					if (itemsa[i].front().size() == 0) break;
					if (itemsa[i].front()[0] == '>') break;
					const s3d::Rect rect_tmp{ pos, rect.size };
					if (rect_tmp.mouseOver()) {
						rect_tmp.draw(s3d::Palette::Skyblue);
					}
					const std::size_t select_index2 = ((is_one_font) ? i : language_index);
					font[select_index2](
						(language_index + start_language_index < itemsa[i].size()) ? itemsa[i][language_index + start_language_index] : itemsa[i][start_language_index]
					).draw((pos + padding), s3d::Palette::Black);
					pos.y += rect.h;
				}
				backRect.drawFrame(1, 0, s3d::Palette::Gray);
			}
		}

		void setPos(const s3d::Point& pos) { rect.setPos(pos); }
		const s3d::Rect& getRect() const { return rect; }
		size_t getIndex() const { return index + start_index; }
		bool getIsItems(std::size_t i) const {
			if(i < is_items.size()) return is_items[i];
			return is_items.front();
		}
		s3d::String getItem() const {
			if (isEmpty()) return{};
			return itemsa[index + start_index].front();
		}

	private:
		std::size_t item_size = 0;
		std::size_t start_index = 0;
		std::size_t start_language_index = 0;
		std::size_t language_index = 0;
		std::vector<s3d::Font> font;
		std::vector<std::vector<s3d::String>> itemsa;
		std::vector<bool> is_items;
		size_t index = 0;
		s3d::Size padding{ 6, 2 };
		s3d::Rect rect;
		s3d::int32 down_button_size = 20;
		bool is_open = false;
		PulldownType pdt;
		bool is_one_font = false;
	};

	class MenuBar {
	public:

		void add(
			const std::vector<std::vector<s3d::String>>& menu_bar_pulldown,
			const std::size_t start_index,
			const std::size_t start_language_index,
			const std::vector<s3d::Font>& font_menu_bar) {

			if (pdv.size() != 0) {
				start_x += pdv.back().getRect().w;
			}
			pdv.emplace_back(paxs::Pulldown(
				menu_bar_pulldown, 
				start_index,
				start_language_index,
				font_menu_bar, 
				s3d::Point{ start_x, 0 },
				paxs::PulldownType::One ));
		}

		void update(std::size_t language_index_) {
			start_x = 0;
			for (auto& pd : pdv) {
				pd.update(language_index_);
				pd.setRectX(start_x);
				start_x += pd.getRect().w;
			}
		}
		void draw() {
			for (auto& pd : pdv) {
				pd.draw();
			}
		}

		paxs::Pulldown& getPulldown(const std::size_t i) {
			return pdv[i];
		}

	private:
		std::vector<paxs::Pulldown> pdv;
		std::size_t start_x = 0;
	};

}

#endif // !PAX_SAPIENTICA_SIV3D_PULLDOWN_HPP