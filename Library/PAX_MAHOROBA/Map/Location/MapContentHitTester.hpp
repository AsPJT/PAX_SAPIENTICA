/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023-2024 As Project
	[Production]	2023-2024 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_MAP_CONTENT_HIT_TESTER_HPP
#define PAX_MAHOROBA_MAP_CONTENT_HIT_TESTER_HPP

#include <string>
#include <vector>

#include <PAX_GRAPHICA/Font.hpp>
#include <PAX_GRAPHICA/Rect.hpp>
#include <PAX_GRAPHICA/Vec2.hpp>

namespace paxs {

/// @brief 地図コンテンツのヒット判定ヘルパークラス
/// @brief Map content hit test helper class
class MapContentHitTester {
public:
	/// @brief 円形ヒット判定
	/// @brief Circle hit test
	/// @param mouse_x マウスX座標 / Mouse X coordinate
	/// @param mouse_y マウスY座標 / Mouse Y coordinate
	/// @param center 中心座標 / Center position
	/// @param radius 半径 / Radius
	/// @return ヒットした場合true / True if hit
	static bool circleHitTest(
		int mouse_x,
		int mouse_y,
		const paxg::Vec2i& center,
		int radius
	);

	/// @brief 矩形ヒット判定
	/// @brief Rectangle hit test
	/// @param mouse_x マウスX座標 / Mouse X coordinate
	/// @param mouse_y マウスY座標 / Mouse Y coordinate
	/// @param rect 矩形 / Rectangle
	/// @return ヒットした場合true / True if hit
	static bool rectHitTest(
		int mouse_x,
		int mouse_y,
		const paxg::Rect& rect
	);

	/// @brief テキストバウンディングボックスのヒット判定
	/// @brief Text bounding box hit test
	/// @param mouse_x マウスX座標 / Mouse X coordinate
	/// @param mouse_y マウスY座標 / Mouse Y coordinate
	/// @param text テキスト / Text
	/// @param pos テキスト位置 / Text position
	/// @param font フォント / Font
	/// @param align アライメント / Alignment
	/// @return ヒットした場合true / True if hit
	static bool textHitTest(
		int mouse_x,
		int mouse_y,
		const std::string& text,
		const paxg::Vec2i& pos,
		paxg::Font* font,
		const char* align = "topCenter"
	);

	/// @brief 経度ラップを考慮した複数座標のヒット判定
	/// @brief Hit test with multiple positions (considering longitude wrapping)
	/// @tparam HitTestFunc ヒット判定関数型 / Hit test function type
	/// @param mouse_x マウスX座標 / Mouse X coordinate
	/// @param mouse_y マウスY座標 / Mouse Y coordinate
	/// @param positions スクリーン座標のリスト / List of screen positions
	/// @param hit_test_func ヒット判定関数 / Hit test function
	/// @return いずれかの座標でヒットした場合true / True if hit at any position
	template<typename HitTestFunc>
	static bool testMultiplePositions(
		int mouse_x,
		int mouse_y,
		const std::vector<paxg::Vec2i>& positions,
		HitTestFunc hit_test_func
	) {
		for (const auto& pos : positions) {
			if (hit_test_func(mouse_x, mouse_y, pos)) {
				return true;
			}
		}
		return false;
	}

	/// @brief テキストのバウンディングボックスを計算（公開メソッド）
	/// @brief Calculate text bounding box (public method)
	static paxg::Rect getTextBoundingRect(
		const std::string& text,
		const paxg::Vec2i& pos,
		paxg::Font* font,
		const char* align
	);
};

// ========================================
// インライン実装
// ========================================

inline bool MapContentHitTester::circleHitTest(
	int mouse_x,
	int mouse_y,
	const paxg::Vec2i& center,
	int radius
) {
	const int dx = mouse_x - center.x();
	const int dy = mouse_y - center.y();
	return (dx * dx + dy * dy) <= (radius * radius);
}

inline bool MapContentHitTester::rectHitTest(
	int mouse_x,
	int mouse_y,
	const paxg::Rect& rect
) {
	return rect.contains(static_cast<float>(mouse_x), static_cast<float>(mouse_y));
}

inline bool MapContentHitTester::textHitTest(
	[[maybe_unused]] int mouse_x,
	[[maybe_unused]] int mouse_y,
	[[maybe_unused]] const std::string& text,
	[[maybe_unused]] const paxg::Vec2i& pos,
	[[maybe_unused]] paxg::Font* font,
	[[maybe_unused]] const char* align
) {
	// TODO: Font::getSize()が実装されたら有効化
	// const auto bounding_rect = getTextBoundingRect(text, pos, font, align);
	// return rectHitTest(mouse_x, mouse_y, bounding_rect);
	return false;
}

inline paxg::Rect MapContentHitTester::getTextBoundingRect(
	[[maybe_unused]] const std::string& text,
	const paxg::Vec2i& pos,
	[[maybe_unused]] paxg::Font* font,
	[[maybe_unused]] const char* align
) {
	// TODO: Font::getSize()が実装されたら有効化
	// if (font == nullptr || text.empty()) {
	// 	return paxg::Rect{0, 0, 0, 0};
	// }
	// const auto text_size = font->getSize(text);
	// float x = static_cast<float>(pos.x());
	// float y = static_cast<float>(pos.y());
	// // アライメントに応じて位置を調整
	// if (std::string(align).find("Center") != std::string::npos) {
	// 	x -= text_size.x / 2.0f;
	// } else if (std::string(align).find("Right") != std::string::npos) {
	// 	x -= text_size.x;
	// }
	// if (std::string(align).find("middle") != std::string::npos) {
	// 	y -= text_size.y / 2.0f;
	// } else if (std::string(align).find("bottom") != std::string::npos) {
	// 	y -= text_size.y;
	// }
	// return paxg::Rect{x, y, text_size.x, text_size.y};

	// 仮実装：position周辺の矩形を返す
	return paxg::Rect{static_cast<float>(pos.x()), static_cast<float>(pos.y()), 100.0f, 20.0f};
}

} // namespace paxs

#endif // !PAX_MAHOROBA_MAP_CONTENT_HIT_TESTER_HPP
