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

		Pulldown(
			const std::vector<s3d::String>& items_, // 表示するもの
			const s3d::Font& font_, 
			const s3d::Point& pos_ = { 0,0 },
			PulldownType pdt_ = PulldownType::Zero)
			: font{ font_ }
			, items{ items_ }
			, rect{ pos_, 0, (font.height() + padding.y * 2) }
			, pdt(pdt_){
			is_items.resize(items_.size());
			for (const auto& is_item : is_items) {
				is_item = true;
			}
			for (const auto& item : items) {
				rect.w = s3d::Max(rect.w, static_cast<s3d::int32>(font(item).region().w));
			}
			rect.w += (padding.x * 2 + down_button_size);
		}

		bool isEmpty() const {
			return items.empty();
		}

		void update() {
			if (isEmpty()) return;
			if (rect.leftClicked()) {
				is_open = (not is_open);
			}
			s3d::Point pos = rect.pos.movedBy(0, rect.h);
			if (is_open) {
				for (auto i : s3d::step(items.size())) {
					const s3d::Rect rect_tmp{ pos, rect.w, rect.h };
					if (rect_tmp.leftClicked()) {
						index = i;
						is_items[i] = !(is_items[i]);
						is_open = false;
						break;
					}
					pos.y += rect.h;
				}
			}
		}

		void draw() const {
			rect.draw();
			if (isEmpty())return;
			rect.drawFrame(1, 0, is_open ? s3d::Palette::Orange : s3d::Palette::Gray);
			s3d::Point pos = rect.pos;
			
			switch (pdt) {
			case paxs::PulldownType::Empty:break;
			case paxs::PulldownType::Zero:
				font(items[index]).draw(pos + padding, s3d::Palette::Black);
				break;
			case paxs::PulldownType::One:
				font(items[0]).draw(pos + padding, s3d::Palette::Black);
				break;
			default:break;
			}
			s3d::Triangle{ (rect.x + rect.w - down_button_size / 2.0 - padding.x), (rect.y + rect.h / 2.0),
				(down_button_size * 0.5), 3.1416 }.draw(s3d::Palette::Black);
			pos.y += rect.h;
			if (is_open) {
				const s3d::Rect backRect{ pos, rect.w, (rect.h * items.size()) };
				backRect.drawShadow({ 1, 1 }, 4, 1).draw();
				//for (const auto& item : items) {
				std::size_t i = 0;
				switch (pdt) {
				case paxs::PulldownType::Empty:break;
				case paxs::PulldownType::Zero:i = 0;break;
				case paxs::PulldownType::One:i = 1;break;
				default:break;
				}
				for (; i < items.size(); ++i) {
					const s3d::Rect rect_tmp{ pos, rect.size };
					if (rect_tmp.mouseOver()) {
						rect_tmp.draw(s3d::Palette::Skyblue);
					}
					font(items[i]).draw((pos + padding), s3d::Palette::Black);
					pos.y += rect.h;
				}
				backRect.drawFrame(1, 0, s3d::Palette::Gray);
			}
		}

		void setPos(const s3d::Point& pos) { rect.setPos(pos); }
		const s3d::Rect& getRect() const { return rect; }
		size_t getIndex() const { return index; }
		bool getIsItems(std::size_t i) const { return is_items[i]; }
		s3d::String getItem() const {
			if (isEmpty()) return{};
			return items[index];
		}

	private:
		s3d::Font font;
		std::vector<s3d::String> items;
		std::vector<bool> is_items;
		size_t index = 0;
		s3d::Size padding{ 6, 2 };
		s3d::Rect rect;
		s3d::int32 down_button_size = 20;
		bool is_open = false;
		PulldownType pdt;
	};

}

#endif // !PAX_SAPIENTICA_SIV3D_PULLDOWN_HPP