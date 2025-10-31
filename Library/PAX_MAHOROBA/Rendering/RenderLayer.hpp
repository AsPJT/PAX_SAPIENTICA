/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023-2024 As Project
	[Production]	2023-2024 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_RENDER_LAYER_HPP
#define PAX_MAHOROBA_RENDER_LAYER_HPP

#include <cstdint>

namespace paxs {

	/// @brief レンダリングレイヤー（Z順序）を表すenum
	/// @brief Rendering layer (Z-order) enumeration
	///
	/// 値が小さいほど後ろ（背景）に描画され、値が大きいほど前面に描画されます。
	/// Lower values are drawn in the back (background), higher values are drawn in front.
	enum class RenderLayer : std::uint16_t {
		// 背景レイヤー（最背面）
		// Background layer (back-most)
		Background = 0,

		// 地図タイルレイヤー（地形、衛星画像など）
		// Map tile layer (terrain, satellite images, etc.)
		MapBase = 100,

		// 地図コンテンツレイヤー（マーカー、エージェント、ルートなど）
		// Map content layer (markers, agents, routes, etc.)
		MapContent = 200,

		// UI背景レイヤー（パネル背景、影など）
		// UI background layer (panel backgrounds, shadows, etc.)
		UIBackground = 300,

		// UIコンテンツレイヤー（ボタン、テキスト、アイコンなど）
		// UI content layer (buttons, text, icons, etc.)
		UIContent = 400,

		// UIオーバーレイレイヤー（プルダウンメニュー、ツールチップなど）
		// UI overlay layer (dropdown menus, tooltips, etc.)
		UIOverlay = 500,

		// デバッグ情報レイヤー（最前面）
		// Debug info layer (front-most)
		Debug = 1000
	};

	/// @brief RenderLayerの比較演算子
	/// @brief Comparison operators for RenderLayer

	inline bool operator<(RenderLayer lhs, RenderLayer rhs) {
		return static_cast<std::uint16_t>(lhs) < static_cast<std::uint16_t>(rhs);
	}

	inline bool operator<=(RenderLayer lhs, RenderLayer rhs) {
		return static_cast<std::uint16_t>(lhs) <= static_cast<std::uint16_t>(rhs);
	}

	inline bool operator>(RenderLayer lhs, RenderLayer rhs) {
		return static_cast<std::uint16_t>(lhs) > static_cast<std::uint16_t>(rhs);
	}

	inline bool operator>=(RenderLayer lhs, RenderLayer rhs) {
		return static_cast<std::uint16_t>(lhs) >= static_cast<std::uint16_t>(rhs);
	}

	inline bool operator==(RenderLayer lhs, RenderLayer rhs) {
		return static_cast<std::uint16_t>(lhs) == static_cast<std::uint16_t>(rhs);
	}

	inline bool operator!=(RenderLayer lhs, RenderLayer rhs) {
		return static_cast<std::uint16_t>(lhs) != static_cast<std::uint16_t>(rhs);
	}

} // namespace paxs

#endif // !PAX_MAHOROBA_RENDER_LAYER_HPP
