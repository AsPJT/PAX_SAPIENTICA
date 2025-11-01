/*##########################################################################################

	PAX SAPIENTICA Library 💀🌿🌏

	[Planning]		2023-2024 As Project
	[Production]	2023-2024 As Project
	[Contact Us]	wanotaitei@gmail.com			https://github.com/AsPJT/PAX_SAPIENTICA
	[License]		Distributed under the CC0 1.0.	https://creativecommons.org/publicdomain/zero/1.0/

##########################################################################################*/

#ifndef PAX_MAHOROBA_UI_PANEL_BACKGROUND_HPP
#define PAX_MAHOROBA_UI_PANEL_BACKGROUND_HPP

#include <functional>
#include <vector>

#include <PAX_GRAPHICA/Color.hpp>
#include <PAX_GRAPHICA/Rect.hpp>
#include <PAX_GRAPHICA/RenderTexture.hpp>
#include <PAX_GRAPHICA/RoundRect.hpp>

#include <PAX_MAHOROBA/Rendering/ShadowRenderer.hpp>

namespace paxs {

	/// @brief UIパネルの背景と影を描画するクラス
	/// @brief Class for rendering UI panel background and shadow
	///
	/// 各UIパネル（CalendarPanel、HeaderPanel、SimulationPanel等）で使用される
	/// 背景と影の描画処理を共通化します。
	/// Siv3Dでは影を一度にまとめて描画することで、影の重複を防ぎます。
	/// Provides common background and shadow rendering for UI panels.
	/// For Siv3D, renders all shadows at once to prevent shadow duplication.
	class PanelBackground {
	public:
		PanelBackground() = default;

		/// @brief 影用のテクスチャを設定
		/// @brief Set textures for shadow rendering
		/// @param shadow_tex 影テクスチャ / Shadow texture
		/// @param internal_tex 内部テクスチャ / Internal texture
		void setShadowTextures(paxg::RenderTexture& shadow_tex, paxg::RenderTexture& internal_tex) {
			shadow_texture_ = &shadow_tex;
			internal_texture_ = &internal_tex;
		}

		/// @brief バッチ描画の開始（Siv3D用）
		/// @brief Begin batch rendering (for Siv3D)
		///
		/// 複数のパネルの影をまとめて描画するため、フレームの最初に呼び出します。
		/// Call this at the beginning of the frame to batch render multiple panel shadows.
		static void beginBatch() {
#ifdef PAXS_USING_SIV3D
			getShadowShapes().clear();
			getPanelShapes().clear();
#endif
		}

		/// @brief バッチ描画の終了と実行（Siv3D用）
		/// @brief End and execute batch rendering (for Siv3D)
		///
		/// 登録されたすべてのパネルの影を一度に描画します。
		/// Renders all registered panel shadows at once.
		static void endBatch(paxg::RenderTexture* shadow_tex, paxg::RenderTexture* internal_tex) {
#ifdef PAXS_USING_SIV3D
			if (!shadow_tex || !internal_tex) return;
			if (getShadowShapes().empty()) return;

			// すべての影を一度に描画
			paxs::ShadowRenderer::renderShadowWithPanels(
				*shadow_tex,
				*internal_tex,
				[]() {
					// すべての影の形状を描画
					for (const auto& draw_func : getShadowShapes()) {
						draw_func();
					}
				},
				[]() {
					// すべてのパネル本体を描画
					for (const auto& draw_func : getPanelShapes()) {
						draw_func();
					}
				}
			);
#endif
		}

		/// @brief 背景と影を描画（角丸）
		/// @brief Draw background and shadow (rounded corners)
		/// @param x パネルのX座標 / Panel X coordinate
		/// @param y パネルのY座標 / Panel Y coordinate
		/// @param width パネルの幅 / Panel width
		/// @param height パネルの高さ / Panel height
		/// @param corner_radius 角の丸み半径 / Corner radius (default: 10)
		/// @param bg_color 背景色 / Background color (default: white)
		void draw(
			int x, int y, int width, int height,
			int corner_radius = 10,
			const paxg::Color& bg_color = paxg::Color{255, 255, 255}
		) const {
#ifdef PAXS_USING_SIV3D
			// Siv3D: バッチ描画に登録
			// Register for batch rendering
			getShadowShapes().push_back([x, y, width, height, corner_radius]() {
				paxg::RoundRect{ x, y, width, height, corner_radius }.draw();
			});
			getPanelShapes().push_back([x, y, width, height, corner_radius, bg_color]() {
				paxg::RoundRect{ x, y, width, height, corner_radius }.draw(bg_color);
			});
#else
			// SFML/DxLib: 即座に描画
			// Draw immediately
			paxg::RoundRect{ x, y, width, height, corner_radius }
				.drawShadow({1, 1}, 4, 1).draw(bg_color);
#endif
		}

		/// @brief 背景と影を描画（矩形）
		/// @brief Draw background and shadow (rectangle)
		/// @param x パネルのX座標 / Panel X coordinate
		/// @param y パネルのY座標 / Panel Y coordinate
		/// @param width パネルの幅 / Panel width
		/// @param height パネルの高さ / Panel height
		/// @param bg_color 背景色 / Background color (default: white)
		void drawRect(
			int x, int y, int width, int height,
			const paxg::Color& bg_color = paxg::Color{255, 255, 255}
		) const {
#ifdef PAXS_USING_SIV3D
			// Siv3D: バッチ描画に登録
			// Register for batch rendering
			getShadowShapes().push_back([x, y, width, height]() {
				paxg::Rect{ static_cast<float>(x), static_cast<float>(y), static_cast<float>(width), static_cast<float>(height) }.draw();
			});
			getPanelShapes().push_back([x, y, width, height, bg_color]() {
				paxg::Rect{ static_cast<float>(x), static_cast<float>(y), static_cast<float>(width), static_cast<float>(height) }.draw(bg_color);
			});
#else
			// SFML/DxLib: 即座に描画
			// Draw immediately
			paxg::Rect{ static_cast<float>(x), static_cast<float>(y), static_cast<float>(width), static_cast<float>(height) }
				.drawShadow({1, 1}, 4, 1).draw(bg_color);
#endif
		}

	private:
		// 影描画用テクスチャ（外部から注入）
		paxg::RenderTexture* shadow_texture_ = nullptr;
		paxg::RenderTexture* internal_texture_ = nullptr;

#ifdef PAXS_USING_SIV3D
		// バッチ描画用の静的コンテナ
		// Static containers for batch rendering
		static std::vector<std::function<void()>>& getShadowShapes() {
			static std::vector<std::function<void()>> shadow_shapes;
			return shadow_shapes;
		}

		static std::vector<std::function<void()>>& getPanelShapes() {
			static std::vector<std::function<void()>> panel_shapes;
			return panel_shapes;
		}
#endif
	};

} // namespace paxs

#endif // !PAX_MAHOROBA_UI_PANEL_BACKGROUND_HPP
